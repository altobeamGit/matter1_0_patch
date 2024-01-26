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
#ifndef __NDS_MATRIX_MATH_H__
#define __NDS_MATRIX_MATH_H__

/**
 * @defgroup matrix Matrix Functions
 *
 * This set of functions provides basic matrix math operations.
 * The funciotn specifies the size of the matrix and then points to an array.
 * For example,
 * the function definition for the floating-point is shown below:
 * <pre>
 *     void nds_funcname_f32(const float32_t *src1,
 *                             const float32_t *src2,
 *                                   float32_t *dst,
 *                                   uint32_t row,
 *                                   uint32_t col,
 *                                   uint32_t row2,
 *                                   uint32_t col2)
 * </pre>
 * where it can be transform to the two matrix. For the matrix 1 is a
 * <code>row * col</code> matrix and the matrix 2 is a
 * <code>rol2 * col2</code> and the output matrix woild be different since
 * the math operation. There are similar definitions for Q15 and Q31 data types.
 */

#ifdef  __cplusplus
extern "C"
{
#endif

#include "riscv_dsp_matrix_math.h"

// Matrix Addition
//
/// Addition of two floating-potint matrices.
#define nds_mat_add_f32 riscv_dsp_mat_add_f32

/// Addition of two q15 matrices.
#define nds_mat_add_q15 riscv_dsp_mat_add_q15

/// Addition of two q31 matrices.
#define nds_mat_add_q31 riscv_dsp_mat_add_q31

// Matrix Inverse
//
/// Compute the inverse matrix of the floating-potint matrix.
#define nds_mat_inv_f32 riscv_dsp_mat_inv_f32
#define nds_mat_inv_f64 riscv_dsp_mat_inv_f64

// Matrix Multiplication
//
/// Multiplication of two floating-point matrices.
#define nds_mat_mul_f32 riscv_dsp_mat_mul_f32
#define nds_mat_mul_f64 riscv_dsp_mat_mul_f64

/// Multiplication of two q15 matrices.
#define nds_mat_mul_q15 riscv_dsp_mat_mul_q15
#define nds_mat_mul_fast_q15 riscv_dsp_mat_mul_fast_q15

/// Multiplication of two q31 matrices.
#define nds_mat_mul_q31 riscv_dsp_mat_mul_q31
#define nds_mat_mul_fast_q31 riscv_dsp_mat_mul_fast_q31
#define nds_mat_oprod_q31 riscv_dsp_mat_oprod_q31

/// Multiplication of two q7 matrices.
#define nds_mat_mul_q7 riscv_dsp_mat_mul_q7
#define nds_mat_mul_vxm_q7 riscv_dsp_mat_mul_vxm_q7

/// Multiplication of two complex matrices.
#define nds_cmat_mul_f32 riscv_dsp_cmat_mul_f32
#define nds_cmat_mul_q15 riscv_dsp_cmat_mul_q15
#define nds_cmat_mul_q31 riscv_dsp_cmat_mul_q31

// Matrix Power 2 Function
//
/// Matrix Power 2 Function
#define nds_mat_pwr2_cache_f64 riscv_dsp_mat_pwr2_cache_f64
#define nds_mat_pow2_cache_f64 riscv_dsp_mat_pwr2_cache_f64

// Matrix Scale
//
/// Multiplt a scale value of floating-potint matrix.
#define nds_mat_scale_f32 riscv_dsp_mat_scale_f32

/// Multiplt a scale value of q15 matrix.
#define nds_mat_scale_q15 riscv_dsp_mat_scale_q15

/// Multiplt a scale value of q31 matrix.
#define nds_mat_scale_q31 riscv_dsp_mat_scale_q31

// Matrix Subtraction
//
/// Substraction of two floating-potint matrices.
#define nds_mat_sub_f32 riscv_dsp_mat_sub_f32

/// Substraction of two q15 matrices.
#define nds_mat_sub_q15 riscv_dsp_mat_sub_q15

/// Substraction of two q31 matrices.
#define nds_mat_sub_q31 riscv_dsp_mat_sub_q31

// Matrix Transpose
//
/// Transpose the floating-potint matricex.
#define nds_mat_trans_f32 riscv_dsp_mat_trans_f32

/// Transpose the q15 matricex.
#define nds_mat_trans_q15 riscv_dsp_mat_trans_q15

/// Transpose the q31 matricex.
#define nds_mat_trans_q31 riscv_dsp_mat_trans_q31

///  Transpose the u8 matricex.
#define nds_mat_trans_u8 riscv_dsp_mat_trans_u8

#ifdef  __cplusplus
}
#endif
#endif // __NDS_MATRIX_MATH_H__
