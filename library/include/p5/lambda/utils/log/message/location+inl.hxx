#pragma once

#include <utility>
#include <type_traits>
#include <string_view>

#ifdef __cpp_lib_source_location
#include <source_location>
#else
#include <experimental/source_location>
#endif

#include "location.hxx"


namespace p5::lambda::utils::log::message::location {

inline constexpr auto unknown() noexcept(true) { return Type{}; }

inline constexpr auto unknown(Type const &value) noexcept(true) {
    return value.file.empty() || value.function.empty();
}

template <class ... T> inline auto to_string(
    Type const &value, T && ... options
) noexcept(false) { return value.to_string(::std::forward<T>(options) ...); }

inline constexpr auto this_::Type::make() noexcept(true) {
    return this_::unknown();
}

inline constexpr auto this_::Type::make(Native const &native) noexcept(true) {
    auto result_ = this_::unknown();

    if (auto const * const pointer_ = native.file_name()) {
        if (*pointer_) result_.file = pointer_;
    }

    if (auto const * const pointer_ = native.function_name()) {
        if (*pointer_) result_.function = pointer_;
    }

    auto const line_ = native.line();
    if (0 < line_) result_.line = static_cast<
        ::std::decay_t<decltype(result_.line)>
    >(line_);

    auto const column_ = native.column();
    if (0 < column_) result_.column = static_cast<
        ::std::decay_t<decltype(result_.column)>
    >(column_);

    return result_;
}

inline constexpr auto this_::Type::current(
    FinalToken_ const &, Native const &native
) noexcept(true) { return Type::make(native); }

inline auto inspect() noexcept(true) {
    static_assert(::std::is_same_v<
        ::p5::lambda::utils::log::message::Location, this_::Type
    >);
    auto const value_ = ::p5::lambda::utils::log::message::Location::current();
    enum class CommonState_ { Yes, No, Invalid };
    enum class ColumnState_ {
        Full, Absolute, Relative, Parentheses, No, Invalid
    };
    struct Result_ final {
        CommonState_ file, line;
        ColumnState_ column;
        CommonState_ function;
    };
    constexpr auto const line_ = static_cast<
        ::std::decay_t<decltype(value_.line)>
    >(64);
    constexpr auto const column_ = static_cast<
        ::std::decay_t<decltype(value_.column)>
    >(25);
    return Result_{
        .file = [&value_ = value_.file] {
            return value_.empty() ? CommonState_::No : CommonState_::Yes;
        } (),
        .line = [value_ = value_.line] {
            switch (value_) {
                default: break;
                case line_: return CommonState_::Yes;
                case 0: return CommonState_::No;
            }
            return CommonState_::Invalid;
        } (),
        .column = [value_ = value_.column] {
            switch (value_) {
            default: break;
            case 0: return ColumnState_::No;
            case column_: return ColumnState_::Full;
            case 2 + column_: return ColumnState_::Absolute;
            case 45 + column_: return ColumnState_::Relative;
            case 52 + column_: return ColumnState_::Parentheses;
            }
            return ColumnState_::Invalid;
        } (),
        .function = [&value_ = value_.function] {
            return value_.empty() ? CommonState_::No : CommonState_::Yes;
        } ()
    };
}

inline constexpr auto make() noexcept(true) { return this_::unknown(); }

template <class ... T> inline constexpr
auto make(T && ... source) noexcept(true) {
    return this_::Type::make(::std::forward<T>(source) ...);
}

} // namespace p5::lambda::utils::log::message::location
