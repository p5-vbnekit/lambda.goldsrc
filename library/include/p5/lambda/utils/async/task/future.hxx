#pragma once

#include <type_traits>

#include "future+fwd.hxx"
#include "../future.hxx"


namespace p5::lambda::utils::async::task::future {
namespace type {

template <class T> struct Wrapper final {
    static_assert(! ::std::is_rvalue_reference_v<T>);
    using Type = parent_::
        parent_::parent_
    ::Future<::std::remove_reference_t<T>>;
};
} // namespace type

namespace issuer {

using namespace parent_::parent_::future::issuer;

} // namespace issuer
} // namespace p5::lambda::utils::async::task::future::type
