#include "catch.hpp"
#include "partition.hpp"

TEST_CASE("partition")
{
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedEvens = {2, 4, 6, 8, 10};
    std::vector<int> expectedOdds = {1, 3, 5, 7, 9};

    auto const isEvenPartition = pipes::partition([](int n){ return n % 2 == 0; });
    
    std::vector<int> evens;
    std::vector<int> odds;
    
    std::copy(begin(input), end(input), isEvenPartition(std::back_inserter(evens), std::back_inserter(odds)));
    
    REQUIRE(evens == expectedEvens);
    REQUIRE(odds == expectedOdds);
}

TEST_CASE("partition can override existing results")
{
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedEvens = {2, 4, 6, 8, 10, 0, 0, 0, 0, 0};
    std::vector<int> expectedOdds = {1, 3, 5, 7, 9, 0, 0, 0, 0, 0};
    
    auto const isEvenPartition = pipes::partition([](int n){ return n % 2 == 0; });
    
    std::vector<int> evens = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    std::vector<int> odds = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    
    std::copy(begin(input), end(input), isEvenPartition(begin(evens), begin(odds)));
    
    REQUIRE(evens == expectedEvens);
    REQUIRE(odds == expectedOdds);
}

TEST_CASE("partition's iterator category should be std::output_iterator_tag")
{
    auto const isEvenPartition = pipes::partition([](int n){ return n % 2 == 0; });
    std::vector<int> output1, output2;
    static_assert(std::is_same<decltype(isEvenPartition(std::back_inserter(output1), std::back_inserter(output2)))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}

TEST_CASE("partition cannot override existing contents")
{
    /* This code should not compile as the output_partition_iterator is plugged on a vector::begin
    
    std::vector<int> input = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedEvens = {2, 4, 6, 8, 10};
    std::vector<int> expectedOdds = {1, 3, 5, 7, 9};
    
    auto const isEvenPartition = pipes::partition([](int n){ return n % 2 == 0; });
    
    std::vector<int> evens = {0, 0, 0, 0, 0};
    std::vector<int> odds = {0, 0, 0, 0, 0};
    
    std::copy(begin(input), end(input), isEvenPartition(begin(evens), end(odds)));
    
    REQUIRE(evens == expectedEvens);
    REQUIRE(odds == expectedOdds);
     
    */
}
