#include <gtest/gtest.h>

#include "../scl/ring.hpp"

TEST(RingTests, Reduce) {
    // Reduce with string return type
    auto str1 = ftl::Ring{9,8,7,6,4}.reduce(
            [](const std::string& s, int n) {
                return s + std::to_string(n);
            }, std::string("AYE")
    );
    ASSERT_STREQ("AYE98764", str1.c_str());

    // Reduce with string return type and iter
    auto str1i = ftl::Ring{9,8,7,6,4}.reduce(
            [](const std::string& s, int n, int i) {
                return s + std::to_string(i) + ":" + std::to_string(n);
            }, std::string("AYE")
    );
    ASSERT_STREQ("AYE0:91:82:73:64:4", str1i.c_str());


    // Reduce with no initial value
    auto str2 = ftl::Ring{4,5,3,2}.reduce([](const std::string& s, int n) {
        return s + std::to_string(n);
    });
    ASSERT_STREQ("4532", str2.c_str());

    // Reduce with index and no initial value
    auto str2i = ftl::Ring{4,5,3,2}.reduce([](const std::string& s, int n, int i) {
        return s + std::to_string(i) + ":" + std::to_string(n);
    });
    ASSERT_STREQ("0:41:52:33:2", str2i.c_str());


    // Reduce with same value type
    auto val1 = ftl::Ring{2,5,4,1}.reduce([](int s, int n) { return s * n; });
    ASSERT_EQ(40, val1);

    // Reduce with same value type and iter
    auto val1i = ftl::Ring{2,5,4,1}.reduce([](int s, int n, int i) { return s * n + i; });
    ASSERT_EQ(49, val1i);

    // Reduce with same type and initial value
    auto val1v = ftl::Ring{2,5,4,1}.reduce([](int s, int n) { return s * n; }, 2);
    ASSERT_EQ(80, val1v);

    // Reduce with same type and initial value
    auto val1vi = ftl::Ring{2,5,4,1}.reduce([](int s, int n, int i) { return s * n + i; }, 2);
    ASSERT_EQ(89, val1vi);


    // Reduce with another number type
    auto val2 = ftl::Ring{100,2,5,4}.reduce([](double s, int n) { return s / n; });
    ASSERT_FLOAT_EQ(2.5, val2);

    // Reduce with another number type and iter
    auto val2i = ftl::Ring{100,2,5,4}.reduce([](double s, int n, int i) { return s / n + i; });
    ASSERT_FLOAT_EQ(6.05, val2i);

    // Reduce with another number type and initial value
    auto val2v = ftl::Ring{10,2,5,4}.reduce([](double s, int n) { return s / n; }, 200);
    ASSERT_FLOAT_EQ(0.5, val2v);

    // Reduce with same non-number type
    auto val3 = ftl::Ring{
            std::string("abc"),
            std::string("defg"),
            std::string("hi"),
            std::string("jklmno")
    }.reduce([](const std::string& s, const std::string& n) { return s + n; });
    ASSERT_STREQ(val3.c_str(), "abcdefghijklmno");

    // Reduce with same non-number type and iter
    auto val3i = ftl::Ring{
            std::string("abc"),
            std::string("defg"),
            std::string("hi"),
            std::string("jklmno")
    }.reduce([](const std::string& s, const std::string& n, int i) { return s + n + std::to_string(i); });
    ASSERT_STREQ(val3i.c_str(), "abc0defg1hi2jklmno3");


    // Reduce with same non-number type and initial value
    auto val3v = ftl::Ring{
            std::string("abc"),
            std::string("defg"),
            std::string("hi"),
            std::string("jklmno")
    }.reduce([](const std::string& s, const std::string& n) { return s + n; }, "kek");
    ASSERT_STREQ(val3v.c_str(), "kekabcdefghijklmno");


    // Reduce non-number with number return
    auto val4 = ftl::Ring{
            std::string("abc"),
            std::string("defg"),
            std::string("hi"),
            std::string("jklmno")
    }.reduce([](int s, const std::string& n) { return s + n.length(); });
    ASSERT_EQ(15, val4);

    // Reduce non-number with number return with iter
    auto val4i = ftl::Ring{
            std::string("abc"),
            std::string("defg"),
            std::string("hi"),
            std::string("jklmno")
    }.reduce([](int s, const std::string& n, int i) { return (s + n.length()) * i; }); // from 0 element
    ASSERT_EQ(54, val4i);

    // Reduce non-number with number return with initial value
    auto val4v = ftl::Ring{
            std::string("abc"),
            std::string("defg"),
            std::string("hi"),
            std::string("jklmno")
    }.reduce([](int s, const std::string& n) { return s + n.length(); }, 1);
    ASSERT_EQ(16, val4v);


    // Iter test
    std::string itest1;
    ftl::Ring{5,2,5,4,3,6}.reduce([&](double s, int n, int i) { itest1 += std::to_string(i); return s / n; });
    ASSERT_STREQ("12345", itest1.c_str());

    // Iter test with initial val
    std::string itest2;
    ftl::Ring{5,2,5,4,3,6}.reduce([&](double s, int n, int i) { itest2 += std::to_string(i); return s / n; }, 5);
    ASSERT_STREQ("012345", itest2.c_str());
}

TEST(RingTests, Map) {
    auto vec1 = ftl::Ring{4,5,2,75,3}.map([](int a) { return a + 1; }).to_string();
    ASSERT_STREQ(vec1.c_str(), "{ 5, 6, 3, 76, 4 }");

    auto vec1i = ftl::Ring{4,5,2,75,3}.map([](int a, int i) { return a + i; }).to_string();
    ASSERT_STREQ(vec1i.c_str(), "{ 4, 6, 4, 78, 7 }");

    auto vec2 = ftl::Ring{4,5,2,75,3}.map([](int a) { return std::to_string(a) + "s"; }).to_string();
    ASSERT_STREQ(vec2.c_str(), "{ 4s, 5s, 2s, 75s, 3s }");

    auto vec2i = ftl::Ring{4,5,2,75,3}.map([](int a, int i) { return std::to_string(a) + std::to_string(i); }).to_string();
    ASSERT_STREQ(vec2i.c_str(), "{ 40, 51, 22, 753, 34 }");
}

TEST(RingTests, Filter) {
    auto vec1 = ftl::Ring{4,5,2,75,3}.filter([](int a) { return a >= 5; }).to_string();
    ASSERT_STREQ(vec1.c_str(), "{ 5, 75 }");

    auto vec1i = ftl::Ring{4,5,75,2,3}.filter([](int a, int i) { return a < 5 && (i % 2) == 0; }).to_string();
    ASSERT_STREQ(vec1i.c_str(), "{ 4, 3 }");

    auto vec2 = ftl::Ring{4,5,2,75,3}.filter([](int a) { return a > 100; }).to_string();
    ASSERT_STREQ(vec2.c_str(), "{}");
}

TEST(RingTests, FilterMapReduce) {
    ftl::Ring v{1, 2, 3, 4, 4, 3, 2, 1};
    auto a = v.filter([](int i) { return i > 2; })
            .map   ([](int i) { return i*1.5; })
            .reduce([](double l, double r) { return l + r; });
    ASSERT_EQ(a, 21);
}

#include <random>

TEST(RingTests, BasicTests) {
    auto str1 = ftl::Ring<int>(5).to_string();
    ASSERT_STREQ(str1.c_str(), "{ 0, 0, 0, 0, 0 }");

    auto str2 = ftl::Ring<int>().to_string();
    ASSERT_STREQ(str2.c_str(), "{}");


    // Random push test
    auto randgen = std::bind(std::uniform_int_distribution<int>(-500, 500), std::mt19937());
    auto deque   = std::deque<int>();
    auto ring    = ftl::Ring <int>();

    repeat(10000) {
        int genval = randgen();
        if (genval % 2) {
            deque.push_back(genval);
            ring .push_back(genval);
        } else {
            deque.push_front(genval);
            ring .push_front(genval);
        }
    }

    std::stringstream ss;
    ftl::_iter_print(deque.begin(), deque.end(), deque.size(), ss);
    ASSERT_STREQ(ss.str().c_str(), ring.to_string().c_str());


    // Iter test
    deque.clear();
    ring .clear();

    repeat(1001) {
        int genval = randgen();
        if (genval % 2) {
            deque.push_back(genval);
            ring .push_back(genval);
        } else {
            deque.push_front(genval);
            ring .push_front(genval);
        }

        ASSERT_EQ(true, std::equal(deque.cbegin(), deque.cend(), ring.cbegin()));
        ASSERT_EQ(true, std::equal(deque.crbegin(), deque.crend(), ring.crbegin()));
    }

    // Random access test
    repeat(100000) {
        int genval = randgen() + 500;
        ASSERT(genval >= 0 && genval < 1001);
        ASSERT_EQ(deque[genval], ring[genval]);
    }


    // Resize tests
    deque.resize(50);
    ring .resize(50);
    ASSERT_EQ(1, std::equal(deque.cbegin(), deque.cend(), ring.cbegin()));

    deque.resize(100, 228);
    ring .resize(100, 228);
    ASSERT_EQ(1, std::equal(deque.cbegin(), deque.cend(), ring.cbegin()));
}