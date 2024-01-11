#define BOOST_TEST_MODULE utils.generator
#include <boost/test/unit_test.hpp>

#include <cstdint>

#include <variant>
#include <type_traits>

#include <p5/lambda/utils/generator.hxx>


BOOST_AUTO_TEST_CASE(function_case) {
    constexpr static auto const size_ = static_cast<::std::size_t>(64);
    constexpr static auto const first_ = static_cast<
        ::std::decay_t<decltype(size_)>
    >(0);
    auto counter_ = static_cast<::std::decay_t<decltype(size_)>>(0);
    namespace generator_ = ::p5::lambda::utils::generator::function;
    for (auto &generated_: generator_::make([state_ = ::std::variant<
        ::std::monostate, ::std::decay_t<decltype(size_)>, ::std::nullptr_t
    > {}] () mutable -> ::std::decay_t<decltype(size_)> * {
        switch (state_.index()) {
        default: break;
        case 0:
            if (size_ > first_) {
                state_.emplace<1>(first_);
                return &(::std::get<1>(state_));
            }
            state_.emplace<2>(nullptr);
            break;
        case 1:
            if constexpr (true) {
                auto &value_ = ++::std::get<1>(state_);
                if (size_ > value_) return &value_;
            }
            state_.emplace<2>(nullptr);
            break;
        }
        return nullptr;
    })) {
        BOOST_REQUIRE_EQUAL(counter_, generated_);
        BOOST_REQUIRE_LT(counter_, size_);
        counter_ += 2; ++generated_;
    }
    BOOST_REQUIRE_EQUAL(size_, counter_);
}

BOOST_AUTO_TEST_CASE(coroutine_case) {
    constexpr static auto const size_ = static_cast<::std::size_t>(64);
    constexpr static auto const first_ = static_cast<
        ::std::decay_t<decltype(size_)>
    >(0);
    auto counter_ = static_cast<::std::decay_t<decltype(size_)>>(0);
    using Generator_ = ::p5::lambda::utils::generator::Coroutine<
        ::std::decay_t<decltype(size_)>
    >;
    for (auto &generated_: [] () -> Generator_ {
        for (auto value_ = first_; size_ > value_; ++value_) co_yield value_;
    } ()) {
        BOOST_REQUIRE_EQUAL(counter_, generated_);
        BOOST_REQUIRE_LT(counter_, size_);
        counter_ += 2; ++generated_;
    }
    BOOST_REQUIRE_EQUAL(size_, counter_);
}
