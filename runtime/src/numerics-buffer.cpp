// A Windows.Foundation.Numerics struct as the block of floats it is.
//
// Every one of the seven structs is a run of floats in C++/WinRT, so NumPy and
// the buffer protocol can see one as an array in a shape - (3,) for a Vector3,
// (4, 4) for a Matrix4x4 - with no copy. This is that view of a value, of an
// array of values, and of a buffer standing for either, and the indexing that
// reads one of the floats: v[i] of a vector or a quaternion and m[row, column]
// of a matrix. numerics.h says where the arithmetic comes from instead.

#include "numerics-buffer.h"

#include "numerics-traits.h"
#include "types.h"

#include <algorithm>
#include <bit>
#include <span>
#include <string>
#include <string_view>
#include <vector>

namespace py::interp::numerics
{
    namespace
    {
        static_assert(std::endian::native == std::endian::little);

        /**
         * The floats of @p self, which is a value of one of the structs, or
         * @c nullptr with a Python error set.
         */
        uint8_t* floats_of(PyObject* self) noexcept
        {
            auto const entry = find_type_entry(Py_TYPE(self));
            if (!entry)
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "'%s' is not a type a projection table built",
                    Py_TYPE(self)->tp_name);
                return nullptr;
            }

            return reinterpret_cast<uint8_t*>(self) + entry->blob_offset;
        }

        /**
         * The strides of traits<K>::shape in a block of floats in C order.
         */
        template<kind K>
        constexpr auto strides_of = []
        {
            constexpr auto& shape = traits<K>::shape;

            std::array<Py_ssize_t, shape.size()> strides{};
            Py_ssize_t stride = sizeof(float);

            for (auto i = shape.size(); i-- > 0;)
            {
                strides[i] = stride;
                stride *= shape[i];
            }

            return strides;
        }();

        /**
         * The buffer protocol: a read-only view of the floats the value is made
         * of, in the wrapper itself and in the shape traits<K>::shape gives.
         */
        template<kind K>
        int slot_getbuffer(PyObject* self, Py_buffer* view, int flags) noexcept
        {
            constexpr auto& shape = traits<K>::shape;
            constexpr auto& strides = strides_of<K>;

            static_assert(
                strides[0] * shape[0] == sizeof(typename traits<K>::type),
                "the struct is not a block of floats in its shape");

            view->obj = nullptr;

            auto const floats = floats_of(self);
            if (!floats)
            {
                return -1;
            }

            if ((flags & PyBUF_WRITABLE) == PyBUF_WRITABLE)
            {
                PyErr_Format(
                    PyExc_BufferError,
                    "'%s' is immutable, so its buffer is read-only",
                    traits<K>::py_name);
                return -1;
            }

            // A matrix is its rows one after another, which is C order.
            if constexpr (shape.size() > 1)
            {
                if ((flags & PyBUF_F_CONTIGUOUS) == PyBUF_F_CONTIGUOUS)
                {
                    PyErr_Format(
                        PyExc_BufferError,
                        "'%s' is not Fortran contiguous",
                        traits<K>::py_name);
                    return -1;
                }
            }

            view->obj = Py_NewRef(self);
            view->buf = floats;
            view->len = sizeof(typename traits<K>::type);
            view->readonly = 1;
            view->itemsize = sizeof(float);

            if ((flags & PyBUF_FORMAT) == PyBUF_FORMAT)
            {
                view->format = const_cast<char*>("f");
            }
            else
            {
                view->format = nullptr;
            }

            // The shape and the strides are the same for every value of the
            // struct, so they are the constants themselves, which nothing
            // writes through.
            if ((flags & PyBUF_ND) == PyBUF_ND)
            {
                view->ndim = static_cast<int>(shape.size());
                view->shape = const_cast<Py_ssize_t*>(shape.data());
            }
            else
            {
                view->ndim = 1;
                view->shape = nullptr;
            }

            if ((flags & PyBUF_STRIDES) == PyBUF_STRIDES)
            {
                view->strides = const_cast<Py_ssize_t*>(strides.data());
            }
            else
            {
                view->strides = nullptr;
            }

            view->suboffsets = nullptr;
            view->internal = nullptr;

            return 0;
        }

        /**
         * len() of a vector or a quaternion, which is how many components it
         * has.
         */
        template<kind K>
        Py_ssize_t slot_length(PyObject* /*unused*/) noexcept
        {
            return traits<K>::shape[0];
        }

        /**
         * One component of a vector or a quaternion, by index. CPython counts
         * a negative index from the end before it gets here.
         */
        template<kind K>
        PyObject* slot_item(PyObject* self, Py_ssize_t index) noexcept
        {
            if (index < 0)
            {
                PyErr_Format(
                    PyExc_IndexError, "'%s' index out of range", traits<K>::py_name);
                return nullptr;
            }

            if (index >= traits<K>::shape[0])
            {
                PyErr_Format(
                    PyExc_IndexError, "'%s' index out of range", traits<K>::py_name);
                return nullptr;
            }

            auto const floats = floats_of(self);
            if (!floats)
            {
                return nullptr;
            }

            float component;
            std::memcpy(&component, floats + index * sizeof(float), sizeof(component));

            return convert(component);
        }

        /**
         * iter() of a vector or a quaternion, which yields its components.
         */
        PyObject* slot_iter(PyObject* self) noexcept
        {
            return PySeqIter_New(self);
        }

        /**
         * One index of m[row, column], counted from the end when it is
         * negative, or -1 with a Python error set.
         */
        Py_ssize_t matrix_index(
            PyObject* key, Py_ssize_t count, char const* what) noexcept
        {
            if (!PyIndex_Check(key))
            {
                PyErr_Format(
                    PyExc_TypeError,
                    "%s indices must be integers, not '%s'",
                    what,
                    Py_TYPE(key)->tp_name);
                return -1;
            }

            auto index = PyNumber_AsSsize_t(key, PyExc_IndexError);
            if (index == -1 && PyErr_Occurred())
            {
                return -1;
            }

            if (index < 0)
            {
                index += count;
            }

            if (index < 0)
            {
                PyErr_Format(PyExc_IndexError, "%s index out of range", what);
                return -1;
            }

            if (index >= count)
            {
                PyErr_Format(PyExc_IndexError, "%s index out of range", what);
                return -1;
            }

            return index;
        }

        /**
         * The error a matrix raises when it is indexed by anything other than
         * a pair of indices.
         */
        template<kind K>
        PyObject* matrix_key_error(PyObject* key) noexcept
        {
            PyErr_Format(
                PyExc_TypeError,
                "'%s' is indexed by a row and a column, as m[row, column], not "
                "by '%s'",
                traits<K>::py_name,
                Py_TYPE(key)->tp_name);
            return nullptr;
        }

        /**
         * m[row, column]: the element of a matrix in that row and column,
         * counted from 0, which is the field m{row + 1}{column + 1}.
         *
         * A matrix takes the pair only. A single index would have to choose
         * between a row and the flat run of floats, and the buffer and
         * unpack() already give both.
         */
        template<kind K>
        PyObject* slot_matrix_subscript(PyObject* self, PyObject* key) noexcept
        {
            constexpr auto& shape = traits<K>::shape;

            if (!PyTuple_Check(key))
            {
                return matrix_key_error<K>(key);
            }

            if (PyTuple_GET_SIZE(key) != 2)
            {
                return matrix_key_error<K>(key);
            }

            auto const row = matrix_index(PyTuple_GET_ITEM(key, 0), shape[0], "row");
            if (row < 0)
            {
                return nullptr;
            }

            auto const column
                = matrix_index(PyTuple_GET_ITEM(key, 1), shape[1], "column");
            if (column < 0)
            {
                return nullptr;
            }

            auto const floats = floats_of(self);
            if (!floats)
            {
                return nullptr;
            }

            float element;
            std::memcpy(
                &element,
                floats + (row * shape[1] + column) * sizeof(float),
                sizeof(element));

            return convert(element);
        }

        /**
         * The slots every one of the structs has: the buffer of its floats.
         */
        template<kind K>
        void push_buffer_slot(std::vector<PyType_Slot>& slots)
        {
            slots.push_back(
                {Py_bf_getbuffer, reinterpret_cast<void*>(slot_getbuffer<K>)});
        }

        /**
         * The slots that make a vector or a quaternion a sequence of its
         * components.
         */
        template<kind K>
        void push_sequence_slots(std::vector<PyType_Slot>& slots)
        {
            slots.push_back({Py_sq_length, reinterpret_cast<void*>(slot_length<K>)});
            slots.push_back({Py_sq_item, reinterpret_cast<void*>(slot_item<K>)});
            slots.push_back({Py_tp_iter, reinterpret_cast<void*>(slot_iter)});
        }

        /**
         * 'f' or 'd' when the PEP 3118 @p format is one float or one double in
         * the byte order of this machine, and 0 for anything else. A missing
         * format is unsigned bytes.
         */
        char float_code(char const* format) noexcept
        {
            if (!format)
            {
                return 0;
            }

            std::string_view code{format};

            // '@', '=' and '<' all say the byte order of this machine.
            if (code.find_first_of("@=<") == 0)
            {
                code.remove_prefix(1);
            }

            if (code == "f")
            {
                return 'f';
            }

            if (code == "d")
            {
                return 'd';
            }

            return 0;
        }

        /**
         * How many floats there are in a block of @p shape.
         */
        Py_ssize_t count_of(std::span<Py_ssize_t const> shape) noexcept
        {
            Py_ssize_t count = 1;

            for (auto const dimension : shape)
            {
                count *= dimension;
            }

            return count;
        }

        /**
         * @p shape spelled as a Python tuple, for a message.
         */
        std::string shape_text(std::span<Py_ssize_t const> shape)
        {
            std::string text{"("};

            for (size_t i = 0; i < shape.size(); i++)
            {
                if (i > 0)
                {
                    text += ", ";
                }

                text += std::to_string(shape[i]);
            }

            if (shape.size() == 1)
            {
                text += ",";
            }

            text += ")";

            return text;
        }

        /**
         * Whether @p view holds one value of the shape @p shape as values of
         * the type @p code, from float_code(): in that shape, or flat.
         */
        bool holds_value(
            Py_buffer const& view,
            char code,
            std::span<Py_ssize_t const> shape) noexcept
        {
            if (code == 0)
            {
                return false;
            }

            auto const itemsize = code == 'f' ? sizeof(float) : sizeof(double);

            if (view.itemsize != static_cast<Py_ssize_t>(itemsize))
            {
                return false;
            }

            if (view.ndim < 1)
            {
                return false;
            }

            if (!view.shape)
            {
                return false;
            }

            if (view.ndim == 1)
            {
                if (view.shape[0] == count_of(shape))
                {
                    return true;
                }
            }

            return std::ranges::equal(
                std::span{view.shape, static_cast<size_t>(view.ndim)}, shape);
        }
    } // namespace

    /**
     * The shape of a value of the struct @p which as a block of floats, which
     * is what its buffer exports and what an array of it adds a dimension to,
     * or nothing for every other type.
     */
    std::span<Py_ssize_t const> element_shape(kind which) noexcept
    {
        switch (which)
        {
        case kind::vector2:
            return traits<kind::vector2>::shape;
        case kind::vector3:
            return traits<kind::vector3>::shape;
        case kind::vector4:
            return traits<kind::vector4>::shape;
        case kind::matrix3x2:
            return traits<kind::matrix3x2>::shape;
        case kind::matrix4x4:
            return traits<kind::matrix4x4>::shape;
        case kind::plane:
            return traits<kind::plane>::shape;
        case kind::quaternion:
            return traits<kind::quaternion>::shape;
        case kind::none:
            break;
        }

        return {};
    }

    /**
     * Adds the slots that give the struct @p which its floats: the buffer of
     * them, which every one of the structs has, and indexing, which reads
     * one - v[i] of a vector or a quaternion, m[row, column] of a matrix. A
     * Plane has the buffer only.
     */
    void add_buffer_slots(kind which, std::vector<PyType_Slot>& slots)
    {
        switch (which)
        {
        case kind::vector2:
            push_buffer_slot<kind::vector2>(slots);
            push_sequence_slots<kind::vector2>(slots);
            break;
        case kind::vector3:
            push_buffer_slot<kind::vector3>(slots);
            push_sequence_slots<kind::vector3>(slots);
            break;
        case kind::vector4:
            push_buffer_slot<kind::vector4>(slots);
            push_sequence_slots<kind::vector4>(slots);
            break;
        case kind::matrix3x2:
            push_buffer_slot<kind::matrix3x2>(slots);
            slots.push_back(
                {Py_mp_subscript,
                 reinterpret_cast<void*>(slot_matrix_subscript<kind::matrix3x2>)});
            break;
        case kind::matrix4x4:
            push_buffer_slot<kind::matrix4x4>(slots);
            slots.push_back(
                {Py_mp_subscript,
                 reinterpret_cast<void*>(slot_matrix_subscript<kind::matrix4x4>)});
            break;
        case kind::plane:
            push_buffer_slot<kind::plane>(slots);
            break;
        case kind::quaternion:
            push_buffer_slot<kind::quaternion>(slots);
            push_sequence_slots<kind::quaternion>(slots);
            break;
        case kind::none:
            break;
        }
    }

    /**
     * How many values of the shape @p element the buffer @p view holds as a
     * block of float32 values with @p element as its trailing dimensions, or
     * flat; or -1, with no error set, when it holds anything else.
     *
     * This is what an array of numerics structs exports, so it is also what
     * one is made from and what a parameter that takes one borrows.
     */
    Py_ssize_t block_count(
        Py_buffer const& view, std::span<Py_ssize_t const> element) noexcept
    {
        if (element.empty())
        {
            return -1;
        }

        if (float_code(view.format) != 'f')
        {
            return -1;
        }

        if (view.itemsize != static_cast<Py_ssize_t>(sizeof(float)))
        {
            return -1;
        }

        if (!view.shape)
        {
            return -1;
        }

        auto const floats = count_of(element);

        if (view.ndim == 1)
        {
            if (view.shape[0] % floats != 0)
            {
                return -1;
            }

            return view.shape[0] / floats;
        }

        if (view.ndim != static_cast<int>(element.size()) + 1)
        {
            return -1;
        }

        for (size_t i = 0; i < element.size(); i++)
        {
            if (view.shape[i + 1] != element[i])
            {
                return -1;
            }
        }

        return view.shape[0];
    }

    /**
     * Reads a value of the numerics struct @p info out of the buffer @p obj
     * exports, which holds the struct's floats as float or double values in
     * the struct's shape or flat: a NumPy array of shape (3,) is a Vector3 and
     * one of shape (4, 4) or (16,) a Matrix4x4. Each double is converted the
     * way a field given in a tuple is.
     *
     * A value of another of the structs is not read as this one, though its
     * floats would fit, because a Quaternion is not a Vector4.
     *
     * @returns @c false, with no error set, when @p info is no numerics struct
     * or @p obj exports no buffer, so that the caller goes on to what else it
     * takes.
     * @throws python_exception if @p obj exports a buffer that is not one of
     * the struct.
     */
    bool read_buffer(type_entry& info, PyObject* obj, void* out)
    {
        auto const shape = element_shape(info.numerics_kind);
        if (shape.empty())
        {
            return false;
        }

        if (!PyObject_CheckBuffer(obj))
        {
            return false;
        }

        if (auto const other = find_type_entry(Py_TYPE(obj)))
        {
            if (other->numerics_kind != kind::none)
            {
                return false;
            }
        }

        buffer_view buffer{obj, PyBUF_RECORDS_RO};
        if (!buffer)
        {
            throw python_exception();
        }

        auto const& view = buffer.view();
        auto const code = float_code(view.format);
        auto const floats = count_of(shape);

        if (!holds_value(view, code, shape))
        {
            auto expected = shape_text(shape);

            if (shape.size() > 1)
            {
                expected += " or (" + std::to_string(floats) + ",)";
            }

            auto const actual
                = view.shape ? shape_text({view.shape, static_cast<size_t>(view.ndim)})
                             : std::string{"()"};

            PyErr_Format(
                PyExc_TypeError,
                "a buffer read as '%s' must hold floats or doubles of shape %s, "
                "not '%s' of shape %s",
                info.py_type ? info.py_type->tp_name : info.winrt_name,
                expected.c_str(),
                view.format ? view.format : "B",
                actual.c_str());
            throw python_exception();
        }

        // The floats are read in C order, each found through the strides, so
        // a transposed view gives the transposed matrix.
        auto* const bytes = static_cast<uint8_t*>(out);

        for (Py_ssize_t i = 0; i < floats; i++)
        {
            auto rest = i;
            auto* item = static_cast<uint8_t const*>(view.buf);

            for (auto d = view.ndim; d-- > 0;)
            {
                item += (rest % view.shape[d]) * view.strides[d];
                rest /= view.shape[d];
            }

            float value;

            if (code == 'f')
            {
                std::memcpy(&value, item, sizeof(value));
            }
            else
            {
                double wide;
                std::memcpy(&wide, item, sizeof(wide));
                value = static_cast<float>(wide);
            }

            std::memcpy(bytes + i * sizeof(float), &value, sizeof(value));
        }

        return true;
    }
} // namespace py::interp::numerics
