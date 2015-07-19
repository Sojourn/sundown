#ifndef SUNDOWN_SYS_PIPE_H
#define SUNDOWN_SYS_PIPE_H

namespace Sundown {

    class Pipe
    {
    public:
        static Optional<std::tuple<Pipe, Pipe>> create();

        Pipe(FileDescriptor fd);
        Pipe(const Pipe &) = delete;
        Pipe(Pipe &&) = default;

        Pipe &operator=(const Pipe &) = delete;
        Pipe &operator=(Pipe &&) = default;

        const FileDescriptor &fd() const;

    private:
        FileDescriptor fd_;
    };

}

#endif // SUNDOWN_SYS_PIPE_H

