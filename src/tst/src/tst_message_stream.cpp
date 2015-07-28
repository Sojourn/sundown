#include <thread>
#include <vector>
#include <iostream>

#include "core/core.h"

using namespace Sundown;

void writerTask(std::shared_ptr<MessageStream> stream)
{
}

void readerTask(std::shared_ptr<MessageStream> stream)
{
}

int main(int argc, char **argv)
{
    const size_t writerCount = 4;
    const size_t writerIters = 1000;
    const size_t readerIters = writerCount * writerIters;

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

