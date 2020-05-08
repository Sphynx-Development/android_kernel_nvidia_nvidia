/*
 * Copyright (c) 2016-2020, NVIDIA Corporation.  All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License.
 *
 *  This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

struct nvhost_streamid_mapping {
	u32 host1x_offset;
	u32 client_offset;
	u32 client_limit;
};

static struct nvhost_streamid_mapping __attribute__((__unused__))
	t23x_host1x_streamid_mapping[] = {

	/* host1x client streamid registers for fw ucode */

	/* HOST1X_THOST_COMMON_SE1_STRMID_0_OFFSET_BASE_0 */
	{ 0x00001650, 0x00000090, 0x00000090},
	/* HOST1X_THOST_COMMON_SE2_STRMID_0_OFFSET_BASE_0 */
	{ 0x00001658, 0x00000090, 0x00000090},
	/* HOST1X_THOST_COMMON_SE4_STRMID_0_OFFSET_BASE_0 */
	{ 0x00001660, 0x00000090, 0x00000090},
	/* HOST1X_THOST_COMMON_ISP_STRMID_0_OFFSET_BASE_0 */
	{ 0x00001680, 0x00000800, 0x00000800},
	/* HOST1X_THOST_COMMON_VIC_STRMID_0_OFFSET_BASE_0 */
	{ 0x00001688, 0x00000034, 0x00000034},
	/* HOST1X_THOST_COMMON_NVENC_STRMID_0_OFFSET_BASE_0 */
	{ 0x00001690, 0x00000034, 0x00000034},
	/* HOST1X_THOST_COMMON_NVDEC_STRMID_0_OFFSET_BASE_0 */
	{ 0x00001698, 0x00000034, 0x00000034},
	/* HOST1X_THOST_COMMON_NVJPG_STRMID_0_OFFSET_BASE_0 */
	{ 0x000016a0, 0x00000034, 0x00000034},
	/* HOST1X_THOST_COMMON_NVJPG1_STRMID_0_OFFSET_BASE_0 */
	{ 0x000016b0, 0x00000034, 0x00000034},
	/* HOST1X_THOST_COMMON_OFA_STRMID_0_OFFSET_BASE_0 */
	{ 0x000016e8, 0x00000034, 0x00000034},
	/* HOST1X_THOST_COMMON_TSEC_STRMID_0_OFFSET_BASE_0 */
	{ 0x000016a8, 0x00000034, 0x00000034},
	/* HOST1X_THOST_COMMON_VI_STRMID_0_OFFSET_BASE_0 */
	{ 0x000016b8, 0x00000800, 0x00000800},
	/* HOST1X_THOST_COMMON_VI_THI_STRMID_0_OFFSET_BASE_0 */
	{ 0x000016c0, 0x00000030, 0x00000034 },
	/* HOST1X_THOST_COMMON_ISP_THI_STRMID_0_OFFSET_BASE_0 */
	{ 0x000016c8, 0x00000030, 0x00000034 },
	/* HOST1X_THOST_COMMON_PVA0_CLUSTER_STRMID_0_OFFSET_BASE_0 */
	{ 0x000016d0, 0x00000000, 0x00000000 },
	/* HOST1X_THOST_COMMON_NVDLA0_STRMID_0_OFFSET_BASE_0 */
	{ 0x000016d8, 0x00000034, 0x00000034 },
	/* HOST1X_THOST_COMMON_NVDLA1_STRMID_0_OFFSET_BASE_0 */
	{ 0x000016e0, 0x00000034, 0x00000034 },
	/* HOST1X_THOST_COMMON_VI2_STRMID_0_OFFSET_BASE_0 */
	{ 0x000016f0, 0x00000800, 0x00000800},
	/* HOST1X_THOST_COMMON_VI2_THI_STRMID_0_OFFSET_BASE_0 */
	{ 0x00001708, 0x00000030, 0x00000034 },

	/* host1x_client ch streamid registers for data buffers */

	/* HOST1X_THOST_COMMON_SE1_CH_STRMID_0_OFFSET_BASE_0 */
	{ 0x00001740, 0x00000090, 0x00000090 },
	/* HOST1X_THOST_COMMON_SE2_CH_STRMID_0_OFFSET_BASE_0 */
	{ 0x00001748, 0x00000090, 0x00000090 },
	/* HOST1X_THOST_COMMON_SE4_CH_STRMID_0_OFFSET_BASE_0 */
	{ 0x00001750, 0x00000090, 0x00000090 },
	/* HOST1X_THOST_COMMON_VIC_CH_STRMID_0_OFFSET_BASE_0 */
	{ 0x000017c8, 0x00000030, 0x00000030 },
	/* HOST1X_THOST_COMMON_NVENC_CH_STRMID_0_OFFSET_BASE_0 */
	{ 0x000017d0, 0x00000030, 0x00000030 },
	/* HOST1X_THOST_COMMON_NVDEC_CH_STRMID_0_OFFSET_BASE_0 */
	{ 0x000017d8, 0x00000030, 0x00000030 },
	/* HOST1X_THOST_COMMON_NVJPG_CH_STRMID_0_OFFSET_BASE_0 */
	{ 0x000017e0, 0x00000030, 0x00000030 },
	/* HOST1X_THOST_COMMON_NVJPG1_CH_STRMID_0_OFFSET_BASE_0 */
	{ 0x000017b8, 0x00000030, 0x00000030 },
	/* HOST1X_THOST_COMMON_OFA_CH_STRMID_0_OFFSET_BASE_0 */
	{ 0x00001778, 0x00000030, 0x00000030 },
	/* HOST1X_THOST_COMMON_TSEC_CH_STRMID_0_OFFSET_BASE_0 */
	{ 0x000017e8, 0x00000030, 0x00000030 },
	/* HOST1X_THOST_COMMON_PVA0_CLUSTER_CH_STRMID_0_OFFSET_BASE_0 */
	{ 0x00001798, 0x00000000, 0x00000000 },
	/* HOST1X_THOST_COMMON_NVDLA0_CH_STRMID_0_OFFSET_BASE_0 */
	{ 0x000017f0, 0x00000030, 0x00000030 },
	/* HOST1X_THOST_COMMON_NVDLA1_CH_STRMID_0_OFFSET_BASE_0 */
	{ 0x000017f8, 0x00000030, 0x00000030 },
	{}
};
