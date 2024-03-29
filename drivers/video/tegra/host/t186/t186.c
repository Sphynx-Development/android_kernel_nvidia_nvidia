/*
 * Tegra Graphics Init for T186 Architecture Chips
 *
 * Copyright (c) 2014-2021, NVIDIA Corporation. All rights reserved.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms and conditions of the GNU General Public License,
 * version 2, as published by the Free Software Foundation.
 *
 * This program is distributed in the hope it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <linux/slab.h>
#include <linux/io.h>
#include <linux/version.h>
#include <linux/platform/tegra/emc_bwmgr.h>

#include "dev.h"
#include "class_ids.h"
#include "class_ids_t186.h"

#include "t186.h"
#include "host1x/host1x.h"
#include "tsec/tsec.h"
#include "flcn/flcn.h"
#include "isp/isp.h"
#include "isp/isp_isr_v2.h"
#if IS_ENABLED(CONFIG_TEGRA_GRHOST_NVCSI)
#include "nvcsi/nvcsi.h"
#endif
#include <video/vi4.h>
#include "nvdec/nvdec.h"
#include "hardware_t186.h"

#include "nvhost_scale.h"
#include "scale_emc.h"

#include "chip_support.h"

#include "streamid_regs.c"
#include "cg_regs.c"
#include "actmon_regs.c"

#define HOST_EMC_FLOOR 204000000
#define HOST_NVDEC_EMC_FLOOR 102000000

static struct host1x_device_info host1x04_info = {
	.nb_channels	= T186_NVHOST_NUMCHANNELS,
	.ch_base	= 0,
	.ch_limit	= T186_NVHOST_NUMCHANNELS,
	.nb_mlocks	= NV_HOST1X_NB_MLOCKS,
	.initialize_chip_support = nvhost_init_t186_support,
	.nb_hw_pts	= NV_HOST1X_SYNCPT_NB_PTS,
	.nb_pts		= NV_HOST1X_SYNCPT_NB_PTS,
	.pts_base	= 0,
	.pts_limit	= NV_HOST1X_SYNCPT_NB_PTS,
	.nb_syncpt_irqs	= 1,
	.syncpt_policy	= SYNCPT_PER_CHANNEL_INSTANCE,
	.channel_policy	= MAP_CHANNEL_ON_SUBMIT,
	.firmware_area_size = SZ_1M,
	.nb_actmons = 1,
};

struct nvhost_device_data t18_host1x_info = {
	.clocks			= {
		{"host1x", 102000000},
		{"actmon", UINT_MAX}
	},
	.can_powergate          = false,
	.autosuspend_delay      = 50,
	.private_data		= &host1x04_info,
	.finalize_poweron = nvhost_host1x_finalize_poweron,
	.prepare_poweroff = nvhost_host1x_prepare_poweroff,
	.isolate_contexts	= true,
};

struct nvhost_device_data t18_host1x_hv_info = {
	.clocks			= {
		{"host1x", 102000000},
		{"actmon", UINT_MAX}
	},
	.can_powergate          = false,
	.autosuspend_delay      = 2000,
	.private_data		= &host1x04_info,
	.finalize_poweron = nvhost_host1x_finalize_poweron,
	.prepare_poweroff = nvhost_host1x_prepare_poweroff,
};

static struct host1x_device_info host1xb04_info = {
	.nb_channels	= T186_NVHOST_NUMCHANNELS,
	.ch_base	= 0,
	.ch_limit	= T186_NVHOST_NUMCHANNELS,
	.nb_mlocks	= NV_HOST1X_NB_MLOCKS,
	.initialize_chip_support = nvhost_init_t186_support,
	.nb_hw_pts	= NV_HOST1X_SYNCPT_NB_PTS,
	.nb_pts		= NV_HOST1X_SYNCPT_NB_PTS,
	.pts_base	= 0,
	.pts_limit	= NV_HOST1X_SYNCPT_NB_PTS,
	.nb_syncpt_irqs	= 1,
	.syncpt_policy	= SYNCPT_PER_CHANNEL_INSTANCE,
	.channel_policy	= MAP_CHANNEL_ON_SUBMIT,
	.firmware_area_size = SZ_1M,
	.ctrl_name	= "ctrlb",
};

struct nvhost_device_data t18_host1xb_info = {
	.clocks			= {
		{"host1x", UINT_MAX},
		{"actmon", UINT_MAX}
	},
	.private_data		= &host1xb04_info,
};

#ifdef CONFIG_TEGRA_GRHOST_ISP
struct nvhost_device_data t18_isp_info = {
	.num_channels		= 1,
	.moduleid		= NVHOST_MODULE_ISP,
	.class			= NV_VIDEO_STREAMING_ISP_CLASS_ID,
	.modulemutexes		= {NV_HOST1X_MLOCK_ID_ISP},
	.devfs_name		= "isp",
	/* HACK: Mark as keepalive until 1188795 is fixed */
	.keepalive		= true,
	.can_powergate          = true,
	.autosuspend_delay      = 500,
	.poweron_reset		= true,
	.clocks			= {
		{"isp", 768000000},
	},
	.finalize_poweron	= nvhost_isp_finalize_poweron,
	.prepare_poweroff	= nvhost_isp_prepare_poweroff,
	.hw_init		= nvhost_isp_register_isr_v2,
	.ctrl_ops		= &tegra_isp_ctrl_ops,
	.resource_policy	= RESOURCE_PER_CHANNEL_INSTANCE,
	.serialize		= 1,
	.push_work_done		= 1,
	.vm_regs		= {{0x50, true} },
	.mlock_timeout_factor	= 10,
};
#endif

#if IS_ENABLED(CONFIG_VIDEO_TEGRA_VI) || IS_ENABLED(CONFIG_VIDEO_TEGRA_VI_MODULE)
struct nvhost_device_data t18_vi_info = {
	.devfs_name		= "vi",
	.exclusive		= true,
	.class			= NV_VIDEO_STREAMING_VI_CLASS_ID,
	.modulemutexes		= {NV_HOST1X_MLOCK_ID_VI},
	/* HACK: Mark as keepalive until 1188795 is fixed */
	.keepalive		= true,
	.can_powergate          = true,
	.autosuspend_delay      = 500,
	.poweron_reset		= true,
	.support_abort_on_close	= true,
	.moduleid		= NVHOST_MODULE_VI,
	.clocks = {
		{"vi", 408000000},
		{"nvcsi", 204000000, 0, 0, true},
		{"nvcsilp", 204000000, 0, 0, true},
	},
	.num_channels		= 15,
	.prepare_poweroff	= nvhost_vi4_prepare_poweroff,
	.finalize_poweron	= nvhost_vi4_finalize_poweron,
	.busy			= nvhost_vi4_busy,
	.idle			= nvhost_vi4_idle,
	.reset			= nvhost_vi4_reset,
	.vm_regs		= {{0x4000 * 4, true},
				   {0x8000 * 4, true},
				   {0xc000 * 4, true},
				   {0x10000 * 4, true},
				   {0x14000 * 4, true},
				   {0x18000 * 4, true},
				   {0x1c000 * 4, true},
				   {0x20000 * 4, true},
				   {0x24000 * 4, true},
				   {0x28000 * 4, true},
				   {0x2c000 * 4, true},
				   {0x30000 * 4, true} },
	.num_ppc			= 8,
	.aggregate_constraints	= nvhost_vi4_aggregate_constraints,
};
#endif

#if IS_ENABLED(CONFIG_TEGRA_GRHOST_NVENC)
struct nvhost_device_data t18_msenc_info = {
	.version		= NVHOST_ENCODE_FLCN_VER(6, 1),
	.devfs_name		= "msenc",
	.class			= NV_VIDEO_ENCODE_NVENC_CLASS_ID,
	.modulemutexes		= {NV_HOST1X_MLOCK_ID_NVENC},
	.can_powergate          = true,
	.autosuspend_delay      = 500,
	.clocks			= {
		{"nvenc", UINT_MAX, 0},
		{"emc", HOST_EMC_FLOOR,
		 NVHOST_MODULE_ID_EXTERNAL_MEMORY_CONTROLLER,
		 TEGRA_SET_EMC_SHARED_BW}
	},
	.engine_cg_regs		= t18x_nvenc_gating_registers,
	.engine_can_cg		= true,
	.poweron_reset		= true,
	.finalize_poweron	= nvhost_flcn_finalize_poweron_t186,
	.moduleid		= NVHOST_MODULE_MSENC,
	.num_channels		= 1,
	.firmware_name		= "nvhost_nvenc061.fw",
	.serialize		= 1,
	.push_work_done		= 1,
	.resource_policy	= RESOURCE_PER_CHANNEL_INSTANCE,
	.vm_regs		= {{0x30, true}, {0x34, false} },
	.transcfg_addr		= 0x1844,
	.transcfg_val		= 0x20,
	.bwmgr_client_id	= TEGRA_BWMGR_CLIENT_MSENC,
	.isolate_contexts	= true,
	.mlock_timeout_factor	= 3,
};
#endif

#if IS_ENABLED(CONFIG_TEGRA_GRHOST_NVDEC)
struct nvhost_device_data t18_nvdec_info = {
	.version		= NVHOST_ENCODE_NVDEC_VER(3, 0),
	.devfs_name		= "nvdec",
	.modulemutexes		= {NV_HOST1X_MLOCK_ID_NVDEC},
	.class			= NV_NVDEC_CLASS_ID,
	.can_powergate          = true,
	.autosuspend_delay      = 500,
	.clocks			= {
		{"nvdec", UINT_MAX, 0},
		{"emc", HOST_NVDEC_EMC_FLOOR,
		 NVHOST_MODULE_ID_EXTERNAL_MEMORY_CONTROLLER,
		 TEGRA_SET_EMC_FLOOR}
	},
	.engine_cg_regs		= t18x_nvdec_gating_registers,
	.engine_can_cg		= true,
	.poweron_reset		= true,
	.finalize_poweron	= nvhost_nvdec_finalize_poweron_t186,
	.prepare_poweroff	= nvhost_nvdec_prepare_poweroff_t186,
	.moduleid		= NVHOST_MODULE_NVDEC,
	.ctrl_ops		= &tegra_nvdec_ctrl_ops,
	.num_channels		= 1,
	.serialize		= 1,
	.push_work_done		= 1,
	.resource_policy	= RESOURCE_PER_CHANNEL_INSTANCE,
	.vm_regs		= {{0x30, true}, {0x34, false} },
	.transcfg_addr		= 0x2c44,
	.transcfg_val		= 0x20,
	.bwmgr_client_id	= TEGRA_BWMGR_CLIENT_NVDEC,
	.isolate_contexts	= true,
	.mlock_timeout_factor	= 3,
};
#endif

#if IS_ENABLED(CONFIG_TEGRA_GRHOST_NVJPG)
struct nvhost_device_data t18_nvjpg_info = {
	.version		= NVHOST_ENCODE_FLCN_VER(1, 1),
	.devfs_name		= "nvjpg",
	.modulemutexes		= {NV_HOST1X_MLOCK_ID_NVJPG},
	.class			= NV_NVJPG_CLASS_ID,
	.can_powergate          = true,
	.autosuspend_delay      = 500,
	.clocks			= {
		{"nvjpg", UINT_MAX, 0},
		{"emc", HOST_EMC_FLOOR,
		 NVHOST_MODULE_ID_EXTERNAL_MEMORY_CONTROLLER,
		 TEGRA_SET_EMC_SHARED_BW}
	},
	.engine_cg_regs		= t18x_nvjpg_gating_registers,
	.engine_can_cg		= true,
	.poweron_reset		= true,
	.finalize_poweron	= nvhost_flcn_finalize_poweron_t186,
	.moduleid		= NVHOST_MODULE_NVJPG,
	.num_channels		= 1,
	.firmware_name		= "nvhost_nvjpg011.fw",
	.serialize		= 1,
	.push_work_done		= 1,
	.resource_policy	= RESOURCE_PER_CHANNEL_INSTANCE,
	.vm_regs		= {{0x30, true}, {0x34, false} },
	.transcfg_addr		= 0x1444,
	.transcfg_val		= 0x20,
	.bwmgr_client_id	= TEGRA_BWMGR_CLIENT_NVJPG,
	.isolate_contexts	= true,
	.mlock_timeout_factor	= 3,
	.module_irq		= 14,
};
#endif

#if IS_ENABLED(CONFIG_TEGRA_GRHOST_TSEC)
struct nvhost_device_data t18_tsec_info = {
	.num_channels		= 1,
	.devfs_name		= "tsec",
	.version		= NVHOST_ENCODE_TSEC_VER(1, 0),
	.modulemutexes		= {NV_HOST1X_MLOCK_ID_TSEC},
	.class			= NV_TSEC_CLASS_ID,
	.clocks			= {
		{"tsec", UINT_MAX, 0},
		{"emc", HOST_EMC_FLOOR,
		 NVHOST_MODULE_ID_EXTERNAL_MEMORY_CONTROLLER,
		 TEGRA_SET_EMC_FLOOR}
	},
	.engine_cg_regs		= t18x_tsec_gating_registers,
	.engine_can_cg		= true,
	.can_powergate          = true,
	.autosuspend_delay      = 500,
	.keepalive		= true,
	.moduleid		= NVHOST_MODULE_TSEC,
	.poweron_reset		= true,
	.finalize_poweron	= nvhost_tsec_finalize_poweron_t186,
	.prepare_poweroff	= nvhost_tsec_prepare_poweroff,
	.serialize		= 1,
	.push_work_done		= 1,
	.resource_policy	= RESOURCE_PER_CHANNEL_INSTANCE,
	.vm_regs		= {{0x30, true}, {0x34, false} },
	.transcfg_addr		= 0x1644,
	.transcfg_val		= 0x20,
	.bwmgr_client_id	= TEGRA_BWMGR_CLIENT_TSEC,
	.mlock_timeout_factor	= 3,
};

struct nvhost_device_data t18_tsecb_info = {
	.num_channels		= 1,
	.devfs_name		= "tsecb",
	.version		= NVHOST_ENCODE_TSEC_VER(1, 0),
	.modulemutexes		= {NV_HOST1X_MLOCK_ID_TSECB},
	.class			= NV_TSECB_CLASS_ID,
	.clocks			= {
		{"tsecb", UINT_MAX, 0},
		{"emc", HOST_EMC_FLOOR,
		 NVHOST_MODULE_ID_EXTERNAL_MEMORY_CONTROLLER,
		 TEGRA_SET_EMC_FLOOR}
	},
	.engine_cg_regs		= t18x_tsec_gating_registers,
	.engine_can_cg		= true,
	.can_powergate          = true,
	.autosuspend_delay      = 500,
	.keepalive		= true,
	.moduleid		= NVHOST_MODULE_TSECB,
	.poweron_reset		= true,
	.finalize_poweron	= nvhost_tsec_finalize_poweron_t186,
	.prepare_poweroff	= nvhost_tsec_prepare_poweroff,
	.serialize		= 1,
	.push_work_done		= 1,
	.resource_policy	= RESOURCE_PER_CHANNEL_INSTANCE,
	.vm_regs		= {{0x30, true}, {0x34, false} },
	.transcfg_addr		= 0x1644,
	.transcfg_val		= 0x20,
	.bwmgr_client_id	= TEGRA_BWMGR_CLIENT_TSECB,
	.mlock_timeout_factor	= 3,
};
#endif

#if IS_ENABLED(CONFIG_TEGRA_GRHOST_VIC)
struct nvhost_device_data t18_vic_info = {
	.num_channels		= 1,
	.devfs_name		= "vic",
	.clocks			= {
		{"vic", UINT_MAX, 0},
		{"emc", HOST_EMC_FLOOR,
		 NVHOST_MODULE_ID_EXTERNAL_MEMORY_CONTROLLER,
		 TEGRA_SET_EMC_SHARED_BW},
	},
	.engine_cg_regs		= t18x_vic_gating_registers,
	.engine_can_cg		= true,
	.version		= NVHOST_ENCODE_FLCN_VER(4, 0),
	.can_powergate          = true,
	.autosuspend_delay      = 500,
	.moduleid		= NVHOST_MODULE_VIC,
	.poweron_reset		= true,
	.modulemutexes		= {NV_HOST1X_MLOCK_ID_VIC},
	.class			= NV_GRAPHICS_VIC_CLASS_ID,
	.finalize_poweron	= nvhost_flcn_finalize_poweron_t186,
	.prepare_poweroff	= nvhost_flcn_prepare_poweroff,
	.flcn_isr		= nvhost_flcn_common_isr,
	.init_class_context	= nvhost_vic_init_context,
	.firmware_name		= "vic04_ucode.bin",
	.serialize		= 1,
	.push_work_done		= 1,
	.resource_policy	= RESOURCE_PER_CHANNEL_INSTANCE,
	.vm_regs		= {{0x30, true}, {0x34, false} },
	.transcfg_addr		= 0x2044,
	.transcfg_val		= 0x20,
	.bwmgr_client_id	= TEGRA_BWMGR_CLIENT_VIC,
	.scaling_init		= nvhost_scale_emc_init,
	.scaling_deinit		= nvhost_scale_emc_deinit,
	.scaling_post_cb	= &nvhost_scale_emc_callback,
	.actmon_regs		= HOST1X_THOST_ACTMON_VIC,
	.actmon_enabled         = true,
	.actmon_irq		= 3,
	.actmon_weight_count	= 213,
	.actmon_setting_regs	= t18x_vic_actmon_registers,
	.devfreq_governor	= "userspace",
	.isolate_contexts	= true,
	.mlock_timeout_factor	= 3,
	.module_irq		= 1,
};
#endif

#if IS_ENABLED(CONFIG_TEGRA_GRHOST_NVCSI)
struct nvhost_device_data t18_nvcsi_info = {
	.num_channels		= 1,
	.clocks			= {
		{"nvcsi", 204000000},
		{"nvcsilp", 204000000},
	},
	.devfs_name		= "nvcsi",
	.modulemutexes		= {NV_HOST1X_MLOCK_ID_NVCSI},
	.class			= NV_VIDEO_STREAMING_NVCSI_CLASS_ID,
	.ctrl_ops		= &tegra_nvcsi_ctrl_ops,
	.can_powergate          = true,
	.autosuspend_delay      = 500,
	.finalize_poweron	= nvcsi_finalize_poweron,
	.prepare_poweroff	= nvcsi_prepare_poweroff,
	.poweron_reset		= true,
	.keepalive		= true,
	.serialize		= 1,
	.push_work_done		= 1,
};
#endif

#include "host1x/host1x_channel_t186.c"

static void t186_set_nvhost_chanops(struct nvhost_channel *ch)
{
	if (!ch)
		return;

	ch->ops = host1x_channel_ops;
}

int nvhost_init_t186_channel_support(struct nvhost_master *host,
				     struct nvhost_chip_support *op)
{
	op->nvhost_dev.set_nvhost_chanops = t186_set_nvhost_chanops;

	return 0;
}

static void t186_remove_support(struct nvhost_chip_support *op)
{
	kfree(op->priv);
	op->priv = NULL;
}

static void t186_init_regs(struct platform_device *pdev, bool prod)
{
	struct nvhost_gating_register *regs = t18x_host1x_gating_registers;
	struct nvhost_streamid_mapping *map_regs = t18x_host1x_streamid_mapping;

	if (nvhost_dev_is_virtual(pdev) == true) {
		return;
	}

	while (regs->addr) {
		if (prod)
			host1x_hypervisor_writel(pdev, regs->addr, regs->prod);
		else
			host1x_hypervisor_writel(pdev, regs->addr,
						 regs->disable);
		regs++;
	}

	while (map_regs->host1x_offset) {
		host1x_hypervisor_writel(pdev,
					 map_regs->host1x_offset,
					 map_regs->client_offset);
		host1x_hypervisor_writel(pdev,
					 map_regs->host1x_offset + sizeof(u32),
					 map_regs->client_limit);
		map_regs++;
	}
}

#include "host1x/host1x_cdma_t186.c"
#include "host1x/host1x_syncpt.c"
#include "host1x/host1x_syncpt_prot_t186.c"
#include "host1x/host1x_intr_t186.c"
#include "host1x/host1x_debug_t186.c"
#include "host1x/host1x_vm_t186.c"
#if IS_ENABLED(CONFIG_TEGRA_GRHOST_SCALE)
#include "host1x/host1x_actmon_t186.c"
#endif

int nvhost_init_t186_support(struct nvhost_master *host,
			     struct nvhost_chip_support *op)
{
	int err;

	op->soc_name = "tegra18x";

	/* don't worry about cleaning up on failure... "remove" does it. */
	err = nvhost_init_t186_channel_support(host, op);
	if (err)
		return err;

	op->cdma = host1x_cdma_ops;
	op->push_buffer = host1x_pushbuffer_ops;
	op->debug = host1x_debug_ops;
	op->nvhost_dev.load_gating_regs = t186_init_regs;

	host->sync_aperture = host->aperture;
	op->syncpt = host1x_syncpt_ops;
	op->intr = host1x_intr_ops;
	op->vm = host1x_vm_ops;
#if IS_ENABLED(CONFIG_TEGRA_GRHOST_SCALE)
	op->actmon = host1x_actmon_ops;
#endif

	/* WAR to bugs 200094901 and 200082771: enable protection
	 * only on silicon/emulation */

	op->syncpt.reset = t186_syncpt_reset;
	op->syncpt.mark_used = t186_syncpt_mark_used;
	op->syncpt.mark_unused = t186_syncpt_mark_unused;

	op->syncpt.mutex_owner = t186_syncpt_mutex_owner;

	op->remove_support = t186_remove_support;

	return 0;
}
