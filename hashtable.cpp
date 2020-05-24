#include <vector>
#include <iostream>
#include "catch.hpp"
#include "hash.h"
#include "hashtable.h"
#include <random>  
#include <ctime>  
#include <string>
TEST_CASE ( "Hashtable works with strings", "[hashtable]" ) {
    myHashMap<std::string, int> h;
    
    REQUIRE(!h.Contains("one"));
    REQUIRE(!h.Contains("two"));
    
    h["one"] = 2;
    h["two"] = 1;
    REQUIRE(h.Contains("one"));
    REQUIRE(h.Contains("two"));
    REQUIRE(!h.Contains("many"));
    REQUIRE(!h.Contains("more"));
    REQUIRE(!h.Contains("everything"));
    
    REQUIRE(h["one"] == 2);
    REQUIRE(h["two"] == 1);
    
    h["one"] = 4;
    h["two"] = 5;
    
    REQUIRE(h.Contains("one"));
    REQUIRE(h.Contains("two"));
    REQUIRE(!h.Contains("many"));
    REQUIRE(!h.Contains("more"));
    REQUIRE(!h.Contains("everything"));
    
    REQUIRE(h["one"] == 4);
    REQUIRE(h["two"] == 5);
}

TEST_CASE ( "Hashtable supports element removal", "[hashtable]" ) {
    myHashMap<std::string, int> h;
    
    REQUIRE(!h.Contains("one"));
    REQUIRE(!h.Contains("two"));
    
    h["one"] = 2;
    h["two"] = 1;
    REQUIRE(h.Contains("one"));
    REQUIRE(h.Contains("two"));
    REQUIRE(!h.Contains("many"));
    REQUIRE(!h.Contains("more"));
    REQUIRE(!h.Contains("everything"));
    
    REQUIRE(h["one"] == 2);
    REQUIRE(h["two"] == 1);
    
    h.Remove("one");
    
    REQUIRE(!h.Contains("one"));
    REQUIRE(h.Contains("two"));
    REQUIRE(!h.Contains("many"));
    REQUIRE(!h.Contains("more"));
    REQUIRE(!h.Contains("everything"));
    
    REQUIRE(h["two"] == 1);
}

struct MyKey {
    uint64_t a, b;
    bool operator==(const MyKey& o) { return (a == o.a) && (b == o.b); }
    
    MyKey() = delete;
    MyKey(MyKey&&) = default;
    MyKey(uint64_t a_, uint64_t b_) : a(a_), b(b_) {}
    MyKey(const MyKey&) = default;
};

template<>
struct myHash<MyKey> {
    myHash() = default;
    
    uint64_t operator() (const MyKey& v) const {
        return v.a + v.b;
    }
};

TEST_CASE ( "Hashtable works with custom Hash overloads", "[hashtable]" ) {
    myHashMap<MyKey, int> h;
    
    REQUIRE(!h.Contains({1, 2}));
    REQUIRE(!h.Contains({2, 1}));
    
    h[{1, 2}] = 2;
    h[{2, 1}] = 1;
    
    REQUIRE(h.Contains({1, 2}));
    REQUIRE(h.Contains({2, 1}));
    REQUIRE(!h.Contains({3, 0}));
    REQUIRE(!h.Contains({0, 3}));
    REQUIRE(!h.Contains({10, 20}));
    
    REQUIRE(h[{1, 2}] == 2);
    REQUIRE(h[{2, 1}] == 1);
    
    h[{1, 2}] = 4;
    h[{2, 1}] = 5;
    
    REQUIRE(h.Contains({1, 2}));
    REQUIRE(h.Contains({2, 1}));
    REQUIRE(!h.Contains({3, 0}));
    REQUIRE(!h.Contains({0, 3}));
    REQUIRE(!h.Contains({10, 20}));
    
    REQUIRE(h[{1, 2}] == 4);
    REQUIRE(h[{2, 1}] == 5);
}

#ifdef HASH_IMPLEMENTED_FOR_INTEGRAL_TYPES

#define MAKE_TEST(type) \
{\
    HashMap < type, int > h;\
    REQUIRE(!h.Contains(1));\
    REQUIRE(!h.Contains(2));\
    h[1] = 2;\
    h[2] = 1;\
    REQUIRE(h.Contains(1));\
    REQUIRE(h.Contains(2));\
    REQUIRE(!h.Contains(3));\
    REQUIRE(!h.Contains(4));\
    REQUIRE(!h.Contains(1008));\
    REQUIRE(h[1] == 2);\
    REQUIRE(h[2] == 1);\
    h[1] = 4;\
    h[2] = 5;\
    REQUIRE(h.Contains(1));\
    REQUIRE(h.Contains(2));\
    REQUIRE(!h.Contains(3));\
    REQUIRE(!h.Contains(4));\
    REQUIRE(!h.Contains(1008));\
    REQUIRE(h[1] == 4);\
    REQUIRE(h[2] == 5);\
}

TEST_CASE ( "Hashtable works with various int types", "[hashtable]" ) {
    MAKE_TEST(int16_t)
    MAKE_TEST(int32_t)
    MAKE_TEST(int64_t)
    
    MAKE_TEST(uint16_t)
    MAKE_TEST(uint32_t)
    MAKE_TEST(uint64_t)
    
}
#endif // HASH_IMPLEMENTED_FOR_INTEGRAL_TYPES

#ifdef HASH_IMPLEMENTED_FOR_CLASSES_WITH_MAGIC_METHOD
struct MyOtherKey {
    uint64_t a, b;
    bool operator==(const MyOtherKey& o) { return (a == o.a) && (b == o.b); }
    
    MyOtherKey() = delete;
    MyOtherKey(MyOtherKey&&) = default;
    MyOtherKey(uint64_t a_, uint64_t b_) : a(a_), b(b_) {}
    MyOtherKey(const MyOtherKey&) = default;
    
    uint64_t GetHash() const {
        return a + b;
    }
};

TEST_CASE ( "Hashtable works with class GetHash method", "[hashtable]" ) {
    HashMap<MyOtherKey, int> h;
    
    REQUIRE(!h.Contains({1, 2}));
    REQUIRE(!h.Contains({2, 1}));
    
    h[{1, 2}] = 2;
    h[{2, 1}] = 1;
    
    REQUIRE(h.Contains({1, 2}));
    REQUIRE(h.Contains({2, 1}));
    REQUIRE(!h.Contains({3, 0}));
    REQUIRE(!h.Contains({0, 3}));
    REQUIRE(!h.Contains({10, 20}));
    
    REQUIRE(h[{1, 2}] == 2);
    REQUIRE(h[{2, 1}] == 1);
    
    h[{1, 2}] = 4;
    h[{2, 1}] = 5;
    
    REQUIRE(h.Contains({1, 2}));
    REQUIRE(h.Contains({2, 1}));
    REQUIRE(!h.Contains({3, 0}));
    REQUIRE(!h.Contains({0, 3}));
    REQUIRE(!h.Contains({10, 20}));
    
    REQUIRE(h[{1, 2}] == 4);
    REQUIRE(h[{2, 1}] == 5);
}
#endif // HASH_IMPLEMENTED_FOR_CLASSES_WITH_MAGIC_METHOD
