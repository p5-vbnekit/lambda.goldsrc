#include <ranges>
#include <string>
#include <sstream>
#include <utility>
#include <filesystem>
#include <string_view>

#include <boost/format.hpp>

#include <p5/lambda/utils/log.hxx>
#include <p5/lambda/utils/generator.hxx>


namespace p5::lambda::utils::log {
namespace raii {
namespace context {

void dispatch(this_::Type &context) noexcept(true) {
    auto * const target_ = ::std::exchange(context.target, nullptr);
    if (! target_) return;
    target_->write(parent_::parent_::Message{
        .text = parent_::text::make(context.text),
        .level = context.level, .location = ::std::move(context.location)
    });
}

} // namespace context

this_::Type::~Type() noexcept(true) {
    if (context_.text) this_::context::dispatch(context_);
}

this_::Type::Type(Context &&context) noexcept(false): context_{[&context] {
    if (! context.target) throw ::std::invalid_argument{"empty target"};
    if (context.text) throw ::std::invalid_argument{"text not empty"};
    return ::std::move(context);
} ()} {}

} // namespace raii

namespace message {
namespace text {
namespace driver {
namespace interface {

this_::Type::~Type() noexcept(true) {}

this_::Type::Type() noexcept(true) {}
this_::Type::Type(Type &&) noexcept(true) {}
this_::Type & this_::Type::operator = (Type &&) noexcept(true) { return *this; }

} // namespace interface
} // namespace driver

namespace normalizer::driver {
namespace private_ {

namespace parent_ = this_;
namespace this_ = parent_::private_;

inline static parent_::Chunk const * pop_one(
    parent_::Interface &source
) noexcept(true) {
    while (
        auto const * const value_ = source()
    ) if (! value_->reference.empty()) return value_;
    return nullptr;
}

} // namespace private_

this_::Chunk const *
this_::Type<this_::Interface *>::operator () () noexcept(true) {
    if (source_) {
        auto const * const value_ = this_::private_::pop_one(*source_);
        if (value_) return value_;
        source_ = nullptr;
    }
    return nullptr;
}

this_::Type<this_::Interface *>::Type(Type &&other) noexcept(true):
    source_{::std::exchange(other.source_, nullptr)}
{}

this_::Type<this_::Interface *>::Type(Source source) noexcept(true):
    source_{source}
{}

this_::Type<this_::Interface *>::Type::~Type() noexcept(true) = default;

} // namespace normalizer::driver
} // namespace text

namespace location {

::std::string this_::Type::to_string(
    ::std::string_view const &root
) const noexcept(false) {
    ::std::ostringstream stream_;
    using Text_ = ::std::decay_t<decltype(stream_.str())>;
    using Generator_ = parent_::parent_::parent_::generator::Coroutine<Text_>;
    ::std::ranges::move([this, &root] () -> Generator_ {
        auto text_ = Text_{function};
        if (! text_.empty()) co_yield ::std::move(text_);
        if ((text_ = file).empty()) co_return;
        if (! (root.empty() || (0 == root.at(0)))) {
            text_ = ::std::filesystem::relative(text_, root).string();
            if (text_.empty()) co_return;
        }
        if (0 < line) text_ = (::boost::format("%s#%u") % text_ % line).str();
        co_yield Text_{"|"};
        co_yield text_;
    } (), ::std::ostream_iterator<Text_>{stream_});
    return stream_.str();
}

} // namespace location
} // namespace message

namespace interface {

this_::Type::RAII this_::Type::write(
    Message::Level level, FinalToken_ &&, Message::Location &&location
) const noexcept(true) {
    auto context_ = this_::RAII::Context{.level = level, .target = this};
    if (Message::Level::Info < level) context_.location = ::std::move(location);
    return this_::RAII{::std::move(context_)};
}

this_::Type::Type::~Type() noexcept(true) = default;

} // namespace interface

void this_::Type::write_(Message &&message) const noexcept(true) {
    if (! handler) return;
    if (message.location && message::location::unknown(
        *(message.location)
    )) message.location.reset();
    try { handler(::std::move(message)); } catch(...) {}
}

} // namespace p5::lambda::utils::log
