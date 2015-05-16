#ifndef SUNDOWN_SYS_ERROR_H
#define SUNDOWN_SYS_ERROR_H

namespace Sundown {

    class Error {
    public:
        bool ok() const;
        const char *msg() const;

    private:
        const char *msg_;
    };

}

#endif // SUNDOWN_SYS_ERROR_H

