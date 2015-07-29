#ifndef SUNDOWN_CORE_MESSAGE_STREAM_H
#define SUNDOWN_CORE_MESSAGE_STREAM_H

namespace Sundown {

    class MessageStream;
    class MessageStreamWriter;
    class MessageStreamReader;

    class Message
    {
        friend class MessageStreamWriter;
    public:
        Message();
        Message(uint64_t begin, uint64_t end, uint64_t size);
        Message(Message &&other);
        Message(const Message &) = delete;
        ~Message();

        Message &operator=(Message &&rhs);
        Message &operator=(const Message &) = delete;

        operator bool() const;
        bool empty() const;

    private:
        void reset();

    private:
        uint64_t begin_;
        uint64_t end_;
        uint64_t size_;
    };

    class MessageStreamCursor
    {
    public:
        MessageStreamCursor(std::shared_ptr<MessageStream> stream);

        uint8_t *addr(uint64_t offset);
        const uint8_t *addr(uint64_t offset) const;

        uint64_t mask() const;
        uint64_t size() const;

        MessageStream &stream();
        const MessageStream &stream() const;

    private:
        std::shared_ptr<MessageStream> stream_;
        uint64_t mask_;
        uint64_t size_;
        void *base_;
    };

    class MessageStreamWriter
    {
    public:
        MessageStreamWriter(std::shared_ptr<MessageStream> stream);

        Message claim(size_t size);
        Optional<Buffer> prepare(const Message &message);
        void commit(Message message);

    private:
        bool readyFast(const Message &message);
        bool readySlow(const Message &message);

    private:
        MessageStreamCursor cursor_;
        uint64_t head_;
    };

    class MessageStreamReader
    {
    public:
        MessageStreamReader(std::shared_ptr<MessageStream> stream);

        Optional<Buffer> read();

    private:
        void clean();

    private:
        MessageStreamCursor cursor_;
        uint64_t head_;
        uint64_t tail_;
    };

    class MessageStream
    {
        friend class MessageStreamCursor;
        friend class MessageStreamWriter;
        friend class MessageStreamReader;
    public:
        static Optional<std::shared_ptr<MessageStream>> create(size_t capacity);

        MessageStream(Reflector reflector);
        MessageStream(const MessageStream &) = delete;
        MessageStream(MessageStream &&) = delete;

        MessageStream &operator=(const MessageStream &) = delete;
        MessageStream &operator=(MessageStream &&) = delete;

    private:
        std::atomic_uint_fast64_t writer_;
        std::atomic_uint_fast64_t reader_;
        Reflector reflector_;
    };

}

#endif // SUNDOWN_CORE_MESSAGE_STREAM_H

