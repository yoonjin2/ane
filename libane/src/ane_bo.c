// SPDX-License-Identifier: MIT
/* Copyright 2022 Eileen Yoon <eyn@gmx.com> */

#include <drm.h>
#include <sys/ioctl.h>
#include <sys/mman.h>

#include "ane_priv.h"

static inline int bo_init(struct ane_device *ane, struct ane_bo *bo)
{
	struct drm_ane_bo_init args = { .size = bo->size };
	int err = ioctl(ane->fd, DRM_IOCTL_ANE_BO_INIT, &args);
	bo->handle = args.handle;
	bo->offset = args.offset;
	return err;
}

static inline int bo_free(struct ane_device *ane, struct ane_bo *bo)
{
	struct drm_ane_bo_free args = { .handle = bo->handle };
	return ioctl(ane->fd, DRM_IOCTL_ANE_BO_FREE, &args);
}

static inline int bo_mmap(struct ane_device *ane, struct ane_bo *bo)
{
	bo->map = mmap(0, bo->size, PROT_READ | PROT_WRITE, MAP_SHARED, ane->fd,
		       bo->offset);

	if (bo->map == MAP_FAILED) {
		fprintf(stderr, "LIBANE: failed to mmap bo\n");
		return -EINVAL;
	}

	return 0;
}

struct ane_bo *ane_bo_init(struct ane_device *ane, uint64_t size)
{
	int err;

	struct ane_bo *bo = ane_zmalloc(sizeof(struct ane_bo));
	if (!bo)
		goto error;

	bo->size = size;

	err = bo_init(ane, bo);
	if (err < 0) {
		fprintf(stderr, "LIBANE: bo_init failed with 0x%x\n", err);
		goto free;
	}

	err = bo_mmap(ane, bo);
	if (err < 0) {
		fprintf(stderr, "LIBANE: bo_mmap failed with 0x%x\n", err);
		goto free2;
	}

	return bo;

free2:
	bo_free(ane, bo);
free:
	free(bo);
error:
	return NULL;
}

int ane_bo_free(struct ane_device *ane, struct ane_bo *bo)
{
	munmap(bo->map, bo->size);
	bo_free(ane, bo);
	free(bo);
	return 0;
}