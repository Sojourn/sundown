#include "sys/sys.h"

using namespace Sundown;

Optional<Reflector> Reflector::create(size_t size)
{
    static const char *path = "reflector.shm";
    FileDescriptor fd;
    void *lower = nullptr;
    void *upper = nullptr;
    void *both = nullptr;
    int sts = -1;

    if ((size == 0) || (size % 4096)) {
        return Optional<Reflector>();
    }

    fd.reset(shm_open(path, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR));
    if (fd.empty()) {
        return Optional<Reflector>();
    }

    sts = shm_unlink(path);
    if (sts < 0) {
        return Optional<Reflector>();
    }

    sts = ftruncate(fd.get(), size);
    if (sts < 0) {
        return Optional<Reflector>();
    }
    
    both = mmap(nullptr, size * 2, PROT_READ | PROT_WRITE, MAP_SHARED, fd.get(), 0);
    if (both == MAP_FAILED) {
        return Optional<Reflector>();
    }

    lower = mremap(both, size * 2, size, 0);
    if (lower == MAP_FAILED) {
        sts = munmap(both, size * 2);
        assert(sts >= 0);
        return Optional<Reflector>();
    }
    else if (lower != both) {
        sts = munmap(lower, size);
        assert(sts);
        return Optional<Reflector>();
    }

    upper = static_cast<uint8_t*>(lower) + size;
    upper = mmap(upper, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FIXED, fd.get(), 0);
    if (upper == MAP_FAILED) {
        sts = munmap(lower, size);
        assert(sts >= 0);
        return Optional<Reflector>();
    }

    return Optional<Reflector>(Reflector(std::move(fd), lower, size));
}

Reflector::Reflector(FileDescriptor fd, void *addr, size_t size)
    : addr_(addr)
    , size_(size)
    , fd_(std::move(fd))
{
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

