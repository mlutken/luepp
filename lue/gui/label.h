#pragma once

#include "data/data_source_base.h"

// PLATFORM_HEADER_INCLUDE_BEGIN
class QLabel;
// PLATFORM_HEADER_INCLUDE_END


namespace lue::gui {

class label
{
public:
    explicit label(lue::data::data_source_base& data_source);

private:
    lue::data::data_source_base& m_data_source;
    // PLATFORM_HEADER_CLASS_BEGIN
    QLabel*  m_label_ptr{nullptr};
// PLATFORM_HEADER_CLASS_END
};

} // namespace lue::gui
