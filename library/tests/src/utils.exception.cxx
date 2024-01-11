#define BOOST_TEST_MODULE utils.exception
#include <boost/test/unit_test.hpp>

#include <list>
#include <utility>
#include <exception>
#include <stdexcept>
#include <string_view>
#include <type_traits>

#include <p5/lambda/utils/exception.hxx>


namespace this_ {

namespace target = ::p5::lambda::utils::exception;

inline static auto compare(
    ::std::exception_ptr const &a,
    ::std::exception_ptr const &b
) noexcept(true) {
    if (&a == &b) return true;
    if (! a) return ! b;
    else if (! b) return false;
#ifdef _MSC_VER
    if (target::next(a)) {
        if (! target::next(b)) return false;
        ::std::exception const *a_ = nullptr, *b_ = nullptr;
        try { ::std::rethrow_exception(a); }
        catch (::std::exception const &caught) { a_ = &caught; } catch (...) {}
        try { ::std::rethrow_exception(b); }
        catch (::std::exception const &caught) { b_ = &caught; } catch (...) {}
        return a_ == b_;
    }
    if (target::next(b)) return false;
#endif
    return a == b;
}

inline static auto collect() noexcept(true) {
    auto const first_ = ::std::current_exception();
    auto list_ = ::std::list<::std::decay_t<decltype(first_)>>{};
    for (auto &exception_: target::iterate()) list_.push_back(
        ::std::move(exception_)
    );
    if (first_) {
        BOOST_TEST_INFO_SCOPE("First = " << target::details(first_));
        BOOST_CHECK(! list_.empty());
        if (! list_.empty()) {
            BOOST_TEST_INFO_SCOPE("Front = " << target::details(list_.front()));
            BOOST_CHECK(compare(first_, list_.front()));
        }
    }
    return list_;
}

inline static auto collect(::std::exception_ptr const &first) noexcept(true) {
    auto list_ = ::std::list<::std::decay_t<decltype(first)>>{};
    for (auto &exception_: target::iterate(first)) list_.push_back(
        ::std::move(exception_)
    );
    if (first) {
        BOOST_TEST_INFO_SCOPE("First = " << target::details(first));
        BOOST_CHECK(! list_.empty());
        if (! list_.empty()) {
            BOOST_TEST_INFO_SCOPE("Front = " << target::details(list_.front()));
            BOOST_CHECK(compare(first, list_.front()));
        }
    }
    return list_;
}

} // namespace this_

BOOST_AUTO_TEST_CASE(main_case) {
    BOOST_CHECK(! static_cast<bool>(::this_::target::next()));
    auto exceptions_ = ::this_::collect();
    BOOST_CHECK(exceptions_.empty());

    auto exception_ = ::std::current_exception();
    BOOST_CHECK(! static_cast<bool>(::this_::target::next(exception_)));
    exceptions_ = ::this_::collect(exception_);
    BOOST_CHECK(exceptions_.empty());

    [&exception_, &exceptions_] {
        exception_ = ::std::make_exception_ptr(::std::runtime_error{"Freeman"});
        BOOST_TEST_INFO_SCOPE(
            "Expected = " << ::this_::target::details(exception_)
        );
        BOOST_CHECK(! static_cast<bool>(::this_::target::next(exception_)));
        exceptions_ = ::this_::collect(exception_);
        BOOST_CHECK(! exceptions_.empty());
        if (exceptions_.empty()) return;
        BOOST_TEST_INFO(
            "Received = " << ::this_::target::details(exceptions_.front())
        );
        BOOST_CHECK(this_::compare(exception_, exceptions_.front()));
        exceptions_.pop_front();
        BOOST_CHECK(exceptions_.empty());
    } ();

    try { ::std::rethrow_exception(exception_); }
    catch (...) {
        try { ::std::throw_with_nested(::std::logic_error{"Gordon"}); }
        catch (...) { exceptions_ = ::this_::collect(); }
    }

    BOOST_CHECK(! exceptions_.empty());
    if (exceptions_.empty()) return;

    [&exceptions_] {
        auto const exception_ = ::this_::target::next(exceptions_.front());
        BOOST_CHECK(static_cast<bool>(exception_));
        BOOST_CHECK(static_cast<bool>(! ::this_::target::next(exception_)));
    } ();

    BOOST_CHECK(! exceptions_.empty());
    if (exceptions_.empty()) return;

    auto const check_current_ = [&exceptions_] (
        ::std::string_view const &expected
    ) {
        try { ::std::rethrow_exception(exceptions_.front()); }
        catch (::std::exception const &caught_) {
            BOOST_TEST_INFO("Expected = " << expected);
            auto const * const what_ = caught_.what();
            auto const received_ = [what_] () -> ::std::decay_t<
                decltype(expected)
            > {
                if (what_) return what_;
                return {};
            } ();
            BOOST_TEST_INFO_SCOPE("Received = " << received_);
            BOOST_CHECK(static_cast<bool>(what_));
            BOOST_CHECK_EQUAL(expected, received_);
            return;
        }
        throw ::std::logic_error{"unexpected state"};
    };

    BOOST_CHECK_NO_THROW(check_current_("Gordon"));

    exceptions_.pop_front();
    BOOST_CHECK(! exceptions_.empty());
    if (exceptions_.empty()) return;

    BOOST_CHECK_NO_THROW(check_current_("Freeman"));

    exceptions_.pop_front();
    BOOST_CHECK(exceptions_.empty());
}
