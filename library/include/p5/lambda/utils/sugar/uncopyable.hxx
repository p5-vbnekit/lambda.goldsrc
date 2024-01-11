#pragma once

#include "uncopyable+fwd.hxx"


namespace p5::lambda::utils::sugar::uncopyable {

class Type {
    Type(Type const &) = delete;
    Type & operator = (Type const &) noexcept(true) = delete;

protected:
    Type() = default;
    Type(Type &&) = default;
    Type & operator = (Type &&) = default;
};

} // namespace p5::lambda::utils::sugar::uncopyable
