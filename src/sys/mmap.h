#ifndef SUNDOWN_SYS_MMAP_H
#define SUNDOWN_SYS_MMAP_H

namespace Sundown {

    class MemoryMap
    {
    public:
        MemoryMap(const FileDescriptor &fd = FileDescriptor());
        ~MemoryMap();

        void resize(size_t len);

        void *data();
        const void *data() const;

    private:
        void *ptr_;
        size_t len_;
        FileDescriptor fd_;
    };

}

#endif // SUNDOWN_SYS_MMAP_H

