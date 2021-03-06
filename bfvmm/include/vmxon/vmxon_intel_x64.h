//
// Bareflank Hypervisor
//
// Copyright (C) 2015 Assured Information Security, Inc.
// Author: Rian Quinn        <quinnr@ainfosec.com>
// Author: Brendan Kerrigan  <kerriganb@ainfosec.com>
//
// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA

#ifndef VMXON_INTEL_X64_H
#define VMXON_INTEL_X64_H

#include <memory>
#include <intrinsics/intrinsics_intel_x64.h>

// -----------------------------------------------------------------------------
// Definition
// -----------------------------------------------------------------------------

/// VMXON (Intel x86_64)
///
/// This class is respobsible for turning Intel's VMX (also know as VT-x)
/// on / off. To do that, it performs a series of checks that are described
/// in the Intel manual, and then runs either vmxon or vmxoff.
///
/// This class is managed by vcpu_intel_x64
///
class vmxon_intel_x64
{
public:

    /// Default Constructor
    ///
    vmxon_intel_x64(std::shared_ptr<intrinsics_intel_x64> intrinsics = nullptr);

    /// Destructor
    ///
    virtual ~vmxon_intel_x64() = default;

    /// Start VMXON
    ///
    /// Starts the VMXON. In the process of starting the VMXON, several
    /// compatibility tests will be run to ensure that the VMXON can in fact
    /// be used. If an error occurs, an exception will be thrown
    ///
    virtual void start();

    /// Stop VMXON
    ///
    /// Stops the VMXON.
    ///
    virtual void stop();

protected:

    virtual void check_cpuid_vmx_supported();
    virtual void check_vmx_capabilities_msr();
    virtual void check_ia32_vmx_cr0_fixed_msr();
    virtual void check_ia32_vmx_cr4_fixed_msr();
    virtual void check_ia32_feature_control_msr();
    virtual void check_v8086_disabled();

    virtual void create_vmxon_region();
    virtual void release_vmxon_region() noexcept;
    virtual void enable_vmx_operation() noexcept;
    virtual void disable_vmx_operation() noexcept;
    virtual void execute_vmxon();
    virtual void execute_vmxoff();

    virtual bool is_vmx_operation_enabled();

private:

    friend class vmxon_ut;

    std::shared_ptr<intrinsics_intel_x64> m_intrinsics;

    bool m_vmxon_enabled;
    uintptr_t m_vmxon_region_phys;
    std::unique_ptr<uint32_t[]> m_vmxon_region;
};

#endif
