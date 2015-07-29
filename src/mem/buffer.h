#ifndef SUNDOWN_MEM_BUFFER_H
#define SUNDOWN_MEM_BUFFER_H

namespace Sundown {

    class Buffer
    {
    public:
        Buffer(uint8_t *first, uint8_t *last);
        
        uint8_t *begin();
        const uint8_t *begin() const;
        uint8_t *end();
        const uint8_t *end() const;

        size_t size() const;
        bool empty() const;

    private:
        uint8_t *first_;
        uint8_t *last_;
    };

}

#endif // SUNDOWN_MEM_BUFFER_H

