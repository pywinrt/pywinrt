﻿// WARNING: Please don't edit this file. It was generated by C++/WinRT v2.0.000000.0
#pragma once
#include "winrt/impl/Component.Composable.1.h"
namespace winrt::Component::Composable
{
    struct WINRT_EBO Base : Component::Composable::IBase
    {
        Base(std::nullptr_t) noexcept {}
        Base(void* ptr, take_ownership_from_abi_t) noexcept : Component::Composable::IBase(ptr, take_ownership_from_abi) {}
        Base();
    };
    struct WINRT_EBO Derived : Component::Composable::IDerived,
        impl::base<Derived, Component::Composable::Base>,
        impl::require<Derived, Component::Composable::IBase>
    {
        Derived(std::nullptr_t) noexcept {}
        Derived(void* ptr, take_ownership_from_abi_t) noexcept : Component::Composable::IDerived(ptr, take_ownership_from_abi) {}
        Derived();
    };
}