#ifndef TST_H
#define TST_H

#include <tuple>
#include <vector>
#include <functional>
#include <algorithm>
#include <iostream>

namespace Sundown {

    class TestModule;

    struct TestFailedException
    {
        const char *statement;
        const char *file;
        int line;
    };

    class TestRunner
    {
        friend class TestModule;
    public:
        const std::vector<std::reference_wrapper<TestModule>> &modules() const
        {
            return modules_;
        }

    private:
        void addModule(TestModule &module)
        {
            modules_.push_back(std::ref(module));
        }

    private:
        std::vector<std::reference_wrapper<TestModule>> modules_;
    };
    extern TestRunner testRunner;

    class Test
    {
    public:
        template<typename Object>
        Test(const char *name, Object *object, void (Object::*func)())
            : name_(name)
            , func_(std::bind(std::mem_fn(func), object))
        {
        }

        Test(const char *name, void (*func)())
            : name_(name)
            , func_(func)
        {
        }

        const char *name() const
        {
            return name_;
        }

        const std::function<void()> &func() const
        {
            return func_;
        }

    private:
        const char *name_;
        std::function<void()> func_;
    };

    class TestModule
    {
    public:
        TestModule(const char *name)
            : name_(name)
        {
            testRunner.addModule(*this);
        }

        virtual ~TestModule()
        {
        }

        const char *name() const
        {
            return name_;
        }

        const std::vector<Test> &tests()
        {
            return tests_;
        }

        virtual void startup() {}
        virtual void shutdown() {}

    protected:
        void addTest(Test test)
        {
            tests_.push_back(test);
        }

        void addTest(const char *name, void (*func)())
        {
            addTest(Test(name, func));
        }
        
        template<typename Object>
        void addTest(const char *name, Object *object, void (Object::*func)())
        {
            addTest(Test(name, object, func));
        }

    private:
        const char *name_;
        std::vector<Test> tests_;
    };
}

#define TST_ASSERT(x) do { \
    if (!(x)) { \
        throw TestFailedException { #x, __FILE__, __LINE__ }; \
    } \
} while(0); \

#endif // TST_H

