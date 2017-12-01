#ifndef SET_AGGREGATOR_HPP
#define SET_AGGREGATOR_HPP

#include <iterator>

namespace fluent
{

template<typename Set, typename Function>
class set_aggregate_iterator
{
public:
    using iterator_category = std::output_iterator_tag;
    using container_type = Set;
    set_aggregate_iterator(Set& set, Function aggregator) : set_(set), aggregator_(aggregator) {}
    set_aggregate_iterator operator++(){ return *this; }
    set_aggregate_iterator operator*(){ return *this; }
    set_aggregate_iterator& operator=(typename Set::value_type const& value)
    {
        auto position = set_.find(value);
        if (position != set_.end())
        {
            auto containedValue = *position;
            position = set_.erase(position);
            set_.insert(position, aggregator_(value, containedValue));
        }
        else
        {
            set_.insert(position, value);
        }
        return *this;
    }
    
private:
    Set& set_;
    Function aggregator_;

};

template<typename Set, typename Function>
set_aggregate_iterator<Set, Function> set_aggregator(Set& set, Function aggregator)
{
    return set_aggregate_iterator<Set, Function>(set, aggregator);
}
    
} // namespace fluent

#endif
