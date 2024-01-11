#include <memory>
#include <utility>
#include <optional>
#include <typeindex>
#include <stdexcept>
#include <functional>
#include <unordered_map>

#include <p5/lambda/utils/container.hxx>
#include <p5/lambda/utils/async/channel.hxx>


namespace p5::lambda::utils::container {
namespace private_ {

namespace parent_ = this_;
namespace this_ = parent_::private_;

struct Driver final: parent_::Driver {
    virtual ::std::shared_ptr<Node> const & node(
        ::std::type_index const &
    ) noexcept(false) override final;

    Driver() noexcept(true) = default;
    virtual ~Driver() noexcept(true) override final = default;

private:
    ::std::unordered_map<
        ::std::type_index, ::std::shared_ptr<Node>
    > nodes_ = {};

    Driver(Driver &&) = delete;
    Driver(Driver const &) = delete;
    Driver & operator = (Driver &&) = delete;
    Driver & operator = (Driver const &) = delete;
};

struct Node final: parent_::driver::Node {
    virtual bool lock(bool) noexcept(true) override final;
    virtual void payload(Payload &&) noexcept(true) override final;
    virtual Payload const & payload() const noexcept(true) override final;

    virtual Subscription subscribe(
        ::std::function<void(void)> &&
    ) const noexcept(false) override final;

    Node() noexcept(true) = default;
    virtual ~Node() noexcept(true) override final;

private:
    struct Context_ final {
        bool lock = false;
        Payload payload = {};
        mutable ::std::optional<
            parent_::parent_::async::channel::Issuer<void>
        > channel = ::std::nullopt;
    } context_ = {};

    Node(Node &&) = delete;
    Node(Node const &) = delete;
    Node & operator = (Node &&) = delete;
    Node & operator = (Node const &) = delete;
};

} // namespace private_

this_::Type::Type() noexcept(false):
    driver_{::std::make_unique<this_::private_::Driver>()}
{}

this_::Type::Type::~Type() noexcept(true) = default;

namespace driver {

this_::Type::Type() noexcept(true) = default;
this_::Type::Type::~Type() noexcept(true) = default;

namespace node {

this_::Type::Type() noexcept(true) = default;
this_::Type::Type::~Type() noexcept(true) = default;

namespace payload {

bool this_::Type::operator != (Type const &other) noexcept(true) {
    return (this != &other) || (
        pointer != other.pointer
    ) || (qualifiers != other.qualifiers);
}

bool this_::Type::operator == (Type const &other) noexcept(true) {
    return ! operator != (other);
}

} // namespace payload
} // namespace node
} // namespace driver

namespace private_ {

inline ::std::shared_ptr<this_::Driver::Node> const & this_::Driver::node(
    ::std::type_index const &key
) noexcept(false) {
    auto const insert_result_ = nodes_.insert({key, {}});
    if (insert_result_.second) {
        auto node_ = nodes_.extract(insert_result_.first);
        node_.mapped().reset(new this_::Node);
        return nodes_.insert(::std::move(node_)).position->second;
    }
    return insert_result_.first->second;
}

inline bool this_::Node::lock(bool state) noexcept(true) {
    if (state == context_.lock) return false;
    context_.lock = state;
    return true;
}

inline void this_::Node::payload(Payload &&payload) noexcept(true) {
    context_.payload = ::std::move(payload);
    if (context_.channel) context_.channel->dispatch();
}

inline this_::Node::Payload const &
this_::Node::payload() const noexcept(true) {
    return context_.payload;
}

inline this_::Node::Subscription this_::Node::subscribe(
    ::std::function<void(void)> &&handler
) const noexcept(false) {
    if (! handler) throw ::std::invalid_argument{"bad handler"};
    if (! context_.channel) context_.channel.emplace();
    return context_.channel->subscribe(::std::move(handler));
}

inline this_::Node::Node::~Node() noexcept(true) {
    auto channel_ = ::std::exchange(context_.channel, ::std::nullopt);
    if (channel_) channel_->dispatch();
}

} // namespace private_
} // namespace p5::lambda::utils::container
