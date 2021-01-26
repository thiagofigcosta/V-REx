#include <iostream>
#include <signal.h>
#include <boost/stacktrace.hpp>
#include <exception>
#include <stdexcept>
#include <sys/resource.h>

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

void setup(){
    ::signal(SIGSEGV, &exceptionHandler);
    ::signal(SIGABRT, &exceptionHandler);
    const rlim_t kStackSize = 512 * 1024 * 1024;   // min stack size = 16 MB
    struct rlimit rl;
    int result;
    result = getrlimit(RLIMIT_STACK, &rl);
    if (result == 0){
        if (rl.rlim_cur < kStackSize){
            rl.rlim_cur = kStackSize;
            result = setrlimit(RLIMIT_STACK, &rl);
            if (result != 0){
                fprintf(stderr, "setrlimit returned result = %d\n", result);
            }
        }
    }
}

int main() {
    setup();
    test();
    return 0;
}