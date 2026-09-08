#pragma once

#include <QApplication>


namespace lue::system {

class app_impl : public QApplication
{
public:
    app_impl(int& argc, char *argv[]);

private:
};

} // namespace lue::system
