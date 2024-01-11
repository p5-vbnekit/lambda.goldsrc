#pragma once

#include <string_view>

#include "chunk+fwd.hxx"


namespace p5::lambda::utils::log::message::text::chunk {

struct Type final {
    ::std::string_view reference = {};
    bool terminated = false;
};

} // namespace p5::lambda::utils::log::message::text::chunk


#include "chunk+inl.hxx"
