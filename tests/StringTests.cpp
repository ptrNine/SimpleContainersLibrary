#include "../scl/string.hpp"
#include "../scl/vector.hpp"

#include "gtest/gtest.h"

TEST(StringTests, StringBase) {
    using scl::String;

    String str1("string");
    String str3(std::string_view("string"));

    auto cstr1 = str1.c_str();
    auto cstr3 = str3.c_str();

    auto size1 = str1.length();
    auto size3 = str3.length();

    auto newStr0 = str1 + String(" heh");
    auto newStr1 = str1 + " heh";

    auto vec = scl::Vector<char>{'v', 's', 't', 'r'};
    auto newStr3 = String(vec.cbegin(), vec.cend());
    auto newStr4 = String(vec.crbegin(), vec.crend());

    auto path1 = String("path")  / String("test");
    auto path2 = String("path")  / String("/test");
    auto path3 = String("path/") / String("test");
    auto path4 = String("path/") / String("/test");

    auto spath1 = String("path")  / "test/";
    auto spath2 = String("path")  / "/test/";
    auto spath3 = String("path/") / "test/";
    auto spath4 = String("path/") / "/test/";

    ASSERT_TRUE(str1 == "string");
    ASSERT_EQ(str3, String("string"));

    ASSERT_EQ(size1, 6);
    ASSERT_EQ(size3, 6);

    ASSERT_EQ(newStr0, "string heh");
    ASSERT_EQ(newStr3, "vstr");
    ASSERT_EQ(newStr4, "rtsv");

    ASSERT_EQ(path1, "path/test");
    ASSERT_EQ(path2, "path/test");
    ASSERT_EQ(path3, "path/test");
    ASSERT_EQ(path4, "path/test");

    ASSERT_EQ(spath1, "path/test/");
    ASSERT_EQ(spath2, "path/test/");
    ASSERT_EQ(spath3, "path/test/");
    ASSERT_EQ(spath4, "path/test/");

    path1 /= String("/kek");
    path2 /= String("/kek");
    path3 /= String("kek");
    path4 /= String("kek");

    spath1 /= "/kek";
    spath2 /= "/kek";
    spath3 /= "kek";
    spath4 /= "kek";

    ASSERT_EQ(path1, "path/test/kek");
    ASSERT_EQ(path2, "path/test/kek");
    ASSERT_EQ(path3, "path/test/kek");
    ASSERT_EQ(path4, "path/test/kek");

    ASSERT_EQ(spath1, "path/test/kek");
    ASSERT_EQ(spath2, "path/test/kek");
    ASSERT_EQ(spath3, "path/test/kek");
    ASSERT_EQ(spath4, "path/test/kek");

    ASSERT_EQ(String().assign_c_str("str"), "str");
    ASSERT_EQ(String().assign_c_str("str", 3), "str");

    path1 = path1.parent_path();
    ASSERT_EQ(path1, "path/test/");

    path1 = path1.parent_path();
    ASSERT_EQ(path1, "path/");

    path1 = path1.parent_path();
    ASSERT_EQ(path1, "");

    path1 = path1.parent_path();
    ASSERT_EQ(path1, "");
}


TEST(StringTests, StringBase16) {
    using String = scl::String16;

    String str1(u"string");
    String str3(std::basic_string_view(u"string"));

    auto cstr1 = str1.c_str();
    auto cstr3 = str3.c_str();

    auto size1 = str1.length();
    auto size3 = str3.length();

    auto newStr0 = str1 + String(u" heh");
    auto newStr1 = str1 + u" heh";

    auto vec = scl::Vector<scl::Char16>{u'v', u's', u't', u'r'};
    auto newStr3 = String(vec.cbegin(), vec.cend());
    auto newStr4 = String(vec.crbegin(), vec.crend());

    auto path1 = String(u"path")  / String(u"test");
    auto path2 = String(u"path")  / String(u"/test");
    auto path3 = String(u"path/") / String(u"test");
    auto path4 = String(u"path/") / String(u"/test");

    auto spath1 = String(u"path")  / u"test/";
    auto spath2 = String(u"path")  / u"/test/";
    auto spath3 = String(u"path/") / u"test/";
    auto spath4 = String(u"path/") / u"/test/";

    ASSERT_EQ(str1, u"string");
    ASSERT_EQ(str3, String(u"string"));

    ASSERT_EQ(size1, 6);
    ASSERT_EQ(size3, 6);

    ASSERT_EQ(newStr0, u"string heh");
    ASSERT_EQ(newStr3, u"vstr");
    ASSERT_EQ(newStr4, u"rtsv");

    ASSERT_EQ(path1,  u"path/test");
    ASSERT_EQ(path2,  u"path/test");
    ASSERT_EQ(path3,  u"path/test");
    ASSERT_EQ(path4,  u"path/test");
    ASSERT_EQ(spath1, u"path/test/");
    ASSERT_EQ(spath2, u"path/test/");
    ASSERT_EQ(spath3, u"path/test/");
    ASSERT_EQ(spath4, u"path/test/");

    path1 /= String(u"/kek");
    path2 /= String(u"/kek");
    path3 /= String(u"kek");
    path4 /= String(u"kek");

    spath1 /= u"/kek";
    spath2 /= u"/kek";
    spath3 /= u"kek";
    spath4 /= u"kek";

    ASSERT_EQ(path1, u"path/test/kek");
    ASSERT_EQ(path2, u"path/test/kek");
    ASSERT_EQ(path3, u"path/test/kek");
    ASSERT_EQ(path4, u"path/test/kek");

    ASSERT_EQ(spath1, u"path/test/kek");
    ASSERT_EQ(spath2, u"path/test/kek");
    ASSERT_EQ(spath3, u"path/test/kek");
    ASSERT_EQ(spath4, u"path/test/kek");

    ASSERT_EQ(String().assign_c_str(u"str"), u"str");
    ASSERT_EQ(String().assign_c_str(u"str", 3), u"str");

    path1 = path1.parent_path();
    ASSERT_EQ(path1, u"path/test/");

    path1 = path1.parent_path();
    ASSERT_EQ(path1, u"path/");

    path1 = path1.parent_path();
    ASSERT_EQ(path1, u"");

    path1 = path1.parent_path();
    ASSERT_EQ(path1, u"");
}

TEST(StringTests, Std) {
    using String = scl::StringBase<char>;
    auto str1  = String("string");
    auto str2  = String("string");
    auto str3  = String("string");
    auto str4  = String("string");
    auto str5  = String("string");
    auto str6  = String("string");
    auto str7  = String("string");
    auto str8  = String("string");
    auto str9  = String("string");
    auto str10 = String("string");
    auto str11 = String("string");
    auto str12 = String("string");
    auto str13 = String("string");
    auto str14 = String("string");
    auto str15 = String("string");
    auto str16 = String("string");
    auto str17 = String("string");
    auto str18 = String("string");
    auto str19 = String("string");
    auto str20 = String("string");
    auto str21 = String("string");
    auto str22 = String("string");
    auto str23 = String("string");

    str1.insert(str1.begin() + 1, 2, 'E');
    ASSERT_EQ(str1, "sEEtring");

    auto vec = scl::Vector{'1', '2'};
    str2.insert(str2.begin() + 1, vec.begin(), vec.end());
    ASSERT_EQ(str2, "s12tring");

    str3.insert(str3.end(), '1');
    ASSERT_EQ(str3, "string1");

    str4.insert(str4.begin(), {'1', '2', '3'});
    ASSERT_EQ(str4, "123string");

    str5.insert(1, str5);
    ASSERT_EQ(str5, "sstringtring");

    str6.insert(3, str6, 1, 2);
    ASSERT_EQ(str6, "strtring");

    str7.insert(3, "str");
    ASSERT_EQ(str7, "strstring");

    str7.insert(1, "");
    ASSERT_EQ(str7, "strstring");

    str8.insert(3, "kaaakk", 1, 3);
    ASSERT_EQ(str8, "straaaing");

    str9.insert(2, 5, 'A');
    ASSERT_EQ(str9, "stAAAAAring");

    str11.insert(3, 3, 'a');
    ASSERT_EQ(str11, "straaaing");

    str11.erase(1, 1);
    ASSERT_EQ(str11, "sraaaing");

    str11.erase(str11.end() - 1);
    ASSERT_EQ(str11, "sraaain");

    str11.erase(0, 1);
    str11.erase(str11.begin() + 1, str11.begin() + 3);
    ASSERT_EQ(str11, "rain");

    str12.replace(1, 2, String("ab"));
    ASSERT_EQ(str12, "sabing");

    str13.replace(1, 2, String("aakeaa"), 2, 2);
    ASSERT_EQ(str13, "skeing");

    str14.replace(1, 2, "aakeaa", 2, 2);
    ASSERT_EQ(str14, "skeing");

    str16.replace(1, 2, 5, 'k');
    ASSERT_EQ(str16, "skkkkking");

    str17.replace(str17.begin(), str17.begin() + 2, String("kekkek"));
    ASSERT_EQ(str17, "kekkekring");

    str18.replace(str18.begin(), str18.begin() + 2, "kekkek");
    ASSERT_EQ(str18, "kekkekring");

    str18.replace(str18.begin(), str18.begin() + 2, "abcd", 1, 2);
    ASSERT_EQ(str18, "bckkekring");

    str20.replace(str20.begin() + 1, str20.begin() + 3, 5, 'k');
    ASSERT_EQ(str20, "skkkkking");

    str20.replace(str20.begin() + 1, str20.end(), str21.begin() + 1, str21.end());
    ASSERT_EQ(str21, "string");

    str21.replace(str21.begin(), str21.begin() + 2, {'1', '2', '3'});
    ASSERT_EQ(str21, "123ring");

    ASSERT_EQ(3, str22.find("in"));
    ASSERT_EQ(3, str22.find(String("in")));
    ASSERT_EQ(3, str22.find('i'));

    ASSERT_EQ(3, str22.rfind("in"));
    ASSERT_EQ(3, str22.rfind(String("in")));
    ASSERT_EQ(3, str22.rfind('i'));

    ASSERT_EQ(3, str22.find_first_of("in"));
    ASSERT_EQ(3, str22.find_first_of(String("in")));
    ASSERT_EQ(3, str22.find_first_of('i'));

    ASSERT_EQ(4, str22.find_last_of("ni"));
    ASSERT_EQ(4, str22.find_last_of(String("ni")));
    ASSERT_EQ(3, str22.find_last_of('i'));

    ASSERT_EQ(3, str22.find_first_not_of("strng"));
    ASSERT_EQ(3, str22.find_first_not_of(String("strng")));
    ASSERT_EQ(1, str22.find_first_not_of('s'));

    ASSERT_EQ(3, str22.find_last_not_of("strng"));
    ASSERT_EQ(3, str22.find_last_not_of(String("strng")));
    ASSERT_EQ(4, str22.find_last_not_of('g'));
}

TEST(StringTests, Split) {
    using Str = scl::String;
    using Vec = scl::Vector<std::string_view>;

    ASSERT_TRUE(Str("").split_view(' ').empty());
    ASSERT_TRUE(Str(" ").split_view(' ').empty());
    ASSERT_TRUE(Str("  ").split_view(' ').empty());
    ASSERT_TRUE(Str("   ").split_view(' ').empty());

    ASSERT_TRUE(Str("abc").split_view(' ') == Vec{"abc"});
    ASSERT_TRUE(Str(" abc").split_view(' ') == Vec{"abc"});
    ASSERT_TRUE(Str("  abc").split_view(' ') == Vec{"abc"});
    ASSERT_TRUE(Str("   abc").split_view(' ') == Vec{"abc"});

    ASSERT_TRUE(Str("abc ").split_view(' ') == Vec{"abc"});
    ASSERT_TRUE(Str("abc  ").split_view(' ') == Vec{"abc"});
    ASSERT_TRUE(Str("abc   ").split_view(' ') == Vec{"abc"});

    ASSERT_TRUE(Str(" abc ").split_view(' ') == Vec{"abc"});
    ASSERT_TRUE(Str(" abc  ").split_view(' ') == Vec{"abc"});
    ASSERT_TRUE(Str("  abc ").split_view(' ') == Vec{"abc"});
    ASSERT_TRUE(Str("  abc  ").split_view(' ') == Vec{"abc"});
    ASSERT_TRUE(Str("  abc   ").split_view(' ') == Vec{"abc"});
    ASSERT_TRUE(Str("   abc  ").split_view(' ') == Vec{"abc"});
    ASSERT_TRUE(Str("   abc   ").split_view(' ') == Vec{"abc"});

    ASSERT_TRUE(Str("  abc   xyz kek 228 ").split_view(' ').to_string() == "{ abc, xyz, kek, 228 }");


    ASSERT_TRUE(Str("").split_view("||").empty());
    ASSERT_TRUE(Str("||").split_view("||").empty());
    ASSERT_TRUE(Str("||||").split_view("||").empty());
    ASSERT_TRUE(Str("||||||").split_view("||").empty());

    ASSERT_TRUE(Str("abc").split_view("||") == Vec{"abc"});
    ASSERT_TRUE(Str("||abc").split_view("||") == Vec{"abc"});
    ASSERT_TRUE(Str("||||abc").split_view("||") == Vec{"abc"});
    ASSERT_TRUE(Str("||||||abc").split_view("||") == Vec{"abc"});

    ASSERT_TRUE(Str("abc||").split_view("||") == Vec{"abc"});
    ASSERT_TRUE(Str("abc||||").split_view("||") == Vec{"abc"});
    ASSERT_TRUE(Str("abc||||||").split_view("||") == Vec{"abc"});

    ASSERT_TRUE(Str("||abc||").split_view("||") == Vec{"abc"});
    ASSERT_TRUE(Str("||abc||||").split_view("||") == Vec{"abc"});
    ASSERT_TRUE(Str("||||abc||").split_view("||") == Vec{"abc"});
    ASSERT_TRUE(Str("||||abc||||").split_view("||") == Vec{"abc"});
    ASSERT_TRUE(Str("||||abc||||||").split_view("||") == Vec{"abc"});
    ASSERT_TRUE(Str("||||||abc||||").split_view("||") == Vec{"abc"});
    ASSERT_TRUE(Str("||||||abc||||||").split_view("||") == Vec{"abc"});

    ASSERT_TRUE(Str("||||abc||||||xyz||kek||228||").split_view("||").to_string() == "{ abc, xyz, kek, 228 }");



    ASSERT_TRUE(Str("").split_view({' ', '\n'}).empty());
    ASSERT_TRUE(Str("\n").split_view({' ', '\n'}).empty());
    ASSERT_TRUE(Str(" \n").split_view({' ', '\n'}).empty());
    ASSERT_TRUE(Str(" \n ").split_view({' ', '\n'}).empty());

    ASSERT_TRUE(Str("abc").split_view({' ', '\n'}) == Vec{"abc"});
    ASSERT_TRUE(Str(" abc").split_view({' ', '\n'}) == Vec{"abc"});
    ASSERT_TRUE(Str("\n abc").split_view({' ', '\n'}) == Vec{"abc"});
    ASSERT_TRUE(Str("\n  abc").split_view({' ', '\n'}) == Vec{"abc"});

    ASSERT_TRUE(Str("abc\n").split_view({' ', '\n'}) == Vec{"abc"});
    ASSERT_TRUE(Str("abc\n ").split_view({' ', '\n'}) == Vec{"abc"});
    ASSERT_TRUE(Str("abc \n ").split_view({' ', '\n'}) == Vec{"abc"});

    ASSERT_TRUE(Str(" abc\n").split_view({' ', '\n'}) == Vec{"abc"});
    ASSERT_TRUE(Str(" abc \n").split_view({' ', '\n'}) == Vec{"abc"});
    ASSERT_TRUE(Str(" \nabc ").split_view({' ', '\n'}) == Vec{"abc"});
    ASSERT_TRUE(Str("\n\nabc\n\n").split_view({' ', '\n'}) == Vec{"abc"});
    ASSERT_TRUE(Str("  abc \n\n").split_view({' ', '\n'}) == Vec{"abc"});
    ASSERT_TRUE(Str(" \n abc\n ").split_view({' ', '\n'}) == Vec{"abc"});
    ASSERT_TRUE(Str(" \n abc \n\n").split_view({' ', '\n'}) == Vec{"abc"});

    ASSERT_TRUE(Str(" \nabc \n xyz\nkek 228\n").split_view({' ', '\n'}).to_string() == "{ abc, xyz, kek, 228 }");
}

TEST(StringTests, Conversions) {
    using std::string;
    using string16 = std::basic_string<scl::Char16>;
    using string32 = std::basic_string<scl::Char32>;

    using std::string_view;
    using string_view16 = std::basic_string_view<scl::Char16>;
    using string_view32 = std::basic_string_view<scl::Char32>;

    scl::String   str1 =  "its a string";
    scl::String16 str2 = u"its a string";
    scl::String32 str3 = U"its a string";
    const scl::String str4 = "its a string";

    string   test1 = str1;
    string16 test2 = str2;
    string32 test3 = str3;
    string   test4 = str4;

    string_view   test5 = str1;
    string_view16 test6 = str2;
    string_view32 test7 = str3;
    string_view   test8 = str4;

    ASSERT_EQ(test1,  "its a string");
    ASSERT_EQ(test2, u"its a string");
    ASSERT_EQ(test3, U"its a string");
    ASSERT_EQ(test4,  "its a string");
    ASSERT_EQ(test5,  "its a string");
    ASSERT_EQ(test6, u"its a string");
    ASSERT_EQ(test7, U"its a string");
    ASSERT_EQ(test8,  "its a string");
}

TEST(StringTests, UtfConversions) {
    auto utf8str  = scl::String  ( "Это строка");
    auto utf16str = scl::String16(u"Это строка");
    auto utf32str = scl::String32(U"Это строка");

    std::cerr << utf32str.to_utf16().to_utf8() << std::endl;
}