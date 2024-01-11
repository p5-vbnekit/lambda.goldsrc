#pragma once

#include <optional>

#include "text.hxx"
#include "level.hxx"
#include "location.hxx"

#include "type+fwd.hxx"


namespace p5::lambda::utils::log::message {

struct Type final {
    using Text = this_::Text;
    using Level = this_::Level;
    using Location = this_::Location;

    Text text{nullptr};
    Level level = Level::Default;
    ::std::optional<Location> location = ::std::nullopt;
};

} // namespace p5::lambda::utils::log::message
