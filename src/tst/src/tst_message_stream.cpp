#include <thread>
#include <vector>
#include <iostream>

#include "core/core.h"

using namespace Sundown;

static const size_t writerCount = 4;
static const size_t writerIters = 1000;
static const size_t readerIters = writerCount * writerIters;

void writerTask(std::shared_ptr<MessageStream> stream)
{
    MessageStreamWriter writer(stream);
    for (size_t i = 0; i < writerIters; ++i) {
        Message message = writer.claim(8);
        Optional<Buffer> buffer;
        while (!(buffer = writer.prepare(message)))
            ;

        for (uint8_t &byte: buffer.get()) {
            byte = 3;
        }

        writer.commit(std::move(message));
    }
}

void readerTask(std::shared_ptr<MessageStream> stream)
{
    MessageStreamReader reader(stream);
    for (size_t i = 0; i < readerIters; ++i) {
        Optional<Buffer> buffer;
        while (!(buffer = reader.read()))
            ;

        for (uint8_t byte: buffer.get()) {
            assert(byte == 3);
        }
    }
}

int main(int argc, char **argv)
{
    std::shared_ptr<MessageStream> stream = MessageStream::create(1 << 16).get();

    std::vector<std::thread> writerThreads;
    for (size_t i = 0; i < writerCount; ++i) {
        writerThreads.push_back(std::thread(writerTask, stream));
    }
    
    std::thread readerThread(readerTask, stream);

    for (std::thread &writerThread: writerThreads) {
        writerThread.join();
    }

    readerThread.join();

    return 0;
}

