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

#include "tensorflow/lite/c/builtin_op_data.h"
#include "tensorflow/lite/c/common.h"
#include "tensorflow/lite/kernels/internal/tensor_ctypes.h"
#include "tensorflow/lite/kernels/kernel_util.h"
#include "tensorflow/lite/micro/kernels/kernel_util.h"

namespace tflite {
namespace ops {
namespace micro {
namespace unpack {
namespace {

constexpr int kInputTensor = 0;

template <typename T>
TfLiteStatus UnpackImpl(TfLiteContext* context, TfLiteNode* node,
                        const TfLiteEvalTensor* input, int output_count,
                        int axis) {
  const TfLiteEvalTensor* output0 =
      tflite::micro::GetEvalOutput(context, node, 0);
  const TfLiteIntArray* input_dims = input->dims;
  const TfLiteIntArray* output_dims = output0->dims;
  const int dimensions = input_dims->size;

  if (axis < 0) {
    axis += input->dims->size;
  }

  TFLITE_DCHECK_LT(axis, dimensions);

  int outer_size = 1;
  for (int i = 0; i < axis; ++i) {
    outer_size *= input_dims->data[i];
  }
  int copy_size = 1;
  for (int i = axis + 1; i < dimensions; ++i) {
    copy_size *= input_dims->data[i];
  }
  int output_size = 1;
  for (int i = 0; i < output_dims->size; ++i) {
    output_size *= output_dims->data[i];
  }
  TFLITE_DCHECK_EQ(output_size, copy_size * outer_size);

  const T* input_data = tflite::micro::GetTensorData<T>(input);

  for (int i = 0; i < output_count; ++i) {
    TfLiteEvalTensor* t = tflite::micro::GetEvalOutput(context, node, i);
    T* output_data = tflite::micro::GetTensorData<T>(t);
    for (int k = 0; k < outer_size; ++k) {
      T* output_ptr = output_data + copy_size * k;
      int loc = k * output_count * copy_size + i * copy_size;
      const T* input_ptr = input_data + loc;
      for (int j = 0; j < copy_size; ++j) output_ptr[j] = input_ptr[j];
    }
  }

  return kTfLiteOk;
}

TfLiteStatus Eval(TfLiteContext* context, TfLiteNode* node) {
  TfLiteUnpackParams* data =
      reinterpret_cast<TfLiteUnpackParams*>(node->builtin_data);

  const TfLiteEvalTensor* input =
      tflite::micro::GetEvalInput(context, node, kInputTensor);

  switch (input->type) {
    case kTfLiteFloat32: {
      return UnpackImpl<float>(context, node, input, data->num, data->axis);
    }
    case kTfLiteInt32: {
      return UnpackImpl<int32_t>(context, node, input, data->num, data->axis);
    }
    case kTfLiteUInt8: {
      return UnpackImpl<uint8_t>(context, node, input, data->num, data->axis);
    }
    case kTfLiteInt8: {
      return UnpackImpl<int8_t>(context, node, input, data->num, data->axis);
    }
    default: {
      TF_LITE_KERNEL_LOG(context, "Type '%s' is not supported by unpack.",
                         TfLiteTypeGetName(input->type));
      return kTfLiteError;
    }
  }

  return kTfLiteOk;
}
}  // namespace
}  // namespace unpack

TfLiteRegistration Register_UNPACK() {
  return {/*init=*/nullptr,
          /*free=*/nullptr,
          /*prepare=*/nullptr,
          /*invoke=*/unpack::Eval,
          /*profiling_string=*/nullptr,
          /*builtin_code=*/0,
          /*custom_name=*/nullptr,
          /*version=*/0};
}

}  // namespace micro
}  // namespace ops
}  // namespace tflite

#endif // end of #if !defined(ESP32)