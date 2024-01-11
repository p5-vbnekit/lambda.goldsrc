#define BOOST_TEST_MODULE utils.container
#include <boost/test/unit_test.hpp>

#include <variant>
#include <exception>
#include <stdexcept>
#include <string_view>
#include <type_traits>

#include <p5/lambda/utils/container.hxx>
#include <p5/lambda/utils/async/task.hxx>


namespace target_ = ::p5::lambda::utils::container;
static_assert(::std::is_same_v<target_::Type, ::p5::lambda::utils::Container>);

BOOST_AUTO_TEST_SUITE(container_suite)

BOOST_AUTO_TEST_CASE(main_case) {
    auto container_ = target_::Type{};
    BOOST_CHECK(static_cast<bool>(container_.linked()));

    struct Payload_ final {
        inline Payload_() noexcept(true) = default;
        inline explicit Payload_(bool &flag) noexcept(true): flag_{&flag} {}
        inline ~Payload_() noexcept(true) { if (flag_) *flag_ = true; }

        Payload_(Payload_ &&) noexcept(true) = default;
        Payload_(Payload_ const &) noexcept(true) = default;
        Payload_ & operator = (Payload_ &&) noexcept(true) = default;
        Payload_ & operator = (Payload_ const &) noexcept(true) = default;

    private:
        bool *flag_ = nullptr;
    };

    auto payload_ = Payload_{};

    auto variant_ = ::std::variant<Payload_ const *, ::std::exception_ptr>{};

    auto const pointer_ = container_.get<Payload_ const>();
    BOOST_CHECK(! static_cast<bool>(pointer_));
    BOOST_CHECK([&] {
        try { pointer_.resolve(); } catch (
            ::std::logic_error const &exception
        ) {
            auto const * const what_ = exception.what();
            if (what_ && (
                "empty pointer" == ::std::string_view{what_}
            )) return true;
        } catch (...) {}
        return false;
    } ());
    BOOST_CHECK_NO_THROW({
        BOOST_CHECK(pointer_.subscribe([&] {
            try { variant_.template emplace<0>(
                pointer_ ? &(pointer_.resolve()) : nullptr
            ); } catch (...) {
                variant_.template emplace<1>(::std::current_exception());
            }
        }).pin());
    });

    if constexpr (true) {
        auto const task_ = [&pointer_] () -> target_::parent_::async::Task<
            Payload_ const *
        > { co_return &(co_await pointer_); } ();
        task_.start();
        using FutureState_ = ::std::decay_t<decltype(task_.future().state())>;
        BOOST_CHECK(FutureState_::Initial == task_.future().state());

        auto lock_ = container_.assign(payload_);
        BOOST_CHECK(lock_.linked());
        BOOST_CHECK(! lock_.pinned());
        BOOST_CHECK(static_cast<bool>(pointer_));
        BOOST_CHECK_EQUAL(&payload_, pointer_.address());
        BOOST_CHECK_EQUAL(&payload_, ::std::get<0>(variant_));
        BOOST_CHECK(FutureState_::Ready == task_.future().state());
        BOOST_CHECK_EQUAL(
            &payload_, *(task_.future().get(::std::rethrow_exception))
        );
        lock_.unlink();
        BOOST_CHECK(! lock_.linked());
        BOOST_CHECK(! static_cast<bool>(pointer_));
        BOOST_CHECK_EQUAL(nullptr, pointer_.address());
        BOOST_CHECK_EQUAL(nullptr, ::std::get<0>(variant_));
        lock_ = container_.assign(payload_);
        BOOST_CHECK(lock_.linked());
        BOOST_CHECK(static_cast<bool>(pointer_));
        lock_.release();
        BOOST_CHECK(! lock_.linked());
        BOOST_CHECK(! static_cast<bool>(pointer_));
        lock_ = container_.emplace(payload_);
        BOOST_CHECK(lock_.linked());
        BOOST_CHECK(static_cast<bool>(pointer_));
        BOOST_CHECK(static_cast<bool>(pointer_.address()));
        BOOST_CHECK(static_cast<bool>(::std::get<0>(variant_)));
        lock_ = {};
        auto flag_ = false;
        BOOST_CHECK(! lock_.linked());
        BOOST_CHECK(! static_cast<bool>(pointer_));
        container_.emplace(Payload_{flag_});
        BOOST_CHECK(flag_);
        BOOST_CHECK(! static_cast<bool>(pointer_));
        BOOST_CHECK(! static_cast<bool>(pointer_.address()));
        BOOST_CHECK(! static_cast<bool>(::std::get<0>(variant_)));
        flag_ = false;
        container_.emplace<Payload_>(flag_);
        BOOST_CHECK(flag_);
    };

    BOOST_CHECK_EQUAL(nullptr, pointer_.address());
    BOOST_CHECK(! static_cast<bool>(pointer_));
    BOOST_CHECK_EQUAL(nullptr, ::std::get<0>(variant_));

    BOOST_CHECK(container_.assign<Payload_ const>(payload_).pin());
    BOOST_CHECK([&] {
        try { container_.assign(payload_); } catch (
            ::std::logic_error const &exception
        ) {
            auto const * const what_ = exception.what();
            if (what_ && (
                "resource busy" == ::std::string_view{what_}
            )) return true;
        } catch (...) {}
        return false;
    } ());
    BOOST_CHECK(static_cast<bool>(pointer_));
    BOOST_CHECK_EQUAL(&payload_, pointer_.address());
    BOOST_CHECK_EQUAL(&payload_, &(pointer_.resolve()));
    BOOST_CHECK_EQUAL(&payload_, ::std::get<0>(variant_));

    BOOST_CHECK_NO_THROW({
        auto const pointer_ = container_.get<Payload_>();
        BOOST_CHECK(pointer_.linked());
        BOOST_CHECK(! static_cast<bool>(pointer_));
        BOOST_CHECK(! pointer_.check());
        BOOST_CHECK(! pointer_.check(true));
        BOOST_CHECK(pointer_.check(false));
        BOOST_CHECK([&] {
            try { pointer_.resolve(); } catch (
                ::std::invalid_argument const &exception
            ) {
                auto const * const what_ = exception.what();
                if (what_ && (
                    "unsatisfied const qualifier" == ::std::string_view{what_}
                )) return true;
            } catch (...) {}
            return false;
        } ());
    });

    container_.unlink();
    BOOST_CHECK(! static_cast<bool>(container_.linked()));
    BOOST_CHECK(! static_cast<bool>(pointer_));
    BOOST_CHECK(! static_cast<bool>(pointer_.address()));
    BOOST_CHECK(! static_cast<bool>(::std::get<0>(variant_)));
}

BOOST_AUTO_TEST_SUITE_END()
