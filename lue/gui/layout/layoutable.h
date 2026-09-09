#pragma once


#include <gui/base/types.h>

namespace lue::gui::layout {

class layoutable
{
public:
    layoutable();
    virtual ~layoutable() = default;
    alignment       align           () const { return align_; }
    void            align_set       (alignment align)   { align_ = align; }

    size            size_hint        () const   { return do_size_hint();    }
    size            minimum_size     () const   { return do_minimum_size(); }
    size            maximum_size     () const   { return do_maximum_size(); }

protected:
    virtual size        do_size_hint        () const = 0;
    virtual size        do_minimum_size     () const = 0;
    virtual size        do_maximum_size     () const = 0;
    virtual void        do_set_geometry     (rect r) = 0;
    virtual rect        do_geometry         () const = 0;

private:
    alignment           align_              {alignment::left};
};


// TODO: Inspiration: /usr/include/x86_64-linux-gnu/qt6/QtWidgets/qlayoutitem.h
/*
class Q_WIDGETS_EXPORT QLayoutItem
{
public:
    inline explicit QLayoutItem(Qt::Alignment alignment = Qt::Alignment());
    virtual ~QLayoutItem();
    virtual QSize sizeHint() const = 0;
    virtual QSize minimumSize() const = 0;
    virtual QSize maximumSize() const = 0;
    virtual Qt::Orientations expandingDirections() const = 0;
    virtual void setGeometry(const QRect&) = 0;
    virtual QRect geometry() const = 0;
    virtual bool isEmpty() const = 0;
    virtual bool hasHeightForWidth() const;
    virtual int heightForWidth(int) const;
    virtual int minimumHeightForWidth(int) const;
    virtual void invalidate();

    virtual QWidget *widget() const;
    virtual QLayout *layout();
    virtual QSpacerItem *spacerItem();

    Qt::Alignment alignment() const { return align; }
    void setAlignment(Qt::Alignment a);
    virtual QSizePolicy::ControlTypes controlTypes() const;

protected:
    Qt::Alignment align;
};
*/

} // namespace lue::gui::layout
