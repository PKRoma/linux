/* SPDX-License-Identifier: (GPL-2.0-only OR BSD-2-Clause) */
/*
 * Copyright (c) 2024, Qualcomm Innovation Center, Inc. All rights reserved.
 */

#ifndef __DT_BINDINGS_INTERCONNECT_QCOM_QCS615_H
#define __DT_BINDINGS_INTERCONNECT_QCOM_QCS615_H

#define MASTER_A1NOC_CFG		1
#define MASTER_QDSS_BAM			2
#define MASTER_QSPI			3
#define MASTER_QUP_0			4
#define MASTER_BLSP_1			5
#define MASTER_CNOC_A2NOC		6
#define MASTER_CRYPTO			7
#define MASTER_IPA			8
#define MASTER_EMAC_EVB			9
#define MASTER_PCIE			10
#define MASTER_QDSS_ETR			11
#define MASTER_SDCC_1			12
#define MASTER_SDCC_2			13
#define MASTER_UFS_MEM			14
#define MASTER_USB2			15
#define MASTER_USB3_0			16
#define SLAVE_A1NOC_SNOC		17
#define SLAVE_LPASS_SNOC		18
#define SLAVE_ANOC_PCIE_SNOC		19
#define SLAVE_SERVICE_A2NOC		20

#define MASTER_CAMNOC_HF0_UNCOMP	1
#define MASTER_CAMNOC_HF1_UNCOMP	2
#define MASTER_CAMNOC_SF_UNCOMP		3
#define SLAVE_CAMNOC_UNCOMP		4

#define MASTER_SPDM			1
#define MASTER_SNOC_CNOC		2
#define MASTER_QDSS_DAP			3
#define SLAVE_A1NOC_CFG			4
#define SLAVE_AHB2PHY_EAST		5
#define SLAVE_AHB2PHY_WEST		6
#define SLAVE_AOP			7
#define SLAVE_AOSS			8
#define SLAVE_CAMERA_CFG		9
#define SLAVE_CLK_CTL			10
#define SLAVE_RBCPR_CX_CFG		11
#define SLAVE_RBCPR_MX_CFG		12
#define SLAVE_CRYPTO_0_CFG		13
#define SLAVE_CNOC_DDRSS		14
#define SLAVE_DISPLAY_CFG		15
#define SLAVE_EMAC_AVB_CFG		16
#define SLAVE_GLM			17
#define SLAVE_GFX3D_CFG			18
#define SLAVE_IMEM_CFG			19
#define SLAVE_IPA_CFG			20
#define SLAVE_CNOC_MNOC_CFG		21
#define SLAVE_PCIE_CFG			22
#define SLAVE_PIMEM_CFG			23
#define SLAVE_PRNG			24
#define SLAVE_QDSS_CFG			25
#define SLAVE_QSPI			26
#define SLAVE_QUP_0			27
#define SLAVE_QUP_1			28
#define SLAVE_SDCC_1			29
#define SLAVE_SDCC_2			30
#define SLAVE_SNOC_CFG			31
#define SLAVE_SPDM_WRAPPER		32
#define SLAVE_TCSR			33
#define SLAVE_TLMM_EAST			34
#define SLAVE_TLMM_SOUTH		35
#define SLAVE_TLMM_WEST			36
#define SLAVE_UFS_MEM_CFG		37
#define SLAVE_USB2			38
#define SLAVE_USB3			39
#define SLAVE_VENUS_CFG			40
#define SLAVE_VSENSE_CTRL_CFG		41
#define SLAVE_CNOC_A2NOC		42
#define SLAVE_SERVICE_CNOC		43

#define MASTER_CNOC_DC_NOC		1
#define SLAVE_DC_NOC_GEMNOC		2
#define SLAVE_LLCC_CFG			3

#define MASTER_APPSS_PROC		1
#define MASTER_GPU_TCU			2
#define MASTER_SYS_TCU			3
#define MASTER_GEM_NOC_CFG		4
#define MASTER_GFX3D			5
#define MASTER_MNOC_HF_MEM_NOC		6
#define MASTER_MNOC_SF_MEM_NOC		7
#define MASTER_SNOC_GC_MEM_NOC		8
#define MASTER_SNOC_SF_MEM_NOC		9
#define SLAVE_MSS_PROC_MS_MPU_CFG	10
#define SLAVE_GEM_NOC_SNOC		11
#define SLAVE_LLCC			12
#define SLAVE_MEM_NOC_PCIE_SNOC		13
#define SLAVE_SERVICE_GEM_NOC		14

#define MASTER_IPA_CORE			1
#define SLAVE_IPA_CORE			2

#define MASTER_LLCC			1
#define SLAVE_EBI1			2

#define MASTER_CNOC_MNOC_CFG		1
#define MASTER_CAMNOC_HF0		2
#define MASTER_CAMNOC_HF1		3
#define MASTER_CAMNOC_SF		4
#define MASTER_MDP0			5
#define MASTER_ROTATOR			6
#define MASTER_VIDEO_P0			7
#define MASTER_VIDEO_PROC		8
#define SLAVE_MNOC_SF_MEM_NOC		9
#define SLAVE_MNOC_HF_MEM_NOC		10
#define SLAVE_SERVICE_MNOC		11

#define MASTER_SNOC_CFG			1
#define MASTER_A1NOC_SNOC		2
#define MASTER_GEM_NOC_SNOC		3
#define MASTER_GEM_NOC_PCIE_SNOC	4
#define MASTER_LPASS_ANOC		5
#define MASTER_ANOC_PCIE_SNOC		6
#define MASTER_PIMEM			7
#define MASTER_GIC			8
#define SLAVE_APPSS			9
#define SLAVE_SNOC_CNOC			10
#define SLAVE_SNOC_GEM_NOC_SF		11
#define SLAVE_SNOC_MEM_NOC_GC		12
#define SLAVE_IMEM			13
#define SLAVE_PIMEM			14
#define SLAVE_SERVICE_SNOC		15
#define SLAVE_PCIE_0			16
#define SLAVE_QDSS_STM			17
#define SLAVE_TCU			18

#endif

