#include <vector>
#include <iostream>
#include <functional>
#include <cassert>

#include "tst.h"

using namespace Sundown;

TestRunner Sundown::testRunner;

int main(int argc, char **argv)
{
    size_t testCount = 0;
    size_t successCount = 0;
    size_t failureCount = 0;

    for (TestModule &module: testRunner.modules()) {
        module.startup();

        std::cout << "----------------------------------------" << std::endl;
        std::cout << "Running: " << module.name() << std::endl;
        for (const Test &test: module.tests()) {
            testCount += 1;
            try {
                const std::function<void()> &func = test.func();
                assert(func);
                func();
                successCount += 1;
                std::cout << "Success: " << test.name() << std::endl;
            }
            catch (const TestFailedException &ex) {
                failureCount += 1;
                std::cout << "Failure: " << test.name() << std::endl;
                std::cout << ex.file << ":" << ex.line
                    << ": Assertion failed(" << ex.statement << ")" << std::endl;
            }
        }
        std::cout << std::endl;

        module.shutdown();
    }

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Total tests: " << testCount << std::endl;
    std::cout << "Passed:      " << successCount << std::endl;
    std::cout << "Failed:      " << failureCount << std::endl;

    return 0;
}

