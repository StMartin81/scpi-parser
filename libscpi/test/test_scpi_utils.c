/*-
 * BSD 2-Clause License
 *
 * Copyright (c) 2012-2018, Jan Breuer, Richard.hmm
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this
 *   list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice,
 *   this list of conditions and the following disclaimer in the documentation
 *   and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "../src/utils_private.h"
#include "scpi/scpi.h"
#include <CUnit/Theory.h>
#include <inttypes.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * CUnit Test Suite
 */

CU_Init(scpi_utils)
{
  return 0;
}

CU_Clean(scpi_utils)
{
  return 0;
}

CU_Test(scpi_utils, strnpbrk)
{
  char str[] = "ahoj";

  CU_ASSERT(strnpbrk(str, 4, "a") == (str + 0))
  CU_ASSERT(strnpbrk(str, 4, "h") == (str + 1))
  CU_ASSERT(strnpbrk(str, 4, "b") == NULL)
  CU_ASSERT(strnpbrk(str, 1, "h") == NULL)
  CU_ASSERT(strnpbrk(str, 4, "xo") == (str + 2))

  CU_ASSERT(strnpbrk(str, 4, "j") == (str + 3))
}

CU_Test(scpi_utils, Int32ToStr)
{
  const size_t max = 32 + 1;
  int32_t val[] = { 0, 1, -1, INT32_MIN, INT32_MAX, 0x01234567, 0x89abcdef };
  int N = sizeof(val) / sizeof(int32_t);
  int i;
  char str[max];
  char ref[max];
  size_t len;

  /* test signed conversion to decimal numbers */
  for (i = 0; i < N; i++) {
    len = SCPI_Int32ToStr(val[i], str, max);
    sprintf(ref, "%" PRIi32, val[i]);
    CU_ASSERT(len == strlen(ref))
    CU_ASSERT_STRING_EQUAL(str, ref)
  }

  int16_t val16[] = { 0, 1, -1, INT16_MIN, INT16_MAX, 0x0123, 0x4567, 0x89ab, 0xcdef };
  int N16 = sizeof(val16) / sizeof(int16_t);
  /* test signed conversion to decimal numbers */
  for (i = 0; i < N16; i++) {
    len = SCPI_Int32ToStr((int32_t)val16[i], str, max);
    sprintf(ref, "%" PRIi16, val16[i]);
    CU_ASSERT(len == strlen(ref))
    CU_ASSERT_STRING_EQUAL(str, ref)
  }
}

CU_Test(scpi_utils, UInt32ToStrBase)
{
  const size_t max = 32 + 1;
  uint32_t val[] = { 0, 1, -1, INT32_MIN, INT32_MAX, 0x01234567, 0x89abcdef };
  int N = sizeof(val) / sizeof(uint32_t);
  int i;
  char str[max];
  char ref[max];
  size_t len;

  /* test conversion to decimal numbers */
  for (i = 0; i < N; i++) {
    len = SCPI_UInt32ToStrBase(val[i], str, max, 10);
    sprintf(ref, "%" PRIu32, val[i]);
    CU_ASSERT(len == strlen(ref))
    CU_ASSERT_STRING_EQUAL(str, ref)
  }

  /* test conversion to hexadecimal numbers */
  for (i = 0; i < N; i++) {
    len = SCPI_UInt32ToStrBase(val[i], str, max, 16);
    sprintf(ref, "%" PRIX32, val[i]);
    CU_ASSERT(len == strlen(ref))
    CU_ASSERT_STRING_EQUAL(str, ref)
  }

  /* test conversion to octal numbers */
  for (i = 0; i < N; i++) {
    len = SCPI_UInt32ToStrBase(val[i], str, max, 8);
    sprintf(ref, "%" PRIo32, val[i]);
    CU_ASSERT(len == strlen(ref))
    CU_ASSERT_STRING_EQUAL(str, ref)
  }

  /* test conversion to binary numbers */
  len = SCPI_UInt32ToStrBase(0, str, max, 2);
  CU_ASSERT(len == 1)
  CU_ASSERT_STRING_EQUAL(str, "0")

  len = SCPI_UInt32ToStrBase(1, str, max, 2);
  CU_ASSERT(len == 1)
  CU_ASSERT_STRING_EQUAL(str, "1")

  len = SCPI_UInt32ToStrBase(-1, str, max, 2);
  CU_ASSERT(len == 32)
  CU_ASSERT_STRING_EQUAL(str, "11111111111111111111111111111111")

  len = SCPI_UInt32ToStrBase(0x01234567, str, max, 2);
  CU_ASSERT(len == 25)
  CU_ASSERT_STRING_EQUAL(str, "1001000110100010101100111")

  len = SCPI_UInt32ToStrBase(0x89abcdef, str, max, 2);
  CU_ASSERT(len == 32)
  CU_ASSERT_STRING_EQUAL(str, "10001001101010111100110111101111")
}

CU_Test(scpi_utils, Int64ToStr)
{
  const size_t max = 64 + 1;
  int64_t val[] = { 0, 1, -1, INT64_MIN, INT64_MAX, 0x0123456789abcdef, 0xfedcba9876543210 };
  int N = sizeof(val) / sizeof(int64_t);
  int i;
  char str[max];
  char ref[max];
  size_t len;

  /* test conversion to decimal numbers */
  for (i = 0; i < N; i++) {
    len = SCPI_Int64ToStr(val[i], str, max);
    sprintf(ref, "%" PRIi64, val[i]);
    CU_ASSERT(len == strlen(ref))
    CU_ASSERT_STRING_EQUAL(str, ref)
  }
}

CU_Test(scpi_utils, UInt64ToStrBase)
{
  const size_t max = 64 + 1;
  uint64_t val[] = { 0, 1, -1, INT64_MIN, INT64_MAX, 0x0123456789abcdef, 0xfedcba9876543210 };
  int N = sizeof(val) / sizeof(uint64_t);
  int i;
  char str[max];
  char ref[max];
  size_t len;

  /* test conversion to decimal numbers */
  for (i = 0; i < N; i++) {
    len = SCPI_UInt64ToStrBase(val[i], str, max, 10);
    sprintf(ref, "%" PRIu64, val[i]);
    CU_ASSERT(len == strlen(ref))
    CU_ASSERT_STRING_EQUAL(str, ref)
  }

  /* test conversion to hexadecimal numbers */
  for (i = 0; i < N; i++) {
    len = SCPI_UInt64ToStrBase(val[i], str, max, 16);
    sprintf(ref, "%" PRIX64, val[i]);
    CU_ASSERT(len == strlen(ref))
    CU_ASSERT_STRING_EQUAL(str, ref)
  }

  /* test conversion to octal numbers */
  for (i = 0; i < N; i++) {
    len = SCPI_UInt64ToStrBase(val[i], str, max, 8);
    sprintf(ref, "%" PRIo64, val[i]);
    CU_ASSERT(len == strlen(ref))
    CU_ASSERT_STRING_EQUAL(str, ref)
  }

  /* test conversion to binary numbers */
  len = SCPI_UInt64ToStrBase(0, str, max, 2);
  CU_ASSERT(len == 1)
  CU_ASSERT_STRING_EQUAL(str, "0")

  len = SCPI_UInt64ToStrBase(1, str, max, 2);
  CU_ASSERT(len == 1)
  CU_ASSERT_STRING_EQUAL(str, "1")

  len = SCPI_UInt64ToStrBase(-1, str, max, 2);
  CU_ASSERT(len == 64)
  CU_ASSERT_STRING_EQUAL(str, "1111111111111111111111111111111111111111111111111111111111111111")

  len = SCPI_UInt64ToStrBase(0x0123456789abcdef, str, max, 2);
  CU_ASSERT(len == 57)
  CU_ASSERT_STRING_EQUAL(str, "100100011010001010110011110001001101010111100110111101111")

  len = SCPI_UInt64ToStrBase(0xfedcba9876543210, str, max, 2);
  CU_ASSERT(len == 64)
  CU_ASSERT_STRING_EQUAL(str, "1111111011011100101110101001100001110110010101000011001000010000")
}

CU_Test(scpi_utils, dtostre)
{
  const size_t strsize = 49 + 1;
  double val[] = {
#ifdef INFINITY
    INFINITY,
    -INFINITY,
#endif
    0,
    1,
    1.1,
    1.01,
    1.001,
    1.0001,
    1.00001,
    1.000001,
    1.0000001,
    1.00000001,
    1.000000001,
    1.0000000001,
    1.00000000001,
    1.000000000001,
    1.0000000000001,
    1e-5,
    1.1e-5,
    1.01e-5,
    1.001e-5,
    1.0001e-5,
    1.00001e-5,
    1.000001e-5,
    1.0000001e-5,
    1.00000001e-5,
    1.000000001e-5,
    1.0000000001e-5,
    1.00000000001e-5,
    1.000000000001e-5,
    1.0000000000001e-5,
    1,
    12,
    123,
    1234,
    12345,
    123456,
    1234567,
    12345678,
    123456789,
    1234567890,
    12345678901,
    123456789012,
    1234567890123,
    12345678901234,
    123456789012345,
    1234567890123456,
    12345678901234567,
    123456789012345678,
    1234567890123456789,
    1234567890123456789e1,
    1.1,
    10.1,
    100.1,
    1000.1,
    10000.1,
    100000.1,
    1000000.1,
    10000000.1,
    100000000.1,
    1000000000.1,
    0.1234567890123456789,
    0.01234567890123456789,
    0.001234567890123456789,
    0.0001234567890123456789,
    0.00001234567890123456789,
    0.000001234567890123456789,
    0.0000001234567890123456789,
    0.00000001234567890123456789,
    0.00000000123456789,
    0.000000000123456789,
    0.0000000000123456789,
    0.00000000000123456789,
    0.000000000000123456789,
    0.0000000000000123456789,
    0.00000000000000123456789,
    0.000000000000000123456789,
    0.0000000000000000123456789,
    -1e-5,
    -1.1e-5,
    -1.01e-5,
    -1.001e-5,
    -1.0001e-5,
    -1.00001e-5,
    -1.00001e-6,
    -1.00001e-10,
    -1.00001e-20,
    -1.00001e-50,
    -1.00001e-100,
    -1.00001e-200,
    -1.00001e-300,
    -1.00001e6,
    -1.00001e10,
    -1.00001e20,
    -1.00001e50,
    -1.00001e100,
    -1.00001e150,
    -1.00001e200,
    -1.00001e300,
    1.7976931348623157e308,
    2.2250738585072014e-308,
    -1.7976931348623157e308,
    -2.2250738585072014e-308
  };
  int N = sizeof(val) / sizeof(*val);
  int i;
  char str[strsize];
  char ref[strsize];
  size_t len;

  for (i = 0; i < N; i++) {
    len = strlen(SCPI_dtostre(val[i], str, strsize, 15, 0));
    sprintf(ref, "%.15lg", val[i]);
    CU_ASSERT(len == strlen(ref))
    CU_ASSERT_STRING_EQUAL(str, ref)
  }

  for (i = 0; i < N; i++) {
    len = strlen(SCPI_dtostre(val[i], str, 2, 15, 0));
    snprintf(ref, 2, "%.15lg", val[i]);
    CU_ASSERT(len == strlen(ref))
    CU_ASSERT_STRING_EQUAL(str, ref)
  }

  for (i = 0; i < N; i++) {
    len = strlen(SCPI_dtostre(val[i], str, 12, 15, 0));
    snprintf(ref, 12, "%.15lg", val[i]);
    CU_ASSERT(len == strlen(ref))
    CU_ASSERT_STRING_EQUAL(str, ref)
  }

#ifdef NAN
    len = strlen(SCPI_dtostre(NAN, str, strsize, 15, 0));
    strncpy(ref, "nan", strsize);
    CU_ASSERT(len == strlen(ref))
    CU_ASSERT_STRING_EQUAL(str, ref)

    len = strlen(SCPI_dtostre(NAN, str, 2, 15, 0));
    strncpy(ref, "nan", 2);
    ref[2 - 1] = '\0';
    CU_ASSERT(len == strlen(ref))
    CU_ASSERT_STRING_EQUAL(str, ref)
#endif
}

CU_Test(scpi_utils, floatToStr)
{
  const size_t max = 49 + 1;
  float val[] = { 1, -1, 1.1, -1.1, 1e3, 1e30, -1.3e30, -1.3e-30 };
  int N = sizeof(val) / sizeof(float);
  int i;
  char str[max];
  char ref[max];
  size_t len;

  for (i = 0; i < N; i++) {
    len = SCPI_FloatToStr(val[i], str, max);
    sprintf(ref, "%g", val[i]);
    CU_ASSERT(len == strlen(ref))
    CU_ASSERT_STRING_EQUAL(str, ref)
  }
}

CU_Test(scpi_utils, doubleToStr)
{
  const size_t max = 49 + 1;
  double val[] = { 1, -1, 1.1, -1.1, 1e3, 1e30, -1.3e30, -1.3e-30 };
  int N = sizeof(val) / sizeof(*val);
  int i;
  char str[max];
  char ref[max];
  size_t len;

  for (i = 0; i < N; i++) {
    len = SCPI_DoubleToStr(val[i], str, max);
    sprintf(ref, "%.15lg", val[i]);
    CU_ASSERT(len == strlen(ref))
    CU_ASSERT_STRING_EQUAL(str, ref)
  }
}

CU_Test(scpi_utils, strBaseToInt32)
{
  size_t result;
  int32_t val;

#define TEST_STR_TO_INT32(s, r, v, b)                   \
    do {                                                \
        result = strBaseToInt32(s, &val, b);            \
        CU_ASSERT_EQUAL(val, v);                        \
        CU_ASSERT_EQUAL(result, r);                     \
    } while(0)                                          \

    /* TODO: extend to corner cases, use scanf as reference */
    TEST_STR_TO_INT32("", 0, 0, 10);
    TEST_STR_TO_INT32("1", 1, 1, 10);
    TEST_STR_TO_INT32("10", 2, 10, 10);
    TEST_STR_TO_INT32("-50", 3, -50, 10);
    TEST_STR_TO_INT32("100MHz", 3, 100, 10);
    TEST_STR_TO_INT32("MHz", 0, 0, 10);
    TEST_STR_TO_INT32("1.4", 1, 1, 10);
    TEST_STR_TO_INT32(" 1", 2, 1, 10);
    TEST_STR_TO_INT32(" +100", 5, 100, 10); /* space and + */
    TEST_STR_TO_INT32("FF", 2, 255, 16); /* hexadecimal FF */
    TEST_STR_TO_INT32("77", 2, 63, 8); /* octal 77 */
    TEST_STR_TO_INT32("18", 1, 1, 8);  /* octal 1, 8 is ignored */
}

CU_Test(scpi_utils, strBaseToUInt32)
{
  size_t result;
  uint32_t val;

#define TEST_STR_TO_UINT32(s, r, v, b)                  \
    do {                                                \
        result = strBaseToUInt32(s, &val, b);           \
        CU_ASSERT_EQUAL(val, v);                        \
        CU_ASSERT_EQUAL(result, r);                     \
    } while(0)                                          \

    /* TODO: extend to corner cases, use scanf as reference */
    TEST_STR_TO_UINT32("", 0, 0, 10);
    TEST_STR_TO_UINT32("1", 1, 1, 10);
    TEST_STR_TO_UINT32("10", 2, 10, 10);
    TEST_STR_TO_UINT32("100MHz", 3, 100, 10);
    TEST_STR_TO_UINT32("MHz", 0, 0, 10);
    TEST_STR_TO_UINT32("1.4", 1, 1, 10);
    TEST_STR_TO_UINT32(" 1", 2, 1, 10);
    TEST_STR_TO_UINT32(" +100", 5, 100, 10); /* space and + */
    TEST_STR_TO_UINT32("FF", 2, 255, 16); /* hexadecimal FF */
    TEST_STR_TO_UINT32("77", 2, 63, 8); /* octal 77 */
    TEST_STR_TO_UINT32("18", 1, 1, 8); /* octal 1, 8 is ignored */
    TEST_STR_TO_UINT32("FFFFFFFF", 8, 0xffffffffu, 16); /* octal 1, 8 is ignored */
}

CU_Test(scpi_utils, strBaseToInt64)
{
  size_t result;
  int64_t val;

#define TEST_STR_TO_INT64(s, r, v, b)                   \
    do {                                                \
        result = strBaseToInt64(s, &val, b);            \
        CU_ASSERT_EQUAL(val, v);                        \
        CU_ASSERT_EQUAL(result, r);                     \
    } while(0)                                          \

    /* TODO: extend to corner cases, use scanf as reference */
    TEST_STR_TO_INT64("", 0, 0, 10);
    TEST_STR_TO_INT64("1", 1, 1, 10);
    TEST_STR_TO_INT64("10", 2, 10, 10);
    TEST_STR_TO_INT64("-50", 3, -50, 10);
    TEST_STR_TO_INT64("100MHz", 3, 100, 10);
    TEST_STR_TO_INT64("MHz", 0, 0, 10);
    TEST_STR_TO_INT64("1.4", 1, 1, 10);
    TEST_STR_TO_INT64(" 1", 2, 1, 10);
    TEST_STR_TO_INT64(" +100", 5, 100, 10); /* space and + */
    TEST_STR_TO_INT64("FF", 2, 255, 16); /* hexadecimal FF */
    TEST_STR_TO_INT64("77", 2, 63, 8); /* octal 77 */
    TEST_STR_TO_INT64("18", 1, 1, 8);  /* octal 1, 8 is ignored */
}

CU_Test(scpi_utils, strBaseToUInt64)
{
  size_t result;
  uint64_t val;

#define TEST_STR_TO_UINT64(s, r, v, b)                  \
    do {                                                \
        result = strBaseToUInt64(s, &val, b);           \
        CU_ASSERT_EQUAL(val, v);                        \
        CU_ASSERT_EQUAL(result, r);                     \
    } while(0)                                          \

    /* TODO: extend to corner cases, use scanf as reference */
    TEST_STR_TO_UINT64("", 0, 0, 10);
    TEST_STR_TO_UINT64("1", 1, 1, 10);
    TEST_STR_TO_UINT64("10", 2, 10, 10);
    TEST_STR_TO_UINT64("100MHz", 3, 100, 10);
    TEST_STR_TO_UINT64("MHz", 0, 0, 10);
    TEST_STR_TO_UINT64("1.4", 1, 1, 10);
    TEST_STR_TO_UINT64(" 1", 2, 1, 10);
    TEST_STR_TO_UINT64(" +100", 5, 100, 10); /* space and + */
    TEST_STR_TO_UINT64("FF", 2, 255, 16); /* hexadecimal FF */
    TEST_STR_TO_UINT64("77", 2, 63, 8); /* octal 77 */
    TEST_STR_TO_UINT64("18", 1, 1, 8); /* octal 1, 8 is ignored */
    TEST_STR_TO_UINT64("FFFFFFFF", 8, 0xffffffffu, 16); /* octal 1, 8 is ignored */
}

CU_Test(scpi_utils, strToDouble)
{
  double val;
  size_t result;

#define TEST_STR_TO_DOUBLE(s, r, v)                                                                                    \
  do {                                                                                                                 \
    result = strToDouble(s, &val);                                                                                     \
    CU_ASSERT_EQUAL(result, r);                                                                                        \
    CU_ASSERT_DOUBLE_EQUAL(v, val, 0.000001);                                                                          \
  } while (0);

  TEST_STR_TO_DOUBLE("", 0, 0.0)

  TEST_STR_TO_DOUBLE(" 1", 2, 1.0)

  TEST_STR_TO_DOUBLE("1", 1, 1.0)
  TEST_STR_TO_DOUBLE("10", 2, 10.0)
  TEST_STR_TO_DOUBLE("10MHz", 2, 10.0)
  TEST_STR_TO_DOUBLE("MHz", 0, 0.0)
  TEST_STR_TO_DOUBLE("1E", 1, 1.0)
  TEST_STR_TO_DOUBLE("1E3", 3, 1000.0)

  TEST_STR_TO_DOUBLE("1.2", 3, 1.2)
  TEST_STR_TO_DOUBLE("10.2", 4, 10.2)
  TEST_STR_TO_DOUBLE("10.2MHz", 4, 10.2)
  TEST_STR_TO_DOUBLE("MHz", 0, 0.0)
  TEST_STR_TO_DOUBLE("1.2E", 3, 1.2)
  TEST_STR_TO_DOUBLE("1.2E3", 5, 1200.0)

  TEST_STR_TO_DOUBLE("-1.2", 4, -1.2)
}

CU_Test(scpi_utils, compareStr){

  CU_ASSERT_TRUE(compareStr("abcd", 1, "afgh", 1)) CU_ASSERT_TRUE(compareStr("ABCD", 4, "abcd", 4))
    CU_ASSERT_TRUE(compareStr("AbCd", 3, "AbCE", 3)) CU_ASSERT_TRUE(compareStr("ABCD", 1, "a", 1))

      CU_ASSERT_FALSE(compareStr("abcd", 1, "efgh", 1)) CU_ASSERT_FALSE(compareStr("ABCD", 4, "abcd", 3))
}

CU_Test(scpi_utils, compareStrAndNum)
{
  int32_t num;

  CU_ASSERT_TRUE(compareStrAndNum("abcd", 1, "afgh", 1, NULL))
  CU_ASSERT_TRUE(compareStrAndNum("ABCD", 4, "abcd", 4, NULL))
  CU_ASSERT_TRUE(compareStrAndNum("AbCd", 3, "AbCE", 3, NULL))
  CU_ASSERT_TRUE(compareStrAndNum("ABCD", 1, "a", 1, NULL))

  CU_ASSERT_FALSE(compareStrAndNum("abcd", 1, "efgh", 1, NULL))
  CU_ASSERT_FALSE(compareStrAndNum("ABCD", 4, "abcd", 3, NULL))

  CU_ASSERT_TRUE(compareStrAndNum("abcd", 4, "abcd1", 5, NULL))
  CU_ASSERT_TRUE(compareStrAndNum("abcd", 4, "abcd123", 7, NULL))
  CU_ASSERT_FALSE(compareStrAndNum("abcd", 4, "abcd12A", 7, NULL))
  CU_ASSERT_FALSE(compareStrAndNum("abcd", 4, "abcdB12", 7, NULL))
  CU_ASSERT_FALSE(compareStrAndNum("abdd", 4, "abcd132", 7, NULL))

#define TEST_COMPARE_STR_AND_NUM(s1, l1, s2, l2, v, r)                                                                 \
  do {                                                                                                                 \
    num = -1;                                                                                                          \
    CU_ASSERT_EQUAL(compareStrAndNum(s1, l1, s2, l2, &num), r);                                                        \
    CU_ASSERT_EQUAL(num, v);                                                                                           \
  } while (0);

  TEST_COMPARE_STR_AND_NUM("abcd", 4, "abcd", 4, -1, TRUE)
  TEST_COMPARE_STR_AND_NUM("abcd", 4, "abcd1", 5, 1, TRUE)
  TEST_COMPARE_STR_AND_NUM("abcd", 4, "abcd123", 7, 123, TRUE)
  TEST_COMPARE_STR_AND_NUM("abcd", 4, "abcd12A", 7, -1, FALSE)
  TEST_COMPARE_STR_AND_NUM("abcd", 4, "abcdB12", 7, -1, FALSE)
  TEST_COMPARE_STR_AND_NUM("abdd", 4, "abcd132", 7, -1, FALSE)
}

CU_Test(scpi_utils, matchPattern)
{
  scpi_bool_t result;

#define TEST_MATCH_PATTERN(p, s, r)                             \
    do {                                                        \
        result = matchPattern(p, strlen(p), s, strlen(s), NULL);\
        CU_ASSERT_EQUAL(result, r);                             \
    } while(0)                                                  \

    TEST_MATCH_PATTERN("A", "a", TRUE);
    TEST_MATCH_PATTERN("Ab", "a", TRUE);
    TEST_MATCH_PATTERN("Ab", "ab", TRUE);
    TEST_MATCH_PATTERN("Ab", "aB", TRUE);
    TEST_MATCH_PATTERN("AB", "a", FALSE);
    TEST_MATCH_PATTERN("Ab#", "aB", TRUE);
    TEST_MATCH_PATTERN("Ab#", "aB10", TRUE);
    TEST_MATCH_PATTERN("Ab#", "a10", TRUE);
}

CU_Test(scpi_utils, matchCommand)
{
  scpi_bool_t result;
  int32_t values[20];

#define TEST_MATCH_COMMAND(p /*attern*/, c /*md*/, /*pat*/ h, r /*esult*/)                                             \
    do {                                                                                                               \
        scpi_token_t cmd = { .type = SCPI_TOKEN_UNKNOWN, .ptr = c, .len = strlen(c) };                                 \
        scpi_token_t path = { .type = SCPI_TOKEN_UNKNOWN, .ptr = h, .len = strlen(h) };                                \
        result = matchCommand(p, cmd, &path, NULL, 0, 0);                                                              \
        CU_ASSERT_EQUAL(result, r);                                                                                    \
    } while (0)

#define NOPAREN(...) __VA_ARGS__

#define TEST_MATCH_COMMAND2(p /*attern*/, c /*md*/, /*pat*/ h, r /*esult*/, v /*alue*/)                                \
    do {                                                                                                               \
        int32_t evalues[] = { NOPAREN v };                                                                             \
        unsigned int cnt = (sizeof(evalues) / 4);                                                                      \
        scpi_token_t cmd = { .type = SCPI_TOKEN_UNKNOWN, .ptr = c, .len = strlen(c) };                                 \
        scpi_token_t path = { .type = SCPI_TOKEN_UNKNOWN, .ptr = h, .len = strlen(h) };                                \
        result = matchCommand(p, cmd, &path, values, 20, -1);                                                          \
        CU_ASSERT_EQUAL(result, r);                                                                                    \
        {                                                                                                              \
            unsigned int i;                                                                                            \
            for (i = 0; i < cnt; i++) {                                                                                \
                CU_ASSERT_EQUAL(evalues[i], values[i]);                                                                \
            }                                                                                                          \
        }                                                                                                              \
    } while (0)

  TEST_MATCH_COMMAND("A", "a", "", TRUE);
  TEST_MATCH_COMMAND("Ab", "a", "", TRUE);
  TEST_MATCH_COMMAND("Ab", "ab", "", TRUE);
  TEST_MATCH_COMMAND("Ab", "aB", "", TRUE);
  TEST_MATCH_COMMAND("AB", "a", "", FALSE);
  TEST_MATCH_COMMAND("ABcc:AACddd", "ab:aac", "", TRUE);
  TEST_MATCH_COMMAND("ABcc:AACddd", "abcc:aac", "", TRUE);
  TEST_MATCH_COMMAND("ABcc:AACddd", "abcc:aacddd", "", TRUE);
  TEST_MATCH_COMMAND("ABcc:AACddd", "abcc:aacdd", "", FALSE);
  TEST_MATCH_COMMAND("ABcc:AACddd", "a:aac", "", FALSE);
  TEST_MATCH_COMMAND("ABcc:AACddd", ":ab:aac", "", TRUE);
  TEST_MATCH_COMMAND("ABcc:AACddd", ":abcc:aac", "", TRUE);
  TEST_MATCH_COMMAND("ABcc:AACddd", ":abcc:aacddd", "", TRUE);
  TEST_MATCH_COMMAND("ABcc:AACddd", ":abcc:aacdd", "", FALSE);
  TEST_MATCH_COMMAND("ABcc:AACddd", ":a:aac", "", FALSE);
  TEST_MATCH_COMMAND("?", "?", "", TRUE);
  TEST_MATCH_COMMAND("A?", "A?", "", TRUE);
  TEST_MATCH_COMMAND("A", "A?", "", FALSE);
  TEST_MATCH_COMMAND("A?", "A", "", FALSE);
  TEST_MATCH_COMMAND("[:ABcc]:AACddd", ":ab:aac", "", TRUE);                   /* test optional keyword */
  TEST_MATCH_COMMAND("[:ABcc]:AACddd", "aac", "", TRUE);                       /* test optional keyword */
  TEST_MATCH_COMMAND("[:ABcc]:AACddd", "aac?", "", FALSE);                     /* test optional keyword */
  TEST_MATCH_COMMAND("[:ABcc]:AACddd?", ":ab:aac?", "", TRUE);                 /* test optional keyword */
  TEST_MATCH_COMMAND("[:ABcc]:AACddd?", "aac?", "", TRUE);                     /* test optional keyword */
  TEST_MATCH_COMMAND("[:ABcc]:AACddd?", "aac", "", FALSE);                     /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd]:CDEFGeeeee", "ab:bcc:cdefg", "", TRUE);   /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd]:CDEFGeeeee", "ab:cdefg", "", TRUE);       /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd]:CDEFGeeeee", "ab:cdefg?", "", FALSE);     /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd]:CDEFGeeeee?", "ab:bcc:cdefg?", "", TRUE); /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd]:CDEFGeeeee?", "ab:cdefg?", "", TRUE);     /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd]:CDEFGeeeee?", "ab:cdefg", "", FALSE);     /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc:BCCdddd[:CDEFGeeeee]", "ab:bcc:cdefg", "", TRUE);   /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc:BCCdddd[:CDEFGeeeee]", "ab:bcc", "", TRUE);         /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc:BCCdddd[:CDEFGeeeee]", "ab:bcc?", "", FALSE);       /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc:BCCdddd[:CDEFGeeeee]?", "ab:bcc:cdefg?", "", TRUE); /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc:BCCdddd[:CDEFGeeeee]?", "ab:bcc?", "", TRUE);       /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc:BCCdddd[:CDEFGeeeee]?", "ab:bcc", "", FALSE);       /* test optional keyword */

  TEST_MATCH_COMMAND("[:ABcc]:BCCdddd[:CDEFGeeeee]", "ab:bcc:cdefg", "", TRUE);   /* test optional keyword */
  TEST_MATCH_COMMAND("[:ABcc]:BCCdddd[:CDEFGeeeee]", "ab:bcc", "", TRUE);         /* test optional keyword */
  TEST_MATCH_COMMAND("[:ABcc]:BCCdddd[:CDEFGeeeee]", "bcc:cdefg", "", TRUE);      /* test optional keyword */
  TEST_MATCH_COMMAND("[:ABcc]:BCCdddd[:CDEFGeeeee]", "ab:bcc?", "", FALSE);       /* test optional keyword */
  TEST_MATCH_COMMAND("[:ABcc]:BCCdddd[:CDEFGeeeee]", "bcc:cdefg?", "", FALSE);    /* test optional keyword */
  TEST_MATCH_COMMAND("[:ABcc]:BCCdddd[:CDEFGeeeee]?", "ab:bcc:cdefg?", "", TRUE); /* test optional keyword */
  TEST_MATCH_COMMAND("[:ABcc]:BCCdddd[:CDEFGeeeee]?", "ab:bcc?", "", TRUE);       /* test optional keyword */
  TEST_MATCH_COMMAND("[:ABcc]:BCCdddd[:CDEFGeeeee]?", "bcc:cdefg?", "", TRUE);    /* test optional keyword */
  TEST_MATCH_COMMAND("[:ABcc]:BCCdddd[:CDEFGeeeee]?", "ab:bcc", "", FALSE);       /* test optional keyword */
  TEST_MATCH_COMMAND("[:ABcc]:BCCdddd[:CDEFGeeeee]?", "bcc:cdefg", "", FALSE);    /* test optional keyword */

  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee]", "ab:bcc:cdefg", "", TRUE);   /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee]", "ab:bcc", "", TRUE);         /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee]", "ab:cdefg", "", TRUE);       /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee]", "ab:bcc?", "", FALSE);       /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee]", "ab:cdefg?", "", FALSE);     /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee]?", "ab:bcc:cdefg?", "", TRUE); /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee]?", "ab:bcc?", "", TRUE);       /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee]?", "ab:cdefg?", "", TRUE);     /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee]?", "ab:bcc", "", FALSE);       /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee]?", "ab:cdefg", "", FALSE);     /* test optional keyword */

  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]",
                     "ab:bcc:cdefg:defffffffff",
                     "",
                     TRUE); /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]",
                     "ab:bcc:cdefg:deffffffffffffffffffff",
                     "",
                     TRUE); /* test optional keyword */
  TEST_MATCH_COMMAND(
    "ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]", "ab:bcc:cdefg:de", "", FALSE); /* test optional keyword
                                                                                            */
  TEST_MATCH_COMMAND(
    "ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]", "ab:bcc:cdefg", "", TRUE); /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]", "ab:bcc:defffffffff", "", TRUE); /* test
                     optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]",
                     "ab:cdefg:defffffffff",
                     "",
                     TRUE); /* test optional keyword */
  TEST_MATCH_COMMAND(
    "ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]", "ab:bcc", "", TRUE); /* test optional keyword */
  TEST_MATCH_COMMAND(
    "ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]", "ab:cdefg", "", TRUE); /* test optional keyword */
  TEST_MATCH_COMMAND(
    "ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]", "ab:defffffffff", "", TRUE); /* test optional keyword */
  TEST_MATCH_COMMAND(
    "ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]", "ab", "", TRUE); /* test optional keyword */
  TEST_MATCH_COMMAND(
    "ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]", "ab:bcc:cdefg?", "", FALSE); /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]",
                     "ab:bcc:defffffffff?",
                     "",
                     FALSE); /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]",
                     "ab:cdefg:defffffffff?",
                     "",
                     FALSE); /* test optional keyword */
  TEST_MATCH_COMMAND(
    "ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]", "ab:bcc?", "", FALSE); /* test optional keyword */
  TEST_MATCH_COMMAND(
    "ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]", "ab:cdefg?", "", FALSE); /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]", "ab:defffffffff?", "", FALSE); /* test
                     optional keyword
                     */
  TEST_MATCH_COMMAND(
    "ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]", "ab?", "", FALSE); /* test optional keyword */

  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]?",
                     "ab:bcc:cdefg:defffffffff?",
                     "",
                     TRUE); /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]?", "ab:bcc:cdefg?", "", TRUE); /* test
                     optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]?",
                     "ab:bcc:defffffffff?",
                     "",
                     TRUE); /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]?",
                     "ab:cdefg:defffffffff?",
                     "",
                     TRUE); /* test optional keyword */
  TEST_MATCH_COMMAND(
    "ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]?", "ab:bcc?", "", TRUE); /* test optional keyword */
  TEST_MATCH_COMMAND(
    "ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]?", "ab:cdefg?", "", TRUE); /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]?", "ab:defffffffff?", "", TRUE); /* test
                     optional keyword
                     */
  TEST_MATCH_COMMAND(
    "ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]?", "ab?", "", TRUE); /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]?", "ab:bcc:cdefg", "", FALSE); /* test
                     optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]?",
                     "ab:bcc:defffffffff",
                     "",
                     FALSE); /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]?",
                     "ab:cdefg:defffffffff",
                     "",
                     FALSE); /* test optional keyword */
  TEST_MATCH_COMMAND(
    "ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]?", "ab:bcc", "", FALSE); /* test optional keyword */
  TEST_MATCH_COMMAND(
    "ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]?", "ab:cdefg", "", FALSE); /* test optional keyword */
  TEST_MATCH_COMMAND("ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]?", "ab:defffffffff", "", FALSE); /* test
                     optional keyword
                     */
  TEST_MATCH_COMMAND(
    "ABcc[:BCCdddd][:CDEFGeeeee][:DEFFFFFFFFFfffffffffff]?", "ab", "", FALSE); /* test optional keyword */
  TEST_MATCH_COMMAND("*IDN?", "idn", "", FALSE);                               /* common command */
  TEST_MATCH_COMMAND("*IDN?", "idn?", "", FALSE);                              /* common command */
  TEST_MATCH_COMMAND("*IDN?", "*idn", "", FALSE);                              /* common command */
  TEST_MATCH_COMMAND("*IDN?", "*idn?", "", TRUE);                              /* common command */
  TEST_MATCH_COMMAND("*IDN?", ":idn", "", FALSE);                              /* common command */
  TEST_MATCH_COMMAND("*IDN?", ":idn?", "", FALSE);                             /* common command */
  TEST_MATCH_COMMAND("*IDN?", ":*idn", "", FALSE);                             /* common command */
  TEST_MATCH_COMMAND("*IDN?", ":*idn?", "", FALSE);                            /* common command */

  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]?", ":MEAS?", "", FALSE);
  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]?", "MEAS?", "", FALSE);
  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]?", ":MEAS:DC?", "", FALSE);
  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]?", "MEAS:DC?", "", FALSE);
  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]?", ":MEAS:CURR?", "", TRUE);
  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]?", "MEAS:CURR?", "", TRUE);
  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]?", ":MEAS:SCAL:CURR?", "", TRUE);
  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]?", "MEAS:SCAL:CURR?", "", TRUE);
  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]?", ":MEAS:SCAL:CURR:DC?", "", TRUE);
  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]?", "MEAS:SCAL:CURR:DC?", "", TRUE);
  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]?", ":MEAS:CURR:DC?", "", TRUE);
  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]?", "MEAS:CURR:DC?", "", TRUE);

  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]", ":MEAS", "", FALSE);
  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]", "MEAS", "", FALSE);
  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]", ":MEAS:DC", "", FALSE);
  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]", "MEAS:DC", "", FALSE);
  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]", ":MEAS:CURR", "", TRUE);
  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]", "MEAS:CURR", "", TRUE);
  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]", ":MEAS:SCAL:CURR", "", TRUE);
  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]", "MEAS:SCAL:CURR", "", TRUE);
  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]", ":MEAS:SCAL:CURR:DC", "", TRUE);
  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]", "MEAS:SCAL:CURR:DC", "", TRUE);
  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]", ":MEAS:CURR:DC", "", TRUE);
  TEST_MATCH_COMMAND("MEASure[:SCALar]:CURRent[:DC]", "MEAS:CURR:DC", "", TRUE);

  TEST_MATCH_COMMAND("ABCdef#", "abc", "", TRUE);         /* test numeric parameter */
  TEST_MATCH_COMMAND("ABCdef#", "abc1324", "", TRUE);     /* test numeric parameter */
  TEST_MATCH_COMMAND("ABCdef#", "abcDef1324", "", TRUE);  /* test numeric parameter */
  TEST_MATCH_COMMAND("ABCdef#", "abcDef124b", "", FALSE); /* test numeric parameter */

  TEST_MATCH_COMMAND("OUTPut#:MODulation#:FM#", "abc", "", FALSE);                 /* test numeric parameter */
  TEST_MATCH_COMMAND("OUTPut#:MODulation#:FM#", "outp1:mod10:fm", "", TRUE);       /* test numeric parameter */
  TEST_MATCH_COMMAND("OUTPut#:MODulation#:FM#", "output1:mod10:fm", "", TRUE);     /* test numeric parameter */
  TEST_MATCH_COMMAND("OUTPut#:MODulation#:FM#", "outp1:modulation:fm5", "", TRUE); /* test numeric parameter */
  TEST_MATCH_COMMAND("OUTPut#:MODulation#:FM#", "output:mod:fm", "", TRUE);        /* test numeric parameter */
  TEST_MATCH_COMMAND("OUTPut#:MODulation#:FM#", "outp1:mod10a:fm", "", FALSE);     /* test numeric parameter */
  TEST_MATCH_COMMAND("OUTPut#[:MODulation#]:FM#", "outp1:fm", "", TRUE);           /* test numeric parameter */
  TEST_MATCH_COMMAND("OUTPut#[:MODulation#]:FM#", "outp1:mod10:fm", "", TRUE);     /* test numeric parameter */
  TEST_MATCH_COMMAND("OUTPut#[:MODulation#]:FM#", "outp1:fm2", "", TRUE);          /* test numeric parameter */
  TEST_MATCH_COMMAND("OUTPut#[:MODulation#]:FM#", "output:fm", "", TRUE);          /* test numeric parameter */

  TEST_MATCH_COMMAND2("OUTPut#:MODulation#:FM#", "outp3:mod10:fm", "", TRUE, (3, 10, -1)); /* test numeric
  parameter
  */
  TEST_MATCH_COMMAND2(
    "OUTPut#:MODulation#:FM#", "output3:mod10:fm", "", TRUE, (3, 10, -1)); /* test numeric parameter */
  TEST_MATCH_COMMAND2("OUTPut#:MODulation#:FM#", "outp30:modulation:fm5", "", TRUE, (30, -1, 5)); /* test numeric
           parameter
           */
  TEST_MATCH_COMMAND2("OUTPut#:MODulation#:FM#", "output:mod:fm", "", TRUE, (-1, -1, -1));        /* test numeric
         parameter
         */
  TEST_MATCH_COMMAND2("OUTPut#[:MODulation#]:FM#", "outp3:fm", "", TRUE, (3, -1, -1));            /* test numeric
      parameter */
  TEST_MATCH_COMMAND2("OUTPut#[:MODulation#]:FM#", "outp3:mod10:fm", "", TRUE, (3, 10, -1));      /* test numeric
             parameter
             */
  TEST_MATCH_COMMAND2("OUTPut#[:MODulation#]:FM#", "outp3:fm2", "", TRUE, (3, -1, 2));            /* test numeric
           parameter
           */
  TEST_MATCH_COMMAND2("OUTPut#[:MODulation#]:FM#", "output:fm", "", TRUE, (-1, -1, -1));          /* test numeric
        parameter */

  TEST_MATCH_COMMAND2("OUTPut#:MODulation:FM#", "outp3:mod:fm", "", TRUE, (3, -1));          /* test numeric parameter
                                                                                              */
  TEST_MATCH_COMMAND2("OUTPut#:MODulation:FM#", "output3:mod:fm", "", TRUE, (3, -1));        /* test numeric
      parameter */
  TEST_MATCH_COMMAND2("OUTPut#:MODulation:FM#", "outp30:modulation:fm5", "", TRUE, (30, 5)); /* test numeric parameter
                                                                                              */
  TEST_MATCH_COMMAND2("OUTPut#:MODulation:FM#", "output:mod:fm", "", TRUE, (-1, -1));        /* test numeric parameter
                                                                                              */
  TEST_MATCH_COMMAND2("OUTPut#[:MODulation]:FM#", "outp3:fm", "", TRUE, (3, -1));            /* test numeric
      parameter */
  TEST_MATCH_COMMAND2("OUTPut#[:MODulation]:FM#", "outp3:mod:fm", "", TRUE, (3, -1));        /* test
numeric parameter */
  TEST_MATCH_COMMAND2("OUTPut#[:MODulation]:FM#", "outp3:fm2", "", TRUE, (3, 2));            /*
test numeric parameter */
  TEST_MATCH_COMMAND2("OUTPut#[:MODulation]:FM#", "output:fm", "", TRUE, (-1, -1));          /*
test numeric parameter */

  TEST_MATCH_COMMAND2("OUTPut#:MODulation#:FM", "outp3:mod10:fm", "", TRUE, (3, 10));        /* test numeric
       parameter */
  TEST_MATCH_COMMAND2("OUTPut#:MODulation#:FM", "output3:mod10:fm", "", TRUE, (3, 10));      /* test
numeric parameter */
  TEST_MATCH_COMMAND2("OUTPut#:MODulation#:FM", "outp30:modulation:fm", "", TRUE, (30, -1)); /* test numeric
         parameter */
  TEST_MATCH_COMMAND2("OUTPut#:MODulation#:FM", "output:mod:fm", "", TRUE, (-1, -1));        /* test numeric
       parameter */
  TEST_MATCH_COMMAND2("OUTPut#[:MODulation#]:FM", "outp3:fm", "", TRUE, (3, -1));            /* test
numeric parameter */
  TEST_MATCH_COMMAND2("OUTPut#[:MODulation#]:FM", "outp3:mod10:fm", "", TRUE, (3, 10));      /*
test numeric parameter */
  TEST_MATCH_COMMAND2("OUTPut#[:MODulation#]:FM", "outp3:fm", "", TRUE, (3, -1));            /*
test numeric parameter */
  TEST_MATCH_COMMAND2("OUTPut#[:MODulation#]:FM", "output:fm", "", TRUE, (-1, -1));          /*
test numeric parameter */
}

CU_Test(scpi_utils, swap)
{
#define TEST_SWAP(l, a, b)                                                                                             \
    do {                                                                                                               \
        CU_ASSERT_EQUAL(SCPI_Swap##l(a), b)                                                                            \
    } while (0)

    TEST_SWAP(16, 0x0011, 0x1100);
    TEST_SWAP(16, 0x1234, 0x3412);

    TEST_SWAP(32, 0x00000011, 0x11000000);
    TEST_SWAP(32, 0x00001234, 0x34120000);
    TEST_SWAP(32, 0x00AB1234, 0x3412AB00);
    TEST_SWAP(32, 0xCDAB1234, 0x3412ABCD);

    TEST_SWAP(64, 0x0000000000000011ull, 0x1100000000000000ull);
    TEST_SWAP(64, 0x0000000000001234ull, 0x3412000000000000ull);
    TEST_SWAP(64, 0x0000000000AB1234ull, 0x3412AB0000000000ull);
    TEST_SWAP(64, 0x00000000CDAB1234ull, 0x3412ABCD00000000ull);
    TEST_SWAP(64, 0x123456789ABCDEF0ull, 0xF0DEBC9A78563412ull);
}


CU_Test(scpi_utils, heap)
{
#if USE_DEVICE_DEPENDENT_ERROR_INFORMATION && !USE_MEMORY_ALLOCATION_FREE

#define ERROR_INFO_HEAP_LENGTH  16
    scpi_error_info_heap_t heap;
    char error_info_heap[ERROR_INFO_HEAP_LENGTH];

    scpiheap_init(&heap, error_info_heap, ERROR_INFO_HEAP_LENGTH);
    CU_ASSERT_EQUAL(heap.size, ERROR_INFO_HEAP_LENGTH);
    CU_ASSERT_EQUAL(heap.data, error_info_heap);
    CU_ASSERT_EQUAL(heap.count, heap.size);

    char * ptr1 = scpiheap_strndup(&heap, "abcd", 4);
    CU_ASSERT_STRING_EQUAL(ptr1, "abcd");

    char * ptr2 = scpiheap_strndup(&heap, "xyz", 3);
    CU_ASSERT_STRING_EQUAL(ptr2, "xyz");

    char * ptr3 = scpiheap_strndup(&heap, "ghijklmnop", 10);
    CU_ASSERT_EQUAL(ptr3, NULL);

    scpiheap_free(&heap, ptr1, false);

    char * ptr4 = scpiheap_strndup(&heap, "ghijklmnop", 10);
    CU_ASSERT_NOT_EQUAL(ptr4, NULL);

    const char * ptr5;
    size_t len1, len2;
    CU_ASSERT_EQUAL(scpiheap_get_parts(&heap, ptr4, &len1, &ptr5, &len2), TRUE);
    CU_ASSERT_EQUAL(len1, 7);
    CU_ASSERT_EQUAL(len2, 3);
    CU_ASSERT_STRING_EQUAL(ptr5, "nop");
    CU_ASSERT_EQUAL(memcmp(ptr4, "ghijklm", len1), 0);

    scpiheap_free(&heap, ptr2, false);

    char * ptr6 = scpiheap_strndup(&heap, "abcd", 4);
    CU_ASSERT_STRING_EQUAL(ptr6, "abcd");

    scpiheap_free(&heap, ptr6, true);

    char * ptr7 = scpiheap_strndup(&heap, "123456789", 9);
    CU_ASSERT_EQUAL(ptr7, NULL);

    char * ptr8 = scpiheap_strndup(&heap, "123456789", 4);
    CU_ASSERT_STRING_EQUAL(ptr8, "1234");

    scpiheap_free(&heap, ptr4, false);
    scpiheap_free(&heap, ptr8, false);
    CU_ASSERT_EQUAL(heap.count, heap.size);
#endif
}
