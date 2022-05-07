#include <stdexcept>
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../vector_s.hpp"
#include <vector>

using namespace testing;
using namespace estl;

class VectorSUnitTest : public testing::Test
{
public:
    VectorSUnitTest()  = default;

    ~VectorSUnitTest() override = default;

    void SetUp() override
    {
    }
    void TearDown() override;

};

void VectorSUnitTest::TearDown()
{
}

// ----------------------------------------
// --- Simple helper test element class ---
// ----------------------------------------

struct Rect {
    Rect() = default;
    Rect(unsigned w, unsigned h)
        : width_(w),
          height_(h)
    {
    }

    unsigned width_ = 0;
    unsigned height_ = 0;
};


inline bool operator== (const Rect& lhs, const Rect& rhs) {
    return  lhs.width_ == rhs.width_ &&
            lhs.height_ == rhs.height_;
}



// -------------------
// -- Constructors ---
// -------------------

TEST_F(VectorSUnitTest, default_constructor)
{
    vector_s<int, 10> v;
    EXPECT_TRUE(v.empty());
    EXPECT_EQ(static_cast<size_t>(0u), v.size());
    EXPECT_EQ(10u, v.capacity());
    EXPECT_EQ(10u, v.max_size());
}

TEST_F(VectorSUnitTest, count_constructor)
{
    vector_s<std::string, 10> vs(5);
    EXPECT_FALSE(vs.empty());
    EXPECT_EQ(5u, vs.size());
    EXPECT_EQ(std::string(""), vs[0]);
    EXPECT_EQ(std::string(""), vs[1]);
    EXPECT_EQ(std::string(""), vs[2]);
    EXPECT_EQ(std::string(""), vs[3]);
    EXPECT_EQ(std::string(""), vs[4]);

    vector_s<std::string, 10> vsv(5, "value");
    EXPECT_FALSE(vsv.empty());
    EXPECT_EQ(5u, vsv.size());
    EXPECT_EQ(std::string("value"), vsv[0]);
    EXPECT_EQ(std::string("value"), vsv[1]);
    EXPECT_EQ(std::string("value"), vsv[2]);
    EXPECT_EQ(std::string("value"), vsv[3]);
    EXPECT_EQ(std::string("value"), vsv[4]);
}

TEST_F(VectorSUnitTest, range_constructor)
{
    std::vector<std::string> vs_src{"0","1","2","3","4"};

    vector_s<std::string, 10> vs(vs_src.begin(), vs_src.end());
    EXPECT_FALSE(vs.empty());
    EXPECT_EQ(5u, vs.size());
    EXPECT_EQ(std::string("0"), vs[0]);
    EXPECT_EQ(std::string("1"), vs[1]);
    EXPECT_EQ(std::string("2"), vs[2]);
    EXPECT_EQ(std::string("3"), vs[3]);
    EXPECT_EQ(std::string("4"), vs[4]);
}

TEST_F(VectorSUnitTest, copy_constructor)
{
    std::vector<std::string> v_raw_src{"0","1","2","3","4"};

    vector_s<std::string, 10> v_src(v_raw_src.begin(), v_raw_src.end());
    vector_s<std::string, 10> vs(v_src);
    EXPECT_FALSE(vs.empty());
    EXPECT_EQ(5u, vs.size());
    EXPECT_EQ(std::string("0"), vs[0]);
    EXPECT_EQ(std::string("1"), vs[1]);
    EXPECT_EQ(std::string("2"), vs[2]);
    EXPECT_EQ(std::string("3"), vs[3]);
    EXPECT_EQ(std::string("4"), vs[4]);

    vector_s<std::string, 10> v_src_move(v_raw_src.begin(), v_raw_src.end());
    vector_s<std::string, 10> vs_move(std::move(v_src_move));
    EXPECT_FALSE(vs_move.empty());
    EXPECT_EQ(5u, vs_move.size());
    EXPECT_EQ(std::string("0"), vs_move[0]);
    EXPECT_EQ(std::string("1"), vs_move[1]);
    EXPECT_EQ(std::string("2"), vs_move[2]);
    EXPECT_EQ(std::string("3"), vs_move[3]);
    EXPECT_EQ(std::string("4"), vs_move[4]);

    vector_s<std::string, 12> vs_diff_capacity(v_src);
    EXPECT_FALSE(vs_diff_capacity.empty());
    EXPECT_EQ(5u, vs_diff_capacity.size());
    EXPECT_EQ(std::string("0"), vs_diff_capacity[0]);
    EXPECT_EQ(std::string("1"), vs_diff_capacity[1]);
    EXPECT_EQ(std::string("2"), vs_diff_capacity[2]);
    EXPECT_EQ(std::string("3"), vs_diff_capacity[3]);
    EXPECT_EQ(std::string("4"), vs_diff_capacity[4]);

    vector_s<std::string, 10> v_src_move_diff_capacity(v_raw_src.begin(), v_raw_src.end());
    vector_s<std::string, 12> vs_diff_capacity_move(std::move(v_src_move_diff_capacity));
    EXPECT_FALSE(vs_diff_capacity_move.empty());
    EXPECT_EQ(5u, vs_diff_capacity_move.size());
    EXPECT_EQ(std::string("0"), vs_diff_capacity_move[0]);
    EXPECT_EQ(std::string("1"), vs_diff_capacity_move[1]);
    EXPECT_EQ(std::string("2"), vs_diff_capacity_move[2]);
    EXPECT_EQ(std::string("3"), vs_diff_capacity_move[3]);
    EXPECT_EQ(std::string("4"), vs_diff_capacity_move[4]);

}

TEST_F(VectorSUnitTest, push_back)
{
    vector_s<unsigned, 6> v{};
    EXPECT_EQ(0u, v.size());

    // Insert 3 elements from start
    v.push_back(0u); // rvalue reference (the (T&& value) overload)
    v.push_back(1u); // rvalue reference (the (T&& value) overload)
    v.push_back(2u); // rvalue reference (the (T&& value) overload)
    EXPECT_EQ(0u, v[0]);
    EXPECT_EQ(1u, v[1]);
    EXPECT_EQ(2u, v[2]);
    EXPECT_EQ(3u, v.size());

    const unsigned v10 = 10u;
    v.push_back(v10); // Normal (const T& value) overload
    EXPECT_EQ(10u, v[3]);
    EXPECT_EQ(4u, v.size());
}

#if (CXX_STANDARD != 98)

TEST_F(VectorSUnitTest, assignment)
{
    std::vector<int> v_raw_src{0,1,2,3,4};

    vector_s<int, 10> v_src(v_raw_src.begin(), v_raw_src.end());
    vector_s<int, 10> v;
    v = v_src;
    EXPECT_FALSE(v.empty());
    EXPECT_EQ(5u, v.size());
    EXPECT_EQ(static_cast<int>(0), v[0]);
    EXPECT_EQ(1, v[1]);
    EXPECT_EQ(2, v[2]);
    EXPECT_EQ(3, v[3]);
    EXPECT_EQ(4, v[4]);

    vector_s<std::string, 10> vs_src{"0","1","2","3","4"};
    vector_s<std::string, 10> vs;
    vs = vs_src;
    EXPECT_FALSE(vs.empty());
    EXPECT_EQ(5u, vs.size());
    EXPECT_EQ(std::string("0"), vs[0]);
    EXPECT_EQ(std::string("1"), vs[1]);
    EXPECT_EQ(std::string("2"), vs[2]);
    EXPECT_EQ(std::string("3"), vs[3]);
    EXPECT_EQ(std::string("4"), vs[4]);

    vector_s<std::string, 20> vsdifferent; // Different capicy from source vector!
    vsdifferent = vs_src;
    EXPECT_FALSE(vsdifferent.empty());
    EXPECT_EQ(5u, vsdifferent.size());
    EXPECT_EQ(std::string("0"), vsdifferent[0]);
    EXPECT_EQ(std::string("1"), vsdifferent[1]);
    EXPECT_EQ(std::string("2"), vsdifferent[2]);
    EXPECT_EQ(std::string("3"), vsdifferent[3]);
    EXPECT_EQ(std::string("4"), vsdifferent[4]);
}

TEST_F(VectorSUnitTest, initializer_list_constructor)
{
    vector_s<int, 10> v{0,1,2,3,4};
    EXPECT_FALSE(v.empty());
    EXPECT_EQ(5u, v.size());
    EXPECT_EQ(static_cast<int>(0), v[0]);
    EXPECT_EQ(1, v[1]);
    EXPECT_EQ(2, v[2]);
    EXPECT_EQ(3, v[3]);
    EXPECT_EQ(4, v[4]);

    vector_s<std::string, 10> vs{"0","1","2","3","4"};
    EXPECT_FALSE(vs.empty());
    EXPECT_EQ(5u, vs.size());
    EXPECT_EQ(std::string("0"), vs[0]);
    EXPECT_EQ(std::string("1"), vs[1]);
    EXPECT_EQ(std::string("2"), vs[2]);
    EXPECT_EQ(std::string("3"), vs[3]);
    EXPECT_EQ(std::string("4"), vs[4]);
}


TEST_F(VectorSUnitTest, initializer_list_assignment)
{
    vector_s<int, 10> v;
    v = {0,1,2,3,4};
    EXPECT_FALSE(v.empty());
    EXPECT_EQ(5u, v.size());
    EXPECT_EQ(static_cast<int>(0), v[0]);
    EXPECT_EQ(1, v[1]);
    EXPECT_EQ(2, v[2]);
    EXPECT_EQ(3, v[3]);
    EXPECT_EQ(4, v[4]);

    vector_s<std::string, 10> vs;
    vs = {"0","1","2","3","4"};
    EXPECT_FALSE(vs.empty());
    EXPECT_EQ(5u, vs.size());
    EXPECT_EQ(std::string("0"), vs[0]);
    EXPECT_EQ(std::string("1"), vs[1]);
    EXPECT_EQ(std::string("2"), vs[2]);
    EXPECT_EQ(std::string("3"), vs[3]);
    EXPECT_EQ(std::string("4"), vs[4]);

}

TEST_F(VectorSUnitTest, assign_function)
{
    vector_s<std::string, 10> vsv;
    vsv.assign(5, "value");
    EXPECT_FALSE(vsv.empty());
    EXPECT_EQ(5u, vsv.size());
    EXPECT_EQ(std::string("value"), vsv[0]);
    EXPECT_EQ(std::string("value"), vsv[1]);
    EXPECT_EQ(std::string("value"), vsv[2]);
    EXPECT_EQ(std::string("value"), vsv[3]);
    EXPECT_EQ(std::string("value"), vsv[4]);
}


TEST_F(VectorSUnitTest, initializer_list_assign_function)
{
    vector_s<std::string, 10> vs;
    vs.assign({"00","11","22","33","44"});
    EXPECT_FALSE(vs.empty());
    EXPECT_EQ(5u, vs.size());
    EXPECT_EQ(std::string("00"), vs[0]);
    EXPECT_EQ(std::string("11"), vs[1]);
    EXPECT_EQ(std::string("22"), vs[2]);
    EXPECT_EQ(std::string("33"), vs[3]);
    EXPECT_EQ(std::string("44"), vs[4]);
}

TEST_F(VectorSUnitTest, range_assignment)
{
    std::vector<int> v_src{0,1,2,3,4};

    vector_s<int, 10> v;
    v.assign(v_src.begin(), v_src.end());
    EXPECT_FALSE(v.empty());
    EXPECT_EQ(5u, v.size());
    EXPECT_EQ(static_cast<int>(0), v[0]);
    EXPECT_EQ(1, v[1]);
    EXPECT_EQ(2, v[2]);
    EXPECT_EQ(3, v[3]);
    EXPECT_EQ(4, v[4]);

    std::vector<std::string> vs_src{"0","1","2","3","4"};
    vector_s<std::string, 10> vs;
    vs.assign(vs_src.begin(), vs_src.end());
    EXPECT_FALSE(vs.empty());
    EXPECT_EQ(5u, vs.size());
    EXPECT_EQ(std::string("0"), vs[0]);
    EXPECT_EQ(std::string("1"), vs[1]);
    EXPECT_EQ(std::string("2"), vs[2]);
    EXPECT_EQ(std::string("3"), vs[3]);
    EXPECT_EQ(std::string("4"), vs[4]);
}


TEST_F(VectorSUnitTest, construct_out_of_range)
{
    std::vector<unsigned> v_src{0u,1u,2u,3u,4u};
    std::vector<unsigned> v_src_too_big{0u,1u,2u,3u,4u,5u};
    using vec_t = vector_s<unsigned, 5>;
    EXPECT_THROW(vec_t(6), std::range_error);
    EXPECT_THROW(vec_t(6, 12u), std::range_error);
    EXPECT_THROW((vec_t{0u, 1u, 2u, 3u, 4u, 5u}), std::range_error);
    EXPECT_NO_THROW((vec_t(v_src.begin(), v_src.end())));
    EXPECT_THROW((vec_t(v_src.end(), v_src.begin())), std::out_of_range);
    EXPECT_THROW((vec_t(v_src_too_big.begin(), v_src_too_big.end())), std::range_error);
}

TEST_F(VectorSUnitTest, assign_out_of_range)
{
    std::vector<unsigned> v_src{0u,1u,2u,3u,4u};
    std::vector<unsigned> v_src_too_big{0u,1u,2u,3u,4u,5u};
    using vec_t = vector_s<unsigned, 5>;
    vec_t v;
    EXPECT_THROW( (v = {0u, 1u, 2u, 3u, 4u, 5u}), std::range_error );
    EXPECT_NO_THROW( v.assign(v_src.begin(), v_src.end()) );
    EXPECT_THROW(v.assign(v_src.end(), v_src.begin()), std::out_of_range);
    EXPECT_THROW(v.assign(v_src_too_big.begin(), v_src_too_big.end()), std::range_error);
}

// ----------------------
// --- Element access ---
// ----------------------
TEST_F(VectorSUnitTest, element_access_at)
{
    vector_s<unsigned, 5> v{0u,1u,2u,3u,4u};
    EXPECT_EQ(0u, v.at(0));
    EXPECT_EQ(1u, v.at(1));
    EXPECT_EQ(2u, v.at(2));
    EXPECT_EQ(3u, v.at(3));
    EXPECT_EQ(4u, v.at(4));
    EXPECT_THROW(v.at(5), std::out_of_range);

    // const variant of at()
    const vector_s<unsigned, 5> vc{0u,1u,2u,3u,4u};
    EXPECT_EQ(0u, vc.at(0));
    EXPECT_EQ(1u, vc.at(1));
    EXPECT_EQ(2u, vc.at(2));
    EXPECT_EQ(3u, vc.at(3));
    EXPECT_EQ(4u, vc.at(4));
    EXPECT_THROW(vc.at(5), std::out_of_range);
}

TEST_F(VectorSUnitTest, element_access_operator_index)
{
    vector_s<unsigned, 5> v{0u,1u,2u,3u,4u};
    EXPECT_EQ(0u, v[0]);
    EXPECT_EQ(1u, v[1]);
    EXPECT_EQ(2u, v[2]);
    EXPECT_EQ(3u, v[3]);
    EXPECT_EQ(4u, v[4]);
    EXPECT_NO_THROW(v[5]);

    // const variant of at()
    vector_s<unsigned, 5> vc{0u,1u,2u,3u,4u};
    EXPECT_EQ(0u, vc[0]);
    EXPECT_EQ(1u, vc[1]);
    EXPECT_EQ(2u, vc[2]);
    EXPECT_EQ(3u, vc[3]);
    EXPECT_EQ(4u, vc[4]);
    EXPECT_NO_THROW(vc[5]);
}

TEST_F(VectorSUnitTest, element_access_front_back)
{
    vector_s<unsigned, 4> v{1u,2u,3u,4u};
    EXPECT_EQ(1u, v.front());
    EXPECT_EQ(4u, v.back());

    // const variant of front()/back()
    const vector_s<unsigned, 4> vc{1u,2u,3u,4u};
    EXPECT_EQ(1u, vc.front());
    EXPECT_EQ(4u, vc.back());
}

TEST_F(VectorSUnitTest, access_raw_data)
{
    vector_s<unsigned, 4> v{1u,2u,3u,4u};
    EXPECT_EQ(1u, *(v.data()));
    EXPECT_EQ(4u, *(v.data() + 3));

    // const variant of data() function
    vector_s<unsigned, 4> vc{1u,2u,3u,4u};
    EXPECT_EQ(1u, *(vc.data()));
    EXPECT_EQ(4u, *(vc.data() + 3));
}

// -----------------
// --- Iterators ---
// -----------------
TEST_F(VectorSUnitTest, iterator_loop)
{
    using vec_t = vector_s<size_t, 4>;
    {
        vec_t v{1u,2u,3u,4u};
        size_t count = 0;
        for (auto& elem : v) {
            ++count;
            EXPECT_EQ(count, elem);
            elem = count + 1u;
        }
        EXPECT_EQ(4u, count);
        EXPECT_EQ(2u, v[0]);
        EXPECT_EQ(3u, v[1]);
        EXPECT_EQ(4u, v[2]);
        EXPECT_EQ(5u, v[3]);
    }

    // const
    {
        const vec_t v{1u,2u,3u,4u};
        size_t count = 0;
        for (const auto& elem : v) {
            ++count;
            EXPECT_EQ(count, elem);
        }
        EXPECT_EQ(4u, count);
    }
}


TEST_F(VectorSUnitTest, range_for_loop)
{
    using vec_t = vector_s<size_t, 4>;
    {
        vec_t v{1u,2u,3u,4u};
        size_t count = 0;
        for (auto& e : v ) {
            ++count;
            EXPECT_EQ(count, e);
            e = count + 1u;
        }
        EXPECT_EQ(4u, count);
        EXPECT_EQ(2u, v[0]);
        EXPECT_EQ(3u, v[1]);
        EXPECT_EQ(4u, v[2]);
        EXPECT_EQ(5u, v[3]);
    }

    // const
    {
        const vec_t v{1u,2u,3u,4u};
        size_t count = 0;
        for (const auto& e : v ) {
            ++count;
            EXPECT_EQ(count, e);
        }
        EXPECT_EQ(4u, count);
    }
}

TEST_F(VectorSUnitTest, reverse_iterator_loop)
{
    using vec_t = vector_s<size_t, 4>;
    {
        vec_t v{4u,3u,2u,1u};
        size_t count = 0;
        for (vec_t::reverse_iterator it = v.rbegin(); it != v.rend(); ++it) {
            ++count;
            EXPECT_EQ(count, *it);
            *it = count + 1u;
        }
        EXPECT_EQ(4u, count);
        EXPECT_EQ(5u, v[0]);
        EXPECT_EQ(4u, v[1]);
        EXPECT_EQ(3u, v[2]);
        EXPECT_EQ(2u, v[3]);
    }

    // const
    {
        const vec_t v{4u,3u,2u,1u};
        size_t count = 0;
        for (auto it = v.crbegin(); it != v.crend(); ++it) {
            ++count;
            EXPECT_EQ(count, *it);
        }
        EXPECT_EQ(4u, count);
    }
}

// ----------------
// --- Capacity ---
// ----------------
// NOTE: Tested in 'default_constructor_test'

// -----------------
// --- Modifiers ---
// -----------------
struct S {
    S() = default;
    explicit S(size_t v) : val(v) { ++count_constructor; }
    S( const S& other) = default;
    ~S() { ++count_destructor; }
    size_t val = 0;
    static size_t count_constructor;
    static size_t count_destructor;
};

size_t S::count_constructor = 0;
size_t S::count_destructor = 0;

TEST_F(VectorSUnitTest, clear)
{
    using vec_t = vector_s<S, 10>;
    vec_t v{S(1u),S(2u),S(3u),S(4u)};
    EXPECT_FALSE(v.empty());
    EXPECT_EQ(4u, v.size());
    EXPECT_EQ(10u, v.capacity());
    EXPECT_EQ(10u, v.max_size());
    EXPECT_EQ(4u, S::count_constructor);
    EXPECT_EQ(4u, S::count_destructor);
    v.clear();
    EXPECT_EQ(static_cast<size_t>(0u), v.size());
    EXPECT_EQ(8u, S::count_destructor);
}

TEST_F(VectorSUnitTest, insert_single_element)
{
    vector_s<std::string, 10> v{"0","1","2"};
    EXPECT_EQ("0", v[0]);
    EXPECT_EQ("1", v[1]);
    EXPECT_EQ("2", v[2]);
    EXPECT_EQ(3u, v.size());

    const auto it1 = v.insert(v.begin(), "10");
    EXPECT_EQ("10", *it1);
    EXPECT_EQ("10", v[0]);
    EXPECT_EQ("0", v[1]);
    EXPECT_EQ("1", v[2]);
    EXPECT_EQ("2", v[3]);
    EXPECT_EQ(4u, v.size());

    const auto it2 = v.insert(v.begin()+2, "20");
    EXPECT_EQ("20", *it2);
    EXPECT_EQ("10", v[0]);
    EXPECT_EQ("0", v[1]);
    EXPECT_EQ("20", v[2]);
    EXPECT_EQ("1", v[3]);
    EXPECT_EQ("2", v[4]);
    EXPECT_EQ(5u, v.size());

    const auto it3 = v.insert(v.end(), "50");
    EXPECT_EQ("50", *it3);
    EXPECT_EQ("10", v[0]);
    EXPECT_EQ("0", v[1]);
    EXPECT_EQ("20", v[2]);
    EXPECT_EQ("1", v[3]);
    EXPECT_EQ("2", v[4]);
    EXPECT_EQ("50", v[5]);
    EXPECT_EQ(6u, v.size());
}

TEST_F(VectorSUnitTest, insert_multiple_elements)
{
    vector_s<unsigned, 10> v{0u,1u,2u};
    EXPECT_EQ(0u, v[0]);
    EXPECT_EQ(1u, v[1]);
    EXPECT_EQ(2u, v[2]);
    EXPECT_EQ(3u, v.size());

    // Insert zero elements
    v.insert(v.begin(), 0, 10u);
    EXPECT_EQ(0u, v[0]);
    EXPECT_EQ(1u, v[1]);
    EXPECT_EQ(2u, v[2]);
    EXPECT_EQ(3u, v.size());

    // Insert 3 elements from start
    const auto it1 = v.insert(v.begin(), 3, 10u);
    EXPECT_EQ(10u, *it1);
    EXPECT_EQ(10u, *(it1 + 1));
    EXPECT_EQ(10u, *(it1 + 2));
    EXPECT_EQ(10u, v[0]);
    EXPECT_EQ(10u, v[1]);
    EXPECT_EQ(10u, v[2]);
    EXPECT_EQ(0u, v[3]);
    EXPECT_EQ(1u, v[4]);
    EXPECT_EQ(2u, v[5]);
    EXPECT_EQ(6u, v.size());

    // Insert 2 elements in the middle
    const auto it2 = v.insert(v.begin()+3, 2, 20u);
    EXPECT_EQ(20u, *it2);
    EXPECT_EQ(20u, *(it2 + 1));
    EXPECT_EQ(10u, v[0]);
    EXPECT_EQ(10u, v[1]);
    EXPECT_EQ(10u, v[2]);
    EXPECT_EQ(20u, v[3]);
    EXPECT_EQ(20u, v[4]);
    EXPECT_EQ(0u, v[5]);
    EXPECT_EQ(1u, v[6]);
    EXPECT_EQ(2u, v[7]);
    EXPECT_EQ(8u, v.size());

    // Insert 2 elements 'in' the end
    const auto it3 = v.insert(v.end(), 2, 50u);
    EXPECT_EQ(50u, *it3);
    EXPECT_EQ(50u, *(it3 + 1));
    EXPECT_EQ(10u, v[0]);
    EXPECT_EQ(10u, v[1]);
    EXPECT_EQ(10u, v[2]);
    EXPECT_EQ(20u, v[3]);
    EXPECT_EQ(20u, v[4]);
    EXPECT_EQ(0u, v[5]);
    EXPECT_EQ(1u, v[6]);
    EXPECT_EQ(2u, v[7]);
    EXPECT_EQ(50u, v[8]);
    EXPECT_EQ(50u, v[9]);
    EXPECT_EQ(10u, v.size());
}


TEST_F(VectorSUnitTest, insert_iterator_range)
{
    constexpr std::array<unsigned, 3> src{10u,20u,30u};
    vector_s<unsigned, 12> v{0u,1u,2u};
    EXPECT_EQ(0u, v[0]);
    EXPECT_EQ(1u, v[1]);
    EXPECT_EQ(2u, v[2]);
    EXPECT_EQ(3u, v.size());

    // Insert zero elements
    v.insert(v.begin(), src.begin(), src.begin());
    EXPECT_EQ(0u, v[0]);
    EXPECT_EQ(1u, v[1]);
    EXPECT_EQ(2u, v[2]);
    EXPECT_EQ(3u, v.size());

    // Insert 3 elements from start
    const auto it1 = v.insert(v.begin(), src.begin(), src.end());
    EXPECT_EQ(v.begin(), it1);
    EXPECT_EQ(src[0], *it1);
    EXPECT_EQ(src[1], *(it1 + 1));
    EXPECT_EQ(src[2], *(it1 + 2));
    EXPECT_EQ(10u, v[0]);
    EXPECT_EQ(20u, v[1]);
    EXPECT_EQ(30u, v[2]);
    EXPECT_EQ(0u, v[3]);
    EXPECT_EQ(1u, v[4]);
    EXPECT_EQ(2u, v[5]);
    EXPECT_EQ(6u, v.size());

    // Insert 3 elements in the middle
    const auto it2 = v.insert(v.begin()+3, src.begin(), src.end());
    EXPECT_EQ(v.begin()+3, it2);
    EXPECT_EQ(src[0], *it2);
    EXPECT_EQ(src[1], *(it2 + 1));
    EXPECT_EQ(src[2], *(it2 + 2));
    EXPECT_EQ(10u, v[0]);
    EXPECT_EQ(20u, v[1]);
    EXPECT_EQ(30u, v[2]);
    EXPECT_EQ(10u, v[3]);
    EXPECT_EQ(20u, v[4]);
    EXPECT_EQ(30u, v[5]);
    EXPECT_EQ(0u, v[6]);
    EXPECT_EQ(1u, v[7]);
    EXPECT_EQ(2u, v[8]);
    EXPECT_EQ(9u, v.size());

    // Insert 3 elements 'in' the end
    const auto it3 = v.insert(v.end(), src.begin(), src.end());
    EXPECT_EQ(v.begin()+9, it3);
    EXPECT_EQ(src[0], *it3);
    EXPECT_EQ(src[1], *(it3 + 1));
    EXPECT_EQ(src[2], *(it3 + 2));
    EXPECT_EQ(10u, v[0]);
    EXPECT_EQ(20u, v[1]);
    EXPECT_EQ(30u, v[2]);
    EXPECT_EQ(10u, v[3]);
    EXPECT_EQ(20u, v[4]);
    EXPECT_EQ(30u, v[5]);
    EXPECT_EQ(0u, v[6]);
    EXPECT_EQ(1u, v[7]);
    EXPECT_EQ(2u, v[8]);
    EXPECT_EQ(10u, v[9]);
    EXPECT_EQ(20u, v[10]);
    EXPECT_EQ(30u, v[11]);
    EXPECT_EQ(12u, v.size());
}

TEST_F(VectorSUnitTest, insert_initializer_list)
{
    vector_s<unsigned, 12> v{0u,1u,2u};
    EXPECT_EQ(0u, v[0]);
    EXPECT_EQ(1u, v[1]);
    EXPECT_EQ(2u, v[2]);
    EXPECT_EQ(3u, v.size());


    // Insert 3 elements from start
    v.insert(v.begin(), {10u,20u,30u});
    EXPECT_EQ(10u, v[0]);
    EXPECT_EQ(20u, v[1]);
    EXPECT_EQ(30u, v[2]);
    EXPECT_EQ(0u, v[3]);
    EXPECT_EQ(1u, v[4]);
    EXPECT_EQ(2u, v[5]);
    EXPECT_EQ(6u, v.size());

    // Insert 3 elements in the middle
    v.insert(v.begin()+3, {10u,20u,30u});
    EXPECT_EQ(10u, v[0]);
    EXPECT_EQ(20u, v[1]);
    EXPECT_EQ(30u, v[2]);
    EXPECT_EQ(10u, v[3]);
    EXPECT_EQ(20u, v[4]);
    EXPECT_EQ(30u, v[5]);
    EXPECT_EQ(0u, v[6]);
    EXPECT_EQ(1u, v[7]);
    EXPECT_EQ(2u, v[8]);
    EXPECT_EQ(9u, v.size());

    // Insert 3 elements 'in' the end
    v.insert(v.end(), {10u,20u,30u});
    EXPECT_EQ(10u, v[0]);
    EXPECT_EQ(20u, v[1]);
    EXPECT_EQ(30u, v[2]);
    EXPECT_EQ(10u, v[3]);
    EXPECT_EQ(20u, v[4]);
    EXPECT_EQ(30u, v[5]);
    EXPECT_EQ(0u, v[6]);
    EXPECT_EQ(1u, v[7]);
    EXPECT_EQ(2u, v[8]);
    EXPECT_EQ(10u, v[9]);
    EXPECT_EQ(20u, v[10]);
    EXPECT_EQ(30u, v[11]);
    EXPECT_EQ(12u, v.size());
}

TEST_F(VectorSUnitTest, emplace)
{
    vector_s<Rect, 6> v{ {1u,1u}, {2u,2u}, {3u,3u}, {4u,4u} };
    EXPECT_EQ(Rect(1u, 1u), v[0]);
    EXPECT_EQ(Rect(2u, 2u), v[1]);
    EXPECT_EQ(Rect(3u, 3u), v[2]);
    EXPECT_EQ(Rect(4u, 4u), v[3]);
    EXPECT_EQ(4u, v.size());

    const auto it = v.emplace(v.begin()+1, 10u, 10u);
    EXPECT_EQ(Rect(10u, 10u), *it);
    EXPECT_EQ(5u, v.size());
    EXPECT_EQ(Rect(1u, 1u), v[0]);
    EXPECT_EQ(Rect(10u, 10u), v[1]);
    EXPECT_EQ(Rect(2u, 2u), v[2]);
    EXPECT_EQ(Rect(3u, 3u), v[3]);
    EXPECT_EQ(Rect(4u, 4u), v[4]);
}


TEST_F(VectorSUnitTest, erase_single_element)
{
    vector_s<unsigned, 12> v{0u, 1u, 2u, 3u, 4u, 5u};
    EXPECT_EQ(6u, v.size());

    // Erase one element from the beginning
    const auto it1 = v.erase(v.begin());
    EXPECT_EQ(1u, *it1);
    EXPECT_EQ(v.begin(), it1);
    EXPECT_EQ(5u, v.size());
    EXPECT_EQ(1u, v[0]);
    EXPECT_EQ(2u, v[1]);
    EXPECT_EQ(3u, v[2]);
    EXPECT_EQ(4u, v[3]);
    EXPECT_EQ(5u, v[4]);

    // Erase one element from the middle
    const auto it2 = v.erase(v.begin()+2);
    EXPECT_EQ(4u, *it2);
    EXPECT_EQ(v.begin()+2, it2);
    EXPECT_EQ(4u, v.size());
    EXPECT_EQ(1u, v[0]);
    EXPECT_EQ(2u, v[1]);
    EXPECT_EQ(4u, v[2]);
    EXPECT_EQ(5u, v[3]);

    // Erase last element
    const auto it3 = v.erase(v.end()-1);
    EXPECT_EQ(v.end(), it3);
    EXPECT_EQ(3u, v.size());
    EXPECT_EQ(1u, v[0]);
    EXPECT_EQ(2u, v[1]);
    EXPECT_EQ(4u, v[2]);
}

TEST_F(VectorSUnitTest, erase_range_of_elements)
{
    vector_s<unsigned, 12> v{0u, 1u, 2u, 3u, 4u, 5u, 6u, 7u, 8u, 9u, 10u, 11u};
    EXPECT_EQ(12u, v.size());

    // Erase 3 elements from the beginning
    const auto it1 = v.erase(v.begin(), v.begin() +3);
    EXPECT_EQ(v.begin(), it1);
    EXPECT_EQ(3u, *it1);
    EXPECT_EQ(9u, v.size());
    EXPECT_EQ(3u, v[0]);
    EXPECT_EQ(4u, v[1]);
    EXPECT_EQ(5u, v[2]);
    EXPECT_EQ(6u, v[3]);
    EXPECT_EQ(7u, v[4]);
    EXPECT_EQ(8u, v[5]);
    EXPECT_EQ(9u, v[6]);
    EXPECT_EQ(10u, v[7]);
    EXPECT_EQ(11u, v[8]);

    // Erase 3 elements from the middle
    const auto it2 = v.erase(v.begin()+3, v.begin()+6);
    EXPECT_EQ(v.begin()+3, it2);
    EXPECT_EQ(9u, *it2);
    EXPECT_EQ(6u, v.size());
    EXPECT_EQ(3u, v[0]);
    EXPECT_EQ(4u, v[1]);
    EXPECT_EQ(5u, v[2]);
    EXPECT_EQ(9u, v[3]);
    EXPECT_EQ(10u, v[4]);
    EXPECT_EQ(11u, v[5]);

    // Erase 3 last element
    const auto it3 = v.erase(v.end()-3, v.end());
    EXPECT_EQ(v.end(), it3);
    EXPECT_EQ(3u, v.size());
    EXPECT_EQ(3u, v[0]);
    EXPECT_EQ(4u, v[1]);
    EXPECT_EQ(5u, v[2]);
}



TEST_F(VectorSUnitTest, emplace_back)
{
    vector_s<Rect, 6> v{};
    EXPECT_EQ(0u, v.size());

    const auto& ret1 = v.emplace_back(1u,2u);
    EXPECT_EQ(Rect(1u, 2u), ret1);
    EXPECT_EQ(Rect(1u, 2u), v.back());
    EXPECT_EQ(1u, v.size());

    const auto& ret2 = v.emplace_back(2u,3u);
    EXPECT_EQ(Rect(2u, 3u), ret2);
    EXPECT_EQ(Rect(2u, 3u), v.back());
    EXPECT_EQ(2u, v.size());
}

TEST_F(VectorSUnitTest, pop_back)
{
    vector_s<unsigned, 12> v{0u, 1u, 2u, 3u, 4u, 5u};
    EXPECT_EQ(6u, v.size());
    EXPECT_EQ(5u, v.back());

    v.pop_back();
    EXPECT_EQ(4u, v.back());
    EXPECT_EQ(5u, v.size());

    v.pop_back();
    EXPECT_EQ(3u, v.back());
    EXPECT_EQ(4u, v.size());
}

TEST_F(VectorSUnitTest, swap)
{
    // NOTE: The reason for using different capacities here
    //       is to exercise the mixed-capacity constructors, assigment
    //       and equality functions/operators.
    vector_s<unsigned, 12> original_v1{0u, 1u, 2u, 3u, 4u, 5u};
    vector_s<unsigned, 11>  original_v2{0u, 1u, 2u};
    vector_s<unsigned, 10> v1 = original_v1;
    vector_s<unsigned, 9>  v2 = original_v2;

    EXPECT_EQ(6u, v1.size());
    EXPECT_EQ(3u, v2.size());
    v1.swap(v2);
    EXPECT_EQ(6u, v2.size());
    EXPECT_EQ(3u, v1.size());
    EXPECT_EQ(v1, original_v2);
    EXPECT_EQ(v2, original_v1);
}

#endif // (CXX_STANDARD != 98)


int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
