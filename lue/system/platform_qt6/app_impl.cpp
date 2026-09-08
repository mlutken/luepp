#include "app_impl.h"

namespace lue::system {

app_impl::app_impl(int argc, char* argv[])
    : QApplication{argc, argv}
{
}

app_impl::~app_impl() = default;


} // namespace lue::system
