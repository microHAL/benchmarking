/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    pawel
 * created on: 22-06-2017
 * last modification: 22-06-2017
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

#ifndef _MICROHAL_OPTIONALTEST_H_
#define _MICROHAL_OPTIONALTEST_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#if defined(OPTIONAL_TEST)
#include <cstdint>
#include <experimental/optional>

#define ADDRESS 0x20000000

/* **************************************************************************************************************************************************
 * CLASS
 */
class Test {
  template <typename T>
  using optional = std::experimental::optional<T>;

 public:
  enum class Error : uint8_t { None, Serious };

#if !defined(TEST_DIFFERENT_TRANSATION_UNIT)
  auto functionReturningUint8_t() {
    uint8_t data = *((uint8_t *)ADDRESS);

    if (data > 100)
      return optional<uint8_t>{data};
    else
      return optional<uint8_t>{};
  }

  auto functionReturningUint32_t() {
    uint32_t data = *((uint32_t *)ADDRESS);

    if (data > 100)
      return optional<uint32_t>{data};
    else
      return optional<uint32_t>{};
  }

  auto functionReturningUint64_t() {
    uint64_t data = *((uint64_t *)ADDRESS);

    if (data > 100)
      return optional<uint64_t>{data};
    else
      return optional<uint64_t>{};
  }

  auto functionReturningFloat() {
    float data = *((float *)ADDRESS);

    if (data > 100)
      return optional<float>{data};
    else
      return optional<float>{};
  }
#else
  optional<uint8_t> functionReturningUint8_t();
  optional<uint32_t> functionReturningUint32_t();
  optional<uint64_t> functionReturningUint64_t();
  optional<float> functionReturningFloat();
#endif
};
#endif
#endif  // _MICROHAL_OPTIONALTEST_H_
