#pragma once

#include <string>
#include <utility>
#include <variant>
#include <sstream>
#include <type_traits>

#include "chunk.hxx"
#include "driver.hxx"


namespace p5::lambda::utils::log::message::text::driver {
namespace private_ {

namespace parent_ = this_;
namespace this_ = private_;

template <class T> inline constexpr static
auto ensure_chunk_(T &&source) noexcept(true) requires(::std::is_base_of_v<
    parent_::parent_::Chunk, ::std::decay_t<T>
> || requires () {
    parent_::parent_::chunk::make(::std::declval<T>());
}) {
    if constexpr (
        ::std::is_base_of_v<parent_::parent_::Chunk, ::std::decay_t<T>>
    ) return ::std::forward<T>(source);
    else return parent_::parent_::chunk::make(::std::forward<T>(source));
}

namespace introspection_ {

namespace parent_ = this_;
namespace this_ = introspection_;

namespace sfinae {

namespace parent_ = this_;
namespace this_ = sfinae;

template <class, class = void> struct EnsureChunk final: ::std::false_type {};
template <class T> struct EnsureChunk<T, ::std::void_t<
    decltype(parent_::parent_::ensure_chunk_(::std::declval<T>()))
>> final: ::std::true_type {};

template <class T> inline constexpr static auto ensure_chunk() noexcept(true) {
    return this_::EnsureChunk<T>::value;
}

} // namespace sfinae
} // namespace introspection_

template <class T> inline static auto to_string_(T &&value) noexcept(false) {
    if constexpr (requires() {
        ::std::to_string(::std::declval<T>());
    }) return ::std::to_string(::std::forward<T>(value));
    else {
        ::std::ostringstream stream_;
        stream_ << ::std::forward<T>(value);
        return stream_.str();
    }
}

template <class T> inline constexpr static
auto make_delegate_(T &&source) noexcept(false) {
    if constexpr (
        this_::introspection_::sfinae::ensure_chunk<T>()
    ) return [done_ = false, holder_ = [&source] {
        if constexpr (::std::is_base_of_v<::std::string, T>) {
            struct Result_ final {
                parent_::Chunk value;
                ::std::string strong;
            };
            return Result_{.value = {}, .strong = ::std::forward<T>(source)};
        } else {
            struct Result_ final { parent_::Chunk value; };
            return Result_{.value = this_::ensure_chunk_(
                ::std::forward<T>(source)
            )};
        }
    } ()] () mutable -> parent_::Chunk const * {
        if (done_) return nullptr;
        if constexpr (
            requires () { holder_.strong; }
        ) holder_.value = parent_::parent_::chunk::make(holder_.strong);
        done_ = true; return &(holder_.value);
    };
    else if constexpr (requires () {{
        ::std::end(source) != ::std::begin(source)
    } -> ::std::convertible_to<bool>; }) return [context_ = [&source] {
        struct State_ final {
        parent_::Chunk value = parent_::parent_::chunk::make();
        ::std::decay_t<decltype(::std::begin(source))> iterator;
        ::std::decay_t<decltype(::std::end(source))> const sentinel;
    };
        struct Result_ final {
        ::std::variant<
            ::std::monostate, State_, ::std::nullptr_t
        > state = {};
        T source;
    };
        return Result_{.source = ::std::forward<T>(source)};
    } ()] () mutable -> parent_::Chunk const * {
        using State_ = ::std::decay_t<decltype(::std::get<1>(context_.state))>;
        switch (context_.state.index()) {
        default: break;
        case 0:
            try {
                context_.state.template emplace<1>(State_{
                    .iterator = ::std::begin(context_.source),
                    .sentinel = ::std::end(context_.source)
                });
                auto &state_ = ::std::get<1>(context_.state);
                if (state_.sentinel != state_.iterator) return &(
                    state_.value = this_::ensure_chunk_(*(state_.iterator))
                );
            }
            catch (...) {}
            context_.state.template emplace<2>(nullptr);
            break;
        case 1:
            try {
                auto &state_ = ::std::get<1>(context_.state);
                ++(state_.iterator);
                if (state_.sentinel != state_.iterator) return &(
                    state_.value = this_::ensure_chunk_(*(state_.iterator))
                );
            }
            catch (...) {}
            context_.state.template emplace<2>(nullptr);
            break;
        }
        return nullptr;
    };
    else return [context_ = [&source] {
        struct Result_ final {
            ::std::string strong = {};
            parent_::Chunk value = {};
            bool done = true;
        } result_;
        try {
            result_.strong = this_::to_string_(::std::forward<T>(source));
            result_.done = false;
        }
        catch (...) {}
        return result_;
    } ()] () mutable -> parent_::Chunk const * {
        if (context_.done) return nullptr;
        context_.value = parent_::parent_::chunk::make(context_.strong);
        context_.done = true; return &(context_.value);
    };
}

} // namespace private_

template <class T> inline
this_::Chunk const * this_::Type<T>::operator () () noexcept(true) {
    if (done_) return nullptr;
    try {
        auto const * const chunk_ = delegate_();
        if (chunk_) return chunk_;
    } catch (...) {}
    done_ = true;
    return nullptr;
}

template <class T> inline constexpr
this_::Type<T>::Type(T &&delegate) noexcept(true):
    delegate_{::std::move(delegate)}
{}

template <class T> inline constexpr
this_::Type<T>::Type(Type &&other) noexcept(true):
    done_(other.done_), delegate_(::std::move(other.delegate_))
{
    other.done_ = true;
}

template <class T> inline constexpr
this_::Type<T> & this_::Type<T>::operator = (Type &&other) noexcept(true) {
    if (this != &other) {
        done_ = other.done_;
        other.done_ = true;
        delegate_ = ::std::move(other.delegate_);
    }
    return *this;
}

template <class T> auto make(T &&source) noexcept(false) {
    auto &&source_ = this_::private_::make_delegate_(::std::forward<T>(source));
    return this_::Type<::std::decay_t<decltype(source_)>>{::std::move(source_)};
}

} // namespace p5::lambda::utils::log::message::text::driver
