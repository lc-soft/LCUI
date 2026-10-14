/*
 * src/widgets/spinner_priv.h: internal frame-advance math for the spinner
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef SRC_WIDGETS_SPINNER_PRIV_H
#define SRC_WIDGETS_SPINNER_PRIV_H

/* Advance a rotation angle by a duration. This is the whole of the spinner's
 * clock path, extracted as a pure function so it can be unit-tested without a
 * running app. It wraps the result to [0, 360) so repeated frames never grow
 * the angle unbounded. */
double ui_spinner_step_angle(double cur, double speed_rps, double dt_sec);

#endif /* SRC_WIDGETS_SPINNER_PRIV_H */
