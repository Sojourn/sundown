#ifndef SUNDOWN_SYS_FD_H
#define SUNDOWN_SYS_FD_H

namespace Sundown {

    class FileDescriptor
    {
    public:
        FileDescriptor(int fd = -1);
        FileDescriptor(const FileDescriptor &) = delete;
        FileDescriptor(FileDescriptor &&other);
        ~FileDescriptor();

        FileDescriptor &operator=(const FileDescriptor &) = delete;
        FileDescriptor &operator=(FileDescriptor &&rhs);

        int get() const;
        bool empty() const;
        void reset(int fd = -1);

    private:
        int fd_;
    };

}

#endif // SUNDOWN_SYS_FD_H

