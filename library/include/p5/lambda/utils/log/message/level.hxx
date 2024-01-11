#pragma once

#include "level+fwd.hxx"


namespace p5::lambda::utils::log::message::level {

enum class Type { Error, Warning, Info, Debug, Developer, Default = Info };

} // namespace p5::lambda::utils::log::message::level


#include "level+inl.hxx"
