#pragma once

#include <type_traits>
#include <tuple>

#define ICB inline constexpr bool
#define ICS inline constexpr std::size_t

namespace scl
{
    // Function traits

    namespace details
    {
        template<typename Ret, typename IsMutable, typename... Args>
        struct lambda_traits {
            using is_mutable  = IsMutable;
            using return_type = Ret;
            static constexpr auto arity = sizeof...(Args);

            template <std::size_t _index>
            using args = std::tuple_element_t<_index, std::tuple<Args...>>;
        };

        template <typename Ret, typename... Args>
        struct function_traits {
            using return_type = Ret;
            static constexpr auto arity = sizeof...(Args);

            template <std::size_t _index>
            using args = std::tuple_element_t<_index, std::tuple<Args...>>;
        };
    }

    // Lambda
    template<typename Lambda>
    struct function_traits
            : public function_traits<decltype(&Lambda::operator())>
    {};

    template<typename Ret, typename Lambda, typename... Args>
    struct function_traits<Ret(Lambda::*)(Args...)>
            : public details::lambda_traits<Ret, std::true_type, Args...>
    {};

    template<typename Ret, typename Lambda, typename... Args>
    struct function_traits<Ret(Lambda::*)(Args...) const>
            : public details::lambda_traits<Ret, std::false_type, Args...>
    {};


    // Function
    template <typename Ret, typename... Args>
    struct function_traits<Ret(*)(Args...)>
            : public function_traits<Ret(Args...)>
    {};

    template <typename Ret, typename... Args>
    struct function_traits<Ret(Args...)>
            : public details::function_traits<Ret, Args...>
    {};

    template <typename Function>
    using return_type_of_t = typename function_traits<Function>::return_type;

    template <typename Function, std::size_t _ArgNumber>
    using arg_type_of_t = typename function_traits<Function>::template args<_ArgNumber>;

    template <typename Function>
    ICS args_count_v = function_traits<Function>::arity;


    // Other traits
    template <typename Type>
    ICB is_number_v = std::is_floating_point_v<Type> || std::is_integral_v<Type>;

    namespace details {
        template<typename T>
        struct is_tuple_impl : std::false_type {};

        template<typename... Ts>
        struct is_tuple_impl<std::tuple<Ts...>> : std::true_type {};
    }

    template <typename Type>
    ICB is_tuple_v = details::is_tuple_impl<std::decay_t<Type>>::value;

    template <typename T, typename ... Types>
    ICB is_any_of_v = (std::is_same_v<T, Types> || ...);

    template <typename T1, typename T2>
    ICB is_same_v = std::is_same_v<T1, T2>;

    template <bool _Cond, typename ReturnT = void>
    using enable_if_t = std::enable_if_t<_Cond, ReturnT>;

    template <typename T>
    using decay_t = std::decay_t<T>;
}


#undef ICB
#undef ICS