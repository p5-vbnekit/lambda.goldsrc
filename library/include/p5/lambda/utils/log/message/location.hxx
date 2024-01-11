#pragma once

#include <cstdint>

#include <string>
#include <string_view>

#ifdef __cpp_lib_source_location
#include <source_location>
#else
#include <experimental/source_location>
#endif

#include "location+fwd.hxx"


namespace p5::lambda::utils::log::message::location {
namespace native {

#ifdef __cpp_lib_source_location
using Type = ::std::source_location;
#else
using Type = ::std::experimental::source_location;
#endif

} // namespace native

using Native = native::Type;

struct Type final {
    using Native = this_::Native;

    ::std::string_view file = {}, function = {};
    ::std::size_t line = 0, column = 0;

    constexpr static auto make() noexcept(true);
    constexpr static auto make(Native const &) noexcept(true);

    ::std::string to_string(
        ::std::string_view const &root = {}
    ) const noexcept(false);

private:
    struct FinalToken_ final {};

public:
    constexpr static auto current(
        FinalToken_ const & = {}, Native const & = Native::current()
    ) noexcept(true);
};

} // namespace p5::lambda::utils::log::message::location


#include "location+inl.hxx"
