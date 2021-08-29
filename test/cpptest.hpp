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
std::ostream& operator<< (std::ostream& ss, const std::vector<T>& s) {
    ss << "{";
    bool first = true;
    for (const auto& x : s) {
        if (!first)
            ss << ", ";

        first = false;
        ss << x;
    }

    return ss << "}";
}

template <class K, class V>
std::ostream& operator<< (std::ostream& ss, const std::unordered_map<K, V>& m) {
    ss << "{";
    bool first = true;
    for (const auto& [k, v] : m) {
        if (!first) {
            ss << ", ";
        }

        first = false;
        ss << k << ": " << v;
    }

    return ss << "}";
}

template<class T, class U>
void assertEqual(const T& t, const U& u, const std::string& hint = "") {
    if (t != u) {
        std::stringstream ss;
        ss << "Assertion failed: " << t << " != " << u ;
        if (!hint.empty())
            ss << "\nhint: " << hint << "\n";

        throw std::runtime_error(ss.str());
    }
}

template<typename T, typename U>
void assertEqualType(const T& t_id, const U& u_id, const std::string& hint = "") {
    if (t_id != u_id) {
        std::stringstream ss;
        ss << "Assertion failed: " << t_id.name() << " != " << u_id.name();
        if (!hint.empty())
            ss << "\nhint: " << hint << "\n";

        throw std::runtime_error(ss.str());
    }
}


#define RUN_TEST(test_obj, func) test_obj.runTestCase(func, #func)

#define ASSERT_EQUAL(x, y) {                \
    std::stringstream ss;                   \
    ss << __FILE__ << ":" << __LINE__;      \
    assertEqual(x, y, ss.str());            \
}

#define ASSERT_THROW(func, expectedException, ...) {                        \
    std::stringstream  ss;                                                  \
    oss << #func << "() throw wrong exception type, "                       \
        << __FILE__ << ":" << __LINE__;                                     \
    try {                                                                   \
        func(__VA_ARGS__);                                                  \
    } catch (const std::exception& ex) {                                    \
        assertEqualType(typeid(expectedException), typeid(ex), ss.str());   \
    }                                                                       \
}

#define ASSERT_THROW_MESSAGE(func, expectedException, whatString, ...) {    \
    std::stringstream  ss;                                                  \
    ss << #func << "() throw wrong exception message, "                     \
        << __FILE__ << ":" << __LINE__;                                     \
    try {                                                                   \
        func(__VA_ARGS__);                                                  \
    } catch (const expectedException& ex) {                                 \
        assertEqual(whatString, ex.what(), ss.str());                       \
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
