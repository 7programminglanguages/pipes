#include <algorithm>

#include "catch.hpp"
#include "pipes/filter.hpp"
#include "pipes/override.hpp"
#include "pipes/push_back.hpp"

TEST_CASE("filter")
{
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const ifIsEven = pipes::filter([](int i){ return i % 2 == 0; });
    
    std::vector<int> expected = {2, 4, 6, 8, 10};
    
    std::vector<int> results;
    std::copy(begin(input), end(input), ifIsEven >>= pipes::push_back(results));
    
    REQUIRE(results == expected);
}

TEST_CASE("filter can override existing results")
{
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto const ifIsEven = pipes::filter([](int i){ return i % 2 == 0; });
    
    std::vector<int> expected = {2, 4, 6, 8, 10, 0, 0, 0, 0, 0};
    
    std::vector<int> results = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::copy(begin(input), end(input), ifIsEven >>= pipes::override(results));
    
    REQUIRE(results == expected);
}

TEST_CASE("filter's iterator category should be std::output_iterator_tag")
{
    auto const isEven = pipes::filter([](int i) { return i % 2 == 0; });
    std::vector<int> output;
    static_assert(std::is_same<decltype(isEven >>= pipes::push_back(output))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}

TEST_CASE("filter operator=")
{
    std::vector<int> results1, results2;
    auto predicate = [](int i){ return i > 0; };
    auto pipeline1 = pipes::filter(predicate) >>= pipes::push_back(results1);
    auto pipeline2 = pipes::filter(predicate) >>= pipes::push_back(results2);
    
    pipeline2 = pipeline1;
    send(pipeline2, 1);
    REQUIRE(results1.size() == 1);
    REQUIRE(results2.size() == 0);
}
