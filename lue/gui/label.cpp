#include "label.h"

// PLATFORM_CPP_INCLUDE_BEGIN
#include <QLabel>
// PLATFORM_CPP_INCLUDE_END

namespace lue::gui {

class label::impl
{
friend class label;

public:

    impl() = default;

private:

// QLabel* qlabel_{nullptr};
};



label::label(data::data_source_base& data_source, const concepts::uri& path)
    : data_source_(data_source),
      path_(path),
      pimpl_{std::make_unique<impl>()}
{

}

} // namespace lue::gui
