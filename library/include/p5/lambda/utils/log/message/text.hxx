#pragma once

#include <type_traits>

#include "text+fwd.hxx"

#include "text/type.hxx"
#include "text/chunk.hxx"
#include "text/driver.hxx"
#include "text/normalizer.hxx"


namespace p5::lambda::utils::log::message::text {
namespace view {
namespace policy {
namespace line {

enum class Type { Copy, Chunk, Chunks, Default = Chunks };

} // namespace line

struct Default final {
    inline constexpr static auto const line = this_::Line::Default;
    bool terminate = false;
};

template <class T> class Traits final {
    static_assert(::std::is_same_v<T, ::std::decay_t<T>>);

    inline constexpr static auto const default_ = this_::Default{};

public:
    constexpr static this_::Line line() noexcept(true);
    constexpr static bool terminate(T const &value) noexcept(true);
};

} // namespace policy

namespace function {

template <class> struct Type final {
    constexpr auto operator () () const noexcept(true);
};

} // namespace function
} // namespace view
} // namespace p5::lambda::utils::log::message::text


#include "text+inl.hxx"
