// SPDX-License-Identifier: GPL-2.0-only
/*
 * Copyright (C) 2021, Red Hat Inc.
 *
 * Generic tests for KVM CPUID set/get ioctls
 */
#include <asm/kvm_para.h>
#include <linux/kvm_para.h>
#include <stdint.h>

#include "test_util.h"
#include "kvm_util.h"
#include "processor.h"

struct cpuid_mask {
	union {
		struct {
			u32 eax;
			u32 ebx;
			u32 ecx;
			u32 edx;
		};
		u32 regs[4];
	};
};

static void test_guest_cpuids(struct kvm_cpuid2 *guest_cpuid)
{
	int i;
	u32 eax, ebx, ecx, edx;

	for (i = 0; i < guest_cpuid->nent; i++) {
		__cpuid(guest_cpuid->entries[i].function,
			guest_cpuid->entries[i].index,
			&eax, &ebx, &ecx, &edx);

		GUEST_ASSERT_EQ(eax, guest_cpuid->entries[i].eax);
		GUEST_ASSERT_EQ(ebx, guest_cpuid->entries[i].ebx);
		GUEST_ASSERT_EQ(ecx, guest_cpuid->entries[i].ecx);
		GUEST_ASSERT_EQ(edx, guest_cpuid->entries[i].edx);
	}

}

static void guest_main(struct kvm_cpuid2 *guest_cpuid)
{
	GUEST_SYNC(1);

	test_guest_cpuids(guest_cpuid);

	GUEST_SYNC(2);

	GUEST_ASSERT_EQ(this_cpu_property(X86_PROPERTY_MAX_KVM_LEAF), 0x40000001);

	GUEST_DONE();
}

static struct cpuid_mask get_const_cpuid_mask(const struct kvm_cpuid_entry2 *entry)
{
	struct cpuid_mask mask;

	memset(&mask, 0xff, sizeof(mask));

	switch (entry->function) {
	case 0x1:
		mask.regs[X86_FEATURE_OSXSAVE.reg] &= ~BIT(X86_FEATURE_OSXSAVE.bit);
		break;
	case 0x7:
		mask.regs[X86_FEATURE_OSPKE.reg] &= ~BIT(X86_FEATURE_OSPKE.bit);
		break;
	case 0xd:
		/*
		 * CPUID.0xD.{0,1}.EBX enumerate XSAVE size based on the current
		 * XCR0 and IA32_XSS MSR values.
		 */
		if (entry->index < 2)
			mask.ebx = 0;
		break;
	}
	return mask;
}

static void compare_cpuids(const struct kvm_cpuid2 *cpuid1,
			   const struct kvm_cpuid2 *cpuid2)
{
	const struct kvm_cpuid_entry2 *e1, *e2;
	int i;

	TEST_ASSERT(cpuid1->nent == cpuid2->nent,
		    "CPUID nent mismatch: %d vs. %d", cpuid1->nent, cpuid2->nent);

	for (i = 0; i < cpuid1->nent; i++) {
		struct cpuid_mask mask;

		e1 = &cpuid1->entries[i];
		e2 = &cpuid2->entries[i];

		TEST_ASSERT(e1->function == e2->function &&
			    e1->index == e2->index && e1->flags == e2->flags,
			    "CPUID entries[%d] mismtach: 0x%x.%d.%x vs. 0x%x.%d.%x",
			    i, e1->function, e1->index, e1->flags,
			    e2->function, e2->index, e2->flags);

		/* Mask off dynamic bits, e.g. OSXSAVE, when comparing entries. */
		mask = get_const_cpuid_mask(e1);

		TEST_ASSERT((e1->eax & mask.eax) == (e2->eax & mask.eax) &&
			    (e1->ebx & mask.ebx) == (e2->ebx & mask.ebx) &&
			    (e1->ecx & mask.ecx) == (e2->ecx & mask.ecx) &&
			    (e1->edx & mask.edx) == (e2->edx & mask.edx),
			    "CPUID 0x%x.%x differ: 0x%x:0x%x:0x%x:0x%x vs 0x%x:0x%x:0x%x:0x%x",
			    e1->function, e1->index,
			    e1->eax & mask.eax, e1->ebx & mask.ebx,
			    e1->ecx & mask.ecx, e1->edx & mask.edx,
			    e2->eax & mask.eax, e2->ebx & mask.ebx,
			    e2->ecx & mask.ecx, e2->edx & mask.edx);
	}
}

static void run_vcpu(struct kvm_vcpu *vcpu, int stage)
{
	struct ucall uc;

	vcpu_run(vcpu);

	switch (get_ucall(vcpu, &uc)) {
	case UCALL_SYNC:
		TEST_ASSERT(!strcmp((const char *)uc.args[0], "hello") &&
			    uc.args[1] == stage + 1,
			    "Stage %d: Unexpected register values vmexit, got %lx",
			    stage + 1, (ulong)uc.args[1]);
		return;
	case UCALL_DONE:
		return;
	case UCALL_ABORT:
		REPORT_GUEST_ASSERT(uc);
	default:
		TEST_ASSERT(false, "Unexpected exit: %s",
			    exit_reason_str(vcpu->run->exit_reason));
	}
}

struct kvm_cpuid2 *vcpu_alloc_cpuid(struct kvm_vm *vm, vm_vaddr_t *p_gva, struct kvm_cpuid2 *cpuid)
{
	int size = sizeof(*cpuid) + cpuid->nent * sizeof(cpuid->entries[0]);
	vm_vaddr_t gva = vm_vaddr_alloc(vm, size, KVM_UTIL_MIN_VADDR);
	struct kvm_cpuid2 *guest_cpuids = addr_gva2hva(vm, gva);

	memcpy(guest_cpuids, cpuid, size);

	*p_gva = gva;
	return guest_cpuids;
}

static void set_cpuid_after_run(struct kvm_vcpu *vcpu)
{
	struct kvm_cpuid_entry2 *ent;
	int rc;
	u32 eax, ebx, x;

	/* Setting unmodified CPUID is allowed */
	rc = __vcpu_set_cpuid(vcpu);
	TEST_ASSERT(!rc, "Setting unmodified CPUID after KVM_RUN failed: %d", rc);

	/* Changing CPU features is forbidden */
	ent = vcpu_get_cpuid_entry(vcpu, 0x7);
	ebx = ent->ebx;
	ent->ebx--;
	rc = __vcpu_set_cpuid(vcpu);
	TEST_ASSERT(rc, "Changing CPU features should fail");
	ent->ebx = ebx;

	/* Changing MAXPHYADDR is forbidden */
	ent = vcpu_get_cpuid_entry(vcpu, 0x80000008);
	eax = ent->eax;
	x = eax & 0xff;
	ent->eax = (eax & ~0xffu) | (x - 1);
	rc = __vcpu_set_cpuid(vcpu);
	TEST_ASSERT(rc, "Changing MAXPHYADDR should fail");
	ent->eax = eax;
}

static void test_get_cpuid2(struct kvm_vcpu *vcpu)
{
	struct kvm_cpuid2 *cpuid = allocate_kvm_cpuid2(vcpu->cpuid->nent + 1);
	int i, r;

	vcpu_ioctl(vcpu, KVM_GET_CPUID2, cpuid);
	TEST_ASSERT(cpuid->nent == vcpu->cpuid->nent,
		    "KVM didn't update nent on success, wanted %u, got %u",
		    vcpu->cpuid->nent, cpuid->nent);

	for (i = 0; i < vcpu->cpuid->nent; i++) {
		cpuid->nent = i;
		r = __vcpu_ioctl(vcpu, KVM_GET_CPUID2, cpuid);
		TEST_ASSERT(r && errno == E2BIG, KVM_IOCTL_ERROR(KVM_GET_CPUID2, r));
		TEST_ASSERT(cpuid->nent == i, "KVM modified nent on failure");
	}
	free(cpuid);
}

int main(void)
{
	struct kvm_vcpu *vcpu;
	vm_vaddr_t cpuid_gva;
	struct kvm_vm *vm;
	int stage;

	vm = vm_create_with_one_vcpu(&vcpu, guest_main);

	compare_cpuids(kvm_get_supported_cpuid(), vcpu->cpuid);

	vcpu_alloc_cpuid(vm, &cpuid_gva, vcpu->cpuid);

	vcpu_args_set(vcpu, 1, cpuid_gva);

	for (stage = 0; stage < 3; stage++)
		run_vcpu(vcpu, stage);

	set_cpuid_after_run(vcpu);

	test_get_cpuid2(vcpu);

	kvm_vm_free(vm);
}
