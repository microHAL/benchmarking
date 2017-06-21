/**
 * @license    BSD 3-Clause
 * @copyright  microHAL
 * @version    $Id$
 * @brief
 *
 * @authors    pawel
 * created on: 20-06-2017
 * last modification: 20-06-2017
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

#ifndef _MICROHAL_RETURNERRORBYARGUMENT_H_
#define _MICROHAL_RETURNERRORBYARGUMENT_H_
/* **************************************************************************************************************************************************
 * INCLUDES
 */
#include <cstdint>

/* **************************************************************************************************************************************************
 * CLASS
 */
class Test {
 public:
  enum class Error { None, Serious };

#if !defined(TEST_DIFFERENT_TRANSATION_UNIT)
  Error functionReturningUint8_t(uint8_t &data) {
    data = *((uint8_t *)0x156884);
    if (data > 10.0) return Error::Serious;

    return Error::None;
  }

  Error functionReturningUint32_t(uint32_t &data) {
    data = *((uint32_t *)0x156884);
    if (data > 10.0) return Error::Serious;

    return Error::None;
  }

  Error functionReturningUint64_t(uint64_t &data) {
    data = *((uint64_t *)0x156884);
    if (data > 10.0) return Error::Serious;

    return Error::None;
  }

  Error functionReturningFloat(float &data) {
    data = *((float *)0x156884);
    if (data > 10.0) return Error::Serious;

    return Error::None;
  }
#else
  Error functionReturningUint8_t(uint8_t &data);
  Error functionReturningUint32_t(uint32_t &data);
  Error functionReturningUint64_t(uint64_t &data);
  Error functionReturningFloat(float &data);
#endif
};

#endif  // _MICROHAL_RETURNERRORBYARGUMENT_H_
