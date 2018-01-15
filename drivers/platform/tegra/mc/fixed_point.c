/*
 * Copyright (C) 2017-2018, NVIDIA CORPORATION. All rights reserved.
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#include <asm/bug.h>
#include "fixed_point.h"

struct fixed_point fixed_point_init(
	unsigned int int_part,
	unsigned int frac_part,
	unsigned int int_prec,
	unsigned int frac_prec)
{
	struct fixed_point ret_fp;

	if (int_prec > (sizeof(unsigned int) * 8))
		BUG_ON(1); /* precision too large */
	else if (int_prec == (sizeof(unsigned int) * 8))
		ret_fp.int_mask = ((unsigned int)(-1));
	else
		ret_fp.int_mask = (1 << int_prec) - 1;

	if (frac_prec > (sizeof(unsigned int) * 8))
		BUG_ON(1); /* precision too large */
	else if (frac_prec == (sizeof(unsigned int) * 8))
		ret_fp.frac_mask = ((unsigned int)(-1));
	else
		ret_fp.frac_mask = (1 << frac_prec) - 1;

	ret_fp.int_part = int_part & ret_fp.int_mask;
	ret_fp.frac_part = frac_part & ret_fp.frac_mask;
	ret_fp.int_prec = int_prec;
	ret_fp.frac_prec = frac_prec;

	return ret_fp;
}

struct fixed_point fixed_point_shift_left(
	struct fixed_point fp_arg,
	unsigned int places)
{
	struct fixed_point ret_fp;
	unsigned int frac_mask;
	unsigned int frac_shift;

	if (places == 0)
		return fp_arg;

	ret_fp = fixed_point_init(
		0,
		0,
		fp_arg.int_prec,
		fp_arg.frac_prec);

	frac_mask = (places >= fp_arg.frac_prec) ? 0 :
		((((1 << places) - 1) << (fp_arg.frac_prec - places)) &
			fp_arg.frac_mask);
	frac_shift = (places >= fp_arg.frac_prec) ? 0 :
		(fp_arg.frac_prec - places);

	ret_fp.int_part = (places < fp_arg.int_prec) ?
		((fp_arg.int_part << places) & fp_arg.int_mask) : 0;
	ret_fp.int_part |= ((fp_arg.frac_part & frac_mask) >> frac_shift);
	ret_fp.frac_part = (places < fp_arg.frac_prec) ?
		((fp_arg.frac_part << places) & fp_arg.frac_mask) : 0;

	return ret_fp;
}

struct fixed_point fixed_point_shift_right(
	struct fixed_point fp_arg,
	unsigned int places)
{
	struct fixed_point ret_fp;
	unsigned int int_mask = 0;
	unsigned int frac_shift = 0;
	int sign_ext = 0;

	if (places == 0)
		return fp_arg;

	ret_fp = fixed_point_init(
		0,
		0,
		fp_arg.int_prec,
		fp_arg.frac_prec);

	ret_fp.int_part = (places < fp_arg.int_part) ?
		((fp_arg.int_part >> places) & fp_arg.int_mask) : 0;
	if (fp_arg.int_part & (1 << (fp_arg.int_prec - 1))) { /* sign extend */
		sign_ext = 1;
		if (places < fp_arg.int_prec) {
			ret_fp.int_part |=
				((((1 << places) - 1) <<
				(fp_arg.int_prec - places)) & fp_arg.int_mask);
		} else {
			ret_fp.int_part =
				((unsigned int)(-1)) & ret_fp.int_mask;
		}
	}
	ret_fp.frac_part = (places < fp_arg.frac_prec) ?
		((fp_arg.frac_part >> places) & fp_arg.frac_mask) : 0;

	if (places < fp_arg.frac_prec) {
		int_mask = fp_arg.int_mask >> (fp_arg.int_prec - places);
		frac_shift = fp_arg.frac_prec - places;
		ret_fp.frac_part |=
			(((fp_arg.int_part & int_mask) << frac_shift) &
				fp_arg.frac_mask);
	} else if (places < (fp_arg.int_prec + fp_arg.frac_prec)) {
		int_mask = (fp_arg.int_mask << (places - fp_arg.frac_prec)) &
			fp_arg.int_mask;
		frac_shift = places - fp_arg.frac_prec;
		ret_fp.frac_part |=
			(((fp_arg.int_part & int_mask) >> frac_shift) &
				fp_arg.frac_mask);
		if (sign_ext) {
			ret_fp.frac_part |=
				(fp_arg.frac_mask <<
					(fp_arg.frac_prec -
					(places - fp_arg.frac_prec)));
		}
	} else {
		if (sign_ext)
			ret_fp.frac_part = fp_arg.frac_mask;
	}


	return ret_fp;
}

struct fixed_point fixed_point_negate(
	struct fixed_point fp_arg)
{
	struct fixed_point ret_fp;
	struct fixed_point fp_one;

	ret_fp = fixed_point_init(
		0,
		0,
		fp_arg.int_prec,
		fp_arg.frac_prec);

	ret_fp.int_part = ~fp_arg.int_part;
	ret_fp.frac_part = ~fp_arg.frac_part;

	fp_one.int_prec = fp_arg.int_prec;
	fp_one.frac_prec = fp_arg.frac_prec;
	fp_one.int_mask = fp_arg.int_mask;
	fp_one.frac_mask = fp_arg.frac_mask;

	fp_one.int_part = 0;
	fp_one.frac_part = 1;

	ret_fp = fixed_point_add(ret_fp, fp_one);

	return ret_fp;
}

struct fixed_point fixed_point_add(
	struct fixed_point fp_arg1,
	struct fixed_point fp_arg2)
{
	struct fixed_point ret_fp;
	unsigned int frac_carry_out;
	unsigned int frac1_msb;
	unsigned int frac2_msb;
	unsigned int sum_msb;

	BUG_ON(fp_arg1.int_prec != fp_arg2.int_prec);
	BUG_ON(fp_arg1.frac_prec != fp_arg2.frac_prec);
	BUG_ON(fp_arg1.int_mask != fp_arg2.int_mask);
	BUG_ON(fp_arg1.frac_mask != fp_arg2.frac_mask);

	ret_fp = fixed_point_init(
		0,
		0,
		fp_arg1.int_prec,
		fp_arg1.frac_prec);

	ret_fp.frac_part = (fp_arg1.frac_part + fp_arg2.frac_part) &
		fp_arg1.frac_mask;

	frac1_msb = (fp_arg1.frac_part >> (fp_arg1.frac_prec - 1)) & 1;
	frac2_msb = (fp_arg2.frac_part >> (fp_arg2.frac_prec - 1)) & 1;
	sum_msb = (ret_fp.frac_part >> (fp_arg1.frac_prec - 1)) & 1;
	frac_carry_out = ((frac1_msb & frac2_msb) |
				((frac1_msb | frac2_msb) & ~sum_msb)) & 1;

	ret_fp.int_part = (fp_arg1.int_part +
					   fp_arg2.int_part +
					   frac_carry_out) & fp_arg1.int_mask;

	return ret_fp;
}

struct fixed_point fixed_point_sub(
	struct fixed_point minuend_arg,
	struct fixed_point subtrahend_arg)
{
	struct fixed_point neg_subtrahend = fixed_point_negate(subtrahend_arg);

	return fixed_point_add(minuend_arg, neg_subtrahend);
}

struct fixed_point fixed_point_mult(
		struct fixed_point fp_arg1,
		struct fixed_point fp_arg2)
{
	struct fixed_point ret_fp;
	struct fixed_point tmp_fp1;
	struct fixed_point tmp_fp2;
	int i;

	BUG_ON(fp_arg1.int_prec != fp_arg2.int_prec);
	BUG_ON(fp_arg1.frac_prec != fp_arg2.frac_prec);
	BUG_ON(fp_arg1.int_mask != fp_arg2.int_mask);
	BUG_ON(fp_arg1.frac_mask != fp_arg2.frac_mask);

	ret_fp = fixed_point_init(
		0,
		0,
		fp_arg1.int_prec,
		fp_arg1.frac_prec);

	tmp_fp2 = fp_arg2;

	for (i = 0; i < fp_arg2.frac_prec; i++) {
		ret_fp = fixed_point_shift_right(ret_fp, 1);
		if (tmp_fp2.frac_part & 1)
			ret_fp = fixed_point_add(ret_fp, fp_arg1);
		tmp_fp2 = fixed_point_shift_right(tmp_fp2, 1);
	}

	ret_fp = fixed_point_shift_right(ret_fp, 1);
	tmp_fp1 = fp_arg1;
	for (i = fp_arg2.frac_prec;
		i < (fp_arg2.int_prec + fp_arg2.frac_prec - 1); i++) {
		if (tmp_fp2.frac_part & 1)
			ret_fp = fixed_point_add(ret_fp, tmp_fp1);
		tmp_fp2 = fixed_point_shift_right(tmp_fp2, 1);
		tmp_fp1 = fixed_point_shift_left(tmp_fp1, 1);
	}

	if (tmp_fp2.frac_part & 1)
		ret_fp = fixed_point_sub(ret_fp, tmp_fp1);

	return ret_fp;
}

struct fixed_point fixed_point_div(
		struct fixed_point dividend_arg,
		struct fixed_point divisor_arg)
{
	struct fixed_point ret_fp;
	struct fixed_point tmp_dividend;
	struct fixed_point tmp_divisor;
	struct fixed_point tmp_accum;
	int negate_num = 0;
	int i;

	BUG_ON(dividend_arg.int_prec != divisor_arg.int_prec);
	BUG_ON(dividend_arg.frac_prec != divisor_arg.frac_prec);
	BUG_ON(dividend_arg.int_mask != divisor_arg.int_mask);
	BUG_ON(dividend_arg.frac_mask != divisor_arg.frac_mask);

	if (fixed_point_eq(divisor_arg,
			fixed_point_init(
				0,
				0,
				dividend_arg.int_prec,
				dividend_arg.frac_prec)
		   )
		){

		if (dividend_arg.int_part &
			(1 << (dividend_arg.int_prec - 1))) {
			ret_fp = fixed_point_init(
				1 << (dividend_arg.int_prec - 1),
				0,
				dividend_arg.int_prec,
				dividend_arg.frac_prec);
		} else {
			ret_fp = fixed_point_init(
				(1 << (dividend_arg.int_prec - 1)) - 1,
				dividend_arg.frac_mask,
				dividend_arg.int_prec,
				dividend_arg.frac_prec);
		}

		return ret_fp;
	}


	ret_fp = fixed_point_init(
		0,
		0,
		dividend_arg.int_prec,
		dividend_arg.frac_prec);

	tmp_accum = fixed_point_init(
		0,
		0,
		dividend_arg.int_prec,
		dividend_arg.frac_prec);

	if (dividend_arg.int_part & (1 << (dividend_arg.int_prec - 1))) {
		tmp_dividend = fixed_point_negate(dividend_arg);
		negate_num++;
	} else {
		tmp_dividend = dividend_arg;
	}

	if (divisor_arg.int_part & (1 << (dividend_arg.int_prec - 1))) {
		tmp_divisor = fixed_point_negate(divisor_arg);
		negate_num++;
	} else {
		tmp_divisor = divisor_arg;
	}

	negate_num = negate_num % 2;

	for (i = 0;
		i < (dividend_arg.int_prec + 2 * dividend_arg.frac_prec);
		i++) {
		struct fixed_point next_dividend_shifted =
			fixed_point_shift_right(tmp_dividend,
				dividend_arg.int_prec +
				dividend_arg.frac_prec - i - 1);
		tmp_accum = fixed_point_shift_left(tmp_accum, 1);
		tmp_accum.frac_part |= (next_dividend_shifted.frac_part & 1);

		ret_fp = fixed_point_shift_left(ret_fp, 1);
		if (fixed_point_loet(tmp_divisor, tmp_accum)) {
			tmp_accum = fixed_point_sub(tmp_accum, tmp_divisor);
			ret_fp.frac_part |= 1;
		}
	}

	if (negate_num)
		ret_fp = fixed_point_negate(ret_fp);

	return ret_fp;
}

int fixed_point_lt(
	struct fixed_point fp_lhs_arg,
	struct fixed_point fp_rhs_arg)
{
	unsigned int int_lhs_msb;
	unsigned int int_rhs_msb;

	BUG_ON(fp_lhs_arg.int_prec != fp_rhs_arg.int_prec);
	BUG_ON(fp_lhs_arg.frac_prec != fp_rhs_arg.frac_prec);
	BUG_ON(fp_lhs_arg.int_mask != fp_rhs_arg.int_mask);
	BUG_ON(fp_lhs_arg.frac_mask != fp_rhs_arg.frac_mask);

	int_lhs_msb = (fp_lhs_arg.int_part >> (fp_lhs_arg.int_prec - 1)) & 1;
	int_rhs_msb = (fp_rhs_arg.int_part >> (fp_rhs_arg.int_prec - 1)) & 1;

	if ((int_lhs_msb == 1) && (int_rhs_msb == 0))
		return 1;

	if ((int_lhs_msb == 0) && (int_rhs_msb == 1))
		return 0;

	/* both are positive or both are negative */
	if (fp_lhs_arg.int_part < fp_rhs_arg.int_part)
		return 1;

	if (fp_lhs_arg.int_part > fp_rhs_arg.int_part)
		return 0;

	if (fp_lhs_arg.frac_part < fp_rhs_arg.frac_part)
		return 1;

	if (fp_lhs_arg.frac_part > fp_rhs_arg.frac_part)
		return 0;

	/* equal */
	return 0;
}

int fixed_point_gt(
		struct fixed_point fp_lhs_arg,
		struct fixed_point fp_rhs_arg)
{

	return fixed_point_lt(fp_rhs_arg, fp_lhs_arg);
}

int fixed_point_loet(
		struct fixed_point fp_lhs_arg,
		struct fixed_point fp_rhs_arg)
{
	return fixed_point_lt(fp_lhs_arg, fp_rhs_arg) |
		fixed_point_eq(fp_lhs_arg, fp_rhs_arg);
}

int fixed_point_goet(
		struct fixed_point fp_lhs_arg,
		struct fixed_point fp_rhs_arg)
{
	return fixed_point_gt(fp_lhs_arg, fp_rhs_arg) |
		fixed_point_eq(fp_lhs_arg, fp_rhs_arg);
}

int fixed_point_eq(
		struct fixed_point fp_lhs_arg,
		struct fixed_point fp_rhs_arg)
{
	BUG_ON(fp_lhs_arg.int_prec != fp_rhs_arg.int_prec);
	BUG_ON(fp_lhs_arg.frac_prec != fp_rhs_arg.frac_prec);
	BUG_ON(fp_lhs_arg.int_mask != fp_rhs_arg.int_mask);
	BUG_ON(fp_lhs_arg.frac_mask != fp_rhs_arg.frac_mask);

	if (fp_lhs_arg.int_part != fp_rhs_arg.int_part)
		return 0;

	if (fp_lhs_arg.frac_part != fp_rhs_arg.frac_part)
		return 0;

	return 1;
}

int fixed_point_to_int(
	struct fixed_point fp_arg)
{
	int ret_int;

	if (fp_arg.int_part & (1 << (fp_arg.int_prec - 1))) /* sign extend */
		ret_int = 0 - ((int)fixed_point_negate(fp_arg).int_part);
	else
		ret_int = fp_arg.int_part;

	return ret_int;
}

int fixed_point_ceil(
	struct fixed_point fp_arg)
{
	int ret_int;

	if (fp_arg.int_part & (1 << (fp_arg.int_prec - 1))) { /* negative */
		struct fixed_point neg_fp = fixed_point_negate(fp_arg);
		if (neg_fp.frac_part != 0)
			ret_int = 0 - ((int)neg_fp.int_part) + 1;
		else
			ret_int = neg_fp.int_part;
	} else {
		if (fp_arg.frac_part != 0)
			ret_int = fp_arg.int_part + 1;
		else
			ret_int = fp_arg.int_part;
	}

	return ret_int;
}

struct fixed_point fixed_point_min(
	struct fixed_point fp_arg1,
	struct fixed_point fp_arg2)
{
	if (fixed_point_lt(fp_arg1, fp_arg2))
		return fp_arg1;
	else
		return fp_arg2;
}

struct fixed_point fixed_point_max(
	struct fixed_point fp_arg1,
	struct fixed_point fp_arg2)
{
	if (fixed_point_gt(fp_arg1, fp_arg2))
		return fp_arg1;
	else
		return fp_arg2;
}
