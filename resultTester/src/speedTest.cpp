/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    pawel
 * created on: 26-06-2017
 * last modification: 26-06-2017
 *
 * @copyright Copyright (c) 2017, microHAL
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 *     2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 *        documentation and/or other materials provided with the distribution.
 *     3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 *        software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <cstdio>
#include <experimental/optional>
#include "microhal_bsp.h"

#include "functions.h"

#if defined(ERROR_BY_ARGUMENT_TEST)
#include "returnErrorByArgument.h"
#endif

#if defined(RETURN_ERROR_TEST)
#include "returnError.h"
#endif

#if defined(OPTIONAL_TEST)
#include "optionalTest.h"
#endif

#if defined(RESULT_TEST)
#include "returnErrorByResultClass.h"
#endif

volatile Test::Error err;
void printError(Test::Error error) { err = error; }

int main() {
  Test test;

  uint32_t i = 100;

  uint32_t ticks = 999999;
  SysTick_Config(ticks);

  uint32_t start = SysTick->VAL;

  //  asm volatile("nop");
  //  asm volatile("nop");
  //  asm volatile("nop");
  //  asm volatile("nop");

  while (i--) {
#if defined(ERROR_BY_ARGUMENT_TEST)
    Test::Error error;

#if defined(UINT8T_TEST)
    uint8_t u8 = test.functionReturningUint8_t(&error);
    if (error != Test::Error::None) {
      dataFunc(u8);
    } else {
      printError(error);
    }
#endif
#if defined(UINT32T_TEST)
    uint32_t u32 = test.functionReturningUint32_t(&error);
    if (error != Test::Error::None) {
      dataFunc(u32);
    } else {
      printError(error);
    }
#endif
#if defined(UINT64T_TEST)
    uint32_t u64 = test.functionReturningUint64_t(&error);
    if (error != Test::Error::None) {
      dataFunc(u64);
    } else {
      printError(error);
    }
#endif
#if defined(FLOAT_TEST)
    float fl = test.functionReturningFloat(&error);
    if (error != Test::Error::None) {
      dataFunc(fl);
    } else {
      printError(error);
    }
#endif
#endif

#if defined(RETURN_ERROR_TEST)
    Test::Error result;
#if defined(UINT8T_TEST)
    uint8_t u8;
    result = test.functionReturningUint8_t(u8);
    if (result == Test::Error::None) {
      dataFunc(u8);
    } else {
      printError(result);
    }
#endif
#if defined(UINT32T_TEST)
    uint32_t u32;
    result = test.functionReturningUint32_t(u32);
    if (result == Test::Error::None) {
      dataFunc(u32);
    } else {
      printError(result);
    }
#endif
#if defined(UINT64T_TEST)
    uint64_t u64;
    result = test.functionReturningUint64_t(u64);
    if (result == Test::Error::None) {
      dataFunc(u64);
    } else {
      printError(result);
    }
#endif
#if defined(FLOAT_TEST)
    float fl;
    result = test.functionReturningFloat(fl);
    if (result == Test::Error::None) {
      dataFunc(fl);
    } else {
      printError(result);
    }
#endif
#endif

#if defined(OPTIONAL_TEST)
#if defined(UINT8T_TEST)
    if (auto result = test.functionReturningUint8_t()) {
      dataFunc(*result);
    } else {
      printError(Test::Error::Serious);
    }
#endif
#if defined(UINT32T_TEST)
    if (auto result = test.functionReturningUint32_t()) {
      dataFunc(*result);
    } else {
      printError(Test::Error::Serious);
    }
#endif
#if defined(UINT64T_TEST)
    if (auto result = test.functionReturningUint64_t()) {
      dataFunc(*result);
    } else {
      printError(Test::Error::Serious);
    }
#endif
#if defined(FLOAT_TEST)
    if (auto result = test.functionReturningFloat()) {
      dataFunc(*result);
    } else {
      printError(Test::Error::Serious);
    }
#endif
#endif

#if defined(RESULT_TEST)
#if defined(UINT8T_TEST)
    if (auto result = test.functionReturningUint8_t()) {
      dataFunc(*result);
    } else {
      printError(result.error());
    }
#endif
#if defined(UINT32T_TEST)
    if (auto result = test.functionReturningUint32_t()) {
      dataFunc(*result);
    } else {
      printError(result.error());
    }
#endif
#if defined(UINT64T_TEST)
    if (auto result = test.functionReturningUint64_t()) {
      dataFunc(*result);
    } else {
      printError(result.error());
    }
#endif
#if defined(FLOAT_TEST)
    if (auto result = test.functionReturningFloat()) {
      dataFunc(*result);
    } else {
      printError(result.error());
    }
#endif
#endif
  }

  uint32_t stop = SysTick->VAL;
  auto cyclesCount = start - stop - 1;
  char buffer[30];
  sprintf(buffer, "%d\n", cyclesCount);
  debugPort.write("Result class benchmarking.\n");
  debugPort.write(buffer);
  // debugPort.write("\n");
  while (1)
    ;
}
