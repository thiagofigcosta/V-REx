#include <iostream>
#include <signal.h>
#include <boost/stacktrace.hpp>
#include <exception>
#include <stdexcept>

#include "Utils.hpp"
#include "test.hpp"

using namespace std;

void exceptionHandler(int signum) {
    ::signal(signum, SIG_DFL);
    // boost::stacktrace::safe_dump_to("./backtrace.dump");
    cerr << endl << "Stack trace:" << endl << boost::stacktrace::stacktrace() << endl;
    try {
        exception_ptr eptr=current_exception();
        if (eptr) {
            rethrow_exception(eptr);
        }
    } catch(const exception& e) {
        cerr << "Caught exception:\n\t\"" << e.what() << "\"\n\n";
    }
    ::raise(SIGABRT);
}

int main() {
    ::signal(SIGSEGV, &exceptionHandler);
    ::signal(SIGABRT, &exceptionHandler);
    test();
    return 0;
}