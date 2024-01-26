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
#ifndef __NDS_STATISTICS_MATH_H__
#define __NDS_STATISTICS_MATH_H__

/**
 * @defgroup statistics Statistics Functions
 */
#ifdef  __cplusplus
extern "C"
{
#endif

#include "riscv_dsp_statistics_math.h"

// Maximum
//
/// Maximum value of the floating-potint vector.
#define nds_max_f32 riscv_dsp_max_f32
#define nds_max_val_f32 riscv_dsp_max_val_f32

/// Maximum value of the q15 vector.
#define nds_max_q15 riscv_dsp_max_q15

/// Maximum value of the q31 vector.
#define nds_max_q31 riscv_dsp_max_q31

/// Maximum value of the q7 vector.
#define nds_max_q7  riscv_dsp_max_q7

/// Maximum value of the u8 vector.
#define nds_max_u8 riscv_dsp_max_u8

// Minimum
//
/// Minimum value of the floating-potint vector.
#define nds_min_f32 riscv_dsp_min_f32

/// Minimum value of the q15 vector.
#define nds_min_q15 riscv_dsp_min_q15

/// Minimum value of the q31 vector.
#define nds_min_q31 riscv_dsp_min_q31

/// Minimum value of the q7 vector.
#define nds_min_q7  riscv_dsp_min_q7

/// Minimum value of the u8 vector.
#define nds_min_u8  riscv_dsp_min_u8

// Mean
//
/// Mean value of the floating-potint vector.
#define nds_mean_f32 riscv_dsp_mean_f32

/// Mean value of the q15 vector.
#define nds_mean_q15 riscv_dsp_mean_q15

/// Mean value of the q31 vector.
#define nds_mean_q31 riscv_dsp_mean_q31

/// Mean value of the q7 vector.
#define nds_mean_q7  riscv_dsp_mean_q7

/// Mean value of the u8 vector.
#define nds_mean_u8  riscv_dsp_mean_u8

// Sun of the Squares
//
/// Sum of the squares of the floating-potint vector.
#define nds_pwr_f32 riscv_dsp_pwr_f32
#define nds_sos_f32 riscv_dsp_pwr_f32

/// Sum of the squares of the q15 vector.
#define nds_pwr_q15 riscv_dsp_pwr_q15
#define nds_sos_q15 riscv_dsp_pwr_q15

/// Sum of the squares of the q31 vector.
#define nds_pwr_q31 riscv_dsp_pwr_q31
#define nds_sos_q31 riscv_dsp_pwr_q31

/// Sum of the squares of the q7 vector.
#define nds_pwr_q7  riscv_dsp_pwr_q7
#define nds_sos_q7  riscv_dsp_pwr_q7

// Root Mean Square
//
/// RMS of the floating-potint vector.
#define nds_rms_f32 riscv_dsp_rms_f32

/// RMS of the q15 vector.
#define nds_rms_q15 riscv_dsp_rms_q15

/// RMS of the q31 vector.
#define nds_rms_q31 riscv_dsp_rms_q31

// Standard deviation
//
/// Standard deviation of the floating-potint vector.
#define nds_std_f32 riscv_dsp_std_f32

/// Standard deviation of the q15 vector.
#define nds_std_q15 riscv_dsp_std_q15

/// Standard deviation of the q31 vector.
#define nds_std_q31 riscv_dsp_std_q31

/// Standard deviation of the u8 vector.
#define nds_std_u8  riscv_dsp_std_u8

// Variance
//
/// Variance of the floating-potint vector.
#define nds_var_f32 riscv_dsp_var_f32

/// Variance of the q15 vector.
#define nds_var_q15 riscv_dsp_var_q15

/// Variance of the q31 vector.
#define nds_var_q31 riscv_dsp_var_q31

#define nds_entropy_f32 riscv_dsp_entropy_f32
#define nds_relative_entropy_f32 riscv_dsp_relative_entropy_f32
#define nds_lse_dprod_f32 riscv_dsp_lse_dprod_f32
#define nds_lse_f32 riscv_dsp_lse_f32
#define nds_gaussian_naivebayes_f32_t riscv_dsp_gaussian_naivebayes_f32_t
#define nds_gaussian_naive_bayes_est_f32 riscv_dsp_gaussian_naive_bayes_est_f32

#ifdef  __cplusplus
}
#endif
#endif // __NDS_STATISTICS_MATH_H__
