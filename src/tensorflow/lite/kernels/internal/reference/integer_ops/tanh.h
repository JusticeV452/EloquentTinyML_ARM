#if !defined(ESP32)
/* Copyright 2019 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/
#ifndef TENSORFLOW_LITE_KERNELS_INTERNAL_REFERENCE_INTEGER_OPS_TANH_H_
#define TENSORFLOW_LITE_KERNELS_INTERNAL_REFERENCE_INTEGER_OPS_TANH_H_

#include <limits>

#include "third_party/gemmlowp/fixedpoint/fixedpoint.h"
#include "tensorflow/lite/kernels/internal/common.h"

namespace tflite {
namespace reference_integer_ops {

inline void Tanh(int32_t input_zero_point, int32_t input_range_radius,
                 int32_t input_multiplier, int32_t input_shift,
                 const RuntimeShape& input_shape, const int8_t* input_data,
                 const RuntimeShape& output_shape, int8_t* output_data) {
  // Integer bits must be in sync with Prepare() function.
  static constexpr int32_t kInputIntegerBits = 4;
  static constexpr int32_t kOutputScale = 7;
  static constexpr int32_t kMinInt8 = std::numeric_limits<int8_t>::min();
  static constexpr int32_t kMaxInt8 = std::numeric_limits<int8_t>::max();
  using F4 = gemmlowp::FixedPoint<int32_t, kInputIntegerBits>;

  const int flat_size = MatchingFlatSize(input_shape, output_shape);

  for (int i = 0; i < flat_size; ++i) {
    const int32_t input =
        static_cast<int32_t>(input_data[i]) - input_zero_point;
    if (input <= -input_range_radius) {
      output_data[i] = kMinInt8;
    } else if (input >= input_range_radius) {
      output_data[i] = kMaxInt8;
    } else {
      const int32_t input_in_q4 =
          MultiplyByQuantizedMultiplier(input, input_multiplier, input_shift);
      const int32_t output_in_q0 =
          gemmlowp::tanh(F4::FromRaw(input_in_q4)).raw();

      // Rescale and downcast.
      using gemmlowp::RoundingDivideByPOT;
      int32_t output_in_q24 =
          RoundingDivideByPOT(output_in_q0, 31 - kOutputScale);
      output_in_q24 = std::min(std::max(output_in_q24, kMinInt8), kMaxInt8);
      output_data[i] = static_cast<int8_t>(output_in_q24);
    }
  }
}

inline void Tanh(int32_t input_multiplier, int32_t input_left_shift,
                 const RuntimeShape& input_shape, const int16_t* ptr_input_data,
                 const RuntimeShape& output_shape, int16_t* ptr_output_data) {
  // We use the LUT for sigmoid and take into account, that
  // tanh(x) = 2*sigmoid(2*x) - 1

  int32_t input_data_mul = (input_multiplier > 0) ? input_multiplier : 1;

  int flat_size = MatchingFlatSize(input_shape, output_shape);

  for (int i = 0; i < flat_size; ++i, ptr_input_data++, ptr_output_data++) {
    int32_t input_data = (*ptr_input_data) * input_data_mul;

    if (input_left_shift == 1) {
      input_data <<= 1;
    }

    // Scale by 3/4 to expand range [-8,8]->[-10.7,10.7].
    uint32_t abs_input_data = 3 * abs(input_data);
    uint32_t uh = abs_input_data >> 8;
    int32_t result;

    if (uh >= 255) {
      // Saturate to maximum.
      result = 0xFFFF << 8;
    } else {
      uint32_t ua = sigmoid_table_uint16[uh];
      uint32_t ub = sigmoid_table_uint16[uh + 1];

      uint8_t ut = abs_input_data & 0xFF;

      result = (ua << 8) + ut * (ub - ua);
    }

    result = (input_data >= 0)
                 ? (result - (1 << (14 + 9)) + (1 << (9 - 2)))
                 : (-result + (1 << (14 + 9)) + (1 << (9 - 2)) - 1);

    // Convert back to 16-bit.
    result >>= (9 - 1);

    *ptr_output_data = result;
  }
}

}  // namespace reference_integer_ops
}  // namespace tflite

#endif  // TENSORFLOW_LITE_KERNELS_INTERNAL_REFERENCE_INTEGER_OPS_TANH_H_

#endif // end of #if !defined(ESP32)