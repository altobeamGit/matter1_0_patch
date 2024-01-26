/******************************************************************************
 * Copyright (C) 2010-2018 Arm Limited or its affiliates. All rights reserved.*
 * Copyright (C) 2018-2021 Andes Technology Corporation. All rights reserved. *
 *                                                                            *
 * SPDX-License-Identifier: Apache-2.0                                        *
 *                                                                            *
 * Licensed under the Apache License, Version 2.0 (the License); you may      *
 * not use this file except in compliance with the License.                   *
 * You may obtain a copy of the License at                                    *
 *                                                                            *
 * www.apache.org/licenses/LICENSE-2.0                                        *
 *                                                                            *
 * Unless required by applicable law or agreed to in writing, software        *
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT    *
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.           *
 * See the License for the specific language governing permissions and        *
 * limitations under the License.                                             *
 ******************************************************************************/
#ifndef __NDS32_UTILS_MATH_H__
#define __NDS32_UTILS_MATH_H__

/**
 * @defgroup utils Utils Functions
 * This set of functions implements sine, cosine, arctanm, and square root.
 * There are separate functions for Q15, Q31, and floating-point data.
 */
#ifdef  __cplusplus
extern "C"
{
#endif

#include "riscv_dsp_utils_math.h"

// Cosine and Sine
#define nds32_cos_f32 riscv_dsp_cos_f32
#define nds32_cos_q31 riscv_dsp_cos_q31
#define nds32_cos_q15 riscv_dsp_cos_q15

#define nds32_sin_f32 riscv_dsp_sin_f32
#define nds32_sin_q31 riscv_dsp_sin_q31
#define nds32_sin_q15 riscv_dsp_sin_q15

// Arc tangent
#define nds32_atan_f32 riscv_dsp_atan_f32
#define nds32_atan_q31 riscv_dsp_atan_q31
#define nds32_atan_q15 riscv_dsp_atan_q15
#define nds32_atan2_f32 riscv_dsp_atan2_f32
#define nds32_atan2_q15 riscv_dsp_atan2_q15
#define nds32_atan2_q31 riscv_dsp_atan2_q31

// Square Root
#define nds32_sqrt_f32 riscv_dsp_sqrt_f32
#define nds32_sqrt_q31 riscv_dsp_sqrt_q31
#define nds32_sqrt_q15 riscv_dsp_sqrt_q15

// Convert function
#define nds32_convert_f32_q15 riscv_dsp_convert_f32_q15
#define nds32_convert_f32_q31 riscv_dsp_convert_f32_q31
#define nds32_convert_f32_q7  riscv_dsp_convert_f32_q7
#define nds32_convert_q15_f32 riscv_dsp_convert_q15_f32
#define nds32_convert_q15_q31 riscv_dsp_convert_q15_q31
#define nds32_convert_q15_q7  riscv_dsp_convert_q15_q7
#define nds32_convert_q31_f32 riscv_dsp_convert_q31_f32
#define nds32_convert_q31_q15 riscv_dsp_convert_q31_q15
#define nds32_convert_q31_q7  riscv_dsp_convert_q31_q7
#define nds32_convert_q7_f32  riscv_dsp_convert_q7_f32
#define nds32_convert_q7_q15  riscv_dsp_convert_q7_q15
#define nds32_convert_q7_q31  riscv_dsp_convert_q7_q31

// Duplicate function
#define nds32_dup_f32 riscv_dsp_dup_f32
#define nds32_dup_q15 riscv_dsp_dup_q15
#define nds32_dup_q31 riscv_dsp_dup_q31
#define nds32_dup_q7  riscv_dsp_dup_q7

// Set function
#define nds32_set_f32 riscv_dsp_set_f32
#define nds32_set_q15 riscv_dsp_set_q15
#define nds32_set_q31 riscv_dsp_set_q31
#define nds32_set_q7  riscv_dsp_set_q7

#ifdef  __cplusplus
}
#endif
#endif // __NDS32_UTILS_MATH_H__
