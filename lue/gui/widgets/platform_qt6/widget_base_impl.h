#pragma once

#include <QWidget>


namespace lue::gui::widgets {

class widget_base_impl : public QObject
{
    Q_OBJECT
public:
    explicit widget_base_impl(QObject* parent = nullptr);
    ~widget_base_impl() override;


    virtual QWidget* widget() { return nullptr; }
    // virtual const QWidget* widget() const{ return nullptr; }
    // virtual QWidget* widget() = 0;  // TODO: USE this when we have basic stuff working !!
// virtual const QWidget*  widget() const = 0;

    template <class WIDGET>
    WIDGET* w()
    {
        return static_cast<WIDGET*>(widget());
    }


signals:
private:
};

} // namespace lue::gui::widgets
