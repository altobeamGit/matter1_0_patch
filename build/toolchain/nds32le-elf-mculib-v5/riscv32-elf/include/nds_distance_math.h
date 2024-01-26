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
#ifndef __NDS_DISTANCE_MATH_H__
#define __NDS_DISTANCE_MATH_H__

/**
 * @defgroup dist Distance Functions
 */
#ifdef  __cplusplus
extern "C"
{
#endif

#include "riscv_dsp_distance_math.h"

// Bray-Curtis distance between two vectors
#define  nds_dist_bray_curtis_f32  riscv_dsp_dist_bray_curtis_f32

// Canberra distance between two vectors
#define nds_dist_canberra_f32 riscv_dsp_dist_canberra_f32

// Chebyshev distance between two vectors
#define  nds_dist_chebyshev_f32    riscv_dsp_dist_chebyshev_f32

// Cityblock (Manhattan) distance between two vectors
#define nds_dist_city_block_f32 riscv_dsp_dist_city_block_f32

// Correlation distance between two vectors
#define nds_dist_corr_f32 riscv_dsp_dist_corr_f32

// Cosine distance between two vectors
#define nds_dist_cos_f32 riscv_dsp_dist_cos_f32

// Euclidean distance between two vectors
#define nds_dist_euclidean_f32 riscv_dsp_dist_euclidean_f32

// Jensen-Shannon distance between two vectors
#define nds_dist_jensen_shannon_f32 riscv_dsp_dist_jensen_shannon_f32

// Minkowski distance between two vectors
#define nds_dist_minkowski_f32 riscv_dsp_dist_minkowski_f32

// Dice distance between two vectors
#define nds_bdist_dice_u32_f32 riscv_dsp_bdist_dice_u32_f32

// Hamming distance between two vectors
#define nds_bdist_hamming_u32_f32 riscv_dsp_bdist_hamming_u32_f32

// Jaccard distance between two vectors
#define nds_bdist_jaccard_u32_f32  riscv_dsp_bdist_jaccard_u32_f32

// Kulsinski distance between two vectors
#define nds_bdist_kulsinski_u32_f32 riscv_dsp_bdist_kulsinski_u32_f32

// Sokal-Michener distance between two vectors
#define nds_bdist_sokal_michener_u32_f32 riscv_dsp_bdist_sokal_michener_u32_f32

// Sokal-Sneath distance between two vectors
#define nds_bdist_sokal_sneath_u32_f32 riscv_dsp_bdist_sokal_sneath_u32_f32

// Roger Stanimoto distance between two vectors
#define nds_bdist_rogers_tanimoto_u32_f32 riscv_dsp_bdist_rogers_tanimoto_u32_f32

// Yule distance between two vectors
#define nds_bdist_yule_u32_f32 riscv_dsp_bdist_yule_u32_f32

// Russell-Rao distance between two vectors
#define nds_bdist_russell_rao_u32_f32 riscv_dsp_bdist_russell_rao_u32_f32

#ifdef  __cplusplus
}
#endif
#endif
