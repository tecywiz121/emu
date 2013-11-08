#ifndef UTIL_HPP
#define UTIL_HPP 1

// make_unique implementation
// From: http://stackoverflow.com/a/12580468/586583
template<typename T, typename ...Args>
std::unique_ptr<T> make_unique( Args&& ...args )
{
    return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
}

// Ceiling Integer Division
#define icdiv(x, y) (((x) % (y)) ? (x) / (y) + 1 : (x) / (y))

// libstdc++ has a bug where some methods that should accept a const iterator don't
// From: http://stackoverflow.com/a/765159/586583
template <typename Container, typename ConstIterator>
typename Container::iterator remove_constness(Container& c, ConstIterator it)
{
    typename Container::iterator i(c.begin());
    std::advance(i, std::distance<ConstIterator>(i, it));
    return i;
}

#endif
