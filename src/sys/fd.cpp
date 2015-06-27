#include "sys.h"

using namespace Sundown;

FileDescriptor::FileDescriptor(int fd)
    : fd_(fd)
{
}

FileDescriptor::FileDescriptor(FileDescriptor &&other)
    : fd_(other.fd_)
{
    other.fd_ = -1;
}

FileDescriptor::~FileDescriptor()
{
    reset();

    if (fd_ >= 0) {
        int sts = close(fd_);
        assert(sts >= 0);

        fd_ = -1;
    }
}

FileDescriptor &FileDescriptor::operator=(FileDescriptor &&rhs)
{
    if (this != &rhs) {
        reset();

        fd_ = rhs.fd_;
        rhs.fd_ = -1;
    }

    return *this;
}

int FileDescriptor::get() const
{
    return fd_;
}

bool FileDescriptor::empty() const
{
    return fd_ < 0;
}

void FileDescriptor::reset(int fd)
{
    if (fd_ >= 0) {
        int sts = close(fd_);
        assert(sts >= 0);
        
    }

    fd_ = fd;
}
