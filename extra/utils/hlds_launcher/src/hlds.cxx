#include <cstdlib>

#include <list>
#include <vector>
#include <iterator>
#include <stdexcept>
#include <filesystem>
#include <type_traits>
#include <system_error>

#include <boost/algorithm/string.hpp>

#include <unistd.h>


namespace utils_ {

inline static auto update_ld_library_path() noexcept(false) {
    auto value_ = [] () -> ::std::string {
        auto const * const pointer_ = ::std::getenv("LD_LIBRARY_PATH");
        if (! pointer_) return {};
        return pointer_;
    } ();
    if (value_.empty()) {
#if defined(__GNUC__) && (13 > __GNUC__) && (! defined (__clang__))
// suppress false positive warning
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wrestrict"
#endif
        value_ = ".";
#if defined(__GNUC__) && (13 > __GNUC__) && (! defined (__clang__))
// suppress false positive warning
#pragma GCC diagnostic pop
#endif
        ::setenv("LD_LIBRARY_PATH", value_.c_str(), 1);
    }
    else {
        auto items_ = ::std::list<::std::string>{};
        auto const splitted_ = ::boost::split(
            items_, value_, ::boost::is_any_of(":")
        );
        auto const sentinel_ = ::std::end(splitted_);
        auto const iterator_ = ::std::find(
            ::std::begin(splitted_), sentinel_, "."
        );
        if (sentinel_ == iterator_) {
            value_ = ".:" + value_;
            ::setenv("LD_LIBRARY_PATH", value_.c_str(), 1);
        }
    }
}

} // namespace utils_


int main(int argc, char **argv) {
    if (! (0 < argc)) throw ::std::invalid_argument{"argument count"};
    auto const directory_ = ::std::filesystem::current_path();
    ::utils_::update_ld_library_path();
    auto const executable_ = [] {
        auto const path_ = (::std::filesystem::current_path() / "hlds_linux");
        auto const type_ = ::std::filesystem::status(path_).type();
        if (
            ::std::decay_t<decltype(type_)>::regular != type_
        ) throw ::std::invalid_argument{"hlds_linux binary not found"};
        return path_.string();
    } ();
    auto arguments_ = ::std::vector<char const *>{};
    arguments_.resize(static_cast<
        ::std::decay_t<decltype(arguments_.size())>
    >(argc));
    arguments_[0] = "./hlds_linux";
    for (
        ::std::decay_t<decltype(argc)> index_ = 1; index_ < argc; ++index_
    ) arguments_[index_] = argv[index_];
    auto const code_ = ::execv(executable_.c_str(), argv);
    if (0 != code_) throw ::std::system_error(code_, ::std::generic_category());
    throw ::std::logic_error{"invalid state"};
    return 0 == code_ ? 1 : code_;
}
