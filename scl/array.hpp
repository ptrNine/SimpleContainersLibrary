#ifndef UNTITLED6_ARRAY_HPP
#define UNTITLED6_ARRAY_HPP

#include <array>
#include <iostream>
#include <sstream>
#include <functional>
#include "containers_base.hpp"
//#include "string.hpp"

namespace scl {
    template <typename T, SizeT _Size, template<typename, SizeT> class Derived>
    class ArrayBase {
    public:
        using ValType = T;
        using Iter    = typename std::array<T, _Size>::iterator;
        using CIter   = typename std::array<T, _Size>::const_iterator;
        using RIter   = typename std::array<T, _Size>::reverse_iterator;
        using CRIter  = typename std::array<T, _Size>::const_reverse_iterator;

        static constexpr SizeT c_size = _Size;

    public:

        // No arguments, noexcept
        constexpr auto size    () const noexcept -> SizeT    { return _stl_array.size(); }
        constexpr auto max_size() const noexcept -> SizeT    { return _stl_array.max_size(); }
        constexpr auto empty   () const noexcept -> bool     { return _stl_array.empty(); }
        constexpr auto back    () noexcept       -> T&       { return _stl_array.back(); }
        constexpr auto back    () const noexcept -> const T& { return _stl_array.back(); }
        constexpr auto front   () noexcept       -> T&       { return _stl_array.front(); }
        constexpr auto front   () const noexcept -> const T& { return _stl_array.front(); }
        constexpr auto data    () noexcept       -> T*       { return _stl_array.data(); }
        constexpr auto data    () const noexcept -> const T* { return _stl_array.data(); }

        // Iterators
        constexpr auto begin   () noexcept       -> Iter   { return _stl_array.begin(); }
        constexpr auto end     () noexcept       -> Iter   { return _stl_array.end(); }
        constexpr auto begin   () const noexcept -> CIter  { return _stl_array.begin(); }
        constexpr auto end     () const noexcept -> CIter  { return _stl_array.end(); }
        constexpr auto cbegin  () const noexcept -> CIter  { return _stl_array.cbegin(); }
        constexpr auto cend    () const noexcept -> CIter  { return _stl_array.cend(); }
        constexpr auto rbegin  () noexcept       -> RIter  { return _stl_array.rbegin(); }
        constexpr auto rend    () noexcept       -> RIter  { return _stl_array.rend(); }
        constexpr auto rbegin  () const noexcept -> CRIter { return _stl_array.rbegin(); }
        constexpr auto rend    () const noexcept -> CRIter { return _stl_array.rend(); }
        constexpr auto crbegin () const noexcept -> CRIter { return _stl_array.crbegin(); }
        constexpr auto crend   () const noexcept -> CRIter { return _stl_array.crend(); }

        constexpr auto at (SizeT position)       -> T&       { return _stl_array.at(position); }
        constexpr auto at (SizeT position) const -> const T& { return _stl_array.at(position); }

        auto fill (const T& value) -> Derived<T, _Size>& {
            _stl_array.fill(std::cref(value)); return
            *static_cast<Derived<T, _Size>*>(this);
        }

        auto swap (Derived<T, _Size>& array) -> Derived<T, _Size>& {
            _stl_array.swap(array._stl_array);
            return *static_cast<Derived<T, _Size>*>(this);
        }

        // Operators
        constexpr auto operator[] (SizeT position) noexcept       -> T&       { return _stl_array[position]; }
        constexpr auto operator[] (SizeT position) const noexcept -> const T& { return _stl_array[position]; }

        friend std::ostream& operator<< (std::ostream& os, const ArrayBase& array) {
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
            return details::_iter_reduce(cbegin(), cend(), callback, init);
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
            return details::_iter_reduce(cbegin(), cend(), callback);
        }
        /**
         *
         * @tparam F - type of callback
         * @param[in] callback - function like auto f(Type item) -> Type
         * @param Type item - one item from vector
         * @return Vector with mapped values
         */
        template <typename F>
        auto map(F callback) const -> Derived<return_type_of_t<F>, _Size> const {
            static_assert(args_count_v<F> == 1 || args_count_v<F> == 2,
                          "Callback has wrong number of arguments");

            auto mapped = Derived<return_type_of_t<F>, _Size>();

            if constexpr (args_count_v<F> == 1) {
                for (SizeT i = 0; i < _Size; ++i)
                    mapped.at(i) = callback(at(i));
            } else if constexpr (args_count_v<F> == 2) {
                for (SizeT i = 0; i < _Size; ++i)
                    mapped.at(i) = callback(at(i), i);
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
        auto foreach(F callback) -> Derived<T, _Size>& {
            static_assert(is_same_v<return_type_of_t<F>, void>, "Foreach callback can't return value!");
            static_assert(args_count_v<F> == 1 || args_count_v<F> == 2,
                          "Callback has wrong number of arguments");

            if constexpr (args_count_v<F> == 1) {
                for (auto &item : _stl_array)
                    callback(item);
            } else if constexpr (args_count_v<F> == 2) {
                SizeT i = 0;
                for (auto &item : _stl_array)
                    callback(item, i++);
            }

            return *static_cast<Derived<T, _Size>*>(this);
        }

        /**
         * Create string from array with delimiter
         * @tparam StrT - string type
         * @param delim - delimiter
         * @return - new string
         */
        template <typename StrT>
        auto str_fold(const StrT& delim) const {
            auto res = std::string();

            if constexpr (_Size == 0)
                return res;
            else if constexpr (_Size == 1)
                return res = fmt::to_string(front());
            else {
                for (SizeT i = 0; i < _Size - 1; ++i)
                    res += fmt::to_string(at(i)) += delim;
                return res += fmt::to_string(back());
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
            static_assert(args_count_v<F> == 1 || args_count_v<F> == 2,
                          "Callback has wrong number of arguments");

            auto res = std::string();
            res.reserve(_Size * std::size(delim));

            if constexpr (_Size == 0)
                return res;
            else if constexpr (_Size == 1) {
                if constexpr (args_count_v<F> == 1)
                    return callback(front()) ? fmt::to_string(front()) : res;
                else
                    return callback(front(), 0) ? fmt::to_string(front()) : res;
            } else {
                if constexpr (args_count_v<F> == 1) {
                    for (SizeT i = 0; i < _Size - 1; ++i)
                        if (callback(at(i)))
                            res += fmt::to_string(at(i)) += delim;
                    return callback(back()) ? res += fmt::to_string(back()) : res;
                }
                else {
                    for (SizeT i = 0; i < _Size - 1; ++i)
                        if (callback(at(i), i))
                            res += fmt::to_string(at(i)) += delim;
                    return callback(back(), _Size - 1) ? res += fmt::to_string(back()) : res;
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
        auto sort(F callback) -> Derived<T, _Size>& {
            std::sort(_stl_array.begin(), _stl_array.end(), callback);
            return *static_cast<Derived<T, _Size>*>(this);
        }

        /**
         * Reverse the Array
         * @return reversed Array
         */
        auto reverse() const {
            auto res = Derived<T, _Size>();
            SizeT i = 0;

            for (auto iter = crbegin(); iter != crend(); ++iter)
                res[i++] = *iter;

            return res;
        }

        auto to_string() const -> std::string {
            return details::_iter_to_string(cbegin(), cend(), _Size);
        }

        void print(std::ostream& os = std::cout) const {
            details::_iter_print(cbegin(), cend(),_Size, os);
        }

        SizeT hash() const {
            return std::hash<std::array<T, _Size>>()(_stl_array);
        }

    public:
        // Operators

        auto operator==(const Derived<T, _Size>& arr) const -> bool {
            return std::equal(begin(), end(), arr.begin());
        }
        auto operator!=(const Derived<T, _Size>& arr) const -> bool {
            return !(*this == arr);
        }
        auto operator<(const Derived<T, _Size>& arr) const -> bool {
            return std::lexicographical_compare(begin(), end(), arr.begin(), arr.end());
        }
        auto operator>(const Derived<T, _Size>& arr) const -> bool {
            return *this < arr;
        }
        auto operator>=(const Derived<T, _Size>& arr) const -> bool {
            return !(*this < arr);
        }
        auto operator<=(const Derived<T, _Size>& arr) const -> bool {
            return !(*this > arr);
        }

        template <SizeT _size>
        auto operator+ (const Derived<T, _size>& arr) const {
            auto res = Derived<T, _Size + _size>();

            auto a =std::array{23, 5, 6};
        }

        template<SizeT _pos>
        friend constexpr auto get(Derived<T, _Size>& array) noexcept -> T& {
            return std::get<_pos>(array._stl_array);
        }
        template<SizeT _pos>
        friend constexpr auto get(const Derived<T, _Size>& array) noexcept -> const T& {
            return std::get<_pos>(array._stl_array);
        }
        template<SizeT _pos>
        friend constexpr auto get(Derived<T, _Size>&& array) noexcept -> T&& {
            return std::move(std::get<_pos>(array._stl_array));
        }

    public:
        std::array<T, _Size> _stl_array;
    };


    template <typename T, SizeT _Size>
    class Array : public ArrayBase<T, _Size, Array> {};


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
