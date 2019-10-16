#include <random>
#include <deque>

#include <catch2/catch.hpp>
#include "../scl/ring.hpp"
using namespace Catch::literals;

#define repeat(COUNT) for (size_t _repeat_counter = 0; _repeat_counter < COUNT; ++_repeat_counter)

TEST_CASE("Ring") {


    SECTION("reduce") {

        SECTION("Reduce with string return type") {
            auto str1 = scl::Ring{9, 8, 7, 6, 4}.reduce(
                    [](const std::string &s, int n) {
                        return s + std::to_string(n);
                    }, std::string("AYE")
            );
            REQUIRE(str1 == "AYE98764");
        }

        SECTION("Reduce with string return type and iter") {
            auto str1i = scl::Ring{9, 8, 7, 6, 4}.reduce(
                    [](const std::string &s, int n, int i) {
                        return s + std::to_string(i) + ":" + std::to_string(n);
                    }, std::string("AYE")
            );
            REQUIRE(str1i == "AYE0:91:82:73:64:4");
        }

        SECTION("Reduce with no initial value") {
            auto str2 = scl::Ring{4, 5, 3, 2}.reduce([](const std::string &s, int n) {
                return s + std::to_string(n);
            });
            REQUIRE(str2 == "4532");
        }

        SECTION("Reduce with index and no initial value") {
            auto str2i = scl::Ring{4, 5, 3, 2}.reduce([](const std::string &s, int n, int i) {
                return s + std::to_string(i) + ":" + std::to_string(n);
            });
            REQUIRE(str2i == "0:41:52:33:2");
        }

        SECTION("Reduce with same value type") {
            auto val1 = scl::Ring{2, 5, 4, 1}.reduce([](int s, int n) { return s * n; });
            REQUIRE(val1 == 40);
        }

        SECTION("Reduce with same value type and iter") {
            auto val1i = scl::Ring{2, 5, 4, 1}.reduce([](int s, int n, int i) { return s * n + i; }, 1);
            REQUIRE(val1i == 49);
        }

        SECTION("Reduce with same type and initial value") {
            auto val1v = scl::Ring{2, 5, 4, 1}.reduce([](int s, int n) { return s * n; }, 2);
            REQUIRE(val1v == 80);
        }

        SECTION("Reduce with same type and initial value") {
            auto val1vi = scl::Ring{2, 5, 4, 1}.reduce([](int s, int n, int i) { return s * n + i; }, 2);
            REQUIRE(val1vi == 89);
        }

        SECTION("Reduce with another number type") {
            auto val2 = scl::Ring{100, 2, 5, 4}.reduce([](double s, int n) { return s / n; });
            REQUIRE(val2 == 2.5_a);
        }

        SECTION("Reduce with another number type and iter") {
            auto val2i = scl::Ring{100, 2, 5, 4}.reduce([](double s, int n, int i) { return s / n + i; }, 100);
            REQUIRE(val2i == 3.575_a);
        }

        SECTION("Reduce with another number type and initial value") {
            auto val2v = scl::Ring{10, 2, 5, 4}.reduce([](double s, int n) { return s / n; }, 200);
            REQUIRE(val2v == 0.5_a);
        }

        SECTION("Reduce with same non-number type") {
            auto val3 = scl::Ring{
                    std::string("abc"),
                    std::string("defg"),
                    std::string("hi"),
                    std::string("jklmno")
            }.reduce([](const std::string &s, const std::string &n) { return s + n; });
            REQUIRE(val3 == "abcdefghijklmno");
        }

        SECTION("Reduce with same non-number type and iter") {
            auto val3i = scl::Ring{
                    std::string("abc"),
                    std::string("defg"),
                    std::string("hi"),
                    std::string("jklmno")
            }.reduce([](const std::string &s, const std::string &n, int i) { return s + n + std::to_string(i); });
            REQUIRE(val3i == "abc0defg1hi2jklmno3");
        }

        SECTION("Reduce with same non-number type and initial value") {
            auto val3v = scl::Ring{
                    std::string("abc"),
                    std::string("defg"),
                    std::string("hi"),
                    std::string("jklmno")
            }.reduce([](const std::string &s, const std::string &n) { return s + n; }, "kek");
            REQUIRE(val3v == "kekabcdefghijklmno");
        }

        SECTION("Reduce non-number with number return") {
            auto val4 = scl::Ring{
                    std::string("abc"),
                    std::string("defg"),
                    std::string("hi"),
                    std::string("jklmno")
            }.reduce([](int s, const std::string &n) { return s + n.length(); });
            REQUIRE(val4 == 15);
        }

        SECTION("Reduce non-number with number return with iter") {
            auto val4i = scl::Ring{
                    std::string("abc"),
                    std::string("defg"),
                    std::string("hi"),
                    std::string("jklmno")
            }.reduce([](int s, const std::string &n, int i) { return (s + n.length()) * i; }); // from 0 element
            REQUIRE(val4i == 54);
        }

        SECTION("Reduce non-number with number return with initial value") {
            auto val4v = scl::Ring{
                    std::string("abc"),
                    std::string("defg"),
                    std::string("hi"),
                    std::string("jklmno")
            }.reduce([](int s, const std::string &n) { return s + n.length(); }, 1);
            REQUIRE(val4v == 16);
        }

        SECTION("Iter test") {
            std::string itest1;
            scl::Ring{5, 2, 5, 4, 3, 6}.reduce([&](double s, int n, int i) {
                itest1 += std::to_string(i);
                return s / n;
            }, 0);
            REQUIRE(itest1 == "012345");
        }

        SECTION("Iter test with initial val") {
            std::string itest2;
            scl::Ring{5, 2, 5, 4, 3, 6}.reduce([&](double s, int n, int i) {
                itest2 += std::to_string(i);
                return s / n;
            }, 5);
            REQUIRE(itest2 == "012345");
        }
    } // Reduce

    SECTION("map") {
        auto vec1 = scl::Ring{4,5,2,75,3}.map([](int a) { return a + 1; }).to_string();
        REQUIRE(vec1 == "{ 5, 6, 3, 76, 4 }");

        auto vec1i = scl::Ring{4,5,2,75,3}.map([](int a, int i) { return a + i; }).to_string();
        REQUIRE(vec1i == "{ 4, 6, 4, 78, 7 }");

        auto vec2 = scl::Ring{4,5,2,75,3}.map([](int a) { return std::to_string(a) + "s"; }).to_string();
        REQUIRE(vec2 == "{ 4s, 5s, 2s, 75s, 3s }");

        auto vec2i = scl::Ring{4,5,2,75,3}.map([](int a, int i) { return std::to_string(a) + std::to_string(i); }).to_string();
        REQUIRE(vec2i == "{ 40, 51, 22, 753, 34 }");
    }

    SECTION("filter") {
        auto vec1 = scl::Ring{4,5,2,75,3}.filter([](int a) { return a >= 5; }).to_string();
        REQUIRE(vec1 == "{ 5, 75 }");

        auto vec1i = scl::Ring{4,5,75,2,3}.filter([](int a, int i) { return a < 5 && (i % 2) == 0; }).to_string();
        REQUIRE(vec1i == "{ 4, 3 }");

        auto vec2 = scl::Ring{4,5,2,75,3}.filter([](int a) { return a > 100; }).to_string();
        REQUIRE(vec2 == "{}");
    }

    SECTION("filter.map.reduce") {
        scl::Ring v{1, 2, 3, 4, 4, 3, 2, 1};
        auto a = v.filter([](int i) { return i > 2; })
                .map   ([](int i) { return i*1.5; })
                .reduce([](double l, double r) { return l + r; });
        REQUIRE(a == 21);
    }

    SECTION("str_fold") {
        const auto strs = scl::Ring{"its", "a", "path"};
        REQUIRE(strs.str_fold("/") == "its/a/path");
    }

    SECTION("str_folt_if") {
        const auto strs = scl::Ring{1, 200, 3, 34, 255};
        REQUIRE(strs.str_fold_if("/", [](int e) { return e < 150; }) == "1/3/34");
        REQUIRE(strs.str_fold_if("|", [](int _, int i) { return i > 1; }) == "3|34|255");
    }

    SECTION("reverse") {
        auto str = scl::Ring{1, 2, 3, 4, 5}.reverse().to_string();
        REQUIRE(str == "{ 5, 4, 3, 2, 1 }");
    }

    SECTION("empty_array") {
        auto result = scl::Ring<int>{}
                . reverse()
                . map    ([](int a) { return a + 1; })
                . reduce ([](int sum, int i) { return sum + 1; }, 1);

        REQUIRE(result == 1);
    }

    SECTION("Validation")
    {
        auto str1 = scl::Ring<int>(5).to_string();
        REQUIRE(str1 == "{ 0, 0, 0, 0, 0 }");

        auto str2 = scl::Ring<int>().to_string();
        REQUIRE(str2 == "{}");


        // Random push test
        auto randgen = std::bind(std::uniform_int_distribution<int>(-500, 500), std::mt19937());
        auto deque = std::deque<int>();
        auto ring = scl::Ring<int>();

        repeat(10000)
        {
            int genval = randgen();
            if (genval % 2) {
                deque.push_back(genval);
                ring.push_back(genval);
            } else {
                deque.push_front(genval);
                ring.push_front(genval);
            }
        }

        std::stringstream ss;
        scl::details::_iter_print(deque.begin(), deque.end(), deque.size(), ss);
        REQUIRE(ring.to_string() == ss.str());


        // Iter test
        deque.clear();
        ring.clear();

        repeat(1001)
        {
            int genval = randgen();
            if (genval % 2) {
                deque.push_back(genval);
                ring.push_back(genval);
            } else {
                deque.push_front(genval);
                ring.push_front(genval);
            }

            REQUIRE(std::equal(deque.cbegin(), deque.cend(), ring.cbegin()));
            REQUIRE(std::equal(deque.crbegin(), deque.crend(), ring.crbegin()));
        }

        // Random access test
        repeat(100000)
        {
            int genval = randgen() + 500;
            REQUIRE(genval >= 0);
            REQUIRE(genval < 1001);
            REQUIRE(deque[genval] == ring[genval]);
        }


        // Resize tests
        deque.resize(50);
        ring.resize(50);
        REQUIRE(std::equal(deque.cbegin(), deque.cend(), ring.cbegin()));

        deque.resize(100, 228);
        ring.resize(100, 228);
        REQUIRE(std::equal(deque.cbegin(), deque.cend(), ring.cbegin()));
    }

    SECTION("Hash") {
        SECTION("Not fragmented") {
            auto ring = scl::Ring{1, 2, 3, 4, 5, 7};
            auto vec = std::vector<int>{1, 2, 3, 4, 5, 7};

        }

        SECTION("Fragmented") {
            auto ring = scl::Ring{4, 5, 7};
            ring.emplace_front(3).emplace_front(2).emplace_front(1);

            auto vec = std::vector<int>{1, 2, 3, 4, 5, 7};

            auto vec_hash = scl::details::hash(vec.data(), vec.size());
            REQUIRE(ring.hash() == vec_hash);
        }

    }
}