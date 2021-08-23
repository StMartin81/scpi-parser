/*-
 * BSD 2-Clause License
 *
 * Copyright (c) 2012-2018, Jan Breuer
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

#include <CUnit/Theory.h>
#include <stdio.h>
#include <stdlib.h>

#include "../src/fifo_private.h"

/*
 * CUnit Test Suite
 */

CU_Init(test_fifo)
{
  return 0;
}

CU_Clean(test_fifo)
{
  return 0;
}

CU_Test(test_fifo, test_fifo)
{
  scpi_fifo_t fifo;
  scpi_error_t fifo_data[4];
  fifo_init(&fifo, fifo_data, 4);
  scpi_error_t value;
  int16_t count_value;

#define TEST_FIFO_COUNT(n)                      \
    do {                                        \
        fifo_count(&fifo, &count_value);        \
        CU_ASSERT_EQUAL(count_value, n);        \
    } while(0)                                  \


    TEST_FIFO_COUNT(0);
    CU_ASSERT_TRUE(fifo_is_empty(&fifo))
    CU_ASSERT_FALSE(fifo_is_full(&fifo))

    value.error_code = 1;
    CU_ASSERT_TRUE(fifo_add(&fifo, &value))
    TEST_FIFO_COUNT(1);
    CU_ASSERT_FALSE(fifo_is_empty(&fifo))
    CU_ASSERT_FALSE(fifo_is_full(&fifo))

    value.error_code = 2;
    CU_ASSERT_TRUE(fifo_add(&fifo, &value))
    TEST_FIFO_COUNT(2);
    CU_ASSERT_FALSE(fifo_is_empty(&fifo))
    CU_ASSERT_FALSE(fifo_is_full(&fifo))

    value.error_code = 3;
    CU_ASSERT_TRUE(fifo_add(&fifo, &value))
    TEST_FIFO_COUNT(3);
    CU_ASSERT_FALSE(fifo_is_empty(&fifo))
    CU_ASSERT_FALSE(fifo_is_full(&fifo))

    value.error_code = 4;
    CU_ASSERT_TRUE(fifo_add(&fifo, &value))
    TEST_FIFO_COUNT(4);
    CU_ASSERT_FALSE(fifo_is_empty(&fifo))
    CU_ASSERT_TRUE(fifo_is_full(&fifo))

    CU_ASSERT_EQUAL(fifo.data[0].error_code, 1)
    CU_ASSERT_EQUAL(fifo.data[1].error_code, 2)
    CU_ASSERT_EQUAL(fifo.data[2].error_code, 3)
    CU_ASSERT_EQUAL(fifo.data[3].error_code, 4)

    value.error_code = 5;
    CU_ASSERT_FALSE(fifo_add(&fifo, &value))
    TEST_FIFO_COUNT(4);
    CU_ASSERT_FALSE(fifo_is_empty(&fifo))
    CU_ASSERT_TRUE(fifo_is_full(&fifo))

    CU_ASSERT_EQUAL(fifo.data[0].error_code, 1)
    CU_ASSERT_EQUAL(fifo.data[1].error_code, 2)
    CU_ASSERT_EQUAL(fifo.data[2].error_code, 3)
    CU_ASSERT_EQUAL(fifo.data[3].error_code, 4)

    CU_ASSERT_TRUE(fifo_remove_last(&fifo, &value))
    CU_ASSERT_EQUAL(value.error_code, 4)
    TEST_FIFO_COUNT(3);
    CU_ASSERT_FALSE(fifo_is_empty(&fifo))
    CU_ASSERT_FALSE(fifo_is_full(&fifo))

    value.error_code = 6;
    CU_ASSERT_TRUE(fifo_add(&fifo, &value))
    TEST_FIFO_COUNT(4);
    CU_ASSERT_FALSE(fifo_is_empty(&fifo))
    CU_ASSERT_TRUE(fifo_is_full(&fifo))

    CU_ASSERT_EQUAL(fifo.data[0].error_code, 1)
    CU_ASSERT_EQUAL(fifo.data[1].error_code, 2)
    CU_ASSERT_EQUAL(fifo.data[2].error_code, 3)
    CU_ASSERT_EQUAL(fifo.data[3].error_code, 6)

    CU_ASSERT_TRUE(fifo_remove(&fifo, &value))
    CU_ASSERT_EQUAL(value.error_code, 1)
    TEST_FIFO_COUNT(3);
    CU_ASSERT_FALSE(fifo_is_empty(&fifo))
    CU_ASSERT_FALSE(fifo_is_full(&fifo))

    value.error_code = 7;
    CU_ASSERT_TRUE(fifo_add(&fifo, &value))
    TEST_FIFO_COUNT(4);

    CU_ASSERT_TRUE(fifo_remove(&fifo, &value))
    CU_ASSERT_EQUAL(value.error_code, 2)
    TEST_FIFO_COUNT(3);

    CU_ASSERT_TRUE(fifo_remove(&fifo, &value))
    CU_ASSERT_EQUAL(value.error_code, 3)
    TEST_FIFO_COUNT(2);

    value.error_code = 10;
    CU_ASSERT_TRUE(fifo_add(&fifo, &value))
    TEST_FIFO_COUNT(3);

    value.error_code = 11;
    CU_ASSERT_TRUE(fifo_add(&fifo, &value))
    TEST_FIFO_COUNT(4);

    CU_ASSERT_TRUE(fifo_remove(&fifo, &value))
    CU_ASSERT_EQUAL(value.error_code, 6)
    TEST_FIFO_COUNT(3);

    CU_ASSERT_TRUE(fifo_remove(&fifo, &value))
    CU_ASSERT_EQUAL(value.error_code, 7)
    TEST_FIFO_COUNT(2);

    CU_ASSERT_TRUE(fifo_remove_last(&fifo, &value))
    CU_ASSERT_EQUAL(value.error_code, 11)
    TEST_FIFO_COUNT(1);

    CU_ASSERT_TRUE(fifo_remove(&fifo, &value))
    CU_ASSERT_EQUAL(value.error_code, 10)
    TEST_FIFO_COUNT(0);

    CU_ASSERT_FALSE(fifo_remove(&fifo, &value))
    TEST_FIFO_COUNT(0);

    CU_ASSERT_FALSE(fifo_remove_last(&fifo, NULL))
}
