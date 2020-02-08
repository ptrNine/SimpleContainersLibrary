#ifndef DECAYENGINE_STRING_HPP
#define DECAYENGINE_STRING_HPP

#include <utf8cpp/utf8.h>
#include "containers_base.hpp"

namespace scl {
    template<typename T, typename AllocT = std::allocator<T>>
    class Vector;

    template <typename CharT, typename TraitsT = std::char_traits<CharT>, typename AllocT = std::allocator<CharT>>
    class StringBase {
        static_assert(is_any_of_v<CharT, Char8, Char16, Char32>,
                      "String must contains char symbols only");

    public:
        using StdStringT = std::basic_string<CharT, TraitsT, AllocT>;
        using value_type  = CharT;
        using traits_type = TraitsT;
        using CharType    = CharT;
        using allocator_type = typename StdStringT::allocator_type;
        using SizeType = typename StdStringT::size_type;
        using IterT    = typename StdStringT::iterator;
        using C_IterT  = typename StdStringT::const_iterator;
        using R_IterT  = typename StdStringT::const_iterator;
        using CR_IterT = typename StdStringT::const_reverse_iterator;
        using StrView  = std::basic_string_view<CharT, TraitsT>;

    public:
        static constexpr auto npos = std::basic_string<CharT, TraitsT, AllocT>::npos;

        // Constructors
        StringBase() = default;

        StringBase(const StringBase& str)                           : _std_v(str._std_v) {}
        StringBase(const StringBase& str, SizeType pos)             : _std_v(str._std_v, pos) {}
        StringBase(const StringBase& str, SizeType pos, SizeType n) : _std_v(str._std_v, pos, n) {}

        StringBase(const std::basic_string<CharT, TraitsT, AllocT>& str) : _std_v(str) {}
        StringBase(const std::basic_string_view<CharT, TraitsT>& str) : _std_v(str) {}
        StringBase(std::basic_string<CharT, TraitsT, AllocT>&& str) : _std_v(std::move(str)) {}
        StringBase(std::basic_string_view<CharT, TraitsT>&& str) : _std_v(std::move(str)) {}

        StringBase(SizeType n, CharT c)            : _std_v(n, c) {}
        StringBase(std::initializer_list<CharT> l) : _std_v(l) {}
        StringBase(StringBase&& str) noexcept      : _std_v(std::move(str._std_v)) {}

        template<typename InputIterator>
        StringBase(InputIterator beg, InputIterator end) : _std_v(beg, end) {}

        StringBase(const CharT* str) : _std_v(str) {}
        template <std::size_t _Size>
        StringBase(const CharT(&str)[_Size]) : _std_v(str, _Size ? _Size-1 : 0) {}
        StringBase(const CharT* str, SizeT size) : _std_v(str, size) {}


        // Basic methods
        inline auto max_size() const { return _std_v.max_size(); }
        inline auto length  () const { return _std_v.length(); }
        inline auto capacity() const { return _std_v.capacity(); }
        inline auto empty   () const { return _std_v.empty(); }
        inline auto size    () const { return _std_v.size(); }
        inline auto c_str   () const { return _std_v.c_str(); }
        inline auto data    () const { return _std_v.data(); }
        inline auto data    ()       { return _std_v.data(); }
        inline auto clear   ()       { _std_v.clear(); return *this; }

        inline auto& resize  (SizeType size)          { _std_v.resize(size);    return *this; }
        inline auto& resize  (SizeType size, CharT c) { _std_v.resize(size, c); return *this; }
        inline auto& reserve (SizeType size)          { _std_v.reserve(size);   return *this; }

        inline auto at    (SizeType pos) -> CharT&             { return _std_v.at(pos); }
        inline auto at    (SizeType pos) const -> const CharT& { return _std_v.at(pos); }
        inline auto front () -> CharT&                         { return _std_v.front(); }
        inline auto front () const -> const CharT&             { return _std_v.front(); }
        inline auto back  () -> CharT&                         { return _std_v.back(); }
        inline auto back  () const -> const CharT&             { return _std_v.back(); }

        inline auto shrink_to_fit()     { _std_v.shrink_to_fit(); return *this; }

        inline auto push_back(CharT c) -> StringBase& { _std_v.push_back(c); return *this; }
        inline auto pop_back ()        -> StringBase& { _std_v.pop_back(); return *this; }

        inline void swap (StringBase& str) { _std_v.swap(str._std_v); }


        // Iterators
        inline auto begin   ()       { return _std_v.begin(); }
        inline auto end     ()       { return _std_v.end(); }
        inline auto rbegin  ()       { return _std_v.rbegin(); }
        inline auto rend    ()       { return _std_v.rend(); }
        inline auto cbegin  () const { return _std_v.cbegin(); }
        inline auto cend    () const { return _std_v.cend(); }
        inline auto crbegin () const { return _std_v.crbegin(); }
        inline auto crend   () const { return _std_v.crend(); }
        inline auto begin   () const { return _std_v.begin(); }
        inline auto end     () const { return _std_v.end(); }
        inline auto rbegin  () const { return _std_v.rbegin(); }
        inline auto rend    () const { return _std_v.rend(); }

        inline auto assign_c_str(const CharT* str) -> StringBase& {
            _std_v.assign(str);
            return *this;
        }

        inline auto assign_c_str(const CharT* str, SizeType n) -> StringBase& {
            _std_v.assign(str, n);
            return *this;
        }

        // Algorithms

        inline auto insert(C_IterT start, SizeType n, CharT c) {
            return _std_v.insert(start, n, c);
        }
        template <typename InputIterT>
        inline auto insert(C_IterT start, InputIterT beg, InputIterT end) {
            return _std_v.insert(start, beg, end);
        }
        inline auto insert(C_IterT start, CharT c) {
            return _std_v.insert(start, c);
        }
        inline void insert(IterT start, std::initializer_list<CharT> l) {
            _std_v.insert(start, l);
        }
        inline auto insert(SizeType pos, const StringBase& str) -> StringBase& {
            _std_v.insert(pos, str._std_v);
            return *this;
        }
        inline auto insert(SizeType pos, const StringBase& str,
                           SizeType pos2, SizeType n = npos) -> StringBase& {
            _std_v.insert(pos, str._std_v, pos2, n);
            return *this;
        }
        template<SizeType _Size>
        inline auto insert(SizeType pos, const CharT(&str)[_Size]) -> StringBase& {
            _std_v.insert(pos, str, _Size ? _Size-1 : 0);
            return *this;
        }
        template<SizeType _Size>
        inline auto insert(SizeType pos, const CharT(&str)[_Size],
                           SizeType pos2, SizeType n) -> StringBase& {
            _std_v.insert(pos, &str[0] + pos2, n);
            return *this;
        }
        inline auto insert(SizeType pos, SizeType n, CharT c) -> StringBase& {
            _std_v.insert(pos, n, c);
            return *this;
        }

        inline auto erase(SizeType pos, SizeType n = npos) -> StringBase& {
            _std_v.erase(pos, n);
            return *this;
        }
        inline auto erase(C_IterT pos) {
            return _std_v.erase(pos);
        }
        inline auto erase(C_IterT first, C_IterT last) {
            return _std_v.erase(first, last);
        }

        inline auto replace(SizeType pos, SizeType n, const StringBase& str) -> StringBase& {
            _std_v.replace(pos, n, str._std_v);
            return *this;
        }
        inline auto replace(SizeType pos,  SizeType n, const StringBase& str,
                            SizeType pos2, SizeType n2 = npos) -> StringBase& {
            _std_v.replace(pos, n, str._std_v, pos2, n2);
            return *this;
        }
        template <SizeType _Size>
        inline auto replace(SizeType pos, SizeType n, const CharT (&str)[_Size],
                            SizeType pos2, SizeType n2) -> StringBase& {
            _std_v.replace(pos, n, &str[0] + pos2, n2);
            return *this;
        }
        inline auto replace(SizeType pos, SizeType n1, SizeType n2, CharT c) -> StringBase& {
            _std_v.replace(pos, n1, n2, c);
            return *this;
        }
        inline auto replace(C_IterT i1, C_IterT i2, const StringBase& str) -> StringBase& {
            _std_v.replace(i1, i2, str._std_v);
            return *this;
        }
        template <SizeType _Size>
        inline auto replace(C_IterT i1, C_IterT i2, const CharT (&str)[_Size],
                            SizeType n2 = _Size ? _Size-1 : 0) -> StringBase& {
            _std_v.replace(i1, i2, &str[0], n2);
            return *this;
        }
        template <SizeType _Size>
        inline auto replace(C_IterT i1, C_IterT i2, const CharT (&str)[_Size],
                            SizeType pos, SizeType n2) -> StringBase& {
            _std_v.replace(i1, i2, &str[0] + pos, n2);
            return *this;
        }
        inline auto replace(C_IterT i1, C_IterT i2, SizeType n2, CharT c) -> StringBase& {
            _std_v.replace(i1, i2, n2, c);
            return *this;
        }
        template <typename InputIterT>
        inline auto replace(C_IterT i1, C_IterT i2, InputIterT s1, InputIterT s2) -> StringBase& {
            _std_v.replace(i1, i2, s1, s2);
            return *this;
        }
        inline auto replace(C_IterT i1, C_IterT i2, std::initializer_list<CharT> l) -> StringBase& {
            _std_v.replace(i1, i2, l);
            return *this;
        }

        #define FIND_METHODS_GENERATOR(METHOD_NAME, POS)                                                \
        template <SizeType _Size>                                                                       \
        inline auto METHOD_NAME(const CharT (&str)[_Size], SizeType pos = POS,                          \
                                SizeType n = _Size ? _Size-1 : 0) const {                               \
            return _std_v.METHOD_NAME(str, pos, n);                                                     \
        }                                                                                               \
        inline auto METHOD_NAME(const StringBase& str, SizeType pos = POS) const {                      \
            return _std_v.METHOD_NAME(str._std_v, pos);                                                 \
        }                                                                                               \
        inline auto METHOD_NAME(CharT c, SizeType pos = POS) const {                                    \
            return _std_v.METHOD_NAME(c, pos);                                                          \
        }

        FIND_METHODS_GENERATOR(find, 0);
        FIND_METHODS_GENERATOR(rfind, npos);
        FIND_METHODS_GENERATOR(find_first_of, 0);
        FIND_METHODS_GENERATOR(find_last_of, npos);
        FIND_METHODS_GENERATOR(find_first_not_of, 0);
        FIND_METHODS_GENERATOR(find_last_not_of, npos);

        inline auto substr(SizeType pos, SizeType n = npos) const {
            return StringBase(_std_v.substr(pos, n));
        }

        inline auto compare(const StringBase& str) const {
            return _std_v.compare(str._std_v);
        }
        template<SizeType _Size>
        inline auto compare(const CharT(&str)[_Size]) const {
            return _std_v.compare(str);
        }
        inline auto compare(SizeType pos, SizeType n, const StringBase& str) const {
            return _std_v.compare(pos, n, str._std_v);
        }
        template<SizeType _Size>
        inline auto compare(SizeType pos, SizeType n, const CharT(&str)[_Size]) const {
            return _std_v.compare(pos, n, str);
        }
        inline auto compare(SizeType pos1, SizeType n1, const StringBase& str,
                            SizeType pos2, SizeType n2) const {
            return _std_v.compare(pos1, n1, str._std_v, pos2, n2);
        }
        template<SizeType _Size>
        inline auto compare(SizeType pos1, SizeType n1, const CharT(&str)[_Size],
                            SizeType pos2, SizeType n2) const {
            return _std_v.compare(pos1, n1, str, std::basic_string_view(str, _Size));
        }
        // Operators

        inline auto& operator=(const StringBase& str) { _std_v = str._std_v; return *this; }

        inline auto operator==(const StringBase& str) const { return _std_v == str._std_v; }
        inline auto operator!=(const StringBase& str) const { return _std_v != str._std_v; }

        template <SizeType _Size>
        inline auto operator==(const CharT(&str)[_Size]) const { return _std_v == str; }
        template <SizeType _Size>
        inline auto operator!=(const CharT(&str)[_Size]) const { return _std_v != str; }

        template <SizeType _Size>
        friend inline auto operator==(const CharT(&str)[_Size], const StringBase& str2) { return str2 == str; }
        template <SizeType _Size>
        friend inline auto operator!=(const CharT(&str)[_Size], const StringBase& str2) { return str2 != str; }


        inline auto operator+(const StringBase& str) const    { return StringBase(_std_v + str._std_v); }
        inline auto operator+(CharT c) const                  { return StringBase(_std_v + c); }
        template <SizeType _Size>
        inline auto operator+(const CharT(&str)[_Size]) const { return StringBase(_std_v + str); }
        template <SizeType _Size>
        friend inline auto operator+(const CharT(&str)[_Size], const StringBase& str2) { return StringBase(str) + str2; }

        inline auto operator+=(const StringBase& str) -> StringBase& { _std_v += str._std_v; return *this; }
        inline auto operator+=(CharT c) -> StringBase&               { _std_v += c; return *this; }
        template <SizeType _Size>
        inline auto operator+=(const CharT(&str)[_Size]) -> StringBase& { _std_v += str; return *this; }

        inline auto operator/(const StringBase& str) const
        {
            if (_std_v.empty() || str.empty())
                return *this + str;
            else {
                if (_std_v.back() == '/' && str.front() == '/')
                    return StringBase(_std_v + (str.data() + 1));
                else if (_std_v.back() == '/' || str.front() == '/')
                    return StringBase(*this + str);
                else
                    return StringBase(*this + '/' + str);
            }
        }
        template <SizeType _Size>
        inline auto operator/(const CharT(&str)[_Size]) const
        {
            if (_std_v.empty() || _Size < 1)
                return *this + str;
            else {
                if (_std_v.back() == '/' && str[0] == '/')
                    return StringBase(_std_v + &str[1]);
                else if (_std_v.back() == '/' || str[0] == '/')
                    return StringBase(*this + str);
                else
                    return StringBase(*this + '/' + str);
            }
        }

        template <SizeType _Size>
        friend inline auto operator/(const CharT(&str)[_Size], const StringBase& str2) { return StringBase(str) / str2; }

        inline auto operator/=(const StringBase& str) -> StringBase&
        {
            if (_std_v.empty() || str.empty())
                *this += str;
            else {
                if (_std_v.back() == '/' && str.front() == '/')
                    _std_v += (str.data() + 1);
                else if (_std_v.back() == '/' || str.front() == '/')
                    *this += str;
                else
                    (*this += '/') += str;
            }
            return *this;
        }
        template <SizeType _Size>
        inline auto operator/=(const CharT(&str)[_Size]) -> StringBase&
        {
            if (_std_v.empty() || _Size < 1)
                _std_v += str;
            else {
                if (_std_v.back() == '/' && str[0] == '/')
                    _std_v += &str[1];
                else if (_std_v.back() == '/' || str[0] == '/')
                    _std_v += str;
                else
                    (_std_v += '/') += str;
            }
            return *this;
        }

    public:
        inline auto operator[](SizeType pos)       -> CharT&       { return _std_v[pos]; }
        inline auto operator[](SizeType pos) const -> const CharT& { return _std_v[pos]; }

        friend std::ostream& operator<< (std::ostream& os, const StringBase& str) {
            os << str.to_utf8().c_str();
            return os;
        }

        using StdStrCrefT = const std::basic_string<CharT, TraitsT, AllocT>&;
        using StdStrView  = std::basic_string_view<CharT, TraitsT>;

        operator StdStrCrefT () const noexcept { return _std_v; }
        operator StdStrView  () const noexcept { return StdStrView(_std_v); }

        friend void swap(StringBase& a, StringBase& b) {
            std::swap(a._std_v, b._std_v);
        }

        template <typename FmtT, typename... ArgsT>
        auto& sprintf(FmtT format, ArgsT... args) {
            _std_v = fmt::format(format, args...);
            return *this;
        }

        SizeT hash() const {
            return std::hash<StdStringT>()(_std_v);
        }

        template <typename StrT>
        auto split(CharT c) const {
            Vector<StrT, std::allocator<StrT>> vec;

            StrView data = _std_v;
            SizeType start = 0;

            for (SizeType i = 0; i < data.size(); ++i) {
                if (data[i] == c) {
                    if (start != i)
                        vec.emplace_back(data.substr(start, i - start));
                    start = i + 1;
                }
            }
            if (start != data.size())
                vec.emplace_back(data.substr(start, data.size()));

            return vec;
        }

        template <typename StrT>
        auto split(const StrView& str) const {
            if (str.empty())
                return Vector<StrT, std::allocator<StrT>>({*this});

            Vector<SizeType, std::allocator<SizeType>> idxs;
            idxs.reserve(128);

            Vector<StrT, std::allocator<StrT>> strs;
            StrView data = _std_v;

            {
                PtrDiff pos = -str.length();
                while ((pos = data.find(str, pos + str.length())) != PtrDiff(StrView::npos))
                    idxs.push_back(static_cast<SizeType>(pos));
            }

            strs.reserve(idxs.size() + 1);

            SizeType pos = 0;
            for (auto idx : idxs) {
                if (pos != idx)
                    strs.emplace_back(data.substr(pos, idx - pos));
                pos = idx + str.length();
            }
            if (pos != data.length())
                strs.emplace_back(data.substr(pos, data.length()));

            return strs;
        }

        template <typename StrT>
        auto split(std::initializer_list<CharT> l, bool createNullStrs = false) const {
            if (l.size() == 0)
                return Vector<StrT, std::allocator<StrT>>({*this});

            Vector<StrT, std::allocator<StrT>> vec;
            StrView data = _std_v;
            SizeType start = 0;

            for (SizeType i = 0; i < data.size(); ++i) {
                bool cmp = false;
                for (auto c : l) {
                    if (data[i] == c) {
                        cmp = true;
                        break;
                    }
                }
                if (cmp) {
                    if (createNullStrs || start != i)
                        vec.emplace_back(data.substr(start, i - start));
                    start = i + 1;
                }
            }
            if (start != data.size())
                vec.emplace_back(data.substr(start, data.size()));

            return vec;
        }

        template <typename T, typename ... Ts>
        auto split_str(T&& arg, Ts&& ... args) const {
            return split<StringBase>(arg, args...);
        }

        auto split_str(std::initializer_list<CharT> l, bool createNullStrs = false) const {
            return split<StringBase>(l, createNullStrs);
        }

        template <typename T, typename ... Ts>
        auto split_view(T&& arg, Ts&& ... args) const {
            return split<std::basic_string_view<CharT, TraitsT>>(arg, args...);
        }

        auto split_view(std::initializer_list<CharT> l, bool createNullStrs = false) const {
            return split<std::basic_string_view<CharT, TraitsT>>(l, createNullStrs);
        }

        auto parent_path() const -> StringBase {
            auto size = length();

            auto p = crbegin();

            if (p != crend() && *p == '/') {
                ++p;
                --size;
            }

            for (; p != crend() && *p != '/'; ++p)
                --size;

            if (p == crend())
                return {};

            return substr(0, size);
        }

        template <
                template<typename> class TraitsTemplate = std::char_traits,
                template<typename> class AllocTemplate  = std::allocator>
        auto to_utf8() const {
            using NewStrT = StringBase<Char8, TraitsTemplate<Char8>, AllocTemplate<Char8>>;

            if constexpr (is_same_v<CharT, Char8>) {
                return NewStrT(cbegin(), cend());
            }
            else if constexpr (is_same_v<CharT, Char16>){
                NewStrT str;
                utf8::utf16to8(cbegin(), cend(), std::back_inserter(str));
                return str;
            }
            else {
                NewStrT str;
                utf8::utf32to8(cbegin(), cend(), std::back_inserter(str));
                return str;
            }
        }

        template <
                template<typename> class TraitsTemplate = std::char_traits,
                template<typename> class AllocTemplate  = std::allocator>
        auto to_utf16() const {
            using NewStrT = StringBase<Char16, TraitsTemplate<Char16>, AllocTemplate<Char16>>;

            if constexpr (is_same_v<CharT, Char16>) {
                return NewStrT(cbegin(), cend());
            }
            else if constexpr (is_same_v<CharT, Char8>){
                NewStrT str;
                utf8::utf8to16(cbegin(), cend(), std::back_inserter(str));
                return str;
            }
            else {
                auto tmp = StringBase<Char8, TraitsTemplate<Char8>, AllocTemplate<Char8>>();
                NewStrT str;
                utf8::utf32to8(cbegin(), cend(), std::back_inserter(tmp));
                utf8::utf8to16(tmp.cbegin(), tmp.cend(), std::back_inserter(str));
                return str;
            }
        }

        template <
                template<typename> class TraitsTemplate = std::char_traits,
                template<typename> class AllocTemplate  = std::allocator>
        auto to_utf32() const {
            using NewStrT = StringBase<Char32, TraitsTemplate<Char32>, AllocTemplate<Char32>>;

            if constexpr (is_same_v<CharT, Char32>) {
                return NewStrT(cbegin(), cend());
            }
            else if constexpr (is_same_v<CharT, Char8>){
                NewStrT str;
                utf8::utf8to32(cbegin(), cend(), std::back_inserter(str));
                return str;
            }
            else {
                auto tmp = StringBase<Char8, TraitsTemplate<Char8>, AllocTemplate<Char8>>();
                NewStrT str;
                utf8::utf16to8(cbegin(), cend(), std::back_inserter(tmp));
                utf8::utf8to32(tmp.cbegin(), tmp.cend(), std::back_inserter(str));
                return str;
            }
        }

    protected:
        std::basic_string<CharT, TraitsT, AllocT> _std_v;
    };

    using String   = StringBase<Char8>;
    using String16 = StringBase<Char16>;
    using String32 = StringBase<Char32>;

} // namespace ftl

template <>
struct fmt::formatter<scl::String> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const scl::String& str, FormatContext& ctx) {
        return format_to(ctx.out(), "{}", str.to_utf8().c_str());
    }
};

template <typename CharT>
struct std::hash<scl::StringBase<CharT>> {
    size_t operator()(const scl::StringBase<CharT>& str) const {
        return str.hash();
    }
};

namespace scl {
    template <typename T>
    inline constexpr bool is_any_string_v =
            is_c_string_v<T> ||
            is_specialization_of_v<T, scl::StringBase> ||
            is_specialization_of_v<T, std::basic_string> ||
            is_specialization_of_v<T, std::basic_string_view> ||
            is_same_v<char*, T> || is_same_v<const char*, T> ||
            is_same_v<char16_t*, const T> || is_same_v<const char16_t*, const T> ||
            is_same_v<char32_t*, const T> || is_same_v<const char32_t*, const T>;

    template <typename T1, typename T2>
    int strcmp_any(T1&& str1, T2&& str2) {
        using CharT1 = std::decay_t<decltype(str1[0])>;
        using CharT2 = std::decay_t<decltype(str2[0])>;

        return scl::StringBase<CharT1>(std::forward<T1>(str1)).to_utf8().compare(
               scl::StringBase<CharT2>(std::forward<T2>(str2)).to_utf8().data());
    }
}

#endif //DECAYENGINE_STRING_HPP
