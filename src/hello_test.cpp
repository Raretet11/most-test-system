// #include "hello.hpp"
// #include <userver/utest/utest.hpp>

// UTEST(SayHelloTo, Basic) {
//     using most::SayHelloTo;
//     using most::UserType;

//     EXPECT_EQ(
//         SayHelloTo("Developer", UserType::kFirstTime), "Hello, Developer!\n"
//     );
//     EXPECT_EQ(SayHelloTo({}, UserType::kFirstTime), "Hello, unknown
//     user!\n");

//     EXPECT_EQ(
//         SayHelloTo("Developer", UserType::kKnown), "Hi again, Developer!\n"
//     );
// }
