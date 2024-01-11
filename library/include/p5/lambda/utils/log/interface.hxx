#pragma once

#include <type_traits>

#include "interface+fwd.hxx"

#include "raii.hxx"
#include "message.hxx"


namespace p5::lambda::utils::log::interface {

using RAII = parent_::RAII;
using Message = parent_::Message;

struct Type {
    using RAII = this_::RAII;
    using Message = this_::Message;

    auto write(Message &&) const noexcept(true);

private:
    struct FinalToken_ final {};

    struct Speculations_ final {
        template <class T> inline constexpr
        static auto location() noexcept(true) {
            return ::std::is_base_of_v<Message::Location, ::std::decay_t<T>>;
        }

        template <class T> inline constexpr
        static auto text() noexcept(true) {
            if (location<T>()) return false;
            using Suspect_ = ::std::decay_t<T>;
            if (::std::is_base_of_v<Message, Suspect_>) return false;
            if (::std::is_base_of_v<Message::Level, Suspect_>) return false;
            return true;
        }
    };

public:
    template <Message::Level = Message::Level::Info> auto write(
        FinalToken_ && = {}, Message::Location && = Message::Location::current()
    ) const noexcept(true);

    template <
        Message::Level = Message::Level::Info, class Location
    > auto write(Location &&) const noexcept(true) requires(
        Speculations_::location<Location>()
    );

    RAII write(
        Message::Level, FinalToken_ && = {},
        Message::Location && = Message::Location::current()
    ) const noexcept(true);

    template <class Location> auto write(
        Message::Level, Location &&
    ) const noexcept(true) requires(Speculations_::location<Location>());

    template <Message::Level = Message::Level::Info, class Text> auto write(
        Text &&, FinalToken_ && = {},
        Message::Location && = Message::Location::current()
    ) const noexcept(true) requires(Speculations_::text<Text>());

    template <
        Message::Level = Message::Level::Info, class Text, class Location
    > auto write(Text &&, Location &&) const noexcept(true) requires(
        Speculations_::text<Text>() && Speculations_::location<Location>()
    );

    template <class Text> auto write(
        Message::Level, Text &&, FinalToken_ && = {},
        Message::Location && = Message::Location::current()
    ) const noexcept(true) requires(Speculations_::text<Text>());

    template <class Text, class Location> auto write(
        Message::Level, Text &&, Location &&
    ) const noexcept(true) requires(
        Speculations_::text<Text>() && Speculations_::location<Location>()
    );

    template <class Text> auto write(
        Text &&, Message::Level, FinalToken_ && = {},
        Message::Location && = Message::Location::current()
    ) const noexcept(true) requires(Speculations_::text<Text>());

    template <class Text, class Location> auto write(
        Text &&, Message::Level, Location &&
    ) const noexcept(true) requires(
        Speculations_::text<Text>() && Speculations_::location<Location>()
    );

    virtual ~Type() noexcept(true);

protected:
    Type() noexcept(true) = default;
    Type(Type &&) noexcept(true) = default;
    Type & operator = (Type &&) noexcept(true) = default;

private:
    virtual void write_(Message &&) const noexcept(true) = 0;

    Type(Type const &) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace p5::lambda::utils::log::interface


#include "interface+inl.hxx"
