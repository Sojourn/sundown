#ifndef SUNDOWN_SOCKET_H
#define SUNDOWN_SOCKET_H

namespace Sundown {

    class Socket
    {
    public:
        enum class Transport {
            Tcp,
            Udp,
            Unix,
            Pipe,
        };

        Socket(Transport transport);

    private:
    };
    
}

#endif // SUNDOWN_SOCKET_H

