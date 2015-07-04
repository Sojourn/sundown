#ifndef SUNDOWN_UTIL_OPTION_H
#define SUNDOWN_UTIL_OPTION_H

namespace Sundown {

    template<typename T>
    class Option
    {
    public:
        Option()
            : empty_(true)
        {
        }

        Option(const Option<T> &) = delete;

        Option(Option<T> &&other)
        {
            new(buffer_) T(other.value());
            empty_ = false;
        }

        template<typename... Args>
        Option(Args&&... args)
        {
            new(buffer_) T(std::forward<Args>(args)...);
            empty_ = false;
        }

        ~Option()
        {
            if (!empty()) {
                value();
            }
        }

        Option<T> &operator=(const Option<T> &) = delete;

        Option<T> &operator=(Option<T> &&rhs)
        {
            if (this != &rhs) {
                value();

                if (!rhs.empty()) {
                    new(buffer_) T(rhs.value());
                    empty_ = false;
                }
            }

            return *this;
        }

        T value()
        {
            assert(!empty());

            empty_ = true;
            return std::move(*reinterpret_cast<T*>(buffer_));
        }

        bool empty() const
        {
            return empty_;
        }

    private:
        uint8_t SUNDOWN_ALIGN buffer_[sizeof(T)];
        bool empty_;
    };

}

#endif // SUNDOWN_UTIL_OPTION_H

