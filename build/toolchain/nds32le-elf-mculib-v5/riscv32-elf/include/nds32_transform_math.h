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
#ifndef __NDS32_TRANSFORM_MATH_H__
#define __NDS32_TRANSFORM_MATH_H__

/**
 * @defgroup transform Transform Functions
 */
#ifdef  __cplusplus
extern "C"
{
#endif

#include "riscv_dsp_transform_math.h"

// CFFT/CIFFT-Radix2
//
/// Floating-point Radix-2 Complex FFT/IFFT Function
#define nds32_cfft_rd2_f32     riscv_dsp_cfft_rd2_f32
#define nds32_cfft_radix2_f32  riscv_dsp_cfft_rd2_f32
#define nds32_cifft_rd2_f32    riscv_dsp_cifft_rd2_f32
#define nds32_cifft_radix2_f32 riscv_dsp_cifft_rd2_f32

/// Q15 Radix-2 Complex FFT/IFFT Function
#define nds32_cfft_rd2_q15     riscv_dsp_cfft_rd2_q15
#define nds32_cfft_radix2_q15  riscv_dsp_cfft_rd2_q15
#define nds32_cifft_rd2_q15    riscv_dsp_cifft_rd2_q15
#define nds32_cifft_radix2_q15 riscv_dsp_cifft_rd2_q15

/// Q31 Radix-2 Complex FFT/IFFT Function
#define nds32_cfft_rd2_q31     riscv_dsp_cfft_rd2_q31
#define nds32_cfft_radix2_q31  riscv_dsp_cfft_rd2_q31
#define nds32_cifft_rd2_q31    riscv_dsp_cifft_rd2_q31
#define nds32_cifft_radix2_q31 riscv_dsp_cifft_rd2_q31

// CFFT/IFFT-Radix4
//
/// Floating-point Radix-4 Complex FFT/IFFT Function
#define nds32_cfft_rd4_f32     riscv_dsp_cfft_rd4_f32
#define nds32_cfft_radix4_f32  riscv_dsp_cfft_rd4_f32
#define nds32_cifft_rd4_f32    riscv_dsp_cifft_rd4_f32
#define nds32_cifft_radix4_f32 riscv_dsp_cifft_rd4_f32

/// Q15 Radix-4 Complex FFT/IFFT Function
#define nds32_cfft_rd4_q15     riscv_dsp_cfft_rd4_q15
#define nds32_cfft_radix4_q15  riscv_dsp_cfft_rd4_q15
#define nds32_cifft_rd4_q15    riscv_dsp_cifft_rd4_q15
#define nds32_cifft_radix4_q15 riscv_dsp_cifft_rd4_q15

/// Q31 Radix-4 Complex FFT/IFFT Function
#define nds32_cfft_rd4_q31     riscv_dsp_cfft_rd4_q31
#define nds32_cfft_radix4_q31  riscv_dsp_cfft_rd4_q31
#define nds32_cifft_rd4_q31    riscv_dsp_cifft_rd4_q31
#define nds32_cifft_radix4_q31 riscv_dsp_cifft_rd4_q31

// CFFT/CIFFT
//
/// Floating-point Complex FFT/IFFT Function
#define nds32_cfft_f32 riscv_dsp_cfft_f32
#define nds32_cifft_f32 riscv_dsp_cifft_f32

/// Q15 Complex FFT/IFFT Function
#define nds32_cfft_q15 riscv_dsp_cfft_q15
#define nds32_cifft_q15 riscv_dsp_cifft_q15

/// Q31 Complex FFT/IFFT Function
#define nds32_cfft_q31 riscv_dsp_cfft_q31
#define nds32_cifft_q31 riscv_dsp_cifft_q31

// Real-FFT/IFFT
//
/// Floating-point Real FFT/IFFT Function
#define nds32_rfft_f32  riscv_dsp_rfft_f32
#define nds32_rifft_f32 riscv_dsp_rifft_f32

/// Q15 Real FFT/IFFT Function
#define nds32_rfft_q15  riscv_dsp_rfft_q15
#define nds32_rifft_q15 riscv_dsp_rifft_q15

/// Q31 Real FFT/IFFT Function
#define nds32_rfft_q31  riscv_dsp_rfft_q31
#define nds32_rifft_q31 riscv_dsp_rifft_q31

// DCT/IDCT Type II
//
/// Floating-point DCT/IDCT Type II Function
#define nds32_dct_f32 riscv_dsp_dct_f32
#define nds32_idct_f32 riscv_dsp_idct_f32

/// Q15 DCT/IDCT Type II Function
#define nds32_dct_q15 riscv_dsp_dct_q15
#define nds32_idct_q15 riscv_dsp_idct_q15

/// Q31 DCT/IDCT Type II Function
#define nds32_dct_q31 riscv_dsp_dct_q31
#define nds32_idct_q31 riscv_dsp_idct_q31

// DCT/IDCT Type IV
//
/// Floating-point DCT/IDCT Type IV Function
#define nds32_dct4_f32 riscv_dsp_dct4_f32
#define nds32_idct4_f32 riscv_dsp_idct4_f32

/// Q15 DCT/IDCT Type IV Function
#define nds32_dct4_q15 riscv_dsp_dct4_q15
#define nds32_idct4_q15 riscv_dsp_idct4_q15

/// Q31 DCT/IDCT Type IV Function
#define nds32_dct4_q31 riscv_dsp_dct4_q31
#define nds32_idct4_q31 riscv_dsp_idct4_q31

#ifdef  __cplusplus
}
#endif
#endif // __NDS32_TRANSFORM_MATH_H__
