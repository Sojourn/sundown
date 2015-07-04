#ifndef SUNDOWN_SYS_SELECTOR_H
#define SUNDOWN_SYS_SELECTOR_H

namespace Sundown {

    class SelectorItem : public std::enable_shared_from_this<SelectorItem>
    {
    public:
        typedef std::shared_ptr<SelectorItem> SP;

        virtual ~SelectorItem();

        const FileDescriptor &fd() const;

    protected:
        SelectorItem(FileDescriptor &&fd);

    private:
        FileDescriptor fd_;
    };

    class SelectorEvent
    {
    public:
        SelectorItem::SP item() const;
        bool readable() const;
        bool writable() const;
        bool urgent() const;
        bool hangup() const;
        bool error() const;

    private:
        struct epoll_event event_;
    };
    static_assert(sizeof(SelectorEvent) == sizeof(struct epoll_event), "Check SelectorEvent layout");

    class Selector
    {
    public:
        typedef std::shared_ptr<Selector> SP;
        typedef std::shared_ptr<Selector> WP;

        enum Events {
            Readable = (1 << 0),
            Writable = (1 << 1),
        };

        static Option<Selector> create();

        Selector(const Selector &) = delete;
        Selector(Selector &&) = default;

        Selector &operator=(const Selector &) = delete;
        Selector &operator=(Selector &&) = default;
        

        void add(const SelectorItem::SP &item, Events events);
        void modify(const SelectorItem::SP &item, Events events);
        void remove(const SelectorItem::SP &item);

        Range<const SelectorEvent *> select();

    protected:
        Selector(FileDescriptor epollFd);

    private:
        FileDescriptor epollFd_;
        std::vector<SelectorItem::SP> items_;
        std::array<SelectorEvent, 64> events_;
    };
    
}

#endif // SUNDOWN_SYS_SELECTOR_H

