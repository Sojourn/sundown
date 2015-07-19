#ifndef SUNDOWN_CORE_MESSAGE_STREAM_H
#define SUNDOWN_CORE_MESSAGE_STREAM_H

namespace Sundown {

    class MessageStream;
    class MessageStreamWriter;
    class MessageStreamReader;

    class MessageStreamWriter
    {
    public:
        MessageStreamWriter(std::shared_ptr<MessageStream> stream);

        void *claim(size_t );
        void commit();

    private:
        std::shared_ptr<MessageStream> stream_;
    };

    class MessageStreamReader
    {
    public:
        MessageStreamReader(std::shared_ptr<MessageStream> stream);

    private:
        std::shared_ptr<MessageStream> stream_;
    };

    class MessageStream
    {
        friend class MessageStreamWriter;
        friend class MessageStreamReader;
    public:
        Optional<std::shared_ptr<MessageStream>> create(size_t capacity);

    private:
        std::atomic_uint_fast64_t writer_;
        std::atomic_uint_fast64_t reader_;
        Reflector reflector_;
    };

}

#endif // SUNDOWN_CORE_MESSAGE_STREAM_H

