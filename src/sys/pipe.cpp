#include "sys.h"

using namespace Sundown;

Optional<std::tuple<Pipe, Pipe>> Pipe::create()
{
    int fds[2];
    int sts;

    sts = pipe2(fds, O_NONBLOCK | O_CLOEXEC);
    if (sts < 0) {
        return Optional<std::tuple<Pipe, Pipe>>();
    }
    else {
        return Optional<std::tuple<Pipe, Pipe>>(std::make_tuple(Pipe(fds[0]),
                                                                Pipe(fds[1])));
    }
}

Pipe::Pipe(FileDescriptor fd)
    : fd_(std::move(fd))
{
}

const FileDescriptor &Pipe::fd() const
{
    return fd_;
}

