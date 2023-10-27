#include <iostream>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

// // gtest
// int add(int a, int b) { return a + b; }
// TEST(testCase1, should_3_when_given_2_and_1) { EXPECT_EQ(add(2, 1), 3); }

// // gmock
// class User {
// public:
//     virtual int getAge() = 0;
// };

// class MockUser : public User {
// public:
//     MOCK_METHOD0(getAge, int());
// };

// TEST(testCase2, should_get5) {
//     MockUser user;
//     EXPECT_CALL(user, getAge())
//         .Times(::testing::AtLeast(3))
//         .WillOnce(::testing::Return(200))
//         .WillOnce(::testing::Return(300))
//         .WillRepeatedly(::testing::Return(500));

//     std::cout << user.getAge() << std::endl;
//     std::cout << user.getAge() << std::endl;
//     std::cout << user.getAge() << std::endl;
//     std::cout << user.getAge() << std::endl;
//     std::cout << user.getAge() << std::endl;
// }

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}