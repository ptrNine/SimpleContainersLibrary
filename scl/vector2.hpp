#ifndef UIBUILDER_VECTOR2_HPP
#define UIBUILDER_VECTOR2_HPP

#include <type_traits>
#include <utility>

#include "math.hpp"
#include "types.hpp"
#include "traits.hpp"
#include "containers_base.hpp"
#include "string.hpp"

#define ICA inline constexpr auto
#define IA inline auto

#ifndef VECTOR_FISR_ITERS_COUNT
#define VECTOR_FISR_ITERS_COUNT 3
#endif

namespace scl {

    template<typename Type, typename DerivedT>
    class Vector2Base {
        static_assert(is_number_v<Type>, "Template type must be number");

    public:
        using ValType = Type;

        constexpr Vector2Base() noexcept : _x(0), _y(0) {}

        constexpr Vector2Base(Type x, Type y) noexcept : _x(x), _y(y) {}

        explicit constexpr
        Vector2Base(Type val) noexcept : _x(val), _y(val) {}


    public:
        ICA& set(Type x, Type y) noexcept {
            _x = x;
            _y = y;
            return *static_cast<DerivedT*>(this);
        }

        ICA& x() noexcept { return _x; }
        ICA& y() noexcept { return _y; }

        const ICA& x() const noexcept { return _x; }
        const ICA& y() const noexcept { return _y; }

        // Same to '+', '-' ... operators
        ICA add(const DerivedT& r) const noexcept {
            return DerivedT(_x + r._x, _y + r._y);
        }

        ICA sub(const DerivedT& r) const noexcept {
            return DerivedT(_x - r._x, _y - r._y);
        }

        ICA scalarAdd(const Type& val) const noexcept {
            return DerivedT(_x + val, _y + val);
        }

        ICA scalarSub(const Type& val) const noexcept {
            return DerivedT(_x - val, _y - val);
        }

        ICA scalarMul(const Type& val) const noexcept {
            return DerivedT(_x * val, _y * val);
        }

        ICA scalarDiv(const Type& val) const {
            return DerivedT(_x / val, _y / val);
        }

        // Same to '+=', '-=', ... operators
        ICA& makeAdd(const DerivedT& r) noexcept {
            _x += r._x;
            _y += r._y;
            return *static_cast<DerivedT*>(this);
        }

        ICA& makeSub(const DerivedT& r) noexcept {
            _x -= r._x;
            _y -= r._y;
            return *static_cast<DerivedT*>(this);
        }

        ICA& makeScalarAdd(const Type& val) noexcept {
            _x += val;
            _y += val;
            return *static_cast<DerivedT*>(this);
        }

        ICA& makeScalarSub(const Type& val) noexcept {
            _x -= val;
            _y -= val;
            return *static_cast<DerivedT*>(this);
        }

        ICA& makeScalarMul(const Type& val) noexcept {
            _x *= val;
            _y *= val;
            return *static_cast<DerivedT*>(this);
        }

        ICA& makeScalarDiv(const Type& val) {
            _x /= val;
            _y /= val;
            return *static_cast<DerivedT*>(this);
        }

        ICA dotProduct(const DerivedT &r) const noexcept { return _x * r._x + _y * r._y; }

        ICA magnitude2() const noexcept { return _x * _x + _y * _y; }


        // Format and print
        auto to_string() const -> scl::String {
            return scl::String().sprintf("{} {}, {} {}", "{", _x, _y, "}");
        }

        void print(std::ostream& os = std::cout) const {
            os << to_string();
        }

        friend std::ostream& operator<< (std::ostream& os, const DerivedT& vec) { vec.print(os); return os; }

        // Operators

        // Equality
        ICA operator==(const DerivedT& r) const noexcept { return _x == r._x && _y == r._y; }
        ICA operator!=(const DerivedT& r) const noexcept { return !(*this == r); }

        // Vectors
        ICA  operator+(const DerivedT& r) const noexcept { return add(r); }
        ICA  operator-(const DerivedT& r) const noexcept { return sub(r); }

        ICA& operator+=(const DerivedT& r) noexcept { return makeAdd(r); }
        ICA& operator-=(const DerivedT& r) noexcept { return makeSub(r); }

        // Scalars
        ICA  operator+(const Type& val) const noexcept { return scalarAdd(val); }
        ICA  operator-(const Type& val) const noexcept { return scalarSub(val); }
        ICA  operator*(const Type& val) const noexcept { return scalarMul(val); }
        ICA  operator/(const Type& val) const          { return scalarDiv(val); }

        ICA& operator+=(const Type& val) noexcept { return makeScalarAdd(val); }
        ICA& operator-=(const Type& val) noexcept { return makeScalarSub(val); }
        ICA& operator*=(const Type& val) noexcept { return makeScalarMul(val); }
        ICA& operator/=(const Type& val)          { return makeScalarDiv(val); }

        ICA& operator=(const DerivedT& r) noexcept {
            _x = r._x;
            _y = r._y;
            return *static_cast<DerivedT*>(this);
        }

        template<SizeT _pos>
        friend constexpr auto get(DerivedT& v) noexcept -> Type& {
            if constexpr (_pos == 0) return v._x;
            return v._y;
        }

        template<SizeT _pos>
        friend constexpr auto get(const DerivedT& v) noexcept -> const Type& {
            if constexpr (_pos == 0) return v._x;
            return v._y;
        }

        template<SizeT _pos>
        friend constexpr auto get(DerivedT&& v) noexcept -> Type&& {
            if constexpr (_pos == 0) return std::move(v._x);
            return std::move(v._y);
        }

    protected:
        Type _x, _y;
    };

    template <typename Type>
    class Vector2 : public Vector2Base<Type, Vector2<Type>> {
        using super = Vector2Base<Type, Vector2<Type>>;
        using ValType = Type;

    public:
        constexpr Vector2() noexcept : super() {}

        constexpr Vector2(Type x, Type y) noexcept : super(x, y) {}

        explicit constexpr
        Vector2(Type val) noexcept : super(val) {}
    };
} // namespace ftl

namespace std {

    template<typename Type>
    struct tuple_size<scl::Vector2<Type>> : public integral_constant<scl::SizeT, 2> {
    };

    template<scl::SizeT _pos, typename Type>
    struct tuple_element<_pos, scl::Vector2<Type>> {
        using type = Type;
    };
}

namespace scl {
    template<typename Type>
    class Vector2Flt : public Vector2Base<Type, Vector2Flt<Type>> {
        using super = Vector2Base<Type, Vector2Flt<Type>>;
    public:
        constexpr Vector2Flt() noexcept : super() {}

        constexpr Vector2Flt(Type x, Type y) noexcept : super(x, y) {}

        explicit constexpr
        Vector2Flt(Type val) noexcept : super(val) {}


        template<std::size_t _steps = 1>
        IA fastInvMagnitude() const { return math::fast_inv_sqrt<_steps>(this->magnitude2()); }

        template<std::size_t _steps = 1>
        IA fastMagnitude() const { return 1 / fastInvMagnitude<_steps>(); }
        IA magnitude    () const { return 1 / fastInvMagnitude<VECTOR_FISR_ITERS_COUNT>(); }

        template<std::size_t _steps = 1>
        IA& makeFastNormalize()  { return ((*this) *= fastInvMagnitude<_steps>()); }
        IA& makeNormalize()      { return ((*this) *= fastInvMagnitude<VECTOR_FISR_ITERS_COUNT>()); }

        template<std::size_t _steps = 1>
        IA fastNormalize() const { Vector2Flt((*this) * fastInvMagnitude<_steps>()); }
        IA normalize    () const { return super::scalarMul(fastInvMagnitude<VECTOR_FISR_ITERS_COUNT>()); }

        /*
        IA& makeRawPerpendicular() {
            using inh = inherited;
            inh::_x = 1;
            inh::_y = -inh::_x / inh::_y;

            return std::isinf(inh::_y) ?
                inh::set(0, 1) :
                *this;
        };

        IA& makePerpendicular() {
            return makeRawPerpendicular().makeNormalize();
        }

        ICA rawPerpendicular() const {
            using inh = inherited;
            Type x = 1;
            Type y = -inh::_x / inh::_y;

            return std::isinf(y) ?
                Vector2Flt(0, 1) :
                Vector2Flt(x, y);
        }

        IA perpendicular() const {
            return rawPerpendicular().makeNormalize();
        }
         */
    };

} // namespace ftl


namespace std {
    template <typename Type>
    struct tuple_size<scl::Vector2Flt<Type>> : public integral_constant<std::size_t, 2> {};

    template <size_t _pos, typename Type>
    struct tuple_element<_pos, scl::Vector2Flt<Type>> { using type = Type; };
}


namespace scl {
    using Vector2u   = Vector2<unsigned long>;
    using Vector2i   = Vector2<long>;

    using Vector2u8  = Vector2<U8>;
    using Vector2u16 = Vector2<U16>;
    using Vector2u32 = Vector2<U32>;
    using Vector2u64 = Vector2<U64>;
    using Vector2s8  = Vector2<S8>;
    using Vector2s16 = Vector2<S16>;
    using Vector2s32 = Vector2<S32>;
    using Vector2s64 = Vector2<S64>;

    using Vector2f   = Vector2Flt<Float32>;
    using Vector2d   = Vector2Flt<Float64>;
    using Vector2f32 = Vector2Flt<Float32>;
    using Vector2f64 = Vector2Flt<Float64>;

    template <typename Type>
    ICA Vector2T(Type x, Type y) -> std::enable_if_t<std::is_integral_v<decay_t<Type>>, Vector2<Type>> {
        return Vector2<Type>(x, y);
    }
    template <typename Type>
    ICA Vector2T(Type x, Type y) -> std::enable_if_t<std::is_floating_point_v<decay_t<Type>>, Vector2Flt<Type>> {
        return Vector2Flt<Type>(x, y);
    }

    template <typename OutType, typename InType, std::enable_if_t<std::is_integral_v<decay_t<InType>>>...>
    ICA Vector2Convert(const Vector2<InType>& vec) {
        return Vector2T<OutType>(vec.x(), vec.y());
    }
    template <typename OutType, typename InType, std::enable_if_t<std::is_floating_point_v<decay_t<InType>>>...>
    ICA Vector2Convert(const Vector2Flt<InType>& vec) {
        return Vector2T<OutType>(vec.x(), vec.y());
    }

} // namespace scl

// fmt format
template <typename Type>
struct fmt::formatter<scl::Vector2<Type>> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const scl::Vector2<Type>& vec, FormatContext& ctx) {
        return format_to(ctx.out(), "{}", vec.to_string());
    }
};

template <typename Type>
struct fmt::formatter<scl::Vector2Flt<Type>> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const scl::Vector2Flt<Type>& vec, FormatContext& ctx) {
        return format_to(ctx.out(), "{}", vec.to_string());
    }
};


#undef ICA // inline constexpr auto
#undef ICA // inline auto

#endif //UIBUILDER_VECTOR2_HPP
