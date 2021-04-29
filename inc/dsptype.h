
#pragma once
#ifndef __HEADER_GUARD_DSP_TYPE__
#define __HEADER_GUARD_DSP_TYPE__
#include <stdint.h>

typedef uint_fast32_t q16_16_t;
static inline q16_16_t q16_16_from_float(float x) { return x * (1 << 16); }
static inline q16_16_t q16_16_to_float(q16_16_t x) {
	return (float)x / (1 << 16);
}

typedef int_fast32_t q15_16_t;
static inline q15_16_t q15_16_from_float(float x) { return x * (1 << 16); }
static inline q15_16_t q15_16_to_float(q16_16_t x) {
	return (float)x / (1 << 16);
}

typedef uint_fast64_t q32_32_t;
static inline q32_32_t q32_32_from_float(float x) { return x * (1 << 16); }
static inline q32_32_t q32_32_to_float(q32_32_t x) {
	return (float)x / (1 << 16);
}

typedef int_fast64_t q31_32_t;
static inline q31_32_t q31_32_from_float(float x) { return x * (1L << 32); }
static inline q31_32_t q31_32_to_float(q31_32_t x) {
	return (float)x / (1L << 32);
}

typedef int_fast64_t q48_16_t;
typedef int_fast64_t q47_16_t;

#endif