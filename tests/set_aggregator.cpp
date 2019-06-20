#include <iterator>
#include <set>
#include <vector>

#include "catch.hpp"
#include "set_aggregator.hpp"

struct Value
{
    int i;
    std::string s;
};

bool operator==(Value const& value1, Value const& value2)
{
    return value1.i == value2.i && value1.s == value2.s;
}

bool operator<(Value const& value1, Value const& value2)
{
    if (value1.i < value2.i) return true;
    if (value2.i < value1.i) return false;
    return value1.s < value2.s;
}

Value concatenateValues(Value const& value1, Value const& value2)
{
    if (value1.i != value2.i) throw std::runtime_error("Incompatible values");
    return { value1.i, value1.s + value2.s };
}

TEST_CASE("set_aggregator")
{
    std::vector<Value> entries = { {1, "a"}, {2, "b"}, {3, "c"}, {4, "d"} };
    std::vector<Value> entries2 = { {2, "b"}, {3, "c"}, {4, "d"}, {5, "e"} };
    
    std::set<Value> expected = { {1, "a"}, {2, "bb"}, {3, "cc"}, {4, "dd"}, {5, "e"} };
    std::set<Value> results;
    
    std::copy(entries.begin(), entries.end(), pipes::set_aggregator(results, concatenateValues));
    std::copy(entries2.begin(), entries2.end(), pipes::set_aggregator(results, concatenateValues));
    
    REQUIRE((results == expected));
}

TEST_CASE("set_aggregator's iterator category should be std::output_iterator_tag")
{
    std::set<Value> results;
    static_assert(std::is_same<decltype(pipes::set_aggregator(results, concatenateValues))::iterator_category,
                  std::output_iterator_tag>::value,
                  "iterator category should be std::output_iterator_tag");
}

TEST_CASE("set_aggregator::operator=")
{
    auto results1 = std::set<Value>{};
    auto set_aggregator1 = pipes::set_aggregator(results1, concatenateValues);
    auto results2 = std::set<Value>{};
    auto set_aggregator2 = pipes::set_aggregator(results2, concatenateValues);
    
    set_aggregator2 = set_aggregator1;
    pipes::send(set_aggregator2, Value{0, "zero"});
    REQUIRE(results1.size() == 1);
    REQUIRE(results2.size() == 0);
}
