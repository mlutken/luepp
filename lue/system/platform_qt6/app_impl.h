#pragma once

#include <QCoreApplication>


// TODO: Not sure we really want to use Qt signal slot etc. in a lue::core application!

namespace lue::system {

class app_impl : public QCoreApplication
{
public:
    app_impl(int& argc, char *argv[]);

private:
};

} // namespace lue::system
