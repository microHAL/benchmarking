/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    pawel
 * created on: 13-06-2017
 * last modification: 13-06-2017
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
#if defined(ERROR_BY_ARGUMENT_TEST)
  Test::Error error;

  uint8_t u8 = test.functionReturningUint8_t(&error);
  if (error != Test::Error::None) {
  } else {
    printError(error);
  }

  uint32_t u32 = test.functionReturningUint32_t(&error);
  if (error != Test::Error::None) {
  } else {
    printError(error);
  }

  uint32_t u64 = test.functionReturningUint64_t(&error);
  if (error != Test::Error::None) {
  } else {
    printError(error);
  }

  float fl = test.functionReturningFloat(&error);
  if (error != Test::Error::None) {
  } else {
    printError(error);
  }
#endif

#if defined(RETURN_ERROR_TEST)
  uint8_t u8;
  auto result = test.functionReturningUint8_t(u8);
  if (result == Test::Error::None) {
    // printf("%d", u8_t);
  } else {
    printError(result);
  }

  uint32_t u32;
  result = test.functionReturningUint32_t(u32);
  if (result == Test::Error::None) {
  } else {
    printError(result);
  }

  uint64_t u64;
  result = test.functionReturningUint64_t(u64);
  if (result == Test::Error::None) {
  } else {
    printError(result);
  }

  float fl;
  result = test.functionReturningFloat(fl);
  if (result == Test::Error::None) {
  } else {
    printError(result);
  }
#endif

#if defined(OPTIONAL_TEST)
  if (auto result = test.functionReturningUint8_t()) {
    uint8_t tt = *result;
  } else {
    printError(Test::Error::Serious);
  }
  if (auto result = test.functionReturningUint32_t()) {
    uint32_t tt = *result;
  } else {
    printError(Test::Error::Serious);
  }
  if (auto result = test.functionReturningUint64_t()) {
    uint64_t tt = *result;
  } else {
    printError(Test::Error::Serious);
  }
  if (auto result = test.functionReturningFloat()) {
    float tt = *result;
  } else {
    printError(Test::Error::Serious);
  }
#endif

#if defined(RESULT_TEST)
  if (auto result = test.functionReturningUint8_t()) {
    uint8_t tt = *result;
  } else {
    printError(result.error());
  }
  if (auto result = test.functionReturningUint32_t()) {
    uint32_t tt = *result;
  } else {
    printError(result.error());
  }
  if (auto result = test.functionReturningUint64_t()) {
    uint64_t tt = *result;
  } else {
    printError(result.error());
  }
  if (auto result = test.functionReturningFloat()) {
    float tt = *result;
  } else {
    printError(result.error());
  }
#endif
}
