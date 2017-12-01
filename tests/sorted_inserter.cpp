#include <algorithm>
#include <set>
#include <vector>
#include "catch.hpp"

#include "sorted_inserter.hpp"

TEST_CASE("sorted_inserter")
{
    std::vector<int> v = {1, 3, -4, 2, 7, 10, 8};
    std::set<int> expected = { -4, 1, 2, 3, 7, 8, 10 };
    std::set<int> results;
    std::copy(begin(v), end(v), sorted_inserter(results));
    
    REQUIRE((results == expected));
}

TEST_CASE("sorted_inserter should not insert existing values")
{
    std::vector<int> v = {1, 3, -4, 2, 7, 10, 8};
    std::set<int> expected = { -4, 1, 2, 3, 7, 8, 10 };
    std::set<int> results = { 3 };
    std::copy(begin(v), end(v), sorted_inserter(results));
    
    REQUIRE((results == expected));
}
