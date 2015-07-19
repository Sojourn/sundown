#ifndef SUNDOWN_SYS_REFLECTOR_H
#define SUNDOWN_SYS_REFLECTOR_H

namespace Sundown {

    // Two adjacent pieces of virtual address space that map
    // to the same physical memory.
    class Reflector
    {
    public:
        static Optional<Reflector> create(size_t size);

        Reflector(const Reflector &) = delete;
        Reflector(Reflector &&other);
        ~Reflector();

        Reflector &operator=(const Reflector &) = delete;
        Reflector &operator=(Reflector &&rhs);

        const FileDescriptor &fd() const;

        void *data();
        const void *data() const;

        void *lower();
        const void *lower() const;

        void *upper();
        const void *upper() const;

        size_t size() const;

    private:
        Reflector(FileDescriptor fd, void *addr, size_t size);
        void reset();

    private:
        void *addr_;
        size_t size_;
        FileDescriptor fd_;
    };

}

#endif // SUNDOWN_SYS_REFLECTOR_H

