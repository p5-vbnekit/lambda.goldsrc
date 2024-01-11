#pragma once

#include <utility>
#include <type_traits>

#include "../../../generator.hxx"

#include "type.hxx"
#include "normalizer.hxx"


namespace p5::lambda::utils::log::message::text::normalizer {
namespace driver {

template <class T> inline
this_::Chunk const * this_::Type<T>::Adapter_::operator () () noexcept(true) {
    if constexpr (::std::is_pointer_v<::std::decay_t<decltype(source_)>>) {
        if (source_) {
            auto const * const value_ = (*source_)();
            if (value_) return value_;
            source_ = nullptr;
        }
        return nullptr;
    }
    else return source_();
}

template <class T> inline
this_::Type<T>::Adapter_::Adapter_(Adapter_ &&other) noexcept(true):
    source_{[&source_ = other.source_] () -> decltype(auto) {
        if constexpr (::std::is_pointer_v<
            ::std::decay_t<decltype(source_)>
        >) return ::std::exchange(source_, nullptr);
        else return ::std::move(source_);
    } ()}
{}

template <class T> inline
this_::Type<T>::Adapter_::Adapter_(Source &&source) noexcept(true):
    source_{::std::move(source)}
{}

template <class T> inline
this_::Chunk const * this_::Type<T>::operator() () noexcept(true) {
    return implementation_();
}

template <class T> inline this_::Type<T>::Type(Source &&source) noexcept(true):
    adapter_{::std::move(source)}
{}

template <class T> inline constexpr auto make(T &&source) noexcept(true) {
    using Source_ = ::std::remove_reference_t<T>;
    if constexpr (::std::is_pointer_v<Source_>) {
        using Type_ = ::std::remove_reference_t<
            decltype(*::std::declval<Source_>())
        >;
        if constexpr (::std::is_base_of_v<
            this_::Interface, Type_
        >) return this_::Type<this_::Interface *>{::std::forward<T>(source)};
        else return this_::Type<Source_>{::std::forward<T>(source)};
    }
    else if constexpr (::std::is_reference_v<T>) {
        if constexpr (::std::is_base_of_v<
            this_::Interface, Source_
        >) return this_::Type<this_::Interface *>{&source};
        else return this_::Type<Source_ *>{&source};
    }
    else return this_::Type<Source_>{::std::forward<T>(source)};
}

} // namespace driver

template <class T> inline constexpr auto make(T &&source) noexcept(true) {
    namespace generator_ = parent_::parent_::parent_::parent_::
        generator::function;
    return generator_::make(this_::driver::make([&source] () -> decltype(auto) {
        if constexpr (::std::is_base_of_v<
            parent_::Type, ::std::decay_t<T>
        >) return source();
        else return ::std::forward<T>(source);
    } ()));
}

} // namespace p5::lambda::utils::log::message::text::normalizer
