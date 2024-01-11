#pragma once

#include <utility>
#include <stdexcept>
#include <type_traits>

#include "raii.hxx"


namespace p5::lambda::utils::log::raii {
namespace item {

template <class Text> inline auto
this_::Type<Text>::Machine_::state() const noexcept(true) { return state_; }

template <class Text> inline parent_::text::Chunk const *
this_::Type<Text>::Machine_::operator () () noexcept(true) {
    switch (state_) {
    case State::Initial: state_ = State::First; [[fallthrough]];
    case State::First: if constexpr (true) {
        auto const * const pointer_ = first_();
        if (pointer_) return pointer_;
        if (next) state_ = State::Next;
        else { state_ = State::Final; break; }
    }; [[fallthrough]];
    case State::Next: if constexpr (true) {
        auto const * const pointer_ = (*next)();
        if (pointer_) return pointer_;
        state_ = State::Final;
    }; [[fallthrough]];
    default: break;
    }
    return nullptr;
}

template <class Text> inline
this_::Type<Text>::Machine_::Machine_(Text &&text) noexcept(true):
    first_{::std::move(text)}
{}

template <class Text> inline this_::Type<Text>::Type::~Type() noexcept(true) {
    if (! machine_.next) parent_::context::dispatch(context_);
}

template <class Text> inline this_::Type<Text>::Type(
    Text &&text, parent_::Context &context,
    parent_::context::Text **pointer
) noexcept(false): machine_{::std::move(text)}, context_{context} {
    if (! pointer) throw ::std::invalid_argument{"empty pointer"};
    if (*pointer) throw ::std::invalid_argument{"busy pointer"};
    if (! context_.target) throw ::std::invalid_argument{
        "empty context target"
    };
    *pointer = &machine_;
}

template <class Text> template <class T> inline
auto this_::Type<Text>::operator << (T &&text) noexcept(false) {
    if (! context_.text) throw ::std::logic_error{
        "invalid context state: empty chain"
    };
    if (! context_.target) throw ::std::logic_error{
        "invalid context state: empty target"
    };
    if (! context_.target) throw ::std::logic_error{
        "invalid machine state: busy next"
    };
    if (! context_.target) throw ::std::logic_error{
        "invalid machine state: not in `initial`"
    };
    auto &&text_ = parent_::text::driver::make(::std::forward<T>(text));
    return this_::Type<::std::decay_t<decltype(text_)>>{
        ::std::move(text_), context_, &machine_.next
    };
}

} // namespace item

template <class T> inline
auto this_::Type::operator << (T &&text) noexcept(false) {
    if (context_.text) throw ::std::logic_error{"invalid state"};
    if (! context_.target) throw ::std::logic_error{"invalid state"};
    auto &&text_ = this_::text::driver::make(::std::forward<T>(text));
    return this_::Item<::std::decay_t<decltype(text_)>>{
        ::std::move(text_), context_, &context_.text
    };
}

} // namespace p5::lambda::utils::log::raii
