// SPDX-License-Identifier: MIT
/* Copyright 2022 Eileen Yoon <eyn@gmx.com> */

#ifndef __ANE_H__
#define __ANE_H__

#if defined(__cplusplus)
extern "C" {
#endif

#include "ane_dev.h"

struct ane_nn *ane_init(const struct ane_model *model);
void ane_free(struct ane_nn *nn);
int ane_exec(struct ane_nn *nn);
void ane_send(struct ane_nn *nn, void *from, int idx);
void ane_read(struct ane_nn *nn, void *to, int idx);

#if defined(__cplusplus)
}
#endif

#endif /* __ANE_H__ */