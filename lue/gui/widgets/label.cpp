#include "label.h"
#include <iostream>

// PLATFORM_CPP_INCLUDE_BEGIN
#include "gui/widgets/platform_qt6/label_impl.h"
// PLATFORM_CPP_INCLUDE_END

using namespace std;

namespace lue::gui::widgets {


label::label():
    widget_base  {},
    pimpl_  {std::make_unique<label_impl>()}
{
    // cerr << "FIXMENM label::CONSTRUCTOR() path: '" << path << "'\n";
}

label::~label() = default;

label::label(label&&) = default;

void label::word_wrap(bool wrap)
{
    pimpl_->word_wrap(wrap);
}

void label::text(const std::string& text)
{
    pimpl_->text(text);
}


widget_base_impl* label::impl() {
    return pimpl_.get();
}

// const widget_base_impl* label::impl() const {
//     return pimpl_.get();
// }

} // namespace lue::gui::widgets
