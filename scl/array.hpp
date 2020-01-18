#ifndef UNTITLED6_ARRAY_HPP
#define UNTITLED6_ARRAY_HPP

#include <array>
#include <iostream>
#include <sstream>
#include <functional>
#include "containers_base.hpp"

namespace scl {
    template <typename T, SizeT _Size>
    class Array : public std::array<T, _Size>{
    public:
        using value_type = T;

    public:
        friend std::ostream& operator<< (std::ostream& os, const Array& array) {
            array.print(os);
            return os;
        }

        /**
         *
         * @tparam RedTp - return type of callback and type of its first argument
         * @tparam F - type of callback
         * @param[in] callback - function like auto f(RedTp res, Type item) -> RedTp
         * @param RedTp res - accumulated value
         * @param Type item - one item from vector
         * @param[in] init - initial value with RedTp type
         * @return accumulated value with RedTp type
         */
        template <typename RedTp, typename F>
        auto reduce(F callback, RedTp init) const {
            return details::_iter_reduce(this->cbegin(), this->cend(), callback, init);
        }

        /**
         *
         * @tparam RedTp - return type of callback and type of its first argument
         * @tparam F - type of callback
         * @param[in] callback - function like auto f(RedTp res, Type item) -> RedTp
         * @param RedTp res - accumulated value
         * @param Type item - one item from vector
         * @return accumulated value with RedTp type
         */
        template <typename F>
        auto reduce(F callback) const {
            return details::_iter_reduce(this->cbegin(), this->cend(), callback);
        }
        /**
         *
         * @tparam F - type of callback
         * @param[in] callback - function like auto f(Type item) -> Type
         * @param Type item - one item from vector
         * @return Vector with mapped values
         */
        template <typename F>
        auto map(F callback) const -> Array<return_type_of_t<F>, _Size> const {
            static_assert(args_count_of_v<F> == 1 || args_count_of_v<F> == 2,
                          "Callback has wrong number of arguments");

            auto mapped = Array<return_type_of_t<F>, _Size>();

            if constexpr (args_count_of_v<F> == 1) {
                for (SizeT i = 0; i < _Size; ++i)
                    mapped.at(i) = callback(this->at(i));
            } else if constexpr (args_count_of_v<F> == 2) {
                for (SizeT i = 0; i < _Size; ++i)
                    mapped.at(i) = callback(this->at(i), i);
            }

            return std::move(mapped);
        }
        /**
         *
         * @tparam F - type of callback
         * @param[in] callback - function like auto f(Type item) -> void
         * @param Type item - one item from vector
         * @return this Vector
         */
        template <typename F>
        auto foreach(F callback) -> Array<T, _Size>& {
            static_assert(is_same_v<return_type_of_t<F>, void>, "Foreach callback can't return value!");
            static_assert(args_count_of_v<F> == 1 || args_count_of_v<F> == 2,
                          "Callback has wrong number of arguments");

            if constexpr (args_count_of_v<F> == 1) {
                for (auto &item : *this)
                    callback(item);
            } else if constexpr (args_count_of_v<F> == 2) {
                SizeT i = 0;
                for (auto &item : *this)
                    callback(item, i++);
            }

            return *this;
        }
        /**
         * Create string from array with delimiter
         * @tparam StrT - string type
         * @param delim - delimiter
         * @return - new string
         */
        template <typename StrT>
        auto str_fold(const StrT& delim) const {
            auto res = scl::StringBase<Char8, std::char_traits<Char8>, std::allocator<Char8>>();

            if constexpr (_Size == 0)
                return res;
            else if constexpr (_Size == 1)
                return res = fmt::to_string(this->front());
            else {
                for (SizeT i = 0; i < _Size - 1; ++i)
                    res += fmt::to_string(this->at(i)) += delim;
                return res += fmt::to_string(this->back());
            }
        }
        /**
         * Create string from array with delimiter
         * @tparam StrT
         * @tparam F - type of callback
         * @param callback - function like auto f(Type item) -> bool
         * @param delim - delimiter
         * @return - new string
         */
        template <typename StrT, typename F>
        auto str_fold_if(const StrT& delim, F callback) const {
            static_assert(args_count_of_v<F> == 1 || args_count_of_v<F> == 2,
                          "Callback has wrong number of arguments");

            auto res = scl::StringBase<Char8, std::char_traits<Char8>, std::allocator<Char8>>();
            res.reserve(_Size * std::size(delim));

            if constexpr (_Size == 0)
                return res;
            else if constexpr (_Size == 1) {
                if constexpr (args_count_of_v<F> == 1)
                    return callback(this->front()) ? fmt::to_string(this->front()) : res;
                else
                    return callback(this->front(), 0) ? fmt::to_string(this->front()) : res;
            } else {
                if constexpr (args_count_of_v<F> == 1) {
                    for (SizeT i = 0; i < _Size - 1; ++i)
                        if (callback(this->at(i)))
                            res += fmt::to_string(this->at(i)) += delim;
                    return callback(this->back()) ? res += fmt::to_string(this->back()) : (res.empty() ? res : res.pop_back());
                }
                else {
                    for (SizeT i = 0; i < _Size - 1; ++i)
                        if (callback(this->at(i), i))
                            res += fmt::to_string(this->at(i)) += delim;
                    return callback(this->back(), _Size - 1) ? res += fmt::to_string(this->back())
                                                       : (res.empty() ? res : res.pop_back());
                }
            }
        }
        /**
         *
         * @tparam F - type of callback
         * @param[in] callback - function like auto f(Type l, Type r) -> bool
         * @param Type l - one item from array
         * @param Type r - next item from array
         * @return this Array
         */
        template <typename F>
        auto sort(F callback) -> Array<T, _Size>& {
            std::sort(this->begin(), this->end(), callback);
            return *this;
        }

        /**
         * Reverse the Array
         * @return reversed Array
         */
        auto reverse() const {
            auto res = Array<T, _Size>();
            SizeT i = 0;

            for (auto iter = this->crbegin(); iter != this->crend(); ++iter)
                res[i++] = *iter;

            return res;
        }

        auto to_string() const {
            using StrT = scl::StringBase<Char8, std::char_traits<Char8>, std::allocator<Char8>>;
            return details::_iter_to_string<StrT>(this->cbegin(), this->cend(), _Size);
        }

        void print(std::ostream& os = std::cout) const {
            details::_iter_print(this->cbegin(), this->cend(),_Size, os);
        }

        SizeT hash() const {
            return std::hash<std::array<T, _Size>>()(*this);
        }

    public:
        // Operators

        auto operator==(const Array<T, _Size>& arr) const -> bool {
            return std::equal(this->begin(), this->end(), arr.begin());
        }
        auto operator!=(const Array<T, _Size>& arr) const -> bool {
            return !(*this == arr);
        }
        auto operator<(const Array<T, _Size>& arr) const -> bool {
            return std::lexicographical_compare(this->begin(), this->end(), arr.begin(), arr.end());
        }
        auto operator>(const Array<T, _Size>& arr) const -> bool {
            return *this < arr;
        }
        auto operator>=(const Array<T, _Size>& arr) const -> bool {
            return !(*this < arr);
        }
        auto operator<=(const Array<T, _Size>& arr) const -> bool {
            return !(*this > arr);
        }

        template <SizeT _size>
        auto operator+ (const Array<T, _size>& arr) const {
            auto res = Array<T, _Size + _size>();
            SizeT j = 0;

            for (SizeT i = 0; i < _Size; ++i, ++j)
                res[j] = (*this)[i];

            for (SizeT i = 0; i < _size; ++i, ++j)
                res[j] = arr[i];

            return res;
        }

        template<SizeT _pos>
        friend constexpr auto get(Array<T, _Size>& array) noexcept -> T& {
            return std::get<_pos>(static_cast<std::array<T, _Size>&>(array));
        }
        template<SizeT _pos>
        friend constexpr auto get(const Array<T, _Size>& array) noexcept -> const T& {
            return std::get<_pos>(static_cast<const std::array<T, _Size>&>(array));
        }
        template<SizeT _pos>
        friend constexpr auto get(Array<T, _Size>&& array) noexcept -> T&& {
            return std::move(std::get<_pos>(static_cast<std::array<T, _Size>&&>(array)));
        }
    };


    template<typename Type, typename... ValType>
    Array(Type, ValType...)
    -> Array<std::enable_if_t<(std::is_same_v<Type, ValType> && ...), Type>, 1 + sizeof...(ValType)>;
}

namespace std {
    template<typename Type, size_t _size>
    struct tuple_size<scl::Array<Type, _size>> : public integral_constant<size_t, _size> {};

    template<size_t _pos, typename Type, size_t _size>
    struct tuple_element<_pos, scl::Array<Type, _size>> : tuple_element<_pos, std::array<Type, _size>> {};
}

// fmt format
template <typename Type, size_t _Size>
struct fmt::formatter<scl::Array<Type, _Size>> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const scl::Array<Type, _Size>& array, FormatContext& ctx) {
        return format_to(ctx.out(), "{}", array.to_string());
    }
};

// std chash
template <typename T, std::size_t _Size>
struct std::hash<scl::Array<T, _Size>> {
    size_t operator()(const scl::Array<T, _Size>& array) const {
        return array.hash();
    }
};

#endif //UNTITLED6_ARRAY_HPP
