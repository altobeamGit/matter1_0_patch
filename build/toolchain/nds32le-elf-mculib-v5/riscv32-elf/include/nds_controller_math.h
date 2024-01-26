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
#ifndef __NDS_CONTROLLER_MATH_H__
#define __NDS_CONTROLLER_MATH_H__

/**
 * @defgroup controller Controller Functions
 */
#ifdef  __cplusplus
extern "C"
{
#endif

#include "riscv_dsp_controller_math.h"

// Clarke Transform
//
/// Clarke transform of floating-point input.
#define nds_clarke_f32 riscv_dsp_clarke_f32

/// Clarke transform of q31 input.
#define nds_clarke_q31 riscv_dsp_clarke_q31

/// Inverse Clarke transform of floating-point input.
#define nds_inv_clarke_f32 riscv_dsp_inv_clarke_f32

/// Inverse Clarke transform of q31 input.
#define nds_inv_clarke_q31 riscv_dsp_inv_clarke_q31

// Park Transform
//
/// Park transform of floating-point input.
#define nds_park_f32 riscv_dsp_park_f32

/// Park transform of q31 input.
#define nds_park_q31 riscv_dsp_park_q31

/// Inverse Park transform of floating-point input.
#define nds_inv_park_f32 riscv_dsp_inv_park_f32

/// Inverse Park transform of q31 input.
#define nds_inv_park_q31 riscv_dsp_inv_park_q31

// PID Control
//
/// Structure for the floating-point PID Control.
#define nds_pid_f32_t riscv_dsp_pid_f32_t

/// PID control of floating-point input.
#define nds_pid_f32 riscv_dsp_pid_f32

/// PID initializatopn control function of floating-point formats.
#define nds_init_pid_f32 riscv_dsp_init_pid_f32

/// Structure for the Q31 PID Control.
#define nds_pid_q31_t riscv_dsp_pid_q31_t

/// PID control of Q31 input.
#define nds_pid_q31 riscv_dsp_pid_q31

/// PID initializatopn control function of Q31 formats.
#define nds_init_pid_q31 riscv_dsp_init_pid_q31

/// Structure for the Q15 PID Control.
#define nds_pid_q15_t riscv_dsp_pid_q15_t

/// PID control of Q15 input.
#define nds_pid_q15 riscv_dsp_pid_q15

/// PID initializatopn control function of Q15 formats.
#define nds_init_pid_q15 riscv_dsp_init_pid_q15

#ifdef  __cplusplus
}
#endif
#endif // __NDS_TRANSFORM_MATH_H__
