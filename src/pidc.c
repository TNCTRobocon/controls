/**
 * @file pidc.c
 * @brief PIDの実装例
 * @author terurin
 * @date 2021/4/29
 */

#include "pidc.h"
#include <assert.h>
#include <dspmath.h>
#include <float.h>
#include <stddef.h>
void pidc_init(pidc_t *pidc, float p, float i, float d, float time) {
	assert(pidc);
	assert(time > FLT_MIN);
	pidc->p = q15_16_from_float(p);
	pidc->i = q15_16_from_float(i * time);
	pidc->d = q15_16_from_float(d / time);
	pidc->last_error = 0;
	pidc->sum_error = 0;
}

int32_t pidc_pid(pidc_t *pidc, int32_t error) {
	assert(pidc);

	//操作量計算
	q47_16_t acc = 0;
	acc += (q47_16_t)pidc->p * error;
	acc += (q47_16_t)pidc->i * pidc->sum_error;
	acc += (q47_16_t)pidc->d * (error - pidc->last_error);

	//更新
	pidc->last_error = error;
	pidc->sum_error = int32_sat_add(pidc->sum_error, error);

	return int64_clamp(acc >> 16, INT32_MIN, INT32_MAX);
}
