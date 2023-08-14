#include <gtest/gtest.h>
#include "parsing.hpp"
#include <cmath>

class ParserSuccessTestInt : public ::testing::TestWithParam<std::pair<int, std::string>> {
protected:
	std::unordered_map<char, int> vars = {};
};

INSTANTIATE_TEST_SUITE_P(BasicArithmetic, ParserSuccessTestInt, ::testing::Values(
    std::make_pair(1+1, "1+1"),
    std::make_pair(1+1*4, "1+1*4")
    )
);

INSTANTIATE_TEST_SUITE_P(Exponentiation, ParserSuccessTestInt, ::testing::Values(
    std::make_pair(16, "4^2"),
    std::make_pair(6, "sqrt(16)+2")
    )
);

TEST_P(ParserSuccessTestInt, Parse) {
	auto params = GetParam();
    Parsing::Expression<int> expression = Parsing::Expression<int>(params.second);
	EXPECT_EQ(params.first, expression.evaluate(vars));
}

TEST(ParserException, Malformed) {
    std::unordered_map<char, int> vars = {};
	EXPECT_THROW(Parsing::Expression<int>("coss(0.2)"), std::invalid_argument);
}
