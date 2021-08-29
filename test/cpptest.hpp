#pragma once

#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>


class Test {
public:
    template<typename TestCase>
    void runTestCase(TestCase func, const std::string& test_name) {
        try {
            func();
            std::cerr << test_name << " OK" << std::endl;
        }
        catch (const std::exception& e) {
            ++fail_count_cases;
            std::cerr << test_name << " fail;\n" << e.what() << std::endl;
        }
    }

    ~Test() {
        if (fail_count_cases > 0) {
            std::cerr << fail_count_cases << " unit tests failed" << std::endl;
        }
        else {
            std::cerr << "\nAll test cases passed successfully" << std::endl;
        }
    }

private:
    int fail_count_cases = 0;
};

template<typename T>
std::ostream& operator<< (std::ostream& os, const std::vector<T>& vec) {
    os << "{";
    bool first = true;
    for (const auto& x : vec) {
        if (!first)
            os << ", ";

        first = false;
        os << x;
    }

    return os << "}";
}

template <typename K, typename V>
std::ostream& operator<< (std::ostream& os, const std::unordered_map<K, V>& m) {
    os << "{";
    bool first = true;
    for (const auto& [k, v] : m) {
        if (!first) {
            os << ", ";
        }

        first = false;
        os << k << ": " << v;
    }

    return os << "}";
}

std::ostream& operator<< (std::ostream& os, const std::type_info& s) {
    return os << s.name();
}

template<typename T, typename U>
void assertEqual(const T& a, const U& b, const std::string& hint = "") {
    if (a != b) {
        std::stringstream ss;
        ss << "Assertion failed: " << a << " != " << b ;
        if (!hint.empty())
            ss << "\nhint: " << hint << "\n";
   

        throw std::runtime_error(ss.str());
    }
}


#define RUN_TEST(test_obj, func) test_obj.runTestCase(func, #func)

#define ASSERT(x) {                         \
    std::stringstream ss;                   \
    ss << #x << "!= true, ";                \
    ss << __FILE__ << ":" << __LINE__;      \
    assertEqual(x, true, ss.str());         \
}

#define ASSERT_EQUAL(x, y) {                \
    std::stringstream ss;                   \
    ss << __FILE__ << ":" << __LINE__;      \
    assertEqual(x, y, ss.str());            \
}

#define ASSERT_THROW(func, expectedException, ...) {                        \
    std::stringstream  ss;                                                  \
    ss << #func << "() throw wrong exception type, "                        \
        << __FILE__ << ":" << __LINE__;                                     \
    try {                                                                   \
        func(__VA_ARGS__);                                                  \
    } catch (const std::exception& ex) {                                    \
        assertEqual(typeid(expectedException), typeid(ex), ss.str());       \
    }                                                                       \
}

#define ASSERT_THROW_MESSAGE(func, expectedException, whatString, ...) {    \
    std::stringstream  ss;                                                  \
    ss << #func << "() throw wrong exception message, "                     \
        << __FILE__ << ":" << __LINE__;                                     \
    try {                                                                   \
        func(__VA_ARGS__);                                                  \
    } catch (const expectedException& ex) {                                 \
        std::string err = ex.what();                                        \
        assertEqual(whatString, err, ss.str());                             \
    }                                                                       \
}

#define ASSERT_NOTHING_THROWN(func, ...) {                                  \
    std::stringstream  ss;                                                  \
    ss << "Assert fail: " << #func                                          \
        << "() received an unexpected exception, "                          \
        << __FILE__ << ":" << __LINE__ << "\n";                             \
    try {                                                                   \
        func(__VA_ARGS__);                                                  \
    } catch(const std::exception& ex) {                                     \
        throw  std::runtime_error(ss.str());                                \
    }                                                                       \
}
