#include "sys.h"

using namespace Sundown;

SelectorItem::SelectorItem(FileDescriptor &&fd)
    : fd_(std::move(fd))
{
}

SelectorItem::~SelectorItem()
{
}

const FileDescriptor &SelectorItem::fd() const
{
    return fd_;
}

SelectorItem::SP SelectorEvent::item() const
{
    return reinterpret_cast<SelectorItem*>(event_.data.ptr)->shared_from_this();
}

bool SelectorEvent::readable() const
{
    return event_.events & EPOLLIN;
}

bool SelectorEvent::writable() const
{
    return event_.events & EPOLLOUT;
}

bool SelectorEvent::urgent() const
{
    return event_.events & EPOLLPRI;
}

bool SelectorEvent::hangup() const
{
    return event_.events & EPOLLHUP;
}

bool SelectorEvent::error() const
{
    return event_.events & EPOLLERR;
}

Selector::Selector()
{
    epollFd_.reset(epoll_create1(EPOLL_CLOEXEC));
    if (epollFd_.empty()) {
        throw std::runtime_error("Selector failed to open epoll file descriptor");
    }
}

void Selector::add(const SelectorItem::SP &item, Events events)
{
    struct epoll_event event;
    std::memset(&event, 0, sizeof(event));
    event.data.ptr = item.get();

    if (events & Readable) {
        event.events |= EPOLLIN;
    }

    if (events & Writable) {
        event.events |= EPOLLOUT;
    }

    int sts = epoll_ctl(epollFd_.get(), EPOLL_CTL_ADD, (item->fd()).get(), &event);
    if (sts < 0) {
        throw std::runtime_error("Selector unable to add file descriptor to watch list");
    }

    items_.push_back(item); 
}

void Selector::modify(const SelectorItem::SP &item, Events events)
{
    struct epoll_event event;
    std::memset(&event, 0, sizeof(event));
    event.data.ptr = item.get();

    if (events & Readable) {
        event.events |= EPOLLIN;
    }

    if (events & Writable) {
        event.events |= EPOLLOUT;
    }

    int sts = epoll_ctl(epollFd_.get(), EPOLL_CTL_ADD, (item->fd()).get(), &event);
    if (sts < 0) {
        throw std::runtime_error("Selector unable to modify watch list item");
    }
}

void Selector::remove(const SelectorItem::SP &item)
{
    struct epoll_event event;
    std::memset(&event, 0, sizeof(event));

    int sts = epoll_ctl(epollFd_.get(), EPOLL_CTL_DEL, (item->fd()).get(), &event);
    if (sts < 0) {
        throw std::runtime_error("Selector unable to remove item from watch list");
    }

    for (auto iter = items_.begin(); iter != items_.end(); ++iter) {
        if (*iter == item) {
            items_.erase(iter);
            break;
        }
    }
}

Range<const SelectorEvent *> Selector::select()
{
    int cnt = epoll_wait(epollFd_.get(), reinterpret_cast<struct epoll_event *>(&events_[0]), events_.size(), -1);
    if (cnt < 0) {
        throw std::runtime_error("Selector unable to select");
    }

    return Range<const SelectorEvent *>(&events_[0], &events_[cnt]);
}

