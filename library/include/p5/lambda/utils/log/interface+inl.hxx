#pragma once

#include <utility>
#include <type_traits>

#include "raii.hxx"
#include "message.hxx"
#include "interface.hxx"


namespace p5::lambda::utils::log::interface {

inline auto this_::Type::write(Message &&message) const noexcept(true) {
    return write_(::std::move(message));
}

template <this_::Type::Message::Level level> inline auto this_::Type::write(
    FinalToken_ &&, Message::Location &&location
) const noexcept(true) {
    auto &&context_ = RAII::Context{.level = level, .target = this};
    if constexpr (
        Message::Level::Info < level
    ) context_.location.emplace(::std::move(location));
    return RAII{::std::move(context_)};
}

template <this_::Type::Message::Level level, class Location> inline
auto this_::Type::write(Location &&location) const noexcept(true) requires(
    Speculations_::location<Location>()
) {
    return RAII{RAII::Context{
        .level = level, .target = this,
        .location = ::std::forward<Location>(location)
    }};
}

template <class Location> inline auto this_::Type::write(
    Message::Level level, Location &&location
) const noexcept(true) requires(Speculations_::location<Location>()) {
    return RAII{RAII::Context{
        .level = level, .target = this,
        .location = ::std::forward<Location>(location)
    }};
}

template <this_::Type::Message::Level level, class Text> inline
auto this_::Type::write(
    Text &&text, FinalToken_ &&, Message::Location &&location
) const noexcept(true) requires(Speculations_::text<Text>()) {
    auto text_ = parent_::message::text::driver::make(
        ::std::forward<Text>(text)
    );
    auto &&message_ = Message{
        .text = parent_::message::text::make(text_), .level = level
    };
    if constexpr (
        Message::Level::Info < level
    ) message_.location.emplace(::std::move(location));
    write(::std::move(message_));
}

template <this_::Type::Message::Level level, class Text, class Location> inline
auto this_::Type::write(
    Text &&text, Location &&location
) const noexcept(true) requires(
    Speculations_::text<Text>() && Speculations_::location<Location>()
) {
    write(Message{.text = parent_::message::text::make(
        parent_::message::text::driver::make(::std::forward<Text>(text))
    ), .level = level, .location = ::std::forward<Location>(location)});
}

template <class Text> inline auto this_::Type::write(
    Message::Level level, Text &&text,
    FinalToken_ &&, Message::Location &&location
) const noexcept(true) requires(Speculations_::text<Text>()) {
    auto text_ = parent_::message::text::driver::make(
        ::std::forward<Text>(text)
    );
    auto &&message_ = Message{
        .text = parent_::message::text::make(text_), .level = level
    };
    if (
        Message::Level::Info < level
    ) message_.location.emplace(::std::move(location));
    write(::std::move(message_));
}

template <class Text, class Location> inline auto this_::Type::write(
    Message::Level level, Text &&text, Location &&location
) const noexcept(true) requires(
    Speculations_::text<Text>() && Speculations_::location<Location>()
) {
    write(Message{.text = parent_::message::text::make(
        parent_::message::text::driver::make(::std::forward<Text>(text))
    ), .level = level, .location = ::std::forward<Location>(location)});
}

template <class Text> inline auto this_::Type::write(
    Text &&text, Message::Level level,
    FinalToken_ &&, Message::Location &&location
) const noexcept(true) requires(Speculations_::text<Text>()) {
    auto text_ = parent_::message::text::driver::make(
        ::std::forward<Text>(text)
    );
    auto &&message_ = Message{
        .text = parent_::message::text::make(text_), .level = level};
    if (
        Message::Level::Info < level
    ) message_.location.emplace(::std::move(location));
    write(::std::move(message_));
}

template <class Text, class Location> inline auto this_::Type::write(
    Text &&text, Message::Level level, Location &&location
) const noexcept(true) requires(
    Speculations_::text<Text>() && Speculations_::location<Location>()
) {
    write(Message{.text = parent_::message::text::make(
        parent_::message::text::driver::make(::std::forward<Text>(text))
    ), .level = level, .location = ::std::forward<Location>(location)});
}

} // namespace p5::lambda::utils::log::interface
