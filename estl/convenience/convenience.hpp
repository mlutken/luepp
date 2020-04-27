#ifndef ESTL_CONVENIENCE_HPP
#define ESTL_CONVENIENCE_HPP

#include <algorithm>

namespace estl {

template <class CONTAINER, class T>
typename CONTAINER::iterator find(CONTAINER& container, const T& value)
{
    return std::find(container.begin(), container.end(), value);
}

template <class CONTAINER, class T>
typename CONTAINER::const_iterator find(const CONTAINER& container, const T& value)
{
    return std::find(container.begin(), container.end(), value);
}

template <class CONTAINER, class MATCH>
typename CONTAINER::iterator find_if(CONTAINER& container, const MATCH& match_fn)
{
    return std::find_if(container.begin(), container.end(), match_fn);
}

template <class CONTAINER, class MATCH>
typename CONTAINER::const_iterator find_if(const CONTAINER& container, const MATCH& match_fn)
{
    return std::find_if(container.begin(), container.end(), match_fn);
}

} // namespace estl


#endif // ESTL_CONVENIENCE_HPP
