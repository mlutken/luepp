#pragma once

#include <memory>
#include <unordered_map>
#include "gui/container.h"

namespace lue::gui {

class page_impl;

class page : public container
{
public:
    explicit page(data::data_source_base& data_source, const concepts::uri& path);
    ~page() override;

    page(const page&) = delete;
    page& operator=(const page&) = delete;
    page(page&&);
    page& operator=(page&&) = delete;

    void        show();

    void        add_widget          (const concepts::uri& path, std::unique_ptr<widget_base> w);

private:
    std::unique_ptr<page_impl> pimpl_;
    std::unordered_map<concepts::uri, std::unique_ptr<widget_base>> widgets_;
};

} // namespace lue::gui
