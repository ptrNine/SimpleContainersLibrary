#pragma once

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
    class Vector3Base {
        static_assert(is_number_v<Type>, "Template type must be number");

    public:
        using ValType = Type;

        constexpr Vector3Base() noexcept : _x(0), _y(0), _z(0) {}

        constexpr Vector3Base(Type x, Type y, Type z) noexcept
            : _x(x), _y(y), _z(z) {}

        constexpr explicit
        Vector3Base(Type val) noexcept : _x(val), _y(val), _z(val) {}


    public:
        ICA& set(Type x, Type y, Type z) noexcept {
            _x = x;
            _y = y;
            _z = z;
            return *static_cast<DerivedT*>(this);
        }

        ICA& x() noexcept { return _x; }
        ICA& y() noexcept { return _y; }
        ICA& z() noexcept { return _z; }

        const ICA& x() const noexcept { return _x; }
        const ICA& y() const noexcept { return _y; }
        const ICA& z() const noexcept { return _z; }

        // Same to '+', '-' ... operators
        ICA add(const DerivedT& r) const noexcept {
            return DerivedT(_x + r._x, _y + r._y, _z + r._z);
        }

        ICA sub(const DerivedT& r) const noexcept {
            return DerivedT(_x - r._x, _y - r._y, _z - r._z);
        }

        ICA scalarAdd(const Type& val) const noexcept {
            return DerivedT(_x + val, _y + val, _z + val);
        }

        ICA scalarSub(const Type& val) const noexcept {
            return DerivedT(_x - val, _y - val, _z - val);
        }

        ICA scalarMul(const Type& val) const noexcept {
            return DerivedT(_x * val, _y * val, _z * val);
        }

        ICA scalarDiv(const Type& val) const {
            return DerivedT(_x / val, _y / val, _z / val);
        }

        // Same to '+=', '-=', ... operators
        ICA& makeAdd(const DerivedT& r) noexcept {
            _x += r._x;
            _y += r._y;
            _z += r._z;
            return *static_cast<DerivedT*>(this);
        }

        ICA& makeSub(const DerivedT& r) noexcept {
            _x -= r._x;
            _y -= r._y;
            _z -= r._z;
            return *static_cast<DerivedT*>(this);
        }

        ICA& makeScalarAdd(const Type& val) noexcept {
            _x += val;
            _y += val;
            _z += val;
            return *static_cast<DerivedT*>(this);
        }

        ICA& makeScalarSub(const Type& val) noexcept {
            _x -= val;
            _y -= val;
            _z -= val;
            return *static_cast<DerivedT*>(this);
        }

        ICA& makeScalarMul(const Type& val) noexcept {
            _x *= val;
            _y *= val;
            _z *= val;
            return *static_cast<DerivedT*>(this);
        }

        ICA& makeScalarDiv(const Type& val) {
            _x /= val;
            _y /= val;
            _z /= val;
            return *static_cast<DerivedT*>(this);
        }

        ICA dotProduct(const DerivedT &r) const noexcept { return _x * r._x + _y * r._y + _z * r._z; }

        ICA& makeCrossProduct(const DerivedT& r) noexcept {
            _x =   _y * r._z - r._y * _z;
            _y = -(_x * r._z - r._x * _z);
            _z =   _x * r._y - r._x * _y;
            return *static_cast<DerivedT*>(this);
        }
        ICA crossProduct(const DerivedT& r) const {
            return DerivedT(
                  _y * r._z - r._y * _z,
                -(_x * r._z - r._x * _z),
                  _x * r._y - r._x * _y
            );
        }

        ICA magnitude2() const noexcept { return _x * _x + _y * _y + _z * _z; }

        // Format and print
        auto to_string() const -> scl::String {
            return scl::String().sprintf("{} {}, {}, {} {}", "{", _x, _y, _z, "}");
        }

        void print(std::ostream& os = std::cout) const {
            os << to_string();
        }

        friend std::ostream& operator<< (std::ostream& os, const DerivedT& vec) { vec.print(os); return os; }

        // Operators

        // Equality
        ICA operator==(const DerivedT& r) const noexcept { return _x == r._x && _y == r._y && _z == r._z; }
        ICA operator!=(const DerivedT& r) const noexcept { return !(*this == r); }

        // Vectors
        ICA  operator+(const DerivedT& r) const noexcept { return add(r); }
        ICA  operator-(const DerivedT& r) const noexcept { return sub(r); }
        ICA  operator*(const DerivedT& r) const noexcept { return crossProduct(r); }

        ICA& operator+=(const DerivedT& r) noexcept { return makeAdd(r); }
        ICA& operator-=(const DerivedT& r) noexcept { return makeAub(r); }
        ICA& operator*=(const DerivedT& r) noexcept { return makeCrossProduct(r);}

        // Scalars
        ICA  operator+(const Type& val) const noexcept { return scalarAdd(val); }
        ICA  operator-(const Type& val) const noexcept { return scalarSub(val); }
        ICA  operator*(const Type& val) const noexcept { return scalarMul(val); }
        ICA  operator/(const Type& val) const { return scalarDiv(val); }

        ICA& operator+=(const Type& val) noexcept { return makeScalarAdd(val); }
        ICA& operator-=(const Type& val) noexcept { return makeScalarSub(val); }
        ICA& operator*=(const Type& val) noexcept { return makeScalarMul(val); }
        ICA& operator/=(const Type& val) { return makeScalarDiv(val); }

        template<SizeT _pos>
        friend constexpr auto get(DerivedT& v) noexcept -> Type& {
            if constexpr      (_pos == 0) return v._x;
            else if constexpr (_pos == 1) return v._y;
            return v._z;
        }

        template<SizeT _pos>
        friend constexpr auto get(const DerivedT& v) noexcept -> const Type& {
            if constexpr      (_pos == 0) return v._x;
            else if constexpr (_pos == 1) return v._y;
            return v._z;
        }

        template<SizeT _pos>
        friend constexpr auto get(DerivedT&& v) noexcept -> Type&& {
            if constexpr      (_pos == 0) return std::move(v._x);
            else if constexpr (_pos == 1) return std::move(v._y);
            return std::move(v._z);
        }

    protected:
        Type _x, _y, _z;
    };


    template <typename Type>
    class Vector3 : public Vector3Base<Type, Vector3<Type>> {
        using super = Vector3Base<Type, Vector3<Type>>;
    public:
        using ValType = Type;

        constexpr Vector3() noexcept : super() {}

        constexpr Vector3(Type x, Type y, Type z) noexcept : super(x, y, z) {}

        constexpr explicit
        Vector3(Type val) noexcept : super(val) {}
    };

} // namespace ftl

namespace std {

    template<typename Type>
    struct tuple_size<scl::Vector3<Type>> : public integral_constant<scl::SizeT, 3> {
    };

    template<scl::SizeT _pos, typename Type>
    struct tuple_element<_pos, scl::Vector3<Type>> {
        using type = Type;
    };
}

namespace scl {
    template<typename Type>
    class Vector3Flt : public Vector3Base<Type, Vector3Flt<Type>> {
        using super = Vector3Base<Type, Vector3Flt<Type>>;
    public:
        constexpr Vector3Flt() noexcept : super() {}

        constexpr Vector3Flt(Type x, Type y, Type z) noexcept
            : super(x, y, z) {}

        constexpr explicit
        Vector3Flt(Type val) noexcept : super(val) {}


        template<std::size_t _steps = 1>
        IA fastInvMagnitude() const { return math::fast_inv_sqrt<_steps>(this->magnitude2()); }

        template<std::size_t _steps = 1>
        IA fastMagnitude() const { return 1 / fastInvMagnitude<_steps>(); }
        IA magnitude    () const { return 1 / fastInvMagnitude<VECTOR_FISR_ITERS_COUNT>(); }

        template<std::size_t _steps = 1>
        IA& makeFastNormalize() { return ((*this) *= fastInvMagnitude<_steps>()); }
        IA& makeNormalize    () { return ((*this) *= fastInvMagnitude<VECTOR_FISR_ITERS_COUNT>()); }

        template<std::size_t _steps = 1>
        IA fastNormalize() const { return Vector3Flt((*this) * fastInvMagnitude<_steps>()); }
        IA normalize    () const { return super::scalarMul(fastInvMagnitude<VECTOR_FISR_ITERS_COUNT>()); }
    };

} // namespace ftl


namespace std {
    template <typename Type>
    struct tuple_size<scl::Vector3Flt<Type>> : public integral_constant<std::size_t, 3> {};

    template <size_t _pos, typename Type>
    struct tuple_element<_pos, scl::Vector3Flt<Type>> { using type = Type; };
}


namespace scl {
    using Vector3u   = Vector3<unsigned long>;
    using Vector3i   = Vector3<long>;

    using Vector3u8  = Vector3<U8>;
    using Vector3u16 = Vector3<U16>;
    using Vector3u32 = Vector3<U32>;
    using Vector3u64 = Vector3<U64>;
    using Vector3s8  = Vector3<S8>;
    using Vector3s16 = Vector3<S16>;
    using Vector3s32 = Vector3<S32>;
    using Vector3s64 = Vector3<S64>;

    using Vector3f   = Vector3Flt<Float32>;
    using Vector3d   = Vector3Flt<Float64>;
    using Vector3f32 = Vector3Flt<Float32>;
    using Vector3f64 = Vector3Flt<Float64>;

    template <typename Type>
    ICA Vector3T(Type x, Type y, Type z) -> std::enable_if_t<std::is_integral_v<decay_t<Type>>, Vector3<Type>> {
        return Vector3<Type>(x, y, z);
    }
    template <typename Type>
    ICA Vector3T(Type x, Type y, Type z) -> std::enable_if_t<std::is_floating_point_v<decay_t<Type>>, Vector3Flt<Type>> {
        return Vector3Flt<Type>(x, y, z);
    }

    template <typename OutType, typename InType, std::enable_if_t<std::is_integral_v<decay_t<InType>>>...>
    ICA Vector3Convert(const Vector3<InType>& vec) {
        return Vector3T<OutType>(vec.x(), vec.y());
    }
    template <typename OutType, typename InType, std::enable_if_t<std::is_floating_point_v<decay_t<InType>>>...>
    ICA Vector3Convert(const Vector3Flt<InType>& vec) {
        return Vector3T<OutType>(vec.x(), vec.y());
    }

} // namespace ftl

// fmt format
template <typename Type>
struct fmt::formatter<scl::Vector3<Type>> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const scl::Vector3<Type>& vec, FormatContext& ctx) {
        return format_to(ctx.out(), "{}", vec.to_string());
    }
};

template <typename Type>
struct fmt::formatter<scl::Vector3Flt<Type>> {
    template <typename ParseContext>
    constexpr auto parse(ParseContext &ctx) { return ctx.begin(); }

    template <typename FormatContext>
    auto format(const scl::Vector3Flt<Type>& vec, FormatContext& ctx) {
        return format_to(ctx.out(), "{}", vec.to_string());
    }
};

#undef ICA // inline constexpr auto
