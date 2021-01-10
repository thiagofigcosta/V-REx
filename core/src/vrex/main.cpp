#include <iostream>
#include <boost/stacktrace.hpp>

#include "Utils.hpp"
#include "test.hpp"

using namespace std;

int main() {
    try{
        test();
    } catch (...) {
        cerr << endl << "Stack trace:" << endl << boost::stacktrace::stacktrace() << endl;
        return 1;
    }
    return 0;
}