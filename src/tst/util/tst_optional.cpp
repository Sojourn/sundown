#include "../tst.h"
#include "util/util.h"

namespace Sundown {

    void testOptionalBasic()
    {
        Optional<int> o1;
        TST_ASSERT(!o1);
        TST_ASSERT(o1.empty());

        Optional<int> o2(3);
        TST_ASSERT(o2);
        TST_ASSERT(!o2.empty());

        o1 = std::move(o2);
        TST_ASSERT(!o2);
        TST_ASSERT(o1);

        TST_ASSERT(o1.get() == 3);
        TST_ASSERT(!o1);
    }

    void testOptionalMoving()
    {
        Optional<int> o1;
        Optional<int> o2;

        o1 = []() -> Optional<int> {
            if (SUNDOWN_DYNAMIC_TRUE) {
                return Optional<int>(3);
            }
            else {
                return None();
            }
        }();
        TST_ASSERT(o1);

        o2 = std::move(o1);
        TST_ASSERT(!o1);
        TST_ASSERT(o2);

        TST_ASSERT(o2.get() == 3);
    }

    struct TestOptionalModule : public TestModule
    {
        TestOptionalModule()
            : TestModule("Optional")
        {
            addTest("testOptionalBasic", testOptionalBasic);
            addTest("testOptionalMoving", testOptionalMoving);
        }
    };
    static TestOptionalModule testOptionalModule_;

}


