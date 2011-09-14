#ifndef KFIFOBUFFER_H
#define KFIFOBUFFER_H

#include <sys/types.h>

namespace Net
{
namespace Buffer
{
    enum BufferStatus
    {
        BufferNotEnoughE = -1,
        BufferOkE = 0,
        BufferHighE = 1
    };

    class KfifoBuffer
    {
    public:
        KfifoBuffer();
        ~KfifoBuffer();

        void init();
        void release();

        BufferStatus getStatus();
        size_t put(const char* const theBuffer, const size_t theLen);
        size_t get(char* const theBuffer, const size_t theLen);
        size_t peek(char* const theBuffer, const size_t theLen);
        size_t commitRead(const size_t theLen);

    private:
        char* rawM;
        size_t sizeM;
        size_t maskM;
        size_t readIndexM;
        size_t writeIndexM;
        size_t highWaterMarkM;
        size_t lowWaterMarkM;
    };
}
}

#endif /* KFIFOBUFFER_H */

