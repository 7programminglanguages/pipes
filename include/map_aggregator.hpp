#ifndef MAP_AGGREGATOR_HPP
#define MAP_AGGREGATOR_HPP

#include <functional>
#include <iterator>
#include "output_iterator.hpp"
#include "helpers/assignable.hpp"

namespace pipes
{

template<typename Map, typename Function>
class map_aggregate_iterator : public OutputIteratorBase<map_aggregate_iterator<Map, Function>>
{
public:
    template<typename T>
    void onReceive(T const& keyValue)
    {
        auto position = map_.get().find(keyValue.first);
        if (position != map_.get().end())
        {
            position->second = (*aggregator_)(position->second, keyValue.second);
        }
        else
        {
            map_.get().insert(keyValue);
        }
    }
    
    using container_type = Map;
    map_aggregate_iterator(Map& map, Function aggregator) : map_(map), aggregator_(aggregator) {}
    
private:
    std::reference_wrapper<Map> map_;
    detail::assignable<Function> aggregator_;
    
public: // but technical
    using OutputIteratorBase<map_aggregate_iterator<Map, Function>>::operator=;
    map_aggregate_iterator& operator=(map_aggregate_iterator const& other) = default;
    map_aggregate_iterator& operator=(map_aggregate_iterator& other) { *this = const_cast<map_aggregate_iterator const&>(other); return *this; }
};

template<typename Map, typename Function>
map_aggregate_iterator<Map, Function> map_aggregator(Map& map, Function aggregator)
{
    return map_aggregate_iterator<Map, Function>(map, aggregator);
}
    
} // namespace pipes


#endif
