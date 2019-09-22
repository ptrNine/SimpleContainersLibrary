#ifndef DECAYENGINE_CONTAINERS_BASE_HPP
#define DECAYENGINE_CONTAINERS_BASE_HPP

#include <iostream>
#include <string>
#include <fmt/format.h>

#include "types.hpp"
#include "traits.hpp"

namespace scl {

    namespace details {

        // Compile-time functions
        template <typename T, typename F, SizeT size, SizeT pos, SizeT count>
        static constexpr auto
        _count_elements(F callback, const T& array) noexcept -> enable_if_t<size == pos, SizeT> {
            return count;
        }

        template <typename T, typename F, SizeT size, SizeT pos = 0, SizeT count = 0>
        static constexpr auto
        _count_elements(F callback, const T& array) noexcept -> typename std::enable_if_t<size != pos, SizeT> {
            return callback(std::get<pos>(array)) ?
                   _count_elements<T, F, size, pos + 1, count + 1>(callback, array) :
                   _count_elements<T, F, size, pos + 1, count>(callback, array);
        }

        // Reduce with initial value
        template<typename I, typename F, typename InitT, typename RetT = return_type_of_t<F>>
        static auto _iter_reduce(I first, I last, F callback, const InitT& init) -> RetT
        {
            static_assert(args_count_v<F> == 2 || args_count_v<F> == 3,
                          "Callback has wrong number of arguments");

            RetT res = init;

            if constexpr (args_count_v<F> == 2) {
                for (; first != last; ++first)
                    res = callback(res, *first);
            }
            else if constexpr (args_count_v<F> == 3) {
                for (SizeT i = 0; first != last; ++first)
                    res = callback(res, *first, i++);
            }

            return res;
        }

        template <typename Type, typename ReturnType>
        constexpr bool is_number_or_same_types_t =
                (is_number_v<decay_t<Type>> && is_number_v<decay_t<ReturnType>>) ||
                 is_same_v<decay_t<Type>, decay_t<ReturnType>>;

        // Reduce without initial value.
        // This implementation works if type of element and return type of callback are numbers or if they are same
        template<typename I, typename F, typename RetT = return_type_of_t<F>>
        static auto _iter_reduce(I first, I last, F callback)
        -> enable_if_t<is_number_or_same_types_t<decltype(*first), RetT>, RetT>
        {
            static_assert(args_count_v<F> == 2 || args_count_v<F> == 3,
                          "Callback has wrong number of arguments");
            static_assert(args_count_v<F> != 3 || !is_number_v<RetT>,
                          "Reduce with indices in number-vector should have default value");

            if (first == last)
                return {};

            if constexpr (is_number_v<RetT>) {
                RetT init = *first++;

                if constexpr (args_count_v<F> == 2) {
                    for (; first != last; ++first)
                        init = callback(init, *first);
                } else if constexpr (args_count_v<F> == 3) {
                    for (SizeT i = 1; first != last; ++first)
                        init = callback(init, *first, i++);
                }

                return init;
            }
            else {
                RetT init = {};

                if constexpr (args_count_v<F> == 2) {
                    for (; first != last; ++first)
                        init = callback(init, *first);
                }
                else if constexpr (args_count_v<F> == 3) {
                    for (SizeT i = 0; first != last; ++first)
                        init = callback(init, *first, i++);
                }

                return init;
            }
        }

        // Reduce without initial value.
        // Inverted condition
        template<typename I, typename F, typename RetT = return_type_of_t<F>>
        static auto _iter_reduce(I first, I last, F callback)
        -> enable_if_t<!is_number_or_same_types_t<decltype(*first), RetT>, RetT>
        {
            static_assert(args_count_v<F> == 2 || args_count_v<F> == 3,
                          "Callback has wrong number of arguments");

            auto init = return_type_of_t<F>();

            if constexpr (args_count_v<F> == 2) {
                for (; first != last; ++first)
                    init = callback(init, *first);
            }
            else if constexpr (args_count_v<F> == 3) {
                for (SizeT i = 0; first != last; ++first)
                    init = callback(init, *first, i++);
            }

            return init;
        }

        template <typename CIterT>
        static void _iter_print(CIterT cbegin, CIterT cend, SizeT size, std::ostream& os = std::cout) {
            switch (size) {
                case 0: os << "{}"; return;
                case 1: os << "{ " << *cbegin << " }"; return;
                default:
                    os << "{ " << *cbegin++;
                    for (; cbegin != cend; ++cbegin)
                        os << ", " << *cbegin;
                    os << " }";
            }
        }

        template <typename CIterT>
        static std::string _iter_to_string(CIterT cbegin, CIterT cend, SizeT size) {
            std::string str;

            switch (size) {
                case 0: return "{}";
                case 1: return std::string("{ ") + fmt::to_string(*cbegin) + " }";
                default:
                    str += std::string("{ ") += fmt::to_string(*cbegin++);
                    for (; cbegin != cend; ++cbegin)
                        str += std::string(", ") += fmt::to_string(*cbegin);
                    str += " }";
                    return str;
            }
        }

    } // namespace details

} // namespace scl



#endif //DECAYENGINE_CONTAINERS_BASE_HPP
