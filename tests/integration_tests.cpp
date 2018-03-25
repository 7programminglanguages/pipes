#include "catch.hpp"
#include "output_transformer.hpp"
#include "output_filter.hpp"
#include "output_partitioner.hpp"
#include "output_unzipper.hpp"
#include "output_demuxer.hpp"

#include <algorithm>
#include <utility>
#include <vector>

TEST_CASE("Mix of various output iterators")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedOutput1 = {6, 12, 18};
    std::vector<int> expectedOutput2 = {2, 4};
    std::vector<int> expectedOutput3 = {2, 10};
    std::vector<int> expectedOutput4 = {1, 5, 7};
    std::vector<char> expectedOutput5 = {'A', 'A', 'A'};
    
    std::vector<int> output1;
    std::vector<int> output2;
    std::vector<int> output3;
    std::vector<int> output4;
    std::vector<char> output5;

    auto const times2 = fluent::make_output_transformer([](int i) { return i*2; });
    auto const divideBy2 = fluent::make_output_transformer([](int i) { return i/2; });
    auto const isEvenPartition = fluent::make_output_partitioner([](int n){ return n % 2 == 0; });
    auto const pairUpWithA = fluent::make_output_transformer([](int i) { return std::make_pair(i, 'A'); });


    std::copy(begin(numbers), end(numbers), output_demuxer(fluent::demux_if( [](int n){ return n % 3 == 0; } ).sendTo(
                                                                                                                      times2(back_inserter(output1))
                                                                                                                     ),
                                                           fluent::demux_if( [](int n){ return n % 2 == 0; } ).sendTo(
                                                                                                                      divideBy2(
                                                                                                                                isEvenPartition(
                                                                                                                                                back_inserter(output2),
                                                                                                                                                times2(back_inserter(output3))
                                                                                                                                               )
                                                                                                                               )
                                                                                                                     ),
                                                           fluent::demux_if( [](int n){ return n % 1 == 0; } ).sendTo(
                                                                                                                      pairUpWithA(
                                                                                                                                  fluent::output_unzipper(
                                                                                                                                                          back_inserter(output4),
                                                                                                                                                          back_inserter(output5)
                                                                                                                                                 )
                                                                                                                                 )
                                                                                                                     )));
    
    REQUIRE(output1 == expectedOutput1);
    REQUIRE(output2 == expectedOutput2);
    REQUIRE(output3 == expectedOutput3);
    REQUIRE(output4 == expectedOutput4);
    REQUIRE(output5 == expectedOutput5);
}

TEST_CASE("Mix of various output iterators with pipe")
{
    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    
    std::vector<int> expectedOutput1 = {6, 12, 18};
    std::vector<int> expectedOutput2 = {2, 4};
    std::vector<int> expectedOutput3 = {2, 10};
    std::vector<int> expectedOutput4 = {1, 5, 7};
    std::vector<char> expectedOutput5 = {'A', 'A', 'A'};
    
    std::vector<int> output1;
    std::vector<int> output2;
    std::vector<int> output3;
    std::vector<int> output4;
    std::vector<char> output5;
    
    auto const times2 = fluent::make_output_transformer([](int i) { return i*2; });
    auto const divideBy2 = fluent::make_output_transformer([](int i) { return i/2; });
    auto const isEvenPartition = fluent::make_output_partitioner([](int n){ return n % 2 == 0; });
    auto const pairUpWithA = fluent::make_output_transformer([](int i) { return std::make_pair(i, 'A'); });
    
    
    std::copy(begin(numbers), end(numbers), output_demuxer(fluent::demux_if( [](int n){ return n % 3 == 0; } ).sendTo(
                                                                                                                      times2 | back_inserter(output1)
                                                                                                                     ),
                                                           fluent::demux_if( [](int n){ return n % 2 == 0; } ).sendTo(
                                                                                                                      divideBy2 | isEvenPartition(
                                                                                                                                                   back_inserter(output2),
                                                                                                                                                   times2 | back_inserter(output3)
                                                                                                                                                 )
                                                                                                                      ),
                                                           fluent::demux_if( [](int n){ return n % 1 == 0; } ).sendTo(
                                                                                                                      pairUpWithA | fluent::output_unzipper(
                                                                                                                                                            back_inserter(output4),
                                                                                                                                                            back_inserter(output5)
                                                                                                                                                           )
                                                                                                                      )));
    
    REQUIRE(output1 == expectedOutput1);
    REQUIRE(output2 == expectedOutput2);
    REQUIRE(output3 == expectedOutput3);
    REQUIRE(output4 == expectedOutput4);
    REQUIRE(output5 == expectedOutput5);
}
