#include "sys/sys.h"

using namespace Sundown;

Reflector::Reflector(size_t size)
    : addr_(nullptr)
    , size_(size)
{
    static const char *path = "ring_buffer.shm";
    void *resultAddr = nullptr;
    int sts = 0;

    if ((size == 0) || (size % 4096)) {
        throw std::runtime_error("invalid size");
    }

    fd_.reset(shm_open(path, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR));
    if (fd_.empty()) {
        throw std::runtime_error("shm_open failed");
    }

    sts = shm_unlink(path);
    if (sts < 0) {
        throw std::runtime_error("shm_unlink failed");
    }

    sts = ftruncate(fd_.get(), size_);
    if (sts < 0) {
        throw std::runtime_error("ftruncate failed");
    }
    
    addr_ = mmap(nullptr, size_ * 2, PROT_READ | PROT_WRITE, MAP_SHARED, fd_.get(), 0);
    if (addr_ == MAP_FAILED) {
        throw std::runtime_error("ftruncate failed");
    }

    resultAddr = mremap(addr_, size_ * 2, size_, 0);
    if (resultAddr == MAP_FAILED) {
        sts = munmap(addr_, size_ * 2);
        assert(sts >= 0);
        throw std::runtime_error("mremap failed");
    }
    else if (resultAddr != addr_) {
        sts = munmap(resultAddr, size_);
        assert(sts);
        throw std::runtime_error("mremap moved mapping");
    }

    void *upperAddr = static_cast<uint8_t*>(addr_) + size_;
    resultAddr = mmap(upperAddr, size_, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fd_.get(), 0);
    if (resultAddr == MAP_FAILED) {
        sts = munmap(addr_, size_);
        assert(sts >= 0);
        throw std::runtime_error("mmap failed");
    }
}

Reflector::Reflector(Reflector &&other)
{
    addr_ = other.addr_;
    other.addr_ = nullptr;

    size_ = other.size_;
    other.size_ = 0;

    fd_ = std::move(other.fd_);
}

Reflector::~Reflector()
{
    reset();
}

void Reflector::reset()
{
    if (!fd_.empty()) {
        int sts1 = munmap(lower(), size_);
        int sts2 = munmap(upper(), size_);
        assert((sts1 >= 0) && (sts2 >= 0));

        addr_ = nullptr;
        size_ = 0;
        fd_.reset();
    }
}

Reflector &Reflector::operator=(Reflector &&rhs)
{
    if (this != &rhs) {
        reset();

        addr_ = rhs.addr_;
        rhs.addr_ = nullptr;

        size_ = rhs.size_;
        rhs.size_ = 0;

        fd_ = std::move(rhs.fd_);
    }

    return *this;
}

const FileDescriptor &Reflector::fd() const
{
    return fd_;
}

void *Reflector::data()
{
    return addr_;
}

const void *Reflector::data() const
{
    return addr_;
}

void *Reflector::lower()
{
    return addr_;
}

const void *Reflector::lower() const
{
    return addr_;
}

void *Reflector::upper()
{
    return static_cast<char *>(addr_) + size_;
}

const void *Reflector::upper() const
{
    return static_cast<char *>(addr_) + size_;
}

size_t Reflector::size() const
{
    return size_;
}

