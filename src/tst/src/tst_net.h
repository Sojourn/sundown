#include <iostream>
#include <thread>

#include <cassert>

#include <unistd.h>

class FileDescriptor
{
public:
    FileDescriptor(int fd = -1);
    FileDescriptor(FileDescriptor &&other);
    FileDescriptor(const FileDescriptor &other) = delete;
    ~FileDescriptor();

    int get() const;
    bool reset(int fd = -1);
    
    FileDescriptor &operator=(FileDescriptor &&rhs);
    FileDescriptor &operator=(const FileDescriptor &rhs) = delete;

private:
    int fd_;
};

FileDescriptor::FileDescriptor(int fd)
    : fd_(fd)
{
}

FileDescriptor::FileDescriptor(FileDescriptor &&other)
{
    fd_ = other.fd_;
    other.fd_ = -1;
}

FileDescriptor::~FileDescriptor()
{
    // FIXME: Need to do something if this fails
    assert(reset());
}

FileDescriptor &FileDescriptor::operator=(FileDescriptor &&rhs)
{
    if (this != &rhs) {
        // FIXME: Need to do something if this fails
        assert(reset());

        fd_ = rhs.fd_;
        rhs.fd_ = -1;
    }

    return *this;
}

int FileDescriptor::get() const
{
    return fd_;
}

bool FileDescriptor::reset(int fd)
{
    if (fd_ >= 0) {
        int sts = close(fd_);
        if (sts < 0) {
            return false;
        }
    }

    fd_ = fd;
    return true;
}

struct Port
{
    uint16_t value;
};

struct Address
{
    const char *host;
};

struct Interface
{
    const char *name;
};

class TcpSocket
{
public:
    TcpSocket();
    TcpSocket(TcpSocket &&other);
    TcpSocket(const TcpSocket &other) = delete;

    TcpSocket &operator=(TcpSocket &&rhs);
    TcpSocket &operator=(const TcpSocket &rhs) = delete;

    bool connect(Address addr, Port port);
    bool bind(Port port);
    bool bind(Interface iface, Port port);
    bool listen(size_t backlog = 0);
    TcpSocket accept();

private:
    FileDescriptor fd_;
};

class TcpSocketIO
{
public:
    TcpSocketIO(TcpSocket *socket);

    std::tuple<size_t, bool> send(const void *buf, size_t len);
    std::tuple<size_t, bool> recv(void *buf, size_t len);

    bool send(MemoryStream *stream);
    bool recv(MemoryStream *stream);

    size_t bytesRead() const;
    size_t bytesWritten() const;

private:
    TcpSocket *socket_;
    size_t bytesRead_;
    size_t bytesWritten_;
};

class MemoryStream
{
public:
    virtual ~MemoryStream() {}

    size_t reader() const;
    size_t writer() const;

    void advanceReader(size_t bytes);
    void advanceWriter(size_t bytes);

    std::tuple<const void *, size_t> readerBuffer() const;
    std::tuple<void *, size_t> writerBuffer() const;

    const FileDescriptor &fd() const;

private:
    FileDescriptor fd_;
};

int main(int argc, char **argv)
{
    return 0;
}

