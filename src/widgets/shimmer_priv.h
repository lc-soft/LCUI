/*
 * src/widgets/shimmer_priv.h: internal frame-advance math for the shimmer
 *
 * Copyright (c) 2026, Liu Chao <hello@lcui.dev> All rights reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * This file is part of LCUI, distributed under the MIT License found
 * in the LICENSE.TXT file in the root directory of this source tree.
 */

#ifndef SRC_WIDGETS_SHIMMER_PRIV_H
#define SRC_WIDGETS_SHIMMER_PRIV_H

/* Advance the highlight band's phase by an elapsed time. `ms_elapsed` is the
 * milliseconds since the previous frame and `duration_ms` the time for one full
 * sweep, so the band advances ms_elapsed / duration_ms of a cycle. This is the
 * whole of the shimmer's clock path, extracted as a pure function so it can be
 * unit-tested without a running app. It wraps the result to [0, 1), folding any
 * negative value back into range so a rewound phase still reads as a valid
 * position. A non-positive duration leaves the phase unchanged. */
float ui_shimmer_step_phase(float cur, double ms_elapsed, unsigned duration_ms);

#endif /* SRC_WIDGETS_SHIMMER_PRIV_H */
