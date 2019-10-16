#include <catch2/catch.hpp>
#include <sstream>
#include "../scl/string.hpp"
#include "../scl/array.hpp"

TEST_CASE("Array") {
    SECTION("decomposition") {
        scl::Array array{1, 3, 3, 7};
        auto&[a, b, c, d] = array;
        REQUIRE(a == 1);
        REQUIRE(b == 3);
        REQUIRE(c == 3);
        REQUIRE(d == 7);
        c = 33;
        REQUIRE(array.at(2) == 33);
    }

    SECTION("map") {
        const scl::Array a{1, 2, 3, 4, 5, 6, 7};
        auto b = a.map([](const int &itm) { return std::to_string(itm); });
        auto str = std::string();
        for(auto& i : b)
            str += i;
        REQUIRE(str == "1234567");
    }

    SECTION("to_string") {
        const scl::Array a{1, 2, 3, 4, 5};
        REQUIRE(a.to_string() == "{ 1, 2, 3, 4, 5 }");
    }

    SECTION("print") {
        const scl::Array a{1, 2, 3, 4, 5};
        const scl::Array<int, 0> b = {};
        scl::Array c{scl::Array<std::string, 2>{"heh", "kek"}, scl::Array<std::string, 2>{"sas", "ses123"}};

        std::stringstream strm1, strm2, strm3;
        strm1 << a;
        strm2 << b;
        strm3 << c;

        REQUIRE(strm1.str() == "{ 1, 2, 3, 4, 5 }");
        REQUIRE(strm2.str() == "{}");
        REQUIRE(strm3.str() == "{ { heh, kek }, { sas, ses123 } }");
    }

    SECTION("str_fold") {
        const auto strs = scl::Array{"its", "a", "path"};
        REQUIRE(strs.str_fold("/") == "its/a/path");
    }

    SECTION("str_folt_if") {
        const auto strs = scl::Array{1, 200, 3, 34, 255};
        REQUIRE(strs.str_fold_if("/", [](int e) { return e < 150; }) == "1/3/34");
        REQUIRE(strs.str_fold_if("|", [](int _, int i) { return i > 1; }) == "3|34|255");
    }

    SECTION("reverse") {
        auto str = scl::Array{1, 2, 3, 4, 5}.reverse().to_string();
        REQUIRE(str == "{ 5, 4, 3, 2, 1 }");
    }

    SECTION("empty_array") {
        auto result = scl::Array<int, 0>{}. reverse()
                . map    ([](int a) { return a + 1; })
                . reduce ([](int sum, int i) { return sum + 1; }, 1);

        REQUIRE(result == 1);
    }
}