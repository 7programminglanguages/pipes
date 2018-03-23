#ifndef fluent_meta_h
#define fluent_meta_h

#include <iterator>
#include <type_traits>
#include <utility>

namespace fluent
{
template<typename Iterator>
using IsOutputIterator = std::is_same<typename std::iterator_traits<Iterator>::iterator_category, std::output_iterator_tag>;
    
namespace detail
{
    
template <class F, class Tuple, std::size_t... I>
F apply_impl(F&& f, Tuple&& t, std::index_sequence<I...>)
{
    return (void)std::initializer_list<int>{(std::forward<F>(f)(std::get<I>(std::forward<Tuple>(t))),0)...}, f;
}

template <class F, class Tuple1, class Tuple2, std::size_t... I>
F apply2_impl(F&& f, Tuple1&& t1, Tuple2&& t2, std::index_sequence<I...>)
{
    return (void)std::initializer_list<int>{(std::forward<F>(f)(std::get<I>(std::forward<Tuple1>(t1)), std::get<I>(std::forward<Tuple2>(t2))),0)...}, f;
}

template <class F, class Tuple>
constexpr decltype(auto) apply(F&& f, Tuple&& t)
{
    return detail::apply_impl(std::forward<F>(f), std::forward<Tuple>(t),
                              std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple>>::value>{});
}

template <class F, class Tuple1, class Tuple2>
constexpr decltype(auto) apply2(F&& f, Tuple1&& t1, Tuple2&& t2)
{
    return detail::apply2_impl(std::forward<F>(f), std::forward<Tuple1>(t1), std::forward<Tuple2>(t2),
                               std::make_index_sequence<std::tuple_size<std::remove_reference_t<Tuple1>>::value>{});
}

}  // namespace detail
}

#endif /* fluent_meta */
