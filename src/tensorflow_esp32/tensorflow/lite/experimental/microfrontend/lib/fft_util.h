#if defined(ESP32)
/* Copyright 2018 The TensorFlow Authors. All Rights Reserved.

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
#ifndef TENSORFLOW_LITE_EXPERIMENTAL_MICROFRONTEND_LIB_FFT_UTIL_H_
#define TENSORFLOW_LITE_EXPERIMENTAL_MICROFRONTEND_LIB_FFT_UTIL_H_

#include "tensorflow_esp32/tensorflow/lite/experimental/microfrontend/lib/fft.h"

#ifdef __cplusplus
extern "C" {
#endif

// Prepares and FFT for the given input size.
int FftPopulateState(struct FftState* state, size_t input_size);

// Frees any allocated buffers.
void FftFreeStateContents(struct FftState* state);

#ifdef __cplusplus
}  // extern "C"
#endif

#endif  // TENSORFLOW_LITE_EXPERIMENTAL_MICROFRONTEND_LIB_FFT_UTIL_H_

#endif // end of #if defined(ESP32)