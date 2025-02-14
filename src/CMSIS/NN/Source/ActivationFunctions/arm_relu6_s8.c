#if !defined(ESP32)
/*
 * Copyright (C) 2010-2019 Arm Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* ----------------------------------------------------------------------
 * Project:      CMSIS NN Library
 * Title:        arm_relu6_s8.c
 * Description:  Basic s8 version of ReLU6
 *
 * $Date:        Spetember 2019
 * $Revision:    V.1.0.0
 *
 * Target Processor:  Cortex-M cores
 *
 * -------------------------------------------------------------------- */

#include "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/DSP/Include/arm_math.h"
#include "tensorflow/lite/micro/tools/make/downloads/cmsis/CMSIS/NN/Include/arm_nnfunctions.h"

/**
 *  @ingroup groupNN
 */

/**
 * @addtogroup Acti
 * @{
 */

  /*
   *  Basic ReLU6 function
   *
   * Refer to header file for details.
   *
   */

void arm_relu6_s8(q7_t *data, uint16_t size)
{
    int32_t i;

    for (i = 0; i < size; i++)
    {
        int32_t ip = data[i];

        ip = MAX(ip, 0);
        data[i] = MIN(ip, 6);
    }
}

/**
 * @} end of Acti group
 */

#endif // end of #if !defined(ESP32)