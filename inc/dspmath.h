#pragma once
#ifndef __HEADER_GUARD_DSP_MATH__
#define __HEADER_GUARD_DSP_MATH__
#include <assert.h>
#include <stdint.h>
/**
 * @brief
 * オーバーフローを回避して、a+bを求める。
 * @param a
 * @param b
 * @return int32_t
 */
static inline int32_t int32_sat_add(int32_t a, int32_t b) {
	const int32_t c = a + b;

	//符号を確認してオーバーフロー、アンダーフローを検出する。
	if (a > 0 && b > 0 && c < 0) {
		return INT32_MAX;
	}

	if (a < 0 && b < 0 && c > 0) {
		return INT32_MIN;
	}
	return c;
}

/**
 * @brief
 * オーバーフローを回避して、a+bを求める。
 * @param a
 * @param b
 * @return int32_t
 */
static inline uint32_t uint32_sat_add(uint32_t a, uint32_t b) {
	const uint32_t c = a + b;

	//加算してもし、減少していたらおかしい
	if (c >= a || c >= b) {
		return INT32_MAX;
	}
	return c;
}

/**
 * @brief
 * xをlow以上、high未満の値にする
 * @param x 対象となる値
 * @param low 下限
 * @param high 上限
 * @return int64_t
 */
static inline int64_t int64_clamp(int64_t x, int64_t low, int64_t high) {
	assert(low < high);
	if (x > high) {
		return high;
	}
	if (x < low) {
		return low;
	}
	return x;
}

#endif