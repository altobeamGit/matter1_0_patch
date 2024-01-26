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
#ifndef __NDS_FILTERING_MATH_H__
#define __NDS_FILTERING_MATH_H__

/**
 * @defgroup filtering Filtering Functions
 */


#ifdef  __cplusplus
extern "C"
{
#endif

#include "riscv_dsp_filtering_math.h"

// Standard FIR filter
//
/// Structure for the floating-point standard FIR filter.
#define nds_fir_f32_t riscv_dsp_fir_f32_t

/// Function for the floating-point FIR filter.
#define nds_fir_f32 riscv_dsp_fir_f32

/// Structure for the Q31 standard FIR filter.
#define  nds_fir_q31_t riscv_dsp_fir_q31_t

/// Function for the q31 FIR filter.
#define nds_fir_q31 riscv_dsp_fir_q31

/// Function for the q31 FIR filter.
#define nds_fir_fast_q31 riscv_dsp_fir_fast_q31

/// Structure for the Q15 standard FIR filter.
#define nds_fir_q15_t riscv_dsp_fir_q15_t

/// Function for the q15 FIR filter.
#define nds_fir_q15 riscv_dsp_fir_q15

/// Function for the q15 FIR fast filter.
#define nds_fir_fast_q15 riscv_dsp_fir_fast_q15

/// Structure for the Q7 standard FIR Filters.
#define nds_fir_q7_t riscv_dsp_fir_q7_t

/// Function for the q31 FIR filter.
#define nds_fir_q7 riscv_dsp_fir_q7

// Lattice FIR filter
//
/// Structure for the floating-point Lattice FIR filter.
#define nds_lfir_f32_t riscv_dsp_lfir_f32_t

/// Function for the floating-point lattice FIR filter.
#define nds_lfir_f32 riscv_dsp_lfir_f32

/// Structure for the q15 Lattice FIR filter.
#define nds_lfir_q15_t riscv_dsp_lfir_q15_t

/// Function for the q15 lattice FIR filter.
#define nds_lfir_q15 riscv_dsp_lfir_q15

/// Structure for the q31 Lattice FIR filter.
#define nds_lfir_q31_t riscv_dsp_lfir_q31_t

/// Function for the q31 lattice FIR filter.
#define nds_lfir_q31 riscv_dsp_lfir_q31

// Decimator FIR Filters
//
/// Structure for the floating-point Decimator FIR Filters.
#define nds_dcmfir_f32_t riscv_dsp_dcmfir_f32_t

/// Function for the floating-point Decimator FIR Filters.
#define nds_dcmfir_f32 riscv_dsp_dcmfir_f32

/// Structure for the q15 Decimator FIR Filters.
#define nds_dcmfir_q15_t riscv_dsp_dcmfir_q15_t

/// Function for the q15 Decimator FIR Filters.
#define nds_dcmfir_q15 riscv_dsp_dcmfir_q15
#define nds_dcmfir_fast_q15 riscv_dsp_dcmfir_fast_q15

/// Structure for the q31 Decimator FIR Filters.
#define nds_dcmfir_q31_t riscv_dsp_dcmfir_q31_t

/// Function for the q31 Decimator FIR Filters.
#define nds_dcmfir_q31 riscv_dsp_dcmfir_q31
#define nds_dcmfir_fast_q31 riscv_dsp_dcmfir_fast_q31

// Upsampling FIR Filters
//
/// Structure for the floating-point Upsampling FIR Filters.
#define nds_upsplfir_f32_t riscv_dsp_upsplfir_f32_t

/// Function for the floating-point Upsampling FIR Filters.
#define nds_upsplfir_f32 riscv_dsp_upsplfir_f32

/// Structure for the Q15 Upsampling FIR Filters.
#define nds_upsplfir_q15_t riscv_dsp_upsplfir_q15_t

/// Function for the Q15 Upsampling FIR Filters.
#define nds_upsplfir_q15 riscv_dsp_upsplfir_q15

/// Structure for the Q31 Upsampling FIR Filters.
#define nds_upsplfir_q31_t riscv_dsp_upsplfir_q31_t

/// Function for the Q31 Upsampling FIR Filters.
#define nds_upsplfir_q31 riscv_dsp_upsplfir_q31

// Sparse FIR Filters
//
/// Structure for the flaoting-point Sparse FIR Filters.
#define nds_spafir_f32_t riscv_dsp_spafir_f32_t

/// Function for the flaoting-point Sparse FIR Filters.
#define nds_spafir_f32 riscv_dsp_spafir_f32

/// Structure for the Q15 Sparse FIR Filters.
#define nds_spafir_q15_t riscv_dsp_spafir_q15_t

/// Function for the Q15 Sparse FIR Filters.
#define nds_spafir_q15 riscv_dsp_spafir_q15

/// Structure for the Q31 Sparse FIR Filters.
#define nds_spafir_q31_t riscv_dsp_spafir_q31_t

/// Function for the Q31 Sparse FIR Filters.
#define nds_spafir_q31 riscv_dsp_spafir_q31

/// Structure for the Q7 Sparse FIR Filters.
#define nds_spafir_q7_t riscv_dsp_spafir_q7_t

/// Function for the Q7 Sparse FIR Filters.
#define nds_spafir_q7 riscv_dsp_spafir_q7

// Standard LMS filte
//
/// Structure for the floatint-point standard LMS Filters.
#define nds_lms_f32_t riscv_dsp_lms_f32_t

/// Function for the floating-point LMS filter.
#define nds_lms_f32 riscv_dsp_lms_f32

/// Structure for the Q31 standard LMS Filters.
#define nds_lms_q31_t riscv_dsp_lms_q31_t

/// Function for the q31 LMS filter.
#define nds_lms_q31 riscv_dsp_lms_q31

/// Structure for the Q15 standard LMS Filters.
#define nds_lms_q15_t riscv_dsp_lms_q15_t

/// Function for the q15 LMS filter.
#define nds_lms_q15 riscv_dsp_lms_q15

// Normalized LMS filter
//
/// Structure for the f32 normalized LMS filter.
#define nds_nlms_f32_t riscv_dsp_nlms_f32_t

/// Function for the f32 normalized LMS filter.
#define nds_nlms_f32 riscv_dsp_nlms_f32

/// Structure for the q31 normalized LMS filter.
#define nds_nlms_q31_t riscv_dsp_nlms_q31_t

/// Function for the q31 normalized LMS filter.
#define nds_nlms_q31 riscv_dsp_nlms_q31

/// Structure for the q15 normalized LMS filter.
#define nds_nlms_q15_t riscv_dsp_nlms_q15_t

/// Function for the q15 normalized LMS filter.
#define nds_nlms_q15 riscv_dsp_nlms_q15

//  Convolution
//
/// Convolution of the floating-point vectors.
#define nds_conv_f32 riscv_dsp_conv_f32

/// Convolution of the q15 vectors.
#define nds_conv_q15 riscv_dsp_conv_q15

/// Convolution of the q31 vectors.
#define nds_conv_q31 riscv_dsp_conv_q31

/// Convolution of the q7 vectors.
#define nds_conv_q7 riscv_dsp_conv_q7

/// Partial Convolution of the floating-point vectors.
#define nds_conv_partial_f32 riscv_dsp_conv_partial_f32

/// Partial Convolution of the q15 vectors.
#define nds_conv_partial_q15 riscv_dsp_conv_partial_q15

/// Partial Convolution of the q31 vectors.
#define nds_conv_partial_q31 riscv_dsp_conv_partial_q31

/// Partial Convolution of the q7 vectors.
#define nds_conv_partial_q7 riscv_dsp_conv_partial_q7

//  Correlation
//
/// Correlation of the floating-point vectors.
#define nds_corr_f32 riscv_dsp_corr_f32

/// Correlation of the q15 vectors.
#define nds_corr_q15 riscv_dsp_corr_q15

/// Correlation of the q31 vectors.
#define nds_corr_q31 riscv_dsp_corr_q31

/// Correlation of the q7 vectors.
#define nds_corr_q7 riscv_dsp_corr_q7


// Biquad Filter
//
/// Structure for the f32 Biquad Filter.
#define nds_bq_df1_f32_t     riscv_dsp_bq_df1_f32_t

/// Function for the f32 Biquad Filter.
#define nds_bq_df1_f32     riscv_dsp_bq_df1_f32

/// Structure for the direct transposed form II Biquad Cascade floating-point filter.
#define nds_bq_df2T_f32_t     riscv_dsp_bq_df2T_f32_t

/// Function for the direct transposed form II Biquad Cascade floating-point filter.
#define nds_bq_df2T_f32     riscv_dsp_bq_df2T_f32

/// Structure for the stereo direct transposed form II Biquad Cascade floating-point filter.
#define nds_bq_stereo_df2T_f32_t     riscv_dsp_bq_stereo_df2T_f32_t

/// Function for the stereo direct transposed form II Biquad Cascade floating-point filter.
#define nds_bq_stereo_df2T_f32     riscv_dsp_bq_stereo_df2T_f32

/// Structure for the f64 Biquad Filter.
#define nds_bq_df2T_f64_t     riscv_dsp_bq_df2T_f64_t

/// Function for the f64 Biquad Filter.
#define nds_bq_df2T_f64     riscv_dsp_bq_df2T_f64

/// Structure for the q15 Biquad Filter.
#define nds_bq_df1_q15_t     riscv_dsp_bq_df1_q15_t

/// Function for the q15 Biquad Filter.
#define nds_bq_df1_q15     riscv_dsp_bq_df1_q15
#define nds_bq_df1_fast_q15 riscv_dsp_bq_df1_fast_q15

/// Structure for the q31 Biquad Filter.
#define nds_bq_df1_q31_t     riscv_dsp_bq_df1_q31_t

/// Function for the q31 Biquad Filter.
#define nds_bq_df1_q31     riscv_dsp_bq_df1_q31
#define nds_bq_df1_fast_q31 riscv_dsp_bq_df1_fast_q31

/// Structure for the 32x64 Q31 biquadratic cascade filter
#define nds_bq_df1_32x64_q31_t     riscv_dsp_bq_df1_32x64_q31_t

/// Function for the 32x64 Q31 biquadratic cascade filter
#define nds_bq_df1_32x64_q31     riscv_dsp_bq_df1_32x64_q31

// Lattice IIR filter
//
/// Structure for the floating-point lattice IIR filter.
#define nds_liir_f32_t riscv_dsp_liir_f32_t

/// Function for the floating-point lattice IIR filter.
#define nds_liir_f32 riscv_dsp_liir_f32

/// Structure for the q31 lattice IIR filter.
#define nds_liir_q31_t riscv_dsp_liir_q31_t

/// Function for the q31 lattice IIR filter.
#define nds_liir_q31 riscv_dsp_liir_q31

/// Function for the q31 lattice fast IIR filter.
#define nds_liir_fast_q31 riscv_dsp_liir_fast_q31

/// Structure for the q15 lattice IIR filter.
#define nds_liir_q15_t riscv_dsp_liir_q15_t

/// Function for the q15 lattice IIR filter.
#define nds_liir_q15 riscv_dsp_liir_q15

/// Function for the q15 lattice fast IIR filter.
#define nds_liir_fast_q15 riscv_dsp_liir_fast_q15

#ifdef  __cplusplus
}
#endif
#endif // __NDS_FILTERING_MATH_H__
