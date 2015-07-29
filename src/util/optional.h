#ifndef SUNDOWN_UTIL_OPTION_H
#define SUNDOWN_UTIL_OPTION_H

namespace Sundown {

    struct None {};
    struct Some {};

    template<typename T>
    class Optional
    {
    public:
        Optional()
            : empty_(true)
        {
        }

        Optional(None)
            : empty_(true)
        {
        }

        Optional(const Optional<T> &) = delete;

        Optional(Optional<T> &&other)
        {
            if (other.empty()) {
                empty_ = true;
            }
            else {
                new(buffer_) T(other.get());
                empty_ = false;
            }
        }

        Optional(const T &value)
        {
            new(buffer_) T(value);
            empty_ = false;
        }

        Optional(T &&value)
        {
            new(buffer_) T(std::move(value));
            empty_ = false;
        }

        ~Optional()
        {
            if (!empty()) {
                get();
            }
        }

        Optional<T> &operator=(const Optional<T> &) = delete;

        Optional<T> &operator=(Optional<T> &&rhs)
        {
            if (this != &rhs) {
                get();

                if (!rhs.empty()) {
                    new(buffer_) T(rhs.get());
                    empty_ = false;
                }
            }

            return *this;
        }

        T get()
        {
            assert(!empty());

            empty_ = true;
            return std::move(*reinterpret_cast<T*>(buffer_));
        }

        bool empty() const
        {
            return empty_;
        }

        operator bool() const
        {
            return !empty_;
        }

    private:
        alignas(alignof(T)) uint8_t buffer_[sizeof(T)];
        bool empty_;
    };

}

#endif // SUNDOWN_UTIL_OPTION_H

