#pragma once

#include <memory>
#include <unordered_map>
#include "gui/widget_base.h"

namespace lue::gui {

class container_impl;

class container : public widget_base
{
public:
    explicit container(data::data_source_base& data_source, const concepts::uri& path);
    ~container() override;

    container(const container&) = delete;
    container& operator=(const container&) = delete;
    container(container&&);
    container& operator=(container&&) = delete;

    void        show();

    void        add_widget          (const concepts::uri& path, std::unique_ptr<widget_base> w);

private:
    std::unique_ptr<container_impl> pimpl_;
    std::unordered_map<concepts::uri, std::unique_ptr<widget_base>> widgets_;
};

} // namespace lue::gui
