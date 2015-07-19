#include <iostream>
#include <thread>
#include <vector>
#include <tuple>
#include <array>
#include <string>
#include <stdexcept>

#include <cassert>
#include <cstdint>
#include <cstdio>

#include "core/core.h"

using namespace Sundown;

class TestSelectorItem : public SelectorItem
{
public:
    TestSelectorItem(Pipe pipe)
        : pipe_(std::move(pipe))
    {
    }
    
    virtual const FileDescriptor &fd() const
    {
        return pipe_.fd();
    }

private:
    Pipe pipe_;
};

int main(int argc, char **argv)
{
    auto pipes = Pipe::create().get();
    auto readerItem = std::make_shared<TestSelectorItem>(std::move(std::get<0>(pipes)));
    auto writerItem = std::make_shared<TestSelectorItem>(std::move(std::get<1>(pipes)));
    auto selector = Selector::create().get();
    auto streamWriter = StreamWriter::create().get();

    selector.add(readerItem, Selector::Readable);
    selector.add(writerItem, Selector::Writable);

    char buffer[64];
    for (size_t i = 0; i < 10; ++i) {
        for (SelectorEvent event: selector.select()) {
            if (event.writable()) {
                void *msg = streamWriter.begin(5);
                std::memcpy(msg, "Hello", 5);
                streamWriter.commit();

                streamWriter.flush(event.item()->fd());
            }
            if (event.readable()) {
                int cnt = read(event.item()->fd().get(), buffer, sizeof(buffer) / sizeof(*buffer));
                assert(cnt >= 0);
                std::cout << i << " " << std::string(buffer, buffer + cnt) << std::endl;
            }
        }
    }

    return 0;
}

