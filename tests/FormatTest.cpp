#include <catch2/catch.hpp>

#include "../scl/vector2.hpp"
#include "../scl/vector3.hpp"
#include "../scl/vector.hpp"
#include "../scl/array.hpp"
#include "../scl/string.hpp"

TEST_CASE("fmt scl") {
    SECTION("Vector2") {
        scl::Vector2u a {15, 20};
        scl::Vector2d b {15.5, 13.9};

        REQUIRE(fmt::format("{}", a) == "{ 15, 20 }");
        REQUIRE(fmt::format("{}", b) == "{ 15.5, 13.9 }");
    }

    SECTION("Vector3") {
        scl::Vector3u a {15, 20, 12};
        scl::Vector3d b {15.5, 13.9, 0.1};

        REQUIRE(fmt::format("{}", a) == "{ 15, 20, 12 }");
        REQUIRE(fmt::format("{}", b) == "{ 15.5, 13.9, 0.1 }");
    }

    SECTION("Vector") {
        scl::Vector vec {1, 2, 3, 3, 4, 5};
        REQUIRE(fmt::format("{}", vec) == "{ 1, 2, 3, 3, 4, 5 }");

        scl::Vector vec2 {scl::Vector2u{2, 2}, scl::Vector2u{3, 5}};
        REQUIRE(fmt::format("{}", vec2) == "{ { 2, 2 }, { 3, 5 } }");
    }

    SECTION("Array") {
        scl::Array arr {1, 2, 3, 3, 4, 5};
        REQUIRE(fmt::format("{}", arr) == "{ 1, 2, 3, 3, 4, 5 }");

        scl::Array arr2 {scl::Vector2u{2, 2}, scl::Vector2u{3, 5}};
        REQUIRE(fmt::format("{}", arr2) == "{ { 2, 2 }, { 3, 5 } }");
    }

    SECTION("String") {
        auto str = scl::String("sampletext");
        REQUIRE(fmt::format("{}", str) == "sampletext");

        str.sprintf("{} and {} and {}", 228, scl::Vector2u{25, 52}, scl::Array{1, 2, 3, 4});
        REQUIRE(str == "228 and { 25, 52 } and { 1, 2, 3, 4 }");
    }

}