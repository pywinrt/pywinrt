#pragma once

namespace pywinrt
{
    using namespace winmd::reader;

    inline auto get_start_time()
    {
        return std::chrono::high_resolution_clock::now();
    }

    inline auto get_elapsed_time(
        std::chrono::time_point<std::chrono::high_resolution_clock> const& start)
    {
        return std::chrono::duration_cast<std::chrono::duration<int64_t, std::milli>>(
                   std::chrono::high_resolution_clock::now() - start)
            .count();
    }

    /**
     * Filters a list of types by a filter.
     * @param [in]  filter  The filter to apply.
     * @param [in]  types   The types to filter.
     * @returns A new vector containing only the types that match the filter.
     */
    auto filter_types(filter const& filter, std::vector<TypeDef> const& types)
    {
        std::vector<TypeDef> filtered{};

        std::copy_if(
            types.begin(),
            types.end(),
            std::back_inserter(filtered),
            [&filter](auto& t)
            {
                return filter.includes(t);
            });

        return filtered;
    }

    auto get_dotted_name_segments(std::string_view ns)
    {
        std::vector<std::string_view> segments;
        size_t pos = 0;

        while (true)
        {
            auto new_pos = ns.find('.', pos);

            if (new_pos == std::string_view::npos)
            {
                segments.push_back(ns.substr(pos));
                return segments;
            }

            segments.push_back(ns.substr(pos, new_pos - pos));
            pos = new_pos + 1;
        };
    };

    bool is_exclusive_to(TypeDef const& type)
    {
        return get_category(type) == category::interface_type
               && get_attribute(
                   type, "Windows.Foundation.Metadata", "ExclusiveToAttribute");
    }

    bool is_flags_enum(TypeDef const& type)
    {
        return get_category(type) == category::enum_type
               && get_attribute(type, "System", "FlagsAttribute");
    }

    /**
     * Tests if a type is a WinRT object type.
     */
    bool is_iunknown(TypeDef const& type)
    {
        auto category = get_category(type);
        return category == category::class_type || category == category::interface_type
               || category == category::delegate_type;
    }

    /**
     * Tests if a type is a WinRT parameterized (generic) object type.
     */
    bool is_ptype(TypeDef const& type)
    {
        return distance(type.GenericParam()) > 0;
    }

    bool is_static_class(TypeDef const& type)
    {
        return get_category(type) == category::class_type && type.Flags().Abstract();
    }

    /* returns true if type is composable (has Composable attribute) */
    auto is_composable(TypeDef const& type)
    {
        return bool(
            get_attribute(type, "Windows.Foundation.Metadata", "ComposableAttribute"));
    }

    enum class fundamental_type
    {
        Boolean,
        Char,
        Int8,
        UInt8,
        Int16,
        UInt16,
        Int32,
        UInt32,
        Int64,
        UInt64,
        Float,
        Double,
        String,
    };

    struct generic_type_instance;
    struct object_type
    {
    };
    struct guid_type
    {
    };
    using type_definition = TypeDef;
    using generic_type_index = GenericTypeIndex;

    using type_semantics = std::variant<
        fundamental_type,
        object_type,
        guid_type,
        type_definition,
        generic_type_instance,
        generic_type_index>;

    struct generic_type_instance
    {
        type_definition generic_type;
        std::vector<type_semantics> generic_args{};
    };

    type_semantics get_type_semantics(TypeSig const& signature);

    type_semantics get_type_semantics(GenericTypeInstSig const& type)
    {
        auto generic_type_helper = [&type]()
        {
            switch (type.GenericType().type())
            {
            case TypeDefOrRef::TypeDef:
                return type.GenericType().TypeDef();
            case TypeDefOrRef::TypeRef:
                return find_required(type.GenericType().TypeRef());
            }

            throw_invalid(
                "invalid TypeDefOrRef value for GenericTypeInstSig.GenericType");
        };

        auto gti = generic_type_instance{generic_type_helper()};

        for (auto&& arg : type.GenericArgs())
        {
            gti.generic_args.push_back(get_type_semantics(arg));
        }

        return gti;
    }

    type_semantics get_type_semantics(coded_index<TypeDefOrRef> const& type)
    {
        switch (type.type())
        {
        case TypeDefOrRef::TypeDef:
            return type.TypeDef();
        case TypeDefOrRef::TypeRef:
        {
            auto type_ref = type.TypeRef();

            if (type_ref.TypeNamespace() == "System" && type_ref.TypeName() == "Guid")
            {
                return guid_type{};
            }

            if (type_ref.TypeNamespace() == "System" && type_ref.TypeName() == "Object")
            {
                return object_type{};
            }

            return find_required(type_ref);
        }
        case TypeDefOrRef::TypeSpec:
            return get_type_semantics(type.TypeSpec().Signature().GenericTypeInst());
        }

        throw_invalid("TypeDefOrRef not supported");
    }

    namespace impl
    {
        template<class... Ts>
        struct overloaded : Ts...
        {
            using Ts::operator()...;
        };
        template<class... Ts>
        overloaded(Ts...) -> overloaded<Ts...>;
    } // namespace impl

    type_semantics get_type_semantics(TypeSig const& signature)
    {
        return std::visit(
            impl::overloaded{
                [](ElementType type) -> type_semantics
                {
                    switch (type)
                    {
                    case ElementType::Boolean:
                        return fundamental_type::Boolean;
                    case ElementType::Char:
                        return fundamental_type::Char;
                    case ElementType::I1:
                        return fundamental_type::Int8;
                    case ElementType::U1:
                        return fundamental_type::UInt8;
                    case ElementType::I2:
                        return fundamental_type::Int16;
                    case ElementType::U2:
                        return fundamental_type::UInt16;
                    case ElementType::I4:
                        return fundamental_type::Int32;
                    case ElementType::U4:
                        return fundamental_type::UInt32;
                    case ElementType::I8:
                        return fundamental_type::Int64;
                    case ElementType::U8:
                        return fundamental_type::UInt64;
                    case ElementType::R4:
                        return fundamental_type::Float;
                    case ElementType::R8:
                        return fundamental_type::Double;
                    case ElementType::String:
                        return fundamental_type::String;
                    case ElementType::Object:
                        return object_type{};
                    }
                    throw_invalid("element type not supported");
                },
                [](coded_index<TypeDefOrRef> type) -> type_semantics
                {
                    return get_type_semantics(type);
                },
                [](GenericTypeIndex var) -> type_semantics
                {
                    return generic_type_index{var.index};
                },
                [](GenericTypeInstSig sig) -> type_semantics
                {
                    return get_type_semantics(sig);
                },
                [](GenericMethodTypeIndex) -> type_semantics
                {
                    throw_invalid("Generic methods not supported");
                }},
            signature.Type());
    }

    struct method_signature
    {
        using param_t = std::pair<Param, ParamSig const*>;

        explicit method_signature(MethodDef const& method)
            : m_method(method.Signature())
        {
            auto params = method.ParamList();

            if (m_method.ReturnType() && params.first != params.second
                && params.first.Sequence() == 0)
            {
                m_return = params.first;
                ++params.first;
            }

            for (uint32_t i{}; i != size(m_method.Params()); ++i)
            {
                m_params.emplace_back(params.first + i, &m_method.Params().first[i]);
            }
        }

        std::vector<param_t>& params()
        {
            return m_params;
        }

        std::vector<param_t> const& params() const
        {
            return m_params;
        }

        auto const& return_signature() const
        {
            return m_method.ReturnType();
        }

        auto return_param_name() const
        {
            std::string_view name;

            if (m_return)
            {
                name = m_return.Name();
            }
            else
            {
                name = "_return_value";
            }

            return name;
        }

        bool has_params() const
        {
            return !m_params.empty();
        }

      private:
        MethodDefSig m_method;
        std::vector<param_t> m_params;
        Param m_return;
    };

    TypeDef get_typedef(type_semantics const& semantics)
    {
        return std::visit(
            impl::overloaded{
                [](type_definition type)
                {
                    return type;
                },
                [](generic_type_instance type_instance)
                {
                    return type_instance.generic_type;
                },
                [](auto) -> TypeDef
                {
                    throw_invalid("type doesn't contain typedef");
                }},
            semantics);
    };

    TypeDef get_typedef(coded_index<TypeDefOrRef> const& type)
    {
        return get_typedef(get_type_semantics(type));
    };

    bool implements_interface(
        TypeDef const& type, std::string_view const& ns, std::string_view const& name)
    {
        auto type_name_matches = [&ns, &name](TypeDef const& td)
        {
            return td.TypeNamespace() == ns && td.TypeName() == name;
        };

        if (get_category(type) == category::interface_type && type_name_matches(type))
            return true;

        for (auto&& ii : type.InterfaceImpl())
        {
            auto interface_type = get_typedef(ii.Interface());
            if (implements_interface(interface_type, ns, name))
                return true;
        }

        return false;
    }

    bool implements_interface(
        TypeDef const& type,
        std::vector<std::tuple<std::string_view, std::string_view>> names)
    {
        for (auto&& [ns, name] : names)
        {
            if (implements_interface(type, ns, name))
            {
                return true;
            }
        }

        return false;
    }

    bool implements_ibuffer(TypeDef const& type)
    {
        return implements_interface(type, "Windows.Storage.Streams", "IBuffer");
    }

    bool implements_imemorybufferreference(TypeDef const& type)
    {
        return implements_interface(
            type, "Windows.Foundation", "IMemoryBufferReference");
    }

    bool implements_istringable(TypeDef const& type)
    {
        return implements_interface(type, "Windows.Foundation", "IStringable");
    }

    bool implements_iclosable(TypeDef const& type)
    {
        return implements_interface(type, "Windows.Foundation", "IClosable");
    }

    bool implements_iasync(TypeDef const& type)
    {
        return implements_interface(
            type,
            {std::make_tuple("Windows.Foundation", "IAsyncAction"),
             std::make_tuple("Windows.Foundation", "IAsyncActionWithProgress`1"),
             std::make_tuple("Windows.Foundation", "IAsyncOperation`1"),
             std::make_tuple("Windows.Foundation", "IAsyncOperationWithProgress`2")});
    }

    bool implements_iiterable(TypeDef const& type)
    {
        return implements_interface(
            type, "Windows.Foundation.Collections", "IIterable`1");
    }

    bool implements_iiterator(TypeDef const& type)
    {
        return implements_interface(
            type, "Windows.Foundation.Collections", "IIterator`1");
    }

    bool implements_ivector(TypeDef const& type)
    {
        return implements_interface(
            type, "Windows.Foundation.Collections", "IVector`1");
    }

    bool implements_ivectorview(TypeDef const& type)
    {
        return implements_interface(
            type, "Windows.Foundation.Collections", "IVectorView`1");
    }

    bool implements_imap(TypeDef const& type)
    {
        return implements_interface(type, "Windows.Foundation.Collections", "IMap`2");
    }

    bool implements_imapview(TypeDef const& type)
    {
        return implements_interface(
            type, "Windows.Foundation.Collections", "IMapView`2");
    }

    bool implements_sequence(TypeDef const& type)
    {
        return implements_ivector(type) || implements_ivectorview(type);
    }

    bool implements_mapping(TypeDef const& type)
    {
        return implements_imap(type) || implements_imapview(type);
    }

    auto get_delegate_invoke(TypeDef const& type)
    {
        assert(get_category(type) == category::delegate_type);

        for (auto&& method : type.MethodList())
        {
            if (method.SpecialName() && (method.Name() == "Invoke"))
            {
                return method;
            }
        }

        throw_invalid("Invoke method not found");
    }

    inline bool is_constructor(MethodDef const& method)
    {
        return method.Flags().RTSpecialName() && method.Name() == ".ctor";
    }

    /** Tests if a method is public (has public accessor flag). */
    inline bool is_public(MethodDef const& method)
    {
        return method.Flags().Access() == MemberAccess::Public;
    }

    /** Gets a list of all public constructors. */
    auto get_public_constructors(TypeDef const& type)
    {
        std::vector<MethodDef> ctors{};

        for (auto&& method : type.MethodList())
        {
            if (is_constructor(method) && is_public(method))
            {
                ctors.push_back(method);
            }
        }

        return ctors;
    }

    inline bool is_static(MethodDef const& method)
    {
        return method.Flags().Static();
    }

    inline bool is_static(Event const& event)
    {
        return event.MethodSemantic().first.Method().Flags().Static();
    }

    inline bool is_static(Property const& prop)
    {
        return prop.MethodSemantic().first.Method().Flags().Static();
    }

    inline auto is_default_overload(MethodDef const& method)
    {
        return bool(get_attribute(
            method, "Windows.Foundation.Metadata", "DefaultOverloadAttribute"));
    }

    auto get_property_methods(Property const& prop)
    {
        MethodDef get_method{}, set_method{};

        for (auto&& method_semantic : prop.MethodSemantic())
        {
            auto semantic = method_semantic.Semantic();

            if (semantic.Getter())
            {
                get_method = method_semantic.Method();
            }
            else if (semantic.Setter())
            {
                set_method = method_semantic.Method();
            }
            else
            {
                throw_invalid("Properties can only have get and set methods");
            }
        }

        assert(get_method);

        if (set_method)
        {
            assert(get_method.Flags().Static() == set_method.Flags().Static());
        }

        return std::make_tuple(get_method, set_method);
    }

    auto get_event_methods(Event const& evt)
    {
        MethodDef add_method{}, remove_method{};

        for (auto&& method_semantic : evt.MethodSemantic())
        {
            auto semantic = method_semantic.Semantic();

            if (semantic.AddOn())
            {
                add_method = method_semantic.Method();
            }
            else if (semantic.RemoveOn())
            {
                remove_method = method_semantic.Method();
            }
            else
            {
                throw_invalid("Events can only have add and remove methods");
            }
        }

        assert(add_method);
        assert(remove_method);
        assert(add_method.Flags().Static() == remove_method.Flags().Static());

        return std::make_tuple(add_method, remove_method);
    }

    enum class param_category
    {
        in,
        out,
        pass_array,
        fill_array,
        receive_array,
    };

    auto get_param_category(method_signature::param_t const& param)
    {
        if (param.second->Type().is_szarray())
        {
            if (param.first.Flags().In())
            {
                return param_category::pass_array;
            }
            else if (param.second->ByRef())
            {
                assert(param.first.Flags().Out());
                return param_category::receive_array;
            }
            else
            {
                assert(param.first.Flags().Out());
                return param_category::fill_array;
            }
        }
        else
        {
            if (param.first.Flags().In())
            {
                assert(!param.first.Flags().Out());
                return param_category::in;
            }
            else
            {
                assert(param.first.Flags().Out());
                return param_category::out;
            }
        }
    }

    auto get_param_category(RetTypeSig const& sig)
    {
        if (sig.Type().is_szarray())
        {
            return param_category::receive_array;
        }
        else
        {
            return param_category::out;
        }
    }

    /**
     * Tests if @p param is an input parameter.
     *
     * This matches WinRT input parameters and pass arrays.
     *
     * @param [in]  param   The parameter to test.
     * @returns @c true if the parameter matches, otherwise @c false.
     */
    bool is_in_param(method_signature::param_t const& param)
    {
        auto category = get_param_category(param);

        return (
            category == param_category::in || category == param_category::pass_array);
    }

    /**
     * Counts the number of WinRT input parameters for @p params of a method.
     *
     * @param [in]  params  The parameters to test.
     * @returns The number of parameters that match.
     */
    auto count_in_param(std::vector<method_signature::param_t> const& params)
    {
        return std::count_if(
            params.begin(),
            params.end(),
            [](auto const& param)
            {
                return is_in_param(param);
            });
    }

    /**
     * Tests if @p param should be treated as an input parameter in Python.
     *
     * This matches WinRT input parameters, pass arrays and fill arrays.
     *
     * @param [in]  param   The parameter to test.
     * @returns @c true if the parameter matches, otherwise @c false.
     */
    bool is_py_in_param(method_signature::param_t const& param)
    {
        auto category = get_param_category(param);

        return (
            category == param_category::in || category == param_category::pass_array
            || category == param_category::fill_array);
    }

    /**
     * Tests if @p param should be treated as an output parameter (part of
     * return tuple) in Python.
     *
     * This matches WinRT output parameters and receive arrays.
     *
     * @param [in]  param   The parameter to test.
     * @returns @c true if the parameter matches, otherwise @c false.
     */
    bool is_py_out_param(method_signature::param_t const& param)
    {
        auto category = get_param_category(param);

        return category == param_category::out
               || category == param_category::receive_array;
    }

    /**
     * Counts the number of Python input parameters for @p params of a method.
     *
     * This matches WinRT input parameters, pass arrays and fill arrays.
     *
     * @param [in]  params  The parameters to test.
     * @returns The number of parameters that match.
     */
    auto count_py_in_param(std::vector<method_signature::param_t> const& params)
    {
        return std::count_if(
            params.begin(),
            params.end(),
            [](auto const& param)
            {
                return is_py_in_param(param);
            });
    }

    /**
     * Counts the number of Python output parameters for @p params of a method.
     *
     * This matches WinRT output parameters, receive arrays and fill arrays.
     *
     * @param [in]  params  The parameters to test.
     * @returns The number of parameters that match.
     */
    auto count_py_out_param(std::vector<method_signature::param_t> const& params)
    {
        return std::count_if(
            params.begin(),
            params.end(),
            [](auto const& param)
            {
                return is_py_out_param(param);
            });
    }

    /**
     * Filters Python input parameters from @p params of a method.
     *
     * This matches WinRT input parameters, pass arrays and fill arrays.
     *
     * @param [in]  params  The parameters to filter.
     * @returns A new vector containing only matching parameters.
     */
    auto filter_py_in_params(std::vector<method_signature::param_t> const& params)
    {
        std::vector<method_signature::param_t> out;
        std::copy_if(
            begin(params), end(params), std::back_inserter(out), is_py_in_param);
        return out;
    }

    /**
     * Filters Python output parameters from @p params of a method.
     *
     * This matches WinRT output parameters and receive arrays.
     *
     * @param [in]  params  The parameters to filter.
     * @returns A new vector containing only matching parameters.
     */
    auto filter_py_out_params(std::vector<method_signature::param_t> const& params)
    {
        std::vector<method_signature::param_t> out;
        std::copy_if(
            begin(params), end(params), std::back_inserter(out), is_py_out_param);
        return out;
    }

    enum class argument_convention
    {
        no_args,
        single_arg,
        variable_args
    };

    auto get_argument_convention(MethodDef const& method)
    {
        if (is_constructor(method))
        {
            return empty(method.ParamList()) ? argument_convention::no_args
                                             : argument_convention::variable_args;
        }
        else if (method.SpecialName())
        {
            method_signature signature{method};

            if (signature.has_params())
            {
                assert(signature.params().size() == 1);
                return argument_convention::single_arg;
            }
            else
            {
                return argument_convention::no_args;
            }
        }
        else
        {
            return argument_convention::variable_args;
        }
    }

    type_semantics get_struct_field_semantics(
        Field const& field, bool convert_enum_to_underlying)
    {
        return std::visit(
            impl::overloaded{
                [&](type_definition const& type) -> type_semantics
                {
                    auto category = get_category(type);
                    assert(
                        category == category::enum_type
                        || category == category::struct_type);

                    if ((category == category::enum_type) && convert_enum_to_underlying)
                    {
                        if (is_flags_enum(type))
                        {
                            return fundamental_type::UInt32;
                        }
                        else
                        {
                            return fundamental_type::Int32;
                        }
                    }

                    return type;
                },
                [](generic_type_instance const& gti) -> type_semantics
                {
                    assert(
                        (gti.generic_type.TypeNamespace() == "Windows.Foundation")
                        && (gti.generic_type.TypeName() == "IReference`1")
                        && gti.generic_args.size() == 1);

                    return gti.generic_args[0];
                },
                [](auto v) -> type_semantics
                {
                    return v;
                }},
            get_type_semantics(field.Signature().Type()));
    }

    /**
     * Indicates that C++/WinRT treats @p type as a different struct than
     * direct bindings.
     */
    bool is_customized_struct(TypeDef const& type)
    {
        if (type.TypeNamespace() == "Windows.Foundation")
        {
            static const std::set<std::string_view> custom_structs
                = {"DateTime", "TimeSpan"};

            return custom_structs.find(type.TypeName()) != custom_structs.end();
        }

        return false;
    }

    bool has_custom_conversion(TypeDef const& type)
    {
        static const std::set<std::string_view> custom_converters
            = {"EventRegistrationToken", "HResult"};
        if (type.TypeNamespace() == "Windows.Foundation")
        {
            return custom_converters.find(type.TypeName()) != custom_converters.end();
        }

        return false;
    }

    template<typename C, typename T>
    bool contains(C const& set, T const& value)
    {
        return set.find(value) != set.end();
    }

    /**
     * Checks if a WinRT type has any features that require a Python metaclass.
     */
    bool requires_metaclass(TypeDef const& type)
    {
        // types with static methods or static properties need a metaclass

        for (auto&& method : type.MethodList())
        {
            if (is_static(method))
            {
                return true;
            }
        }

        for (auto&& event : type.EventList())
        {
            if (is_static(event))
            {
                return true;
            }
        }

        for (auto&& prop : type.PropertyList())
        {
            if (is_static(prop))
            {
                return true;
            }
        }

        return false;
    }

    void depedency_sort(std::vector<TypeDef>& types)
    {
        // sort in dpendency order using topological sort
        // https://en.wikipedia.org/wiki/Topological_sorting

        enum class mark
        {
            unmarked,
            temporary,
            permanent
        };

        std::map<TypeDef, mark> marked_types{};

        for (auto&& t : types)
        {
            marked_types[t] = mark::unmarked;
        }

        std::vector<TypeDef> sorted_types{};

        auto visit = [&](TypeDef const& visit_type, auto const& lambda) -> void
        {
            if (marked_types[visit_type] == mark::permanent)
            {
                return;
            }

            if (marked_types[visit_type] == mark::temporary)
            {
                throw_invalid("circular dependency detected");
            }

            marked_types[visit_type] = mark::temporary;

            for (auto&& check_type : types)
            {
                if (!std::visit(
                        impl::overloaded{
                            [&](type_definition const& t)
                            {
                                return t.TypeNamespace() == visit_type.TypeNamespace()
                                       && t.TypeName() == visit_type.TypeName();
                            },
                            [](auto)
                            {
                                return false;
                            }},
                        get_type_semantics(check_type.Extends())))
                {
                    continue;
                }

                lambda(check_type, lambda);
            }

            marked_types[visit_type] = mark::permanent;

            sorted_types.push_back(visit_type);
        };

        while (std::any_of(
            marked_types.begin(),
            marked_types.end(),
            [](auto& t)
            {
                return t.second != mark::permanent;
            }))
        {
            visit(
                std::find_if(
                    marked_types.begin(),
                    marked_types.end(),
                    [](auto& t)
                    {
                        return t.second == mark::unmarked;
                    })
                    ->first,
                visit);
        }

        std::reverse(sorted_types.begin(), sorted_types.end());
        types = sorted_types;
    }
} // namespace pywinrt
