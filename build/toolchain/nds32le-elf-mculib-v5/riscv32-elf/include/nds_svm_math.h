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
#ifndef __NDS_SVM_MATH_H__
#define __NDS_SVM_MATH_H__

/**
 * @defgroup svm SVM Functions
 */
#ifdef  __cplusplus
extern "C"
{
#endif

#include "riscv_dsp_svm_math.h"

// SVM linear prediction
#define nds_svm_linear_f32_t riscv_dsp_svm_linear_f32_t
#define nds_svm_linear_est_f32 riscv_dsp_svm_linear_est_f32

// SVM Sigmoid prediction
#define nds_svm_sigmoid_f32_t riscv_dsp_svm_sigmoid_f32_t
#define nds_svm_sigmoid_est_f32 riscv_dsp_svm_sigmoid_est_f32

// SVM rbf prediction
#define nds_svm_rbf_f32_t riscv_dsp_svm_rbf_f32_t
#define nds_svm_rbf_est_f32 riscv_dsp_svm_rbf_est_f32

// SVM polynomial prediction
#define nds_svm_poly_f32_t riscv_dsp_svm_poly_f32_t
#define nds_svm_poly_est_f32 riscv_dsp_svm_poly_est_f32


#ifdef  __cplusplus
}
#endif
#endif // __NDS_BASIC_MATH_H__
