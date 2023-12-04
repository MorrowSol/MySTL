#include <gtest/gtest.h>
#include "algo.h" // include the header file where the function is defined

// Define a fixture for the unit tests
class LowerBoundTest : public ::testing::Test {
protected:
    // Add any necessary member variables and functions here
    std::vector<int> input = {1, 2, 3, 4, 5,7,9};
    std::vector<int>::iterator result;
};

TEST_F(LowerBoundTest, Test1) {
    // Call the function
    result = mySTL::lower_bound(input.begin(), input.end(), 3);

    // Check the result
    EXPECT_EQ(*result, 3);
    EXPECT_EQ(result, input.begin() + 2);
}

TEST_F(LowerBoundTest, Test2) {
    result = mySTL::lower_bound(input.begin(), input.end(), 6);

    // Check the result
    EXPECT_EQ(*result, 7);
    EXPECT_EQ(result, input.begin() + 5);
}

TEST_F(LowerBoundTest, Test3) {
    result = mySTL::lower_bound(input.begin(), input.end(), 2);

    // Check the result
    EXPECT_EQ(*result, 2);
    EXPECT_EQ(result, input.begin() + 1);
}
TEST_F(LowerBoundTest, Test4) {
    result = mySTL::lower_bound(input.begin(), input.end(), 10);

    // Check the result
    EXPECT_EQ(result, input.end());
}
