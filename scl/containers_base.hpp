#ifndef DECAYENGINE_CONTAINERS_BASE_HPP
#define DECAYENGINE_CONTAINERS_BASE_HPP

#include <iostream>
#include <string>
#include <fmt/format.h>

#include "types.hpp"
#include "traits.hpp"

namespace scl {

    template <typename CharT, typename TraitsT, typename AllocT>
    class StringBase;

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

        template <typename StrT, typename CIterT>
        static StrT _iter_to_string(CIterT cbegin, CIterT cend, SizeT size) {
            StrT str;

            switch (size) {
                case 0: return "{}";
                case 1: return StrT("{ ") + StrT(fmt::to_string(*cbegin)) + " }";
                default:
                    str += StrT("{ ") += StrT(fmt::to_string(*cbegin++));
                    for (; cbegin != cend; ++cbegin)
                        str += StrT(", ") += StrT(fmt::to_string(*cbegin));
                    str += " }";
                    return str;
            }
        }

        template <typename OutStrT, typename Container, typename StrT>
        auto _str_fold(const Container& c, const StrT& delim) {
            auto res = OutStrT();
            auto sz = c.size();

            if (sz == 0)
                return res;
            else if (sz == 1)
                return OutStrT(fmt::to_string(c.front()));
            else {
                for (SizeT i = 0; i < sz - 1; ++i)
                    res += fmt::to_string(c[i]) += delim;
                return res + fmt::to_string(c.back());
            }
        }


        template <typename OutStrT, typename Container, typename StrT, typename F>
        auto _str_fold_if(const Container& c, const StrT& delim, F callback) {
            static_assert(args_count_v<F> == 1 || args_count_v<F> == 2,
                          "Callback has wrong number of arguments");

            auto res = OutStrT();
            res.reserve(c.size() * std::size(delim));
            auto sz = c.size();

            if (sz == 0)
                return res;
            else if (sz == 1) {
                if constexpr (args_count_v<F> == 1)
                    return callback(c.front()) ? OutStrT(fmt::to_string(c.front())) : res;
                else
                    return callback(c.front(), 0) ? OutStrT(fmt::to_string(c.front())) : res;
            } else {
                if constexpr (args_count_v<F> == 1) {
                    for (SizeT i = 0; i < sz - 1; ++i)
                        if (callback(c[i]))
                            res += fmt::to_string(c[i]) += delim;
                    return callback(c.back()) ? res += fmt::to_string(c.back()) : (res.empty() ? res : res.pop_back());
                }
                else {
                    for (SizeT i = 0; i < sz - 1; ++i)
                        if (callback(c[i], i))
                            res += fmt::to_string(c[i]) += delim;
                    return callback(c.back(), sz - 1) ? res += fmt::to_string(c.back())
                                                    : (res.empty() ? res : res.pop_back());
                }
            }
        }

        // FNV1a32 for 32-bit arch
        template <typename T>
        inline auto hash(T* data, SizeT size, uint32_t hash = 0x811c9dc5)
        -> std::enable_if_t<sizeof(T*) == 4, SizeT>
        {
            auto ptr = reinterpret_cast<uint8_t*>(data);
            size *= sizeof(T);

            while (size--)
                hash = (hash ^ *ptr++) * 0x01000193;

            return hash;
        }

        // FNV1a64 for 64-bit arch
        template <typename T>
        inline auto hash(T* data, SizeT size, uint64_t hash = 0xcbf29ce484222325)
        -> std::enable_if_t<sizeof(T*) == 8, SizeT>
        {
            auto ptr = reinterpret_cast<uint8_t*>(data);
            size *= sizeof(T);

            while (size--)
                hash = (hash ^ *ptr++) * 0x100000001b3;

            return hash;
        }

    } // namespace details

} // namespace scl



#endif //DECAYENGINE_CONTAINERS_BASE_HPP
