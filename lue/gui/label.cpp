#include "label.h"

// PLATFORM_CPP_INCLUDE_BEGIN
#include "gui/qt/label_impl.h"
// PLATFORM_CPP_INCLUDE_END

namespace lue::gui {


label::label(data::data_source_base& data_source, const concepts::uri& path):
    widget_base  {data_source, path},
    pimpl_  {std::make_unique<label_impl>()}
{

}

} // namespace lue::gui
