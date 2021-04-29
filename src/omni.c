#include "omni.h"
#include "dspmath.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

void omni_wheel_init(omni_wheel_t *wheel, float theta, int32_t length_mm) {
	assert(wheel);
	assert(length_mm > 0);
	wheel->s = q15_16_from_float(sinf(theta));
	wheel->c = q15_16_from_float(cosf(theta));
	wheel->length_mm = length_mm;
}

int32_t omni_wheel_solve(const omni_wheel_t *wheel, int32_t vx_mm,
						 int32_t vy_mm, q15_16_t omega) {
	assert(wheel);

	q47_16_t sum = 0;
	sum += (q47_16_t)vx_mm * wheel->c;
	sum += (q47_16_t)vy_mm * wheel->s;
	sum += (q47_16_t)omega * wheel->length_mm;
	return (int32_t)int64_clamp(sum >> 16, INT32_MIN, INT32_MAX);
}