#pragma once

#include "page_type_a.h"

class test_page_a1 : public page_type_a
{
public:
    explicit test_page_a1(data_source_base& data_source, QWidget* parent = nullptr);

private:
    void    build_page_qt_way   ();
    void    build_page          ();
};
