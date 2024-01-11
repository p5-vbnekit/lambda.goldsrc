#include <string>
#include <exception>

#include <boost/exception/diagnostic_information.hpp>

#include <p5/lambda/utils/exception.hxx>


namespace p5::lambda::utils::exception {

::std::exception_ptr next(
    ::std::exception_ptr const &exception
) noexcept(true) {
    if (exception) try { ::std::rethrow_exception(exception); }
    catch (::std::exception const &exception) { return this_::next(exception); }
    return {};
}

::std::string details(
    ::std::exception_ptr const &exception, bool verbose
) noexcept(false) {
    if (exception) try {
        ::std::rethrow_exception(exception);
    } catch (...) {
        return ::boost::current_exception_diagnostic_information(verbose);
    }
    return ::boost::diagnostic_information(exception, verbose);
}

} // namespace p5::lambda::utils::exception
