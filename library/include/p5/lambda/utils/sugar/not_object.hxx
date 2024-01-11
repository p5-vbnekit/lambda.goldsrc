#pragma once

#include "not_object+fwd.hxx"

#include "uncopyable.hxx"


namespace p5::lambda::utils::sugar::not_object {

class Type: parent_::Uncopyable {
    template <class ... T> Type(T && ...) = delete;
    template <class ... T> Type & operator = (T && ...) = delete;
};

} // namespace p5::lambda::utils::sugar::not_object
