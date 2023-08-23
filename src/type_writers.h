#pragma once

namespace pywinrt
{
    using namespace std::literals;
    using namespace std::filesystem;
    using namespace winmd::reader;
    using namespace pywinrt::text;

    template<typename First, typename... Rest>
    auto get_impl_name(First const& first, Rest const&... rest)
    {
        std::string result;

        auto convert = [&](auto&& value)
        {
            for (auto&& c : value)
            {
                result += c == '.' ? '_' : c;
            }
        };

        convert(first);
        ((result += '_', convert(rest)), ...);
        return result;
    }

    struct writer : indented_writer_base<writer>
    {
        using indented_writer_base<writer>::write;

        std::string_view current_namespace{};
        std::set<std::string> needed_namespaces{};

#pragma region generic param handling
        std::vector<std::vector<std::string>> generic_param_stack;

        struct generic_param_guard
        {
            explicit generic_param_guard(writer* arg = nullptr) : owner(arg)
            {
            }

            ~generic_param_guard()
            {
                if (owner)
                {
                    owner->generic_param_stack.pop_back();
                }
            }

            generic_param_guard(generic_param_guard&& other) : owner(other.owner)
            {
                owner = nullptr;
            }

            generic_param_guard& operator=(generic_param_guard&& other)
            {
                owner = std::exchange(other.owner, nullptr);
                return *this;
            }

            generic_param_guard& operator=(generic_param_guard const&) = delete;
            writer* owner;
        };

        [[nodiscard]] auto push_generic_params(
            std::pair<GenericParam, GenericParam>&& params)
        {
            if (empty(params))
            {
                return generic_param_guard{nullptr};
            }

            std::vector<std::string> names;

            for (auto&& param : params)
            {
                names.push_back(std::string{param.Name()});
            }

            generic_param_stack.push_back(std::move(names));
            return generic_param_guard{this};
        }

        [[nodiscard]] auto push_generic_params(GenericTypeInstSig const& signature)
        {
            std::vector<std::string> names;

            for (auto&& arg : signature.GenericArgs())
            {
                names.push_back(write_temp("%", arg));
            }

            generic_param_stack.push_back(std::move(names));
            return generic_param_guard{this};
        }

        [[nodiscard]] auto push_generic_params(
            std::vector<type_semantics> const& type_arguments)
        {
            if (type_arguments.size() == 0)
            {
                return generic_param_guard{nullptr};
            }

            std::vector<std::string> names;

            for (auto&& arg : type_arguments)
            {
                // TODO real code here
                names.push_back(write_temp("%", arg));
            }

            generic_param_stack.push_back(std::move(names));
            return generic_param_guard{this};
        }

#pragma endregion

        void write_value(bool value)
        {
            write(value ? "TRUE"sv : "FALSE"sv);
        }

        void write_value(char16_t value)
        {
            write_printf("%#0hx", value);
        }

        void write_value(int8_t value)
        {
            write_printf("%hhd", value);
        }

        void write_value(uint8_t value)
        {
            write_printf("%#0hhx", value);
        }

        void write_value(int16_t value)
        {
            write_printf("%hd", value);
        }

        void write_value(uint16_t value)
        {
            write_printf("%#0hx", value);
        }

        void write_value(int32_t value)
        {
            write_printf("%d", value);
        }

        void write_value(uint32_t value)
        {
            write_printf("%#0x", value);
        }

        void write_value(int64_t value)
        {
            write_printf("%lld", value);
        }

        void write_value(uint64_t value)
        {
            write_printf("%#0llx", value);
        }

        void write_value(float value)
        {
            write_printf("%f", value);
        }

        void write_value(double value)
        {
            write_printf("%f", value);
        }

        void write_value(std::u16string_view value)
        {
            static_assert(sizeof(std::u16string_view::value_type) == sizeof(WCHAR));
            static_assert(sizeof(std::string_view::value_type) == sizeof(CHAR));

            auto input = reinterpret_cast<WCHAR const*>(value.data());
            auto input_length = static_cast<int>(value.size());

            auto required_size = WideCharToMultiByte(
                CP_UTF8,
                WC_ERR_INVALID_CHARS,
                input,
                input_length,
                nullptr,
                0,
                nullptr,
                nullptr);

            auto buffer = std::make_unique<CHAR[]>(required_size);

            auto result = WideCharToMultiByte(
                CP_UTF8,
                WC_ERR_INVALID_CHARS,
                input,
                input_length,
                buffer.get(),
                required_size,
                nullptr,
                nullptr);

            if (result == 0)
            {
                switch (GetLastError())
                {
                case ERROR_INSUFFICIENT_BUFFER:
                    throw_invalid("Insufficient buffer size");

                case ERROR_NO_UNICODE_TRANSLATION:
                    throw_invalid("Untranslatable string");

                default:
                    throw("WideCharToMultiByte failed");
                }
            }

            std::string_view converted_value{buffer.get()};

            write("\"%\"", converted_value);
        }

        void write(Constant const& value)
        {
            switch (value.Type())
            {
            case ConstantType::Boolean:
                write_value(value.ValueBoolean());
                break;
            case ConstantType::Char:
                write_value(value.ValueChar());
                break;
            case ConstantType::Int8:
                write_value(value.ValueInt8());
                break;
            case ConstantType::UInt8:
                write_value(value.ValueUInt8());
                break;
            case ConstantType::Int16:
                write_value(value.ValueInt16());
                break;
            case ConstantType::UInt16:
                write_value(value.ValueUInt16());
                break;
            case ConstantType::Int32:
                write_value(value.ValueInt32());
                break;
            case ConstantType::UInt32:
                write_value(value.ValueUInt32());
                break;
            case ConstantType::Int64:
                write_value(value.ValueInt64());
                break;
            case ConstantType::UInt64:
                write_value(value.ValueUInt64());
                break;
            case ConstantType::Float32:
                write_value(value.ValueFloat32());
                break;
            case ConstantType::Float64:
                write_value(value.ValueFloat64());
                break;
            case ConstantType::String:
                write_value(value.ValueString());
                break;
            case ConstantType::Class:
                write("null");
                break;
            }
        }

        void write_code(std::string_view const& value)
        {
            for (auto&& c : value)
            {
                if (c == '.')
                {
                    write("::");
                }
                else if (c == '`')
                {
                    return;
                }
                else
                {
                    write(c);
                }
            }
        }

        void write(TypeRef const& type)
        {
            auto ns = type.TypeNamespace();
            auto name = type.TypeName();

            if (ns == "System" && name == "Guid")
            {
                write("winrt::guid");
            }
            else
            {
                write(find_required(type));
            }
        }

        void write_python(TypeRef const& type)
        {
            auto ns = type.TypeNamespace();
            auto name = type.TypeName();

            if (ns == "System" && name == "Guid")
            {
                write("_uuid.UUID");
            }
            else
            {
                write_python(find_required(type));
            }
        }

        void register_type_namespace(std::string_view ns)
        {
            if (ns != current_namespace && ns != "System")
            {
                needed_namespaces.emplace(ns);
            }
        }

        void register_type_namespace(GenericTypeInstSig const& t)
        {
            register_type_namespace(t.GenericType());
            for (auto&& type_arg : t.GenericArgs())
            {
                register_type_namespace(type_arg);
            }
        }

        void register_type_namespace(coded_index<TypeDefOrRef> const& type)
        {
            switch (type.type())
            {
            case TypeDefOrRef::TypeDef:
                register_type_namespace(type.TypeDef().TypeNamespace());
                break;

            case TypeDefOrRef::TypeRef:
            {
                auto tr = type.TypeRef();
                if (tr.TypeNamespace() != "System" || tr.TypeName() != "Guid")
                {
                    register_type_namespace(type.TypeRef().TypeNamespace());
                }
            }
            break;

            case TypeDefOrRef::TypeSpec:
                register_type_namespace(type.TypeSpec().Signature().GenericTypeInst());
                break;
            }
        }

        void register_type_namespace(TypeSig const& type)
        {
            call(
                type.Type(),
                [&](ElementType)
                {
                },
                [&](GenericTypeIndex)
                {
                },
                [&](GenericMethodTypeIndex)
                {
                    throw_invalid("Generic methods not supported");
                },
                [&](auto&& t)
                {
                    register_type_namespace(t);
                });
        }

        void write(TypeDef const& type)
        {
            auto ns = type.TypeNamespace();
            auto name = type.TypeName();

            if (ns != current_namespace)
            {
                needed_namespaces.emplace(ns);
            }

            if ((ns == "Windows.Foundation") && (name == "HResult"))
            {
                write("winrt::hresult");
            }
            else if ((ns == "Windows.Foundation") && (name == "EventRegistrationToken"))
            {
                write("winrt::event_token");
            }
            else
            {
                if (ns == "Windows.Foundation.Numerics")
                {
                    static const std::map<std::string_view, std::string_view>
                        custom_numerics
                        = {{"Matrix3x2", "float3x2"},
                           {"Matrix4x4", "float4x4"},
                           {"Plane", "plane"},
                           {"Quaternion", "quaternion"},
                           {"Vector2", "float2"},
                           {"Vector3", "float3"},
                           {"Vector4", "float4"}};

                    auto custom_numeric = custom_numerics.find(name);
                    if (custom_numeric != custom_numerics.end())
                    {
                        name = custom_numeric->second;
                    }
                }

                write("winrt::@::@", ns, name);
            }
        }

        void write_python(TypeDef const& type)
        {
            auto ns = type.TypeNamespace();
            auto name = type.TypeName();

            if (ns == "Windows.Foundation")
            {
                // special case for IReference since the type is not used directly
                if (name == "IReference`1")
                {
                    write("typing.Optional");
                    return;
                }

                if (name == "DateTime")
                {
                    write("datetime.datetime");
                    return;
                }

                if (name == "TimeSpan")
                {
                    write("datetime.timedelta");
                    return;
                }
            }

            if (ns == current_namespace)
            {
                write("@", name);
            }
            else
            {
                write("winrt.");

                for (auto c : ns)
                {
                    write(static_cast<char>(::tolower(c)));
                }

                write(".@", name);
            }
        }

        void write(coded_index<TypeDefOrRef> const& type)
        {
            switch (type.type())
            {
            case TypeDefOrRef::TypeDef:
                write(type.TypeDef());
                break;

            case TypeDefOrRef::TypeRef:
                write(type.TypeRef());
                break;

            case TypeDefOrRef::TypeSpec:
                write(type.TypeSpec().Signature().GenericTypeInst());
                break;
            }
        }

        void write_python(coded_index<TypeDefOrRef> const& type)
        {
            switch (type.type())
            {
            case TypeDefOrRef::TypeDef:
                write_python(type.TypeDef());
                break;
            case TypeDefOrRef::TypeRef:
                write_python(type.TypeRef());
                break;
            case TypeDefOrRef::TypeSpec:
                write_python(type.TypeSpec().Signature().GenericTypeInst());
                break;
            }
        }

        void write(GenericTypeInstSig const& type)
        {
            write("%<%>", type.GenericType(), bind_list(", ", type.GenericArgs()));
        }

        void write_python(GenericTypeInstSig const& type)
        {
            // TODO: figure out how to use bind<> here
            // write("%[%]", bind<write_python>(type.GenericType()),
            //      bind_list<write_python>(", ", type.GenericArgs()));

            write_python(type.GenericType());
            write("[");

            auto count = type.GenericArgCount();

            for (auto&& arg : type.GenericArgs())
            {
                write_python(arg);

                if (--count)
                {
                    write(", ");
                }
            }

            write("]");
        }

        static std::string_view get_cpp_type(ElementType type)
        {
            switch (type)
            {
            case ElementType::Boolean:
                return "bool";
            case ElementType::Char:
                return "char16_t";
            case ElementType::I1:
                return "int8_t";
            case ElementType::U1:
                return "uint8_t";
            case ElementType::I2:
                return "int16_t";
            case ElementType::U2:
                return "uint16_t";
            case ElementType::I4:
                return "int32_t";
            case ElementType::U4:
                return "uint32_t";
            case ElementType::I8:
                return "int64_t";
            case ElementType::U8:
                return "uint64_t";
            case ElementType::R4:
                return "float";
            case ElementType::R8:
                return "double";
            case ElementType::String:
                return "winrt::hstring";
            case ElementType::Object:
                return "winrt::Windows::Foundation::IInspectable";
            default:
                throw_invalid("element type not supported");
            }
        }

        void write(ElementType type)
        {
            write(writer::get_cpp_type(type));
        }

        static std::string get_python_type(ElementType type)
        {
            switch (type)
            {
            case ElementType::Boolean:
                return "bool";
            case ElementType::Char:
                return "winrt.system.Char16";
            case ElementType::I1:
                return "winrt.system.Int8";
            case ElementType::U1:
                return "winrt.system.UInt8";
            case ElementType::I2:
                return "winrt.system.Int16";
            case ElementType::U2:
                return "winrt.system.UInt16";
            case ElementType::I4:
                return "winrt.system.Int32";
            case ElementType::U4:
                return "winrt.system.UInt32";
            case ElementType::I8:
                return "winrt.system.Int64";
            case ElementType::U8:
                return "winrt.system.UInt64";
            case ElementType::R4:
                return "winrt.system.Single";
            case ElementType::R8:
                return "winrt.system.Double";
            case ElementType::String:
                return "str";
            case ElementType::Object:
                return "winrt.system.Object";
            default:
                throw_invalid("element type not supported");
            }
        }

        void write_python(ElementType type)
        {
            write("%", writer::get_python_type(type));
        }

        void write(GenericTypeIndex var)
        {
            write(generic_param_stack.back()[var.index]);
        }

        static void double_colon_to_dot(writer& w, std::string_view const& str)
        {
            auto prev_was_colon = false;

            for (auto c : str)
            {
                if (c == ':')
                {
                    // two colons become one dot
                    if (prev_was_colon)
                    {
                        prev_was_colon = false;
                    }
                    else
                    {
                        prev_was_colon = true;
                        w.write('.');
                    }
                }
                else
                {
                    w.write(c);
                }
            }
        }

        void write_cpp_type_as_python_type(std::string const& type)
        {
            // special cases

            if (type == "winrt::hstring")
            {
                write("str");
                return;
            }

            if (type == "winrt::Windows::Foundation::IInspectable")
            {
                write("winrt.system.Object");
                return;
            }

            // generic case

            auto angle_brace_index = type.find('<');

            if (angle_brace_index == std::string::npos)
            {
                // if no angle brace, this is a normal (non-generic) type
                auto last_colons_index = type.rfind("::");

                if (last_colons_index == std::string::npos)
                {
                    // not a qualified type, just write the name
                    write(type);
                }
                else
                {
                    // for qualified names, we have to convert :: to . and to lower case
                    auto ns = write_temp(
                        "%",
                        bind<double_colon_to_dot>(
                            type.substr(0, last_colons_index + 2)));
                    auto type_name = type.substr(last_colons_index + 2);

                    // ns includes "winrt." prefix and "." suffix that current_namespace
                    // doesn't have
                    if (ns.substr(6, ns.length() - 7) != current_namespace)
                    {
                        for (auto c : ns)
                        {
                            write(static_cast<char>(::tolower(c)));
                        }
                    }

                    write(type_name);
                }
            }
            else
            {
                // there is an angle brace, so this is a generic type

                // write the generic type
                write_cpp_type_as_python_type(type.substr(0, angle_brace_index));

                // then the type parameters
                write('[');

                auto start_index = angle_brace_index + 1;
                auto angle_brace_level = 1;

                for (;;)
                {
                    auto count = 0;

                    // there could be nested generics, so we have to do special
                    // parsing to be sure we split strings in the right place
                    for (auto c : type.substr(start_index))
                    {
                        if (c == '<')
                        {
                            angle_brace_level++;
                        }
                        else if (c == '>')
                        {
                            angle_brace_level--;
                            if (angle_brace_level == 0)
                            {
                                // this was the last type parameter
                                break;
                            }
                        }
                        else if (c == ',')
                        {
                            if (angle_brace_level == 1)
                            {
                                // this is the end of a type parameter
                                break;
                            }
                        }

                        count++;
                    }

                    write_cpp_type_as_python_type(type.substr(start_index, count));

                    if (angle_brace_level == 0)
                    {
                        break;
                    }
                    else
                    {
                        write(", ");
                        // + 2 accounts for the ", " in the cpp type string
                        start_index += count + 2;
                    }
                }

                write(']');
            }
        }

        void write_python(GenericTypeIndex var)
        {
            write_cpp_type_as_python_type(generic_param_stack.back()[var.index]);
        }

        void write(GenericMethodTypeIndex)
        {
            throw_invalid("Generic methods not supported");
        }

        void write_python(GenericMethodTypeIndex)
        {
            throw_invalid("Generic methods not supported");
        }

        void write(TypeSig const& signature)
        {
            call(
                signature.Type(),
                [&](auto&& type)
                {
                    write(type);
                });
        }

        void write_python(TypeSig const& signature)
        {
            call(
                signature.Type(),
                [&](auto&& type)
                {
                    write_python(type);
                });
        }

        void write(fundamental_type type)
        {
            switch (type)
            {
            case fundamental_type::Boolean:
                write(ElementType::Boolean);
                break;
            case fundamental_type::Char:
                write(ElementType::Char);
                break;
            case fundamental_type::Int8:
                write(ElementType::I1);
                break;
            case fundamental_type::UInt8:
                write(ElementType::U1);
                break;
            case fundamental_type::Int16:
                write(ElementType::I2);
                break;
            case fundamental_type::UInt16:
                write(ElementType::U2);
                break;
            case fundamental_type::Int32:
                write(ElementType::I4);
                break;
            case fundamental_type::UInt32:
                write(ElementType::U4);
                break;
            case fundamental_type::Int64:
                write(ElementType::I8);
                break;
            case fundamental_type::UInt64:
                write(ElementType::U8);
                break;
            case fundamental_type::Float:
                write(ElementType::R4);
                break;
            case fundamental_type::Double:
                write(ElementType::R8);
                break;
            case fundamental_type::String:
                write(ElementType::String);
                break;
            }
        }

        void write(object_type const&)
        {
            write(ElementType::Object);
        }

        void write(guid_type const&)
        {
            write("winrt::guid");
        }

        void write(generic_type_instance type)
        {
            write("%<%>", type.generic_type, bind_list(", ", type.generic_args));
        }

        void write(type_semantics semantics)
        {
            call(
                semantics,
                [&](auto&& type)
                {
                    write(type);
                });
        }
    };

    struct separator
    {
        writer& w;
        std::string_view _separator{", "};
        bool first{true};

        void operator()()
        {
            if (first)
            {
                first = false;
            }
            else
            {
                w.write(_separator);
            }
        }
    };

    template<typename F>
    void enumerate_required_types(writer& w, TypeDef const& type, F func)
    {
        std::set<TypeDef> types;

        auto enumerate_types_impl
            = [&](type_semantics const& semantics, auto const& lambda) -> void
        {
            auto type = get_typedef(semantics);
            auto generic_args = std::visit(
                impl::overloaded{
                    [](type_definition) -> std::vector<type_semantics>
                    {
                        return {};
                    },
                    [](generic_type_instance type_instance)
                    {
                        return type_instance.generic_args;
                    },
                    [](auto) -> std::vector<type_semantics>
                    {
                        throw_invalid("type doesn't contain typedef");
                    }},
                semantics);

            auto guard{w.push_generic_params(generic_args)};

            if (!contains(types, type))
            {
                types.insert(type);
                func(type);
            }

            if (get_category(type) == category::interface_type)
            {
                for (auto&& ii : type.InterfaceImpl())
                {
                    lambda(get_type_semantics(ii.Interface()), lambda);
                }
            }
        };

        enumerate_types_impl(type, enumerate_types_impl);
    }

    /**
     * Calls @p func on all methods in @p type.
     *
     * @param [in]  w       A writer.
     * @param [in]  type    The type that contains the methods.
     * @param [in]  func    A function that takes two args, the MethodDef and
     *                      a boolean indicating if the method is overloaded.
     */
    template<typename F>
    void enumerate_methods(writer& w, TypeDef const& type, F func)
    {
        std::set<std::string_view> method_names{};

        // collect all unique method names

        enumerate_required_types(
            w,
            type,
            [&](TypeDef const& required_type)
            {
                for (auto&& method : required_type.MethodList())
                {
                    if (method.SpecialName())
                    {
                        // skip .ctor, get_/put_ and add_/remove_ methods
                        continue;
                    }

                    method_names.insert(method.Name());
                }
            });

        // call callback for each method with overloads grouped together
        // REVISIT: this doesn't handle the case if an interface has an overload
        // with the same number of parameters as a required interface

        for (auto&& method_name : method_names)
        {
            enumerate_required_types(
                w,
                type,
                [&](TypeDef const& required_type)
                {
                    // map of overloads by number of parameters
                    std::map<int, std::vector<MethodDef>> overloads;

                    for (auto&& method : required_type.MethodList())
                    {
                        if (method.Name() != method_name)
                        {
                            continue;
                        }

                        method_signature signature{method};
                        auto arg_count = count_py_in_param(signature.params());
                        overloads[arg_count].push_back(method);
                    }

                    for (auto o : overloads)
                    {
                        // if there are multiple overloads with the same number of
                        // arguments, we need to use the default overload
                        // https://devblogs.microsoft.com/oldnewthing/20210528-00/?p=105259
                        auto default_overload = std::find_if(
                            o.second.begin(),
                            o.second.end(),
                            [](auto m)
                            {
                                for (auto a : m.CustomAttribute())
                                {
                                    if (a.TypeNamespaceAndName().second
                                        == "DefaultOverloadAttribute")
                                    {
                                        return true;
                                    }
                                }

                                return false;
                            });

                        // if there was no default, just use the first (and
                        // hopefully only) overload
                        auto i
                            = default_overload == o.second.end()
                                  ? 0
                                  : std::distance(o.second.begin(), default_overload);

                        func(o.second.at(i));
                    }
                });
        }
    }

    template<typename F>
    void enumerate_properties(writer& w, TypeDef const& type, F func)
    {
        enumerate_required_types(
            w,
            type,
            [&](TypeDef const& required_type)
            {
                for (auto&& prop : required_type.PropertyList())
                {
                    func(prop);
                }
            });
    }

    template<typename F>
    void enumerate_events(writer& w, TypeDef const& type, F func)
    {
        enumerate_required_types(
            w,
            type,
            [&](TypeDef const& required_type)
            {
                for (auto&& evt : required_type.EventList())
                {
                    func(evt);
                }
            });
    }

    /**
     * Tests if a method has any overloads.
     * @param w Required for generic arg context.
     * @param type The type that contains the method.
     * @param name The name of the method to test.
     * @returns True if there are overloads, otherwise false.
     */
    bool is_method_overloaded(
        writer& w, TypeDef const& type, std::string_view const& name)
    {
        auto guard{w.push_generic_params(type.GenericParam())};

        auto count = 0;

        if (name == ".ctor")
        {
            // REVISIT: this may return incorrect result if all constructors
            // have the same number of arguments and there is more than one
            // constructor
            count = std::count_if(
                type.MethodList().first,
                type.MethodList().second,
                [](auto&& method)
                {
                    return method.Name() == ".ctor";
                });
        }
        else
        {
            enumerate_methods(
                w,
                type,
                [&](MethodDef const& method)
                {
                    if (method.Name() == name)
                    {
                        count++;
                    }
                });
        }

        return count > 1;
    }

} // namespace pywinrt
