#pragma once
#include <memory>

#include "gui/widgets/widget_base.h"

namespace lue::gui::widgets {

class label_impl;

class label : public widget_base
{
public:
    explicit label();
    ~label() override;
    // Disable copy and move assignment operators.
    label(const label&) = delete;
    label& operator=(const label&) = delete;
    label(label&&);
    label& operator=(label&&) = delete;

    void word_wrap(bool wrap);

    void text       (const std::string& text);

    widget_base_impl* impl() override;
    // const widget_base_impl* impl() const override;

private:
    std::unique_ptr<label_impl>     pimpl_;
};

} // namespace lue::gui::widgets
