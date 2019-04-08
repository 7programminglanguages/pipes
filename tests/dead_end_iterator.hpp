#ifndef dead_end_iterator_h
#define dead_end_iterator_h

#include <iterator>

namespace pipes
{

class dead_end_iterator
{
public:
    using iterator_category = std::output_iterator_tag;
    using value_type = void;
    using difference_type = void;
    using pointer = void;
    using reference = void;
    
    dead_end_iterator& operator++() { return *this; }
    dead_end_iterator& operator++(int) { return *this; }
    
    dead_end_iterator& operator*() { return *this; }
    
    template<typename T>
    dead_end_iterator& operator=(T const&) { return *this; }
};

} // namespace pipes

#endif /* dead_end_iterator_h */
