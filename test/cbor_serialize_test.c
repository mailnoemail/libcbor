#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <cmocka.h>
#include <stdio.h>
#include "cbor.h"
#include "../src/cbor.h"
#include <inttypes.h>


unsigned char buffer[512];

static void test_serialize_uint8(void **state) {
	cbor_item_t * item = cbor_new_int8();
	cbor_set_uint8(item, 0);
	assert_int_equal(1, cbor_serialize(item, buffer, 512));
	assert_memory_equal(buffer, (unsigned char[]){ 0x00 }, 1);
}

static void test_serialize_uint16(void **state) {
	cbor_item_t * item = cbor_new_int16();
	cbor_set_uint16(item, 1000);
	assert_int_equal(3, cbor_serialize(item, buffer, 512));
	assert_memory_equal(buffer, ((unsigned char[]){ 0x19, 0x03, 0xE8 }), 3);
}

static void test_serialize_uint32(void **state) {
	cbor_item_t * item = cbor_new_int32();
	cbor_set_uint32(item, 1000000);
	assert_int_equal(5, cbor_serialize(item, buffer, 512));
	assert_memory_equal(buffer, ((unsigned char[]){ 0x1A, 0x00, 0x0F, 0x42, 0x40 }), 5);
}

static void test_serialize_uint64(void **state) {
	cbor_item_t * item = cbor_new_int64();
	cbor_set_uint64(item, 1000000000000);
	assert_int_equal(9, cbor_serialize(item, buffer, 512));
	assert_memory_equal(buffer, ((unsigned char[]){ 0x1B, 0x00, 0x00, 0x00, 0xE8, 0xD4, 0xA5, 0x10, 0x00 }), 9);
}


static void test_serialize_negint8(void **state) {
	cbor_item_t * item = cbor_new_int8();
	cbor_set_uint8(item, 0);
	cbor_mark_negint(item);
	assert_int_equal(1, cbor_serialize(item, buffer, 512));
	assert_memory_equal(buffer, (unsigned char[]){ 0x20 }, 1);
}

static void test_serialize_negint16(void **state) {
	cbor_item_t * item = cbor_new_int16();
	cbor_set_uint16(item, 1000);
	cbor_mark_negint(item);
	assert_int_equal(3, cbor_serialize(item, buffer, 512));
	assert_memory_equal(buffer, ((unsigned char[]){ 0x39, 0x03, 0xE8 }), 3);
}

static void test_serialize_negint32(void **state) {
	cbor_item_t * item = cbor_new_int32();
	cbor_set_uint32(item, 1000000);
	cbor_mark_negint(item);
	assert_int_equal(5, cbor_serialize(item, buffer, 512));
	assert_memory_equal(buffer, ((unsigned char[]){ 0x3A, 0x00, 0x0F, 0x42, 0x40 }), 5);
}

static void test_serialize_negint64(void **state) {
	cbor_item_t * item = cbor_new_int64();
	cbor_set_uint64(item, 1000000000000);
	cbor_mark_negint(item);
	assert_int_equal(9, cbor_serialize(item, buffer, 512));
	assert_memory_equal(buffer, ((unsigned char[]){ 0x3B, 0x00, 0x00, 0x00, 0xE8, 0xD4, 0xA5, 0x10, 0x00 }), 9);
}

static void test_serialize_float2(void **state) {
	cbor_item_t * item = cbor_new_float2();
	cbor_set_float2(item, 0.0f);
	assert_int_equal(3, cbor_serialize(item, buffer, 512));
	assert_memory_equal(buffer, ((unsigned char[]){ 0xF9, 0x00, 0x00 }), 3);
}

static void test_serialize_float4(void **state) {
        cbor_item_t * item = cbor_new_float4();
	cbor_set_float4(item, 3.4028234663852886e+38);
	assert_int_equal(5, cbor_serialize(item, buffer, 512));
	assert_memory_equal(buffer, ((unsigned char[]){ 0xfa, 0x7f, 0x7f, 0xff, 0xff }), 5);
}

static void test_serialize_float8(void **state) {
        cbor_item_t * item = cbor_new_float8();
	cbor_set_float8(item, 1.0e+300);
	assert_int_equal(9, cbor_serialize(item, buffer, 512));
	assert_memory_equal(buffer, ((unsigned char[]){ 0xfb, 0x7e, 0x37, 0xe4, 0x3c, 0x88, 0x00, 0x75, 0x9c }), 9);
}

static void test_serialize_definite_bytestring(void **state) {
	cbor_item_t * item = cbor_new_definite_bytestring();
	unsigned char * data = malloc(256);
	cbor_bytestring_set_handle(item, data, 256);
	assert_int_equal(256 + 3, cbor_serialize(item, buffer, 512));
	assert_memory_equal(buffer, ((unsigned char[]){ 0x59, 0x01, 0x00 }), 3);
	assert_memory_equal(buffer + 3, data, 256);
}

static void test_serialize_definite_string(void **state) {
        cbor_item_t *item = cbor_new_definite_string();
        unsigned char *data = malloc(256);
        cbor_string_set_handle(item, data, 256);
        assert_int_equal(256 + 3, cbor_serialize(item, buffer, 512));
        assert_memory_equal(buffer,
                            ((unsigned char[]){ 0x79, 0x01, 0x00 }), 3);
        assert_memory_equal(buffer + 3, data, 256);
}

static void test_serialize_definite_map0(void **state) {
        cbor_item_t *item = cbor_new_definite_map(0);
        assert_int_equal(1, cbor_serialize(item, buffer, 512));
        assert_memory_equal(buffer,
                            ((unsigned char[]){ 0xA0 }), 1);
}

static void test_serialize_definite_map1(void **state) {
        cbor_item_t *item = cbor_new_definite_map(2);
        struct cbor_pair first, second;
        first.key = cbor_new_int8();
        first.value = cbor_new_int8();

        cbor_set_uint8(first.key, 1);        
        cbor_set_uint8(first.value, 2);

        second.key = cbor_new_int8();
        second.value = cbor_new_int8();

        cbor_set_uint8(second.key, 3);
        cbor_set_uint8(second.value, 4);

        cbor_map_add(item, first);
        cbor_map_add(item, second);
        assert_int_equal(5, cbor_serialize(item, buffer, 512));
        assert_memory_equal(buffer,
                            ((unsigned char[]){ 0xA2, 0x01, 0x02, 0x03, 0x04 }), 5);
}

static void test_serialize_definite_map2(void **state) {
        cbor_item_t *item = cbor_new_definite_map(2);
        struct cbor_pair first, second;
        cbor_item_t *array_first = cbor_new_int8();
        cbor_item_t *array_second = cbor_new_int8();

        cbor_set_uint8(array_first, 2);
        cbor_set_uint8(array_second, 3);

        first.key = cbor_new_definite_string();
        first.value = cbor_new_int8();

        cbor_string_set_handle(first.key, "a", 1);        
        cbor_set_uint8(first.value, 1);

        second.key = cbor_new_definite_string();
        second.value = cbor_new_definite_array(2);

        cbor_string_set_handle(second.key, "b", 1);
        cbor_array_push(second.value, array_first);
        cbor_array_push(second.value, array_second);

        cbor_map_add(item, first);
        cbor_map_add(item, second);
        assert_int_equal(9, cbor_serialize(item, buffer, 512));
        assert_memory_equal(buffer,
                            ((unsigned char[]){ 0xA2, 0x61, 0x61, 0x01, 0x61, 0x62, 0x82, 0x02, 0x03 }), 9);
       
}

int main(void) {
	const UnitTest tests[] = {
		unit_test(test_serialize_uint8),
		unit_test(test_serialize_uint16),
		unit_test(test_serialize_uint32),
		unit_test(test_serialize_uint64),
		unit_test(test_serialize_negint8),
		unit_test(test_serialize_negint16),
		unit_test(test_serialize_negint32),
		unit_test(test_serialize_negint64),
		unit_test(test_serialize_definite_bytestring),
                unit_test(test_serialize_definite_string),
                unit_test(test_serialize_definite_map0),
                unit_test(test_serialize_definite_map1),
                unit_test(test_serialize_definite_map2),
                unit_test(test_serialize_float2),
                unit_test(test_serialize_float4),
                unit_test(test_serialize_float8)
	};
	return run_tests(tests);
}
