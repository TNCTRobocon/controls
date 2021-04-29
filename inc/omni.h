#pragma once
#ifndef __HEADER_GUARD_OMNI__
#define __HEADER_GUARD_OMNI__

#include "dsptype.h"
#include <stddef.h>
typedef struct omni_wheel {
	q15_16_t s, c; //リアルタイムに計算できないため、事前に計算しておく
	int32_t length_mm; // mm
} omni_wheel_t;

/**
 * @brief オムニホイールの制御に用いる構造体を初期化する
 * @param wheel パラメーターを格納する場所
 * @param theta モーターの取付角度(ラジアン表記)
 * @param length モーターと中心の距離[m]
 */
void omni_wheel_init(omni_wheel_t *wheel, float theta, int32_t length_mm);

/**
 * @brief　オムニホイールに与える操作量を計算する
 *
 * @param wheel 初期化済みのパラメータ
 * @param vx_mm [mm/s]
 * @param vy_mm [mm/s]
 * @param omega [rad/s]
 * @return int32_t [mm/s]
 */
int32_t omni_wheel_solve(const omni_wheel_t *wheel, int32_t vx_mm,
						 int32_t vy_mm, q15_16_t omega);

#endif
