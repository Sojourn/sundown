#ifndef SUNDOWN_STRING_REF_H
#define SUNDOWN_STRING_REF_H

namespace Sundown {
    
    class StringRef
    {
    public:
        StringRef();
        StringRef(const char *str);
        StringRef(const char *first, const char *last);
        StringRef(const String &str);

        const char *begin() const;
        const char *end() const;
        size_t size() const;
        bool empty() const;

    private:
        const char *str_;
        size_t len_;
    };

}

#endif // SUNDOWN_STRING_REF_H

