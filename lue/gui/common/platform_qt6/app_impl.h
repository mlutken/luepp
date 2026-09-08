#pragma once

#include <QApplication>


namespace lue::gui::common {

class app_impl : public QApplication
{
public:
    app_impl(int& argc, char* argv[]);

private:
};

} // namespace lue::gui::common
