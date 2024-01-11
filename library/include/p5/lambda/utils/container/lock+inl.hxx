#pragma once

#include <cstddef>

#include <memory>
#include <variant>
#include <utility>
#include <stdexcept>
#include <functional>
#include <type_traits>

#include "lock.hxx"


namespace p5::lambda::utils::container::lock {
namespace driver {
namespace holder {

namespace parent_ = this_;
namespace this_ = parent_::holder;

template <class = void> struct Type;

template <> struct Type<void> {
    void * const pointer;

    virtual ~Type() noexcept(true);

protected:
    Type(void * = nullptr) noexcept(true);

private:
    Type(Type &&) = delete;
    Type(Type const &) = delete;
    Type & operator = (Type &&) = delete;
    Type & operator = (Type const &) = delete;
};

template <class Instance> struct Type final: Type<void> {
    static_assert(::std::is_class_v<Instance>);

    template <class ... T> inline explicit Type(
        ::std::nullptr_t const &, T && ... payload
    ) noexcept(
        ::std::is_nothrow_constructible_v<Instance, T && ...>
    ):
        Type<void>{(void *)&instance_},
        instance_{::std::forward<T>(payload) ...}
    {}

private:
    Instance instance_;
};

} // namespace holder

template <class T> using Holder = holder::Type<T>;

struct Type {
    using Holder = this_::Holder<void>;

    virtual void put(::std::variant<
        void *, ::std::unique_ptr<Holder>,
        ::std::function<void *(void)>,
        ::std::function<::std::unique_ptr<Holder>(void)>
    > &&) noexcept(false) = 0;

    virtual void take() noexcept(true) = 0;

    virtual ~Type() noexcept(true);

protected:
    Type() noexcept(true) = default;
    Type(Type &&) noexcept(true) = default;
    Type & operator = (Type &&) noexcept(true) = default;

private:
    Type(Type const &) = delete;
    Type & operator = (Type const &) = delete;
};

} // namespace driver

template <class Interface> inline
auto && this_::Type<Interface>::take() noexcept(true) {
    if (driver_) driver_->take();
    return ::std::move(*this);
}

template <class Interface> inline
auto this_::Type<Interface>::release() noexcept(true) {
    if (! driver_) return;
    driver_->take();
    driver_.reset();
}

template <class Interface> template <class Factory> inline
auto this_::Type<Interface>::inject(Factory &&factory) noexcept(false) {
    if (! driver_) throw ::std::logic_error{"expired"};
    return driver_->put([
        factory_ = ::std::forward<Factory>(factory)
    ] () mutable {
        auto &&result_ = factory_();

        using Result_ = decltype(result_);
        using WithoutReference_ = ::std::remove_reference_t<Result_>;
        using Instance_ = ::std::remove_pointer_t<WithoutReference_>;

        static_assert(::std::is_base_of_v<Interface, Instance_>);
        static_assert(::std::is_convertible_v<Instance_ *, Interface *>);

        if constexpr(
            ::std::is_pointer_v<WithoutReference_>
        ) return (void *)result_;

        else if constexpr(
            ::std::is_lvalue_reference_v<Result_>
        ) return (void *)(&result_);

        else return ::std::make_unique<this_::driver::Holder<
            ::std::remove_reference_t<decltype(factory_())>>
        >(nullptr, ::std::move(result_));
    });
}

template <class Interface> template <class Instance, class Factory> inline
auto this_::Type<Interface>::inject(
    ::std::in_place_t const &, Factory &&factory
) noexcept(false) {
    static_assert(::std::is_base_of_v<Interface, Instance>);
    static_assert(::std::is_convertible_v<Instance *, Interface *>);
    if (! driver_) throw ::std::logic_error{"expired"};
    return driver_->put([
        factory_ = ::std::forward<Factory>(factory)
    ] () mutable { return ::std::apply([] (auto && ... payload) {
        return ::std::make_unique<this_::driver::Holder<Instance>>(
            nullptr, ::std::forward<decltype(payload)>(payload) ...
        );
    }, factory_()); });
}

template <class Interface> template <class Instance, class Factory> inline
auto this_::Type<Interface>::inject(
    ::std::in_place_type_t<Instance> const &, Factory &&factory
) noexcept(false) {
    return this->inject<Instance>(
        ::std::in_place_t{}, ::std::forward<Factory>(factory)
    );
}

template <class Interface> template <class Instance> inline
auto this_::Type<Interface>::assign(Instance *instance) noexcept(false) {
    static_assert(::std::is_base_of_v<Interface, Instance>);
    static_assert(::std::is_convertible_v<Instance *, Interface *>);
    if (! driver_) throw ::std::logic_error{"expired"};
    driver_->put((void *)instance);
}

template <class Interface> template <class Instance> inline
auto this_::Type<Interface>::assign(Instance &instance) noexcept(false) {
    return this->assign(&instance);
}

template <class Interface> template <class Instance, class ... T> inline
auto this_::Type<Interface>::emplace(T && ... payload) noexcept(false) {
    static_assert(::std::is_base_of_v<Interface, Instance>);
    static_assert(::std::is_convertible_v<Instance *, Interface *>);
    if (! driver_) throw ::std::logic_error{"expired"};
    driver_->put(::std::make_unique<this_::driver::Holder<Instance>>(
        nullptr, ::std::forward<T>(payload) ...
    ));
}

template <class Interface> inline
this_::Type<Interface>::Type(::std::unique_ptr<Driver> &&driver) noexcept(true):
    driver_{::std::move(driver)}
{}

} // namespace p5::lambda::utils::container::lock
