#ifndef SUNDOWN_SYS_STREAM_WRITER_H
#define SUNDOWN_SYS_STREAM_WRITER_H

namespace Sundown {

    class StreamWriter
    {
    public:
        static Optional<StreamWriter> create();

        void *begin(size_t size);
        void commit();
        void rollback();

        void flush(const FileDescriptor &fd);
        size_t writable() const;
        size_t backlog() const;

    private:
        StreamWriter(Reflector reflector);

        void *reflectorData(uint64_t offset);
        size_t reflectorSpace() const;
        
    private:
        const uint64_t mask_;
        uint64_t writer_;
        uint64_t reader_;
        uint64_t pending_;
        Reflector reflector_;
        std::vector<char> backlog_;
    };

}

#endif // SUNDOWN_SYS_STREAM_WRITER_H
