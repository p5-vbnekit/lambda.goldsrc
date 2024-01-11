#pragma once

#include <optional>
#include <type_traits>

#include "message.hxx"
#include "raii+fwd.hxx"
#include "interface+fwd.hxx"


namespace p5::lambda::utils::log::raii {

namespace text = parent_::message::text;

namespace context {

using Text = parent_::text::driver::Interface;
using Level = parent_::parent_::message::Level;
using Target = parent_::parent_::Interface;
using Location = parent_::parent_::message::Location;

struct Type final {
    using Text = this_::Text;
    using Level = this_::Level;
    using Target = this_::Target;
    using Location = this_::Location;

    Text *text = nullptr;
    Level level = Level::Default;
    Target const *target = nullptr;
    ::std::optional<Location> location = ::std::nullopt;
};

} // namespace context

namespace item {

template <class Text> struct Type final {
    static_assert(::std::is_same_v<::std::decay_t<Text>, Text>);
    static_assert(::std::is_base_of_v<parent_::text::driver::Interface, Text>);

    ~Type() noexcept(true);

    explicit Type(
        Text &&, parent_::Context &,
        parent_::context::Text **
    ) noexcept(false);

    template <class T> auto operator << (T &&) noexcept(false);

private:
    struct Machine_ final: parent_::text::driver::Interface {
        enum class State { Initial, First, Next, Final };

        parent_::text::driver::Interface *next = nullptr;

        auto state() const noexcept(true);

        virtual parent_::text::Chunk const *
        operator () () noexcept(true) override final;

        explicit Machine_(Text &&) noexcept(true);

    private:
        State state_ = State::Initial;
        Text first_;

        Machine_() = delete;
        Machine_(Machine_ &&) = delete;
        Machine_(Machine_ const &) = delete;
        Machine_ & operator = (Machine_ &&) = delete;
        Machine_ & operator = (Machine_ const &) = delete;
    } machine_;

    parent_::Context &context_;

    Type() = delete;
    Type(Type &&) = delete;
    Type(Type const &) = delete;
    Type & operator = (Type &&) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace item

struct Type final {
    using Context = this_::Context;

    ~Type() noexcept(true);

    explicit Type(Context &&) noexcept(false);

    template <class T> auto operator << (T &&) noexcept(false);

private:
    Context context_;

    Type() = delete;
    Type(Type const &) = delete;
    Type & operator = (Type &&) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace p5::lambda::utils::log::raii


#include "raii+inl.hxx"
