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
#ifndef __NDS_BASIC_MATH_H__
#define __NDS_BASIC_MATH_H__

/**
 * @defgroup basic Basic Functions
 */
#ifdef  __cplusplus
extern "C"
{
#endif

#include "riscv_dsp_basic_math.h"

// Absolute value
/// Absolute value of floating-potint vectors
#define nds_abs_f32   riscv_dsp_abs_f32

/// Absolute value of Q31 vectors.
#define nds_abs_q31   riscv_dsp_abs_q31

/// Absolute value of Q15 vectors.
#define nds_abs_q15   riscv_dsp_abs_q15

/// Absolute value of Q7 vectors.
#define nds_abs_q7    riscv_dsp_abs_q7

// Addition
/// Addition of floating-potint vectors.
#define nds_add_f32   riscv_dsp_add_f32

/// Addition of Q31 vectors.
#define nds_add_q31   riscv_dsp_add_q31

/// Addition of Q15 vectors.
#define nds_add_q15   riscv_dsp_add_q15

/// Addition of Q7 vectors.
#define nds_add_q7    riscv_dsp_add_q7

/// Addition of U8 vectors.
#define nds_add_u8_u16 riscv_dsp_add_u8_u16

// Subtraction
/// Subtraction of floating-point vectors.
#define nds_sub_f32   riscv_dsp_sub_f32

/// Subtraction of Q31 vectors.
#define nds_sub_q31   riscv_dsp_sub_q31

/// Subtraction of Q15 vectors.
#define nds_sub_q15 riscv_dsp_sub_q15

/// Subtraction of Q7 vectors.
#define nds_sub_q7  riscv_dsp_sub_q7

/// Subtraction of u8 vectors.
#define nds_sub_u8_q7 riscv_dsp_sub_u8_q7

// Multiplication
/// Multiplication of floating-point vectors.
#define nds_mul_f32 riscv_dsp_mul_f32

/// Multiplication of Q31 vectors.
#define nds_mul_q31 riscv_dsp_mul_q31

/// Multiplication of Q15 vectors.
#define nds_mul_q15 riscv_dsp_mul_q15

/// Multiplication of Q7 vectors.
#define nds_mul_q7  riscv_dsp_mul_q7

/// Multiplication of u8 vectors.
#define nds_mul_u8_u16 riscv_dsp_mul_u8_u16

// Division
/// Division of floating-point vectors.
#define nds_div_f32 riscv_dsp_div_f32

/// Division of q31 inputs.
#define nds_div_q31 riscv_dsp_div_q31

/// Division of q63 inputs divided by a positive 32 bits.
#define nds_div_s64_u32 riscv_dsp_div_s64_u32

/// Division of positive 64-bits inputs divided by a positive 32-bits.
#define nds_div_u64_u32 riscv_dsp_div_u64_u32

// Negation
/// Negation of floating-potint vectors.
#define nds_neg_f32 riscv_dsp_neg_f32

/// Negation of q31 vectors.
#define nds_neg_q31 riscv_dsp_neg_q31

/// Negation of q15 vectors.
#define nds_neg_q15 riscv_dsp_neg_q15

/// Negation of q15 vectors.
#define nds_neg_q7 riscv_dsp_neg_q7

// Dot Production
/// Dot production of floating-point vectors.
#define nds_dprod_f32 riscv_dsp_dprod_f32

/// Dot production of q31 vectors.
#define nds_dprod_q31 riscv_dsp_dprod_q31

/// Dot production of q15 vectors.
#define nds_dprod_q15 riscv_dsp_dprod_q15

/// Dot production of u8 * q15 vectors.
#define nds_dprod_u8xq15    riscv_dsp_dprod_u8xq15

/// Dot production of q7 vectors.
#define nds_dprod_q7    riscv_dsp_dprod_q7
#define nds_dprod_q7xq15 riscv_dsp_dprod_q7xq15

/// Dot production of u8 vectors.
#define nds_dprod_u8    riscv_dsp_dprod_u8

// Offset
/// The offset of floating-point vectors.
#define nds_offset_f32 riscv_dsp_offset_f32

/// The offset of q31 vectors.
#define nds_offset_q31 riscv_dsp_offset_q31

/// The offset of q15 vectors.
#define nds_offset_q15 riscv_dsp_offset_q15

/// The offset of q7 vectors.
void nds_offset_q7(q7_t *src, q7_t offset, q7_t *dst, uint32_t size);
#define nds_offset_q7  riscv_dsp_offset_q7

/// The offset of U8 vectors.
#define nds_offset_u8 riscv_dsp_offset_u8

// Scale
/// To multiply a floating-point vectors by a floating-point scale.
#define nds_scale_f32 riscv_dsp_scale_f32

/// To multiply a q31 vectors by a q31 scale.
#define nds_scale_q31 riscv_dsp_scale_q31

/// To multiply a q15 vectors by a q15 scale.
#define nds_scale_q15 riscv_dsp_scale_q15

/// To multiply a q7 vectors by a q7 scale.
#define nds_scale_q7 riscv_dsp_scale_q7

/// To multiply a u8 vectors by a q7 scale.
#define nds_scale_u8 riscv_dsp_scale_u8

// Shift
/// Shifts a q15 vector with a specified shift number.
#define nds_shift_q15 riscv_dsp_shift_q15

/// Shifts a q31 vector with a specified shift number.
#define nds_shift_q31 riscv_dsp_shift_q31

/// Shifts a q7 vector with a specified shift number.
#define nds_shift_q7 riscv_dsp_shift_q7

/// Shifts a u8 vector for a specified shift number.
#define nds_shift_u8 riscv_dsp_shift_u8

#ifdef  __cplusplus
}
#endif
#endif // __NDS_BASIC_MATH_H__
