#pragma once

#include <QApplication>


namespace lue::system {

class app_impl : public QApplication
{
    Q_OBJECT
public:
    app_impl(int argc, char *argv[]);
    ~app_impl() override;



signals:
private:
};

} // namespace lue::system
