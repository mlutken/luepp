#include "container.h"

namespace lue::gui {

class container::impl
{
public:
    friend class container;
    impl() = default;

private:

};

container::container() :
    pimpl_{std::make_unique<impl>()}
{

}

} // namespace lue::gui
