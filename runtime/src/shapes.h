// The trampolines that carry a call between the interpreter and the WinRT ABI.
//
// Every WinRT method is HRESULT __stdcall f(void* this, ...) and every output is
// a pointer, so the only thing that varies between two calls is the list of
// arguments. That list is a "shape", the generator takes a census of every shape
// in the metadata, and shapes-generated.h instantiates one trampoline per
// shape. The compiler implements the calling convention: what the interpreter
// has to know is only where each argument sits in the buffer it fills, which
// arg_layout below computes from the same types.
//
// See runtime/src/table-format.md for what a shape is and why the set of them is
// fixed at build time.

#pragma once

#include <array>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <utility>

namespace py::shapes
{
    /**
     * A function pointer with no signature of its own, which is what a vtable
     * slot holds and what a trampoline is stored as.
     */
    using vtable_entry = void(__stdcall*)();

    /**
     * Calls @p fn - a WinRT vtable entry - with @p self and the arguments laid
     * out in @p args, and returns its HRESULT.
     */
    using forward_fn
        = int32_t(__stdcall*)(void* fn, void* self, void const* args) noexcept;

    /**
     * Where each argument of a shape sits in the buffer that carries it between
     * the interpreter and the trampoline.
     *
     * The buffer is packed the way a struct of the same members would be: each
     * argument starts at the next offset that its own alignment allows. Both
     * ends compute it from the same types, so neither has to write it down.
     */
    template<class... Abi>
    struct arg_layout
    {
        static constexpr size_t count = sizeof...(Abi);

        struct layout
        {
            // a zero length array is not allowed, and a shape with no arguments
            // still needs a buffer to point at
            std::array<uint16_t, count == 0 ? 1 : count> offsets;
            uint16_t size;
        };

        static constexpr layout compute() noexcept
        {
            layout result{};
            uint16_t offset = 0;
            // both are only read by the fold below, which a shape with no
            // arguments does not expand at all
            [[maybe_unused]] size_t index = 0;

            // a fold over the comma operator is sequenced left to right, so the
            // running offset and the index are well defined
            ((result.offsets[index++] = offset = static_cast<uint16_t>(
                  (offset + alignof(Abi) - 1) / alignof(Abi) * alignof(Abi)),
              offset = static_cast<uint16_t>(offset + sizeof(Abi))),
             ...);

            result.size = offset == 0 ? 1 : offset;

            return result;
        }

        static constexpr layout value = compute();
    };

    /**
     * One shape: the trampoline that calls it and the buffer its arguments go
     * in.
     */
    struct shape_desc
    {
        forward_fn invoke;
        uint16_t const* offsets;
        uint16_t count;
        uint16_t buffer_size;
    };

    /**
     * One (shape, slot) pair: the vtable entry a WinRT caller enters and the
     * buffer the arguments are spilled into.
     */
    struct reverse_desc
    {
        vtable_entry entry;
        uint16_t const* offsets;
        uint16_t count;
        uint16_t buffer_size;
        uint16_t slot;
    };

    template<class T>
    T load(uint8_t const* args, uint16_t offset) noexcept
    {
        T value;
        std::memcpy(&value, args + offset, sizeof(T));
        return value;
    }

    template<class... Abi, size_t... I>
    int32_t invoke_at(
        void* fn,
        void* self,
        [[maybe_unused]] uint8_t const* args,
        std::index_sequence<I...>) noexcept
    {
        using abi_fn = int32_t(__stdcall*)(void*, Abi...);

        // Each argument names its own index, so nothing depends on the order in
        // which the compiler evaluates them - which is unspecified.
        return reinterpret_cast<abi_fn>(fn)(
            self, load<Abi>(args, arg_layout<Abi...>::value.offsets[I])...);
    }

    template<class... Abi>
    int32_t __stdcall invoke(void* fn, void* self, void const* args) noexcept
    {
        return invoke_at<Abi...>(
            fn,
            self,
            static_cast<uint8_t const*>(args),
            std::index_sequence_for<Abi...>{});
    }

    template<class... Abi>
    constexpr shape_desc make_shape() noexcept
    {
        return {
            &invoke<Abi...>,
            arg_layout<Abi...>::value.offsets.data(),
            static_cast<uint16_t>(sizeof...(Abi)),
            arg_layout<Abi...>::value.size};
    }

    /**
     * What a call that arrives from WinRT is sent on to.
     *
     * Every COM object the runtime assembles - the delegate a Python callable
     * becomes, the tearoff of an interface a Python object implements - puts
     * one of these behind the vtable pointer WinRT sees, because the shared
     * dispatcher below has nothing else to go on: a vtable entry knows the
     * slot it stands for and the address it was called with, and that address
     * is the object.
     */
    struct reverse_target
    {
        virtual int32_t invoke(uint16_t slot, void* args) noexcept = 0;

      protected:
        ~reverse_target() = default;
    };

    /**
     * The first two words of every COM object the runtime assembles.
     *
     * A COM interface pointer is a pointer to a vtable pointer, so this is
     * what a WinRT caller holds, and everything else about the object is
     * reached from the second word.
     */
    struct com_head
    {
        vtable_entry const* vtable;
        reverse_target* target;
    };

    /**
     * Where a call from WinRT into Python arrives, after the arguments have been
     * spilled into @p args.
     *
     * A vtable entry cannot learn in portable C++ which slot it was called
     * through, which is why the slot is a template argument of the entry and an
     * ordinary argument here. Defined in shapes.cpp.
     */
    int32_t reverse_dispatch(void* self, uint16_t slot, void* args) noexcept;

    // A shape with no arguments still goes through these, and then there is
    // nothing to read from or write to the buffer.
    template<class... Abi, size_t... I>
    void store_at(
        [[maybe_unused]] uint8_t* args,
        std::index_sequence<I...>,
        Abi... values) noexcept
    {
        ((std::memcpy(
             args + arg_layout<Abi...>::value.offsets[I], &values, sizeof(Abi))),
         ...);
    }

    template<uint16_t Slot, class... Abi>
    int32_t __stdcall reverse_entry(void* self, Abi... values) noexcept
    {
        alignas(alignof(std::max_align_t)) uint8_t args[arg_layout<Abi...>::value.size];

        store_at<Abi...>(args, std::index_sequence_for<Abi...>{}, values...);

        return reverse_dispatch(self, Slot, args);
    }

    template<uint16_t Slot, class... Abi>
    reverse_desc make_reverse() noexcept
    {
        return {
            reinterpret_cast<vtable_entry>(&reverse_entry<Slot, Abi...>),
            arg_layout<Abi...>::value.offsets.data(),
            static_cast<uint16_t>(sizeof...(Abi)),
            arg_layout<Abi...>::value.size,
            Slot};
    }
} // namespace py::shapes
