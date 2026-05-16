#include "label.h"
#include <iostream>

// PLATFORM_CPP_INCLUDE_BEGIN
#include "gui/qt/label_impl.h"
// PLATFORM_CPP_INCLUDE_END

using namespace std;

namespace lue::gui {


label::label(data::data_source_base& data_source, const concepts::uri& path):
    widget_base  {data_source, path},
    pimpl_  {std::make_unique<label_impl>()}
{
    // cerr << "FIXMENM label::CONSTRUCTOR() path: '" << path << "'\n";
    cerr << "FIXMENM label::CONSTRUCTOR, " << path << ": '" << data_source.as_string(path) << "'\n";
    cerr << data_source.to_string() << "'\n";
    // pimpl_->setText(data_source.as_string(path).c_str());
    pimpl_->on_data_changed(data_source, path);
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

} // namespace lue::gui
