#define BOOST_TEST_MODULE utils.log
#include <boost/test/unit_test.hpp>

#include <cstdint>

#include <list>
#include <array>
#include <string>
#include <random>
#include <utility>
#include <optional>
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <string_view>

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include <p5/lambda/utils.hxx>


namespace this_ {
namespace utils {

namespace target = ::p5::lambda::utils;

template <class Type, class ... T> inline constexpr static
auto make_array(T && ... source) noexcept(true) {
    return ::std::array<Type, sizeof ... (T)>{::std::forward<T>(source) ...};
}

namespace location {

using Type = target::log::message::Location;

inline static auto const & state() noexcept(true) {
    static auto const instance_ = [] {
        auto state_ = target::log::message::location::inspect();
        BOOST_CHECK(::std::decay_t<decltype(state_.file)>::Yes == state_.file);
        BOOST_CHECK(::std::decay_t<decltype(state_.line)>::Yes == state_.line);
#if defined(__clang__)
        BOOST_CHECK(::std::decay_t<
            decltype(state_.column)
        >::No >= state_.column);
#else
        BOOST_CHECK(::std::decay_t<
            decltype(state_.column)
        >::No > state_.column);
#endif
        BOOST_CHECK(::std::decay_t<
            decltype(state_.function)
        >::Yes == state_.function);

        static_assert(::std::is_same_v<
            ::p5::lambda::utils::log::message::Location, Type
        >);
        auto const value_ = (
            ::p5::lambda::utils::log::message::Location::current()
        );
        constexpr auto const line_ = static_cast<
            ::std::decay_t<decltype(value_.line)>
        >(59);
        constexpr auto const at_begin_ = static_cast<
            ::std::decay_t<decltype(value_.column)>
        >(13);
        constexpr auto const at_absolute_ = 2 + at_begin_;
        constexpr auto const at_relative_ = 43 + at_absolute_;
        constexpr auto const at_parentheses_ = 7 + at_relative_;

        switch(state_.file) {
        default: break;
        case ::std::decay_t<decltype(state_.file)>::Yes:
            BOOST_CHECK(! value_.file.empty());
            if (value_.file.empty()) state_.file = ::std::decay_t<
                decltype(state_.file)
            >::Invalid;
            break;
        case ::std::decay_t<decltype(state_.file)>::No:
            BOOST_CHECK(value_.file.empty());
            if (! value_.file.empty()) state_.file = ::std::decay_t<
                decltype(state_.file)
            >::Invalid;
            break;
        }

        switch(state_.line) {
        default: break;
        case ::std::decay_t<decltype(state_.line)>::Yes:
            BOOST_CHECK_EQUAL(value_.line, line_);
            if (line_ != value_.line) state_.line = ::std::decay_t<
                decltype(state_.line)
            >::Invalid;
            break;
        case ::std::decay_t<decltype(state_.line)>::No:
            BOOST_CHECK_EQUAL(value_.line, 0);
            if (0 != value_.line) state_.line = ::std::decay_t<
                decltype(state_.line)
            >::Invalid;
            break;
        }

        switch(state_.column) {
        default: break;
        case ::std::decay_t<decltype(state_.column)>::Full:
            BOOST_CHECK_EQUAL(value_.column, at_begin_);
            if (
                at_begin_ != value_.column
            ) state_.column = ::std::decay_t<decltype(state_.column)>::Invalid;
            break;
        case ::std::decay_t<decltype(state_.column)>::Absolute:
            BOOST_CHECK_EQUAL(value_.column, at_absolute_);
            if (
                at_absolute_ != value_.column
            ) state_.column = ::std::decay_t<decltype(state_.column)>::Invalid;
            break;
        case ::std::decay_t<decltype(state_.column)>::Relative:
            BOOST_CHECK_EQUAL(value_.column, at_relative_);
            if (
                at_relative_ != value_.column
            ) state_.column = ::std::decay_t<decltype(state_.column)>::Invalid;
            break;
        case ::std::decay_t<decltype(state_.column)>::Parentheses:
            BOOST_CHECK_EQUAL(value_.column, at_parentheses_);
            if (
                at_parentheses_ != value_.column
            ) state_.column = ::std::decay_t<decltype(state_.column)>::Invalid;
            break;
        case ::std::decay_t<decltype(state_.column)>::No:
            if (
                0 != value_.column
            ) state_.column = ::std::decay_t<decltype(state_.column)>::Invalid;
            break;
        }

        switch(state_.function) {
        default: break;
        case ::std::decay_t<decltype(state_.function)>::Yes:
            BOOST_CHECK(! value_.function.empty());
            if (value_.function.empty()) state_.function = ::std::decay_t<
                decltype(state_.function)
            >::Invalid;
            break;
        case ::std::decay_t<decltype(state_.function)>::No:
            BOOST_CHECK(value_.function.empty());
            break;
        }

        return state_;
    } ();
    return instance_;
}

inline static auto make(
    Type const &source = Type::current()
) noexcept(true) {
    auto const &state_ = state();
    auto result_ = source;

    if (::std::decay_t<
        decltype(state_.file)
    >::Yes != state_.file) result_.file = {};

    if (::std::decay_t<
        decltype(state_.line)
    >::Yes > state_.line) result_.line = 0;

    if (! (::std::decay_t<
        decltype(state_.column)
    >::No > state_.column)) result_.column = 0;

    if (::std::decay_t<
        decltype(state_.function)
    >::Yes != state_.function) result_.function = {};

    return result_;
}

inline static auto & fix_column(
    Type &value,
    ::std::decay_t<decltype(::std::declval<Type>().column)> absolute,
    ::std::decay_t<decltype(::std::declval<Type>().column)> relative = 0,
    bool double_colon_at_begin = true
) noexcept(true) {
    if (! (0 < value.column)) { value.column = 0; return value; }
    if (! (relative <= absolute)) return value;
    if (! (static_cast<
        ::std::decay_t<decltype(absolute)>
    >(double_colon_at_begin ? 2 : 0) < absolute)) return value;
    auto const &state_ = state().column;
    switch(state_) {
    default:
        value.column = 0;
        break;
    case ::std::decay_t<decltype(state_)>::Full: break;
    case ::std::decay_t<decltype(state_)>::Absolute:
        if (double_colon_at_begin) {
            if (1 < value.column) value.column -= 2;
            else value.column = 0;
        }
        break;
    case ::std::decay_t<decltype(state_)>::Relative:
        absolute -= relative;
        if (absolute <= value.column) value.column -= absolute;
        else value.column = 0;
        break;
    case ::std::decay_t<decltype(state_)>::Parentheses:
        if (absolute <= value.column) value.column -= absolute;
        else value.column = 0;
        break;
    }
    return value;
}

} // namespace location

using Location = location::Type;

namespace speech {

inline constexpr static auto paragraphs() noexcept(true) {
    return [] (auto const * ... source) {
        return make_array<::std::string_view>(source ...);
    } (
"Gordon Freeman in the flesh. Or rather, in the hazard suit.\n"
"I took the liberty of relieving you of your weapons; most of them were government property.\n"
"As for the suit, I think you've earned it.",

"The borderworld, Xen, is in our control for the time being, thanks to you.\n"
"Quite a nasty piece of work you managed over there. I am impressed.",

"That's why I'm here, Mr. Freeman.\n"
"I have recommended your services to my employers, and they have authorized me to offer you a job.\n"
"They agree with me that you have limitless potential.",

"You've proved yourself a decisive man, so I don't expect you'll have any trouble deciding what to do.\n"
"If you're interested, just step into the portal and I will take that as a yes.",

"Otherwise... well... I can offer you a battle you have no chance of winning.\n"
"Rather an anticlimax, after what you've just survived.",

"Time to choose... It's time to choose.",
"Wisely done, Mr. Freeman. I will see you up ahead.",
"Well, it looks like we won't be working together. No regrets, Mr. Freeman."
    );
}

inline static ::std::string_view whole() noexcept(false) {
    static auto const instance_ = [] () {
        ::std::list<::std::string> list_;
        for (auto const &paragraph_: paragraphs()) {
            if (paragraph_.empty()) continue;
            list_.emplace_back(paragraph_);
        }
        return ::boost::algorithm::join(list_, "\n");
    } ();
    return {instance_};
}

inline static auto random_split() noexcept(false) {
    auto whole_ = whole();
    using Value_ = ::std::decay_t<decltype(whole_)>;
    using Generator_ = target::generator::Coroutine<Value_>;
    return [] (auto data) -> Generator_ {
        if (data.empty()) { co_yield data; co_return; }
        auto make_random_size_ = [
            &data, engine_ = ::std::default_random_engine{
                ::std::random_device{}()
            }
        ] () mutable {
            return ::std::uniform_int_distribution<
                ::std::decay_t<decltype(data.size())>
            >{1, data.size()}(engine_);
        };
        auto const end_ = ::std::end(data);
        do {
            auto const begin_ = ::std::begin(data);
            auto const iterator_ = make_random_size_() + begin_;
            co_yield Value_{begin_, iterator_};
            data = Value_{iterator_, end_};
        } while (! data.empty());
    } (::std::move(whole_));
}

} // namespace speech
} // namespace utils
} // namespace this_

BOOST_AUTO_TEST_CASE(message_text_case) {
    namespace target_ = ::this_::utils::target;

    for (auto &&test_: [] {
        namespace message_ = target_::log::message;
        namespace chunk_ = message_::text::chunk;
        using Chunk_ = message_::text::Chunk;
        struct Value_ final {
            ::std::size_t line = message_::Location::current().line;
            Chunk_ expected, received;
        };
        using Generator_ = target_::generator::Coroutine<Value_ const>;
        constexpr static auto const generate_ = [] (
            char const *source
        ) -> Generator_ {
            auto const * const source_ = source ? source : "";

            co_yield {
                .expected = {.reference = source_, .terminated = true},
                .received = chunk_::make(source_)
            };

            co_yield {
                .expected = {
                    .reference = source_,
                    .terminated = static_cast<bool>(source)
                },
                .received = chunk_::make(source)
            };

            co_yield {
                .expected = {.terminated = true},
                .received = chunk_::make(::std::string{})
            };
            co_yield {
                .expected = {.reference = source_, .terminated = true},
                .received = chunk_::make(::std::string{source_})
            };

            co_yield {
                .expected = {.terminated = false},
                .received = chunk_::make(::std::string_view{})
            };
            co_yield {
                .expected = {.terminated = true},
                .received = chunk_::make(::std::string_view{}, true)
            };
            co_yield {
                .expected = {.terminated = false},
                .received = chunk_::make(::std::string_view{}, false)
            };

            co_yield {
                .expected = {.reference = source_, .terminated = false},
                .received = chunk_::make(::std::string_view{source_})
            };
            co_yield {
                .expected = {.reference = source_, .terminated = true},
                .received = chunk_::make(::std::string_view{source_}, true)
            };
            co_yield {
                .expected = {.reference = source_, .terminated = false},
                .received = chunk_::make(::std::string_view{source_}, false)
            };

            auto strong_ = ::std::string{};
            co_yield {
                .expected = {.terminated = true},
                .received = chunk_::make(strong_)
            };

            strong_ = source_;
            co_yield {
                .expected = {.reference = strong_, .terminated = true},
                .received = chunk_::make(strong_)
            };

            auto weak_ = ::std::string_view{};
            co_yield {
                .expected = {.terminated = false},
                .received = chunk_::make(weak_)
            };
            co_yield {
                .expected = {.terminated = true},
                .received = chunk_::make(weak_, true)
            };
            co_yield {
                .expected = {.terminated = false},
                .received = chunk_::make(weak_, false)
            };

            weak_ = source_;
            co_yield {
                .expected = {.reference = weak_, .terminated = false},
                .received = chunk_::make(weak_)
            };
            co_yield {
                .expected = {.reference = weak_, .terminated = true},
                .received = chunk_::make(weak_, true)
            };
            co_yield {
                .expected = {.reference = weak_, .terminated = false},
                .received = chunk_::make(weak_, false)
            };

            weak_ = strong_;
            co_yield {
                .expected = {.reference = weak_, .terminated = false},
                .received = chunk_::make(weak_)
            };
            co_yield {
                .expected = {.reference = weak_, .terminated = true},
                .received = chunk_::make(weak_, true)
            };
            co_yield {
                .expected = {.reference = weak_, .terminated = false},
                .received = chunk_::make(weak_, false)
            };
        };
        return [] () -> Generator_ {
            co_yield {
                .expected = {.terminated = false},
                .received = chunk_::make(nullptr)
            };
            for (auto const &value_: generate_(nullptr)) co_yield value_;
            for (auto const &value_: generate_("")) co_yield value_;
            for (auto const &value_: generate_("not empty")) co_yield value_;
        } ();
    } ()) {
        BOOST_TEST_CONTEXT("Line = " << test_.line) {
            BOOST_CHECK_EQUAL(
                test_.expected.reference, test_.received.reference
            );
            BOOST_TEST_INFO("Reference = " << test_.expected.reference);
            BOOST_CHECK_EQUAL(
                test_.expected.terminated, test_.received.terminated
            );
        };
    }

    if constexpr (true) {
        auto speech_ = ::std::list<::std::string_view>{};
        namespace text_ = target_::log::message::text;
        for (
            auto const &part_: ::this_::utils::speech::random_split()
        ) speech_.push_back(part_);

        auto const try_ = [&speech_] (auto &&driver) {
            auto const end_ = ::std::end(speech_);
            auto iterator_ = ::std::begin(speech_);
            auto generator_ = *text_::make(driver);
            for (auto const &chunk_: generator_) {
                BOOST_CHECK(! chunk_.terminated);
                BOOST_CHECK(end_ != iterator_);
                if (end_ == iterator_) return;
                BOOST_CHECK_EQUAL(*iterator_, chunk_.reference);
                ++iterator_;
            }
            BOOST_CHECK(end_ == iterator_);
        };

        try_(text_::driver::make(
            ::std::decay_t<decltype(speech_)>{speech_}
        ));

        auto copy_ = speech_;
        try_(text_::driver::make(copy_));
        BOOST_CHECK(! copy_.empty());

        try_(text_::driver::make(::std::move(copy_)));
        BOOST_CHECK(copy_.empty());
        copy_ = speech_;

        if constexpr (true) {
            auto driver_ = text_::driver::make(
                ::std::decay_t<decltype(speech_)>{speech_}
            );
            try_(driver_);
        }

        if constexpr (true) {
            auto driver_ = text_::driver::make(copy_);
            try_(driver_);
            BOOST_CHECK(! copy_.empty());
        }

        if constexpr (true) {
            auto driver_ = text_::driver::make(::std::move(copy_));
            try_(driver_);
            BOOST_CHECK(copy_.empty());
            copy_ = speech_;
        }

        if constexpr (true) {
            auto driver_ = text_::driver::make(
                ::std::decay_t<decltype(speech_)>{speech_}
            );
            try_(::std::move(driver_));
        }

        if constexpr (true) {
            auto driver_ = text_::driver::make(copy_);
            try_(::std::move(driver_));
            BOOST_CHECK(! copy_.empty());
        }

        if constexpr (true) {
            copy_ = speech_;
            auto driver_ = text_::driver::make(::std::move(copy_));
            try_(::std::move(driver_));
            BOOST_CHECK(copy_.empty());
        }
    }
}

BOOST_AUTO_TEST_CASE(write_case) {
    using Level_ = ::this_::utils::target::log::message::Level;

    constexpr static auto const check_location_ = [] (
        auto const &expected,
        auto const &received
    ) {
        BOOST_CHECK_EQUAL(expected.file, received.file);
        BOOST_CHECK_EQUAL(expected.line, received.line);
        BOOST_CHECK_EQUAL(expected.column, received.column);
        BOOST_CHECK_EQUAL(expected.function, received.function);
    };

    auto tester_ = [] {
        using Location_ = ::std::decay_t<decltype(
            ::this_::utils::location::make()
        )>;
        struct Result_ final {
            struct Buffer_ final {
                ::std::list<::std::string> text = {};
                Level_ level = Level_::Default;
                ::std::optional<Location_> location = ::std::nullopt;
            };
            ::this_::utils::target::Log log = {};
            ::std::optional<Buffer_> buffer = {};
        };
        return Result_{};
    } ();

    tester_.log >> [&buffer_ = tester_.buffer] (auto &&message) {
        if (buffer_) throw ::std::logic_error{"buffer is not empty"};
        buffer_.emplace(
            ::std::decay_t<decltype(*buffer_)>{.level = message.level}
        );
        if (message.location) buffer_->location.emplace(
            ::std::move(*(message.location))
        );
        for (
            auto const &chunk_: *(message.text)
        ) buffer_->text.emplace_back(chunk_.reference);
    };

    for (auto const &expected_level_: [] {
        struct Value_ final { ::std::string text; Level_ value; };
        return ::this_::utils::make_array<Value_>(
            Value_{"", Level_::Default},
            Value_{"Error", Level_::Error},
            Value_{"Warning", Level_::Warning},
            Value_{"Info", Level_::Info},
            Value_{"Debug", Level_::Debug},
            Value_{"Developer", Level_::Developer}
        );
    } ()) {
        namespace level_ = ::this_::utils::target::log::message::level;
        if (! expected_level_.text.empty()) BOOST_CHECK_EQUAL(level_::to_string(
            expected_level_.value
        ), expected_level_.text);
        tester_.buffer.reset();
        auto expected_location_ = ::this_::utils::location::make();
        auto column_fix_ = [] {
            constexpr auto const absolute_ = static_cast<
                ::std::decay_t<decltype(expected_location_.column)>
            >(30); // length of `::this_::utils::location::make`
            constexpr auto const relative_ = static_cast<
                ::std::decay_t<decltype(absolute_)>
            >(4); // length of `make`
            struct Result_ final {
                ::std::decay_t<decltype(absolute_)> absolute = absolute_;
                ::std::decay_t<decltype(relative_)> relative = relative_;
            };
            return Result_{};
        } ();
        ::this_::utils::location::fix_column(
            expected_location_, column_fix_.absolute, column_fix_.relative
        );
        BOOST_CHECK_EQUAL(
            ::std::decay_t<decltype(
                ::this_::utils::location::state().line
            )>::No > ::this_::utils::location::state().line ? 562 : 0,
            expected_location_.line
        );
        BOOST_CHECK_EQUAL(
            ::std::decay_t<decltype(
                ::this_::utils::location::state().column
            )>::No > ::this_::utils::location::state().column ? 35 : 0,
            expected_location_.column
        );
        auto expected_speech_ = [] () {
            auto generator_ = ::this_::utils::speech::random_split();
            auto result_ = ::std::list<
                ::std::decay_t<decltype(*::std::begin(generator_))>
            >{};
            for (auto &part_: generator_) result_.push_back(::std::move(part_));
            return result_;
        } ();
        if (expected_level_.text.empty()) {
            tester_.log.write() << expected_speech_
            << "\nEnd of speech!!!" << 1 << "!!!!!!";
            if (0 < expected_location_.line) expected_location_.line += 38;
            else expected_location_.line = 0;
            if (0 < expected_location_.column) expected_location_.column = 13;
            else expected_location_.column = 0;
            column_fix_.absolute = 17; // length of `tester_.log.write`
            column_fix_.relative = 5; // length of `write`
        }
        else {
            tester_.log.write(
                expected_level_.value
            ) << expected_speech_ << "\nEnd of speech!!!" << 1 << "!!!!!!";
            if (0 < expected_location_.line) expected_location_.line += 48;
            else expected_location_.line = 0;
            if (0 < expected_location_.column) expected_location_.column = 13;
            else expected_location_.column = 0;
            column_fix_.absolute = 17; // length of `tester_.log.write`
            column_fix_.relative = 5; // length of `write`
        }
        BOOST_TEST_INFO("Level = " << expected_level_.text);
        BOOST_CHECK(static_cast<bool>(tester_.buffer));
        if (tester_.buffer) {
            BOOST_TEST_INFO("Level = " << expected_level_.text);
            BOOST_CHECK_EQUAL(
                Level_::Info < expected_level_.value,
                static_cast<bool>(tester_.buffer->location)
            );
            if (tester_.buffer->location) {
                BOOST_TEST_INFO("Level = " << expected_level_.text);
                auto const &expected_ = expected_location_;
                BOOST_TEST_INFO("Location = " << expected_.to_string());
                check_location_(expected_, ::this_::utils::location::fix_column(
                    *(tester_.buffer->location), column_fix_.absolute,
                    column_fix_.relative, false
                ));
            };
            while (! expected_speech_.empty()) {
                BOOST_CHECK_EQUAL(
                    expected_speech_.front(), tester_.buffer->text.front()
                );
                expected_speech_.pop_front();
                tester_.buffer->text.pop_front();
            }
            [text_ = ::std::move(tester_.buffer->text)] () mutable {
                BOOST_CHECK(! text_.empty());
                if (text_.empty()) return;
                BOOST_CHECK_EQUAL("\nEnd of speech!!!", text_.front());
                text_.pop_front();
                BOOST_CHECK(! text_.empty());
                if (text_.empty()) return;
                BOOST_CHECK_EQUAL("1", text_.front());
                text_.pop_front();
                BOOST_CHECK(! text_.empty());
                if (text_.empty()) return;
                BOOST_CHECK_EQUAL("!!!!!!", text_.front());
                text_.pop_front();
                BOOST_CHECK(text_.empty());
            } ();
            BOOST_CHECK(tester_.buffer->text.empty());
        }
    };
}
