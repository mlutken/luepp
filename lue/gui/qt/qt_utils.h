#pragma once

#include <QString>

// Qt Threading GUI information
// https://somcosoftware.com/en/blog/mastering-qt-multithreading-without-losing-your-mind
// https://www.codegenes.net/blog/simple-multithreading-with-qt-am-i-doing-this-right/

namespace lue::gui {

inline QString to_qstring(const std::string& s  )   { return QString::fromStdString(s);         }
inline QString to_qstring(std::string_view s    )   { return QString::fromStdString(s.data());  }

} // namespace lue::gui
