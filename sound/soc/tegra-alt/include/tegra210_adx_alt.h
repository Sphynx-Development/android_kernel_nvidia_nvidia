/*
 * tegra210_adx_alt.h - Definitions for Tegra210 ADX driver
 *
 * Copyright (c) 2014-2019, NVIDIA CORPORATION.  All rights reserved.
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

#ifndef __TEGRA210_ADX_ALT_H__
#define __TEGRA210_ADX_ALT_H__

#define TEGRA210_ADX_AUDIOCIF_CH_STRIDE 4

#define TEGRA210_ADX_AUDIOCIF_CH_STRIDE 4

/* Register offsets from TEGRA210_ADX*_BASE */
#define TEGRA210_ADX_AXBAR_RX_STATUS		0x0c
#define TEGRA210_ADX_AXBAR_RX_INT_STATUS	0x10
#define TEGRA210_ADX_AXBAR_RX_INT_MASK		0x14
#define TEGRA210_ADX_AXBAR_RX_INT_SET		0x18
#define TEGRA210_ADX_AXBAR_RX_INT_CLEAR		0x1c
#define TEGRA210_ADX_AXBAR_RX_CIF_CTRL		0x20
#define TEGRA210_ADX_AXBAR_TX_STATUS		0x4c
#define TEGRA210_ADX_AXBAR_TX_INT_STATUS	0x50
#define TEGRA210_ADX_AXBAR_TX_INT_MASK		0x54
#define TEGRA210_ADX_AXBAR_TX_INT_SET		0x58
#define TEGRA210_ADX_AXBAR_TX_INT_CLEAR		0x5c
#define TEGRA210_ADX_AXBAR_TX1_CIF_CTRL		0x60
#define TEGRA210_ADX_AXBAR_TX2_CIF_CTRL		0x64
#define TEGRA210_ADX_AXBAR_TX3_CIF_CTRL		0x68
#define TEGRA210_ADX_AXBAR_TX4_CIF_CTRL		0x6c
#define TEGRA210_ADX_ENABLE					0x80
#define TEGRA210_ADX_SOFT_RESET				0x84
#define TEGRA210_ADX_CG						0x88
#define TEGRA210_ADX_STATUS					0x8c
#define TEGRA210_ADX_INT_STATUS				0x90
#define TEGRA210_ADX_CTRL					0xa4
#define TEGRA210_ADX_IN_BYTE_EN0			0xa8
#define TEGRA210_ADX_IN_BYTE_EN1			0xac
#define TEGRA210_ADX_CYA					0xb0
#define TEGRA210_ADX_DBG					0xb4
#define TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL	0xb8
#define TEGRA210_ADX_AHUBRAMCTL_ADX_DATA	0xbc


/* Fields in TEGRA210_ADX_AXBAR_RX_CIF_CTRL */
/* Uses field from TEGRA210_AUDIOCIF_CTRL_* in tegra210_xbar_alt.h */

/* Fields in TEGRA210_ADX_AXBAR_TX1_CIF_CTRL */
/* Uses field from TEGRA210_AUDIOCIF_CTRL_* in tegra210_xbar_alt.h */

/* Fields in TEGRA210_ADX_AXBAR_TX2_CIF_CTRL */
/* Uses field from TEGRA210_AUDIOCIF_CTRL_* in tegra210_xbar_alt.h */

/* Fields in TEGRA210_ADX_AXBAR_TX3_CIF_CTRL */
/* Uses field from TEGRA210_AUDIOCIF_CTRL_* in tegra210_xbar_alt.h */

/* Fields in TEGRA210_ADX_AXBAR_TX_CIF_CTRL */
/* Uses field from TEGRA210_AUDIOCIF_CTRL_* in tegra210_xbar_alt.h */

/* Fields in TEGRA210_ADX_ENABLE */
#define TEGRA210_ADX_ENABLE_SHIFT					0
#define TEGRA210_ADX_ENABLE_MASK					(1 << TEGRA210_ADX_ENABLE_SHIFT)
#define TEGRA210_ADX_EN								(1 << TEGRA210_ADX_ENABLE_SHIFT)

/* Fields inTEGRA210_ADX_CTRL */
#define TEGRA210_ADX_CTRL_TX4_FORCE_DISABLE_SHIFT	11
#define TEGRA210_ADX_CTRL_TX4_FORCE_DISABLE_MASK	(1 << TEGRA210_ADX_CTRL_TX4_FORCE_DISABLE_SHIFT)
#define TEGRA210_ADX_CTRL_TX4_FORCE_DISABLE_EN		(1 << TEGRA210_ADX_CTRL_TX4_FORCE_DISABLE_SHIFT)

#define TEGRA210_ADX_CTRL_TX3_FORCE_DISABLE_SHIFT	10
#define TEGRA210_ADX_CTRL_TX3_FORCE_DISABLE_MASK	(1 << TEGRA210_ADX_CTRL_TX3_FORCE_DISABLE_SHIFT)
#define TEGRA210_ADX_CTRL_TX3_FORCE_DISABLE_EN		(1 << TEGRA210_ADX_CTRL_TX3_FORCE_DISABLE_SHIFT)

#define TEGRA210_ADX_CTRL_TX2_FORCE_DISABLE_SHIFT	9
#define TEGRA210_ADX_CTRL_TX2_FORCE_DISABLE_MASK	(1 << TEGRA210_ADX_CTRL_TX2_FORCE_DISABLE_SHIFT)
#define TEGRA210_ADX_CTRL_TX2_FORCE_DISABLE_EN		(1 << TEGRA210_ADX_CTRL_TX2_FORCE_DISABLE_SHIFT)

#define TEGRA210_ADX_CTRL_TX1_FORCE_DISABLE_SHIFT	8
#define TEGRA210_ADX_CTRL_TX1_FORCE_DISABLE_MASK	(1 << TEGRA210_ADX_CTRL_TX1_FORCE_DISABLE_SHIFT)
#define TEGRA210_ADX_CTRL_TX1_FORCE_DISABLE_EN		(1 << TEGRA210_ADX_CTRL_TX1_FORCE_DISABLE_SHIFT)

#define TEGRA210_ADX_CTRL_TX4_ENABLE_SHIFT			3
#define TEGRA210_ADX_CTRL_TX4_ENABLE_MASK			(1 << TEGRA210_ADX_CTRL_TX4_ENABLE_SHIFT)
#define TEGRA210_ADX_CTRL_TX4_EN					(1 << TEGRA210_ADX_CTRL_TX4_ENABLE_SHIFT)

#define TEGRA210_ADX_CTRL_TX3_ENABLE_SHIFT			2
#define TEGRA210_ADX_CTRL_TX3_ENABLE_MASK			(1 << TEGRA210_ADX_CTRL_TX3_ENABLE_SHIFT)
#define TEGRA210_ADX_CTRL_TX3_EN					(1 << TEGRA210_ADX_CTRL_TX3_ENABLE_SHIFT)

#define TEGRA210_ADX_CTRL_TX2_ENABLE_SHIFT			1
#define TEGRA210_ADX_CTRL_TX2_ENABLE_MASK			(1 << TEGRA210_ADX_CTRL_TX2_ENABLE_SHIFT)
#define TEGRA210_ADX_CTRL_TX2_EN					(1 << TEGRA210_ADX_CTRL_TX2_ENABLE_SHIFT)

#define TEGRA210_ADX_CTRL_TX1_ENABLE_SHIFT			0
#define TEGRA210_ADX_CTRL_TX1_ENABLE_MASK			(1 << TEGRA210_ADX_CTRL_TX1_ENABLE_SHIFT)
#define TEGRA210_ADX_CTRL_TX1_EN					(1 << TEGRA210_ADX_CTRL_TX1_ENABLE_SHIFT)

/* Fields in TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL */
#define TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_READ_BUSY_SHIFT		31
#define TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_READ_BUSY_MASK			(1 << TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_READ_BUSY_SHIFT)
#define TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_READ_BUSY				(1 << TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_READ_BUSY_SHIFT)

#define TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_SEQ_READ_COUNT_SHIFT	16
#define TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_SEQ_READ_COUNT_MASK	(0xff << TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_SEQ_READ_COUNT_SHIFT)

#define TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_RW_SHIFT				14
#define TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_RW_MASK				(1 << TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_RW_SHIFT)
#define TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_RW_WRITE				(1 << TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_RW_SHIFT)

#define TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_ADDR_INIT_EN_SHIFT		13
#define TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_ADDR_INIT_EN_MASK		(1 << TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_ADDR_INIT_EN_SHIFT)
#define TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_ADDR_INIT_EN			(1 << TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_ADDR_INIT_EN_SHIFT)

#define TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_SEQ_ACCESS_EN_SHIFT	12
#define TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_SEQ_ACCESS_EN_MASK		(1 << TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_SEQ_ACCESS_EN_SHIFT)
#define TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_SEQ_ACCESS_EN			(1 << TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_SEQ_ACCESS_EN_SHIFT)

#define TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_RAM_ADDR_SHIFT			0
#define TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_RAM_ADDR_MASK			(0xff << TEGRA210_ADX_AHUBRAMCTL_ADX_CTRL_RAM_ADDR_SHIFT)

/* Fields in TEGRA210_ADX_SOFT_RESET */
#define TEGRA210_ADX_SOFT_RESET_SOFT_RESET_SHIFT		0
#define TEGRA210_ADX_SOFT_RESET_SOFT_RESET_MASK			(1 << TEGRA210_ADX_SOFT_RESET_SOFT_RESET_SHIFT)
#define TEGRA210_ADX_SOFT_RESET_SOFT_EN			(1 << TEGRA210_ADX_SOFT_RESET_SOFT_RESET_SHIFT)
#define TEGRA210_ADX_SOFT_RESET_SOFT_DEFAULT			(0 << TEGRA210_ADX_SOFT_RESET_SOFT_RESET_SHIFT)

/*
 * Those defines are not in register field.
 */
#define TEGRA210_ADX_NUM_OUTPUTS		4
#define TEGRA210_ADX_RAM_DEPTH					16
#define TEGRA210_ADX_MAP_STREAM_NUMBER_SHIFT	6
#define TEGRA210_ADX_MAP_WORD_NUMBER_SHIFT		2
#define TEGRA210_ADX_MAP_BYTE_NUMBER_SHIFT		0

enum {
	TEGRA210_ADX_TX_DISABLE,
	TEGRA210_ADX_TX_ENABLE,
};

enum {
	/* Code assumes that OUT_STREAM values of ADX start at 0 */
	/* OUT_STREAM# is equilvant to hw OUT_CH# */
	TEGRA210_ADX_OUT_STREAM0 = 0,
	TEGRA210_ADX_OUT_STREAM1,
	TEGRA210_ADX_OUT_STREAM2,
	TEGRA210_ADX_OUT_STREAM3,
	TEGRA210_ADX_IN_STREAM,
	TEGRA210_ADX_TOTAL_STREAM
};

struct tegra210_adx {
	struct regmap *regmap;
	unsigned int map[TEGRA210_ADX_RAM_DEPTH];
	unsigned int byte_mask[2];
	int input_channels;
	int output_channels[TEGRA210_ADX_NUM_OUTPUTS];
	bool is_shutdown;
};

#endif
