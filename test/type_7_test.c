#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include "cbor.h"
#include "assertions.h"
#include <inttypes.h>

cbor_item_t * float_ctrl;
struct cbor_load_result res;


unsigned char float2_data[] = { 0xF9, 0x7B, 0xFF };
static void test_float2(void **state) {
	float_ctrl = cbor_load(float2_data, 3, CBOR_FLAGS_NONE, &res);
	assert_true(cbor_isa_float_ctrl(float_ctrl));
	assert_true(cbor_is_float(float_ctrl));
	assert_true(cbor_float_get_width(float_ctrl) == CBOR_FLOAT_16);
	assert_true(cbor_float_get_float2(float_ctrl) == 65504.0F);
	cbor_decref(&float_ctrl);
	assert_null(float_ctrl);
}

unsigned char float4_data[] = { 0xFA, 0x47, 0xC3, 0x50, 0x00 };
static void test_float4(void **state) {
	float_ctrl = cbor_load(float4_data, 5, CBOR_FLAGS_NONE, &res);
	assert_true(cbor_isa_float_ctrl(float_ctrl));
	assert_true(cbor_is_float(float_ctrl));
	assert_true(cbor_float_get_width(float_ctrl) == CBOR_FLOAT_32);
	assert_true(cbor_float_get_float4(float_ctrl) == 100000.0F);
	cbor_decref(&float_ctrl);
	assert_null(float_ctrl);
}

unsigned char float8_data[] = { 0xFB, 0x7E, 0x37, 0xE4, 0x3C, 0x88, 0x00, 0x75, 0x9C };
static void test_float8(void **state) {
	float_ctrl = cbor_load(float8_data, 9, CBOR_FLAGS_NONE, &res);
	assert_true(cbor_isa_float_ctrl(float_ctrl));
	assert_true(cbor_is_float(float_ctrl));
	assert_true(cbor_float_get_width(float_ctrl) == CBOR_FLOAT_64);
	assert_true(cbor_float_get_float8(float_ctrl) == 1.0e+300);
	cbor_decref(&float_ctrl);
	assert_null(float_ctrl);
}

unsigned char null_data[] = { 0xF6 };
static void test_null(void **state) {
	float_ctrl = cbor_load(null_data, 1, CBOR_FLAGS_NONE, &res);
	assert_true(cbor_isa_float_ctrl(float_ctrl));
	assert_true(cbor_is_null(float_ctrl));
	cbor_decref(&float_ctrl);
	assert_null(float_ctrl);
}

unsigned char undef_data[] = { 0xF7 };
static void test_undef(void **state) {
	float_ctrl = cbor_load(undef_data, 1, CBOR_FLAGS_NONE, &res);
	assert_true(cbor_isa_float_ctrl(float_ctrl));
	assert_true(cbor_is_undef(float_ctrl));
	cbor_decref(&float_ctrl);
	assert_null(float_ctrl);
}

unsigned char bool_data[] = { 0xF4, 0xF5 };
static void test_bool(void **state) {
	float_ctrl = cbor_load(bool_data, 1, CBOR_FLAGS_NONE, &res);
	assert_true(cbor_isa_float_ctrl(float_ctrl));
	assert_true(cbor_is_bool(float_ctrl));
	assert_false(cbor_ctrl_bool(float_ctrl));
	cbor_decref(&float_ctrl);
	assert_null(float_ctrl);

	float_ctrl = cbor_load(bool_data + 1, 1, CBOR_FLAGS_NONE, &res);
	assert_true(cbor_isa_float_ctrl(float_ctrl));
	assert_true(cbor_is_bool(float_ctrl));
	assert_true(cbor_ctrl_bool(float_ctrl));
	cbor_decref(&float_ctrl);
	assert_null(float_ctrl);
}

int main(void) {
	const UnitTest tests[] = {
		unit_test(test_float2),
		unit_test(test_float4),
		unit_test(test_float8),
		unit_test(test_null),
		unit_test(test_undef),
		unit_test(test_bool)
	};
	return run_tests(tests);
}
