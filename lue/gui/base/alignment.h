#pragma once


namespace lue::gui {

enum class alignment {
    none        = 0x0000,
    left        = 0x0001,
    right       = 0x0002,
    center_h    = 0x0004,
    justify     = 0x0008,
    absolute    = 0x0010,
    horizontal  = left | right | center_h | justify | absolute,

    top         = 0x0020,
    bottom      = 0x0040,
    center_v    = 0x0080,
    baseline    = 0x0100,
    vertical    = top | bottom | center_v | baseline,
    center      = center_v | center_h
};


} // namespace lue::gui
