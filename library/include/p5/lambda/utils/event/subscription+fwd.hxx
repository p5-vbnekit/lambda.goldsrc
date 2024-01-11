#pragma once

#include "namespace_.hxx"


namespace p5::lambda::utils::event {
namespace subscription {

namespace parent_ = this_;
namespace this_ = parent_::subscription;

struct Type;

namespace storage {

namespace parent_ = this_;
namespace this_ = parent_::storage;

namespace entry {

namespace parent_ = this_;
namespace this_ = parent_::entry;

struct Type;

} // namespace entry

using Entry = entry::Type;

} // namespace storage
} // namespace subscription

using Subscription = subscription::Type;

} // namespace p5::lambda::utils::event
