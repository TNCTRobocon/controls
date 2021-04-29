/**
 * @file pidc.h
 * @brief PIDの実装例
 * @author terurin
 * @date 2021/4/29
 */

#pragma once
#ifndef __HEADER_GUARD_PIDC__
#include "dsptype.h"
#include <stdbool.h>
/**
 * @struct pidc
 * PID制御を行うための構造体
 */
typedef struct pidc {
	//パラメーター
	q15_16_t p, i, d;
	//内部パラメータ
	q15_16_t last_error;
	int32_t sum_error;
} pidc_t;

/**
 * @fn pidc_init
 * @brief 初期化を行う。
 * @param pidc
 * すでに確保されているpidc用の領域を指定してください。(静的でも動的でもいいです)
 * @param p　PIDのPパラメータ(使わない場合は0を代入すること)
 * @param i　PIDのIパラメータ(使わない場合は0を代入すること)
 * @param d　PIDのDパラメータ(使わない場合は0を代入すること
 * @param
 * time　PIDの制御周期。正の値を入れてください(I,Dパラメータの正規化に使用します。迷ったら1を入れてください)
 */
void pidc_init(pidc_t *pidc, float p, float i, float d, float time);

/**
 * @fn pid制御を行う
 * @param pidc
 * すでに初期化されているpidc用の領域を指定してください。
 * @param error 偏差
 * @return 操作量
 */
int32_t pidc_pid(pidc_t *pidc, int32_t error);

#endif
