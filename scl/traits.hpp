#pragma once

#include <type_traits>
#include <tuple>

#define ICB inline constexpr bool
#define ICS inline constexpr std::size_t
#define SIC static inline constexpr

namespace scl::details {
    template <typename T = void>
    struct Void {
        using type = void;
    };
}

#define SCL_GENERATE_NESTED_CHECKER(checker_name, type_to_check)                   \
template <typename T, typename U = void>                                           \
struct checker_name {                                                              \
	SIC bool value = false;                                                        \
};                                                                                 \
                                                                                   \
template <typename T>                                                              \
struct checker_name<T, typename details::Void<typename T::type_to_check>::type> {  \
	SIC bool value = true;                                                         \
};                                                                                 \
                                                                                   \
template <typename T>                                                              \
inline constexpr bool checker_name##_v  = checker_name<T>::value


namespace scl {
    SCL_GENERATE_NESTED_CHECKER(is_transparent, is_transparent);

    enum class FunctionType : uint8_t { function, member, transparent };

    using no_type = void;

    namespace details {
        template <size_t _Num, typename... ArgsT>
        struct _arg_type {
            static_assert(_Num < sizeof...(ArgsT), "Attempt to get type of nonexistent argument.");
            using type = std::tuple_element_t<_Num, std::tuple<ArgsT...>>;
        };

        template <typename ReturnT, bool _IsMutable, FunctionType _Type, typename... ArgsT>
        struct _function {
            SIC bool value = true;

            SIC size_t       arity          = sizeof...(ArgsT);
            SIC bool         is_mutable     = _IsMutable;
            SIC bool         is_transparent = false;
            SIC FunctionType type           = _Type;

            using return_type = ReturnT;

            template <size_t _Num>
            using arg_type = typename _arg_type<_Num, ArgsT...>::type;
        };

        template <typename F, typename Enable = void>
        struct transparent_args_counter;

        template <typename F>
        struct transparent_args_counter<F, std::enable_if_t<!std::is_same_v<decltype(std::declval<F>().operator()(0)), void>>> {
            SIC size_t value = 1;
        };
        template <typename F>
        struct transparent_args_counter<F, std::enable_if_t<!std::is_same_v<decltype(std::declval<F>().operator()(0, 0)), void>>> {
            SIC size_t value = 2;
        };
        template <typename F>
        struct transparent_args_counter<F, std::enable_if_t<!std::is_same_v<decltype(std::declval<F>().operator()(0, 0, 0)), void>>> {
            SIC size_t value = 3;
        };
        // Is enough?
        template <typename F>
        struct transparent_args_counter<F, std::enable_if_t<!std::is_same_v<decltype(std::declval<F>().operator()(0, 0, 0, 0)), void>>> {
            SIC size_t value = 4;
        };

        template <typename F, typename = void, typename... ArgsT>
        struct transparent_safe_return_type;

        template <typename F, typename... ArgsT>
        struct transparent_safe_return_type<F, std::enable_if_t<transparent_args_counter<F>::value == sizeof...(ArgsT)>, ArgsT...> {
            using type = decltype(std::declval<F>().operator()(ArgsT()...));
        };

        template <typename F, typename... ArgsT>
        struct transparent_safe_return_type<F, std::enable_if_t<transparent_args_counter<F>::value != sizeof...(ArgsT)>, ArgsT...> {
            using type = void;
        };

        template <typename F>
        struct transparent_comparator {
            SIC bool value = true;

            SIC size_t arity = transparent_args_counter<F>::value;
            // Consider that comparator's operator() is not mutable
            SIC bool         is_mutable     = false;
            SIC bool         is_transparent = true;
            SIC FunctionType type           = FunctionType::transparent;

            template <typename... ArgsT>
            using return_type = typename transparent_safe_return_type<F, void, ArgsT...>::type;

            template <size_t _Num, typename... ArgsT>
            using arg_type = typename _arg_type<_Num, ArgsT...>::type;
        };



        // Function traits
        template <typename T, typename = void>
        struct _function_traits {
            SIC bool value = false;
        };

        template <typename FunctorT>
        struct _function_traits<FunctorT, typename Void<decltype(&FunctorT::operator())>::type>
                : public _function_traits<decltype(&FunctorT::operator())> {};

        template <typename FunctorT, typename ReturnT, typename... ArgsT>
        struct _function_traits<ReturnT(FunctorT::*)(ArgsT...)>
                : public _function<ReturnT, true, FunctionType::member, ArgsT...> {};

        template <typename FunctorT, typename ReturnT, typename... ArgsT>
        struct _function_traits<ReturnT(FunctorT::*)(ArgsT...) const>
                : public _function<ReturnT, false, FunctionType::member, ArgsT...> {};


        template <typename ReturnT, typename... ArgsT>
        struct _function_traits<ReturnT(ArgsT...)>
                : public _function<ReturnT, false, FunctionType::function, ArgsT...> {};

        template <typename ReturnT, typename... ArgsT>
        struct _function_traits<ReturnT(*)(ArgsT...)>
                : public _function_traits<ReturnT(ArgsT...)> {};


        template <typename FunctionT, typename Enable = void>
        struct function : public _function_traits<FunctionT> {};

        template <typename FunctionT>
        struct function<FunctionT, std::enable_if_t<is_transparent_v<FunctionT>>>
                : transparent_comparator<FunctionT> {};
    }


    // Getting return type of function
    template <typename F, typename Enable = void, typename... ArgsT>
    struct return_type_of;

    template <typename F, typename... ArgsT>
    struct return_type_of<F, std::enable_if_t<is_transparent_v<F>>, ArgsT...> {
        using type = typename details::function<F>::template return_type<ArgsT...>;
    };

    template <typename F, typename... ArgsT>
    struct return_type_of<F, std::enable_if_t<!is_transparent_v<F>>, ArgsT...> {
        using type = typename details::function<F>::return_type;
    };


    // Getting argument type of function
    template <typename F, size_t _ArgNum, typename Enable = void, typename... ArgsT>
    struct arg_type_of;

    template <typename F, size_t _ArgNum, typename... ArgsT>
    struct arg_type_of<F, _ArgNum, std::enable_if_t<is_transparent_v<F>>, ArgsT...> {
        using type = typename details::function<F>::template arg_type<_ArgNum, ArgsT...>;
    };

    template <typename F, size_t _ArgNum, typename... ArgsT>
    struct arg_type_of<F, _ArgNum, std::enable_if_t<!is_transparent_v<F>>, ArgsT...> {
        using type = typename details::function<F>::template arg_type<_ArgNum>;
    };


    template <typename T, typename U = void>
    struct is_function : public details::function<T> {};

    ////////////////////////////// Safe function traits //////////////////////////////

    // Getting return type
    template <typename F, typename... ArgsT>
    using return_type_of_t = typename return_type_of<F, void, ArgsT...>::type;

    // Getting args count
    template <typename F>
    SIC size_t args_count_of_v = details::function<F>::arity;

    // Getting is mutable
    template <typename F>
    SIC bool is_mutable_func_v = details::function<F>::is_mutable;

    // Getting type
    template <typename F>
    SIC FunctionType function_enum_type_t = details::function<F>::type;

    // Getting argument type
    template <typename F, size_t _ArgNum, typename... ArgsT>
    using arg_type_of_t = typename arg_type_of<F, _ArgNum, void, ArgsT...>::type;

    // Is function
    template <typename T>
    SIC bool is_function_v = is_function<T>::value;


    ////////////////////////////////// Other traits //////////////////////////////////

    template <typename Type>
    ICB is_number_v = std::is_floating_point_v<Type> || std::is_integral_v<Type>;

    template<typename T>
    struct is_tuple : std::false_type {};

    template<typename... Ts>
    struct is_tuple<std::tuple<Ts...>> : std::true_type {};

    template <typename Type>
    ICB is_tuple_v = is_tuple<std::decay_t<Type>>::value;

    template <typename T, typename ... Types>
    ICB is_any_of_v = (std::is_same_v<T, Types> || ...);

    template <typename T1, typename T2>
    ICB is_same_v = std::is_same_v<T1, T2>;

    template <bool _Cond, typename ReturnT = void>
    using enable_if_t = std::enable_if_t<_Cond, ReturnT>;

    template <typename T>
    using decay_t = std::decay_t<T>;


    template <typename T>
    struct is_c_string {
        SIC bool value = false;
    };

    template <typename T, size_t _Size>
    struct is_c_string<const T(&)[_Size]> {
        SIC bool value = is_same_v<char, T> || is_same_v<char16_t, T> || is_same_v<char32_t, T>;
    };

    template <typename T, size_t _Size>
    struct is_c_string<T[_Size]> {
        SIC bool value = is_same_v<char, T> || is_same_v<char16_t, T> || is_same_v<char32_t, T>;
    };

    template <typename T>
    ICB is_c_string_v = is_c_string<T>::value;

    template <typename T, template <typename...> typename T2>
    struct is_specialization_of : std::false_type {};

    template <template <typename...> typename T, typename... ArgsT>
    struct is_specialization_of<T<ArgsT...>, T>: std::true_type {};

    template <typename T1, template <typename...> typename T2>
    ICB is_specialization_of_v = is_specialization_of<
            std::remove_cv_t<decay_t<T1>>, T2>::value;

}


#undef ICB
#undef ICS
#undef SIC
