#include <iostream>
#include <thread>
#include <vector>
#include <tuple>
#include <array>

#include <cassert>
#include <cstdint>

#include "sys/sys.h"

using namespace Sundown;

class Item : public SelectorItem
{
public:
    Item(FileDescriptor fd)
        : SelectorItem(std::move(fd))
    {
    }
};

int main(int argc, char **argv) {
    Selector selector;
    selector.add(std::make_shared<Item>(dup(0)), Selector::Readable);

    for (const SelectorEvent &event: selector.select()) {
        char buffer[32];

        if (event.readable()) {
            int len = read(event.item()->fd().get(), buffer, sizeof(buffer));
            assert(len > 0);

            std::cout << buffer << std::endl;
        }
    }

    return 0;
}

