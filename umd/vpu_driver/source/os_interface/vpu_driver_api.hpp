/*
 * Copyright (C) 2022 Intel Corporation
 *
 * SPDX-License-Identifier: MIT
 *
 */

#pragma once

#include "vpu_driver/source/os_interface/os_interface_imp.hpp"

#include <chrono>
#include <drm/drm.h>
#include <string>
#include <memory>
#include <cstdint>
#include <uapi/drm/ivpu_drm.h>

namespace VPU {

class OsInterface;

/**
 * VPUDriverApi
 * Interface between upper layer L0 calls and KMD.
 */
class VPUDriverApi final {
  public:
    VPUDriverApi(std::string devnode, OsInterface &osInfc);
    VPUDriverApi(VPUDriverApi &&v);
    ~VPUDriverApi();

    VPUDriverApi(const VPUDriverApi &) = delete;
    VPUDriverApi &operator=(const VPUDriverApi &) = delete;
    VPUDriverApi &&operator=(VPUDriverApi &&v) = delete;

    static std::unique_ptr<VPUDriverApi> openDriverApi(std::string devnode, OsInterface &osInfc);

    bool isVpuDevice() const;
    int submitCommandBuffer(drm_ivpu_submit *submitParams) const;
    int getDeviceParam(drm_ivpu_param *deviceParam) const;
    bool checkDeviceStatus() const;
    size_t getPageSize() const;

    void *alloc(size_t size) const;
    int free(void *ptr) const;

    int createBufferFromUserPointer(drm_ivpu_bo_userptr *userPtrParams) const;
    int wait(void *args) const;
    int closeBuffer(uint32_t handle) const;

    int createBuffer(size_t size, uint32_t flags, uint32_t &handle, uint64_t &vpuAddr) const;
    int getBufferInfo(uint32_t handle, uint64_t &mmap_offset) const;
    void *mmap(size_t size, uint64_t offset) const;
    int unmap(void *ptr, size_t size) const;

  private:
    bool openDevice();
    bool closeDevice();
    int doIoctl(unsigned long request, void *arg) const;

    const static int32_t umdIoctlVersionMajor = DRM_IVPU_DRIVER_MAJOR;
    const static int32_t umdIoctlVersionMinor = DRM_IVPU_DRIVER_MINOR;
    constexpr static char const *umdIoctlDeviceName = "intel_vpu";

    std::string devnode;
    OsInterface &osInfc;
    int vpuFd;
};
} // namespace VPU