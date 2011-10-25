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
        BufferHighE = 1,
        BufferLowE = 2
    };

    class KfifoBuffer
    {
    public:
        KfifoBuffer();
        ~KfifoBuffer();

        void init();
        void release();

        inline BufferStatus getStatus()
        {
            size_t s = size();
            return (s > highWaterMarkM) ? BufferHighE
                 : (s > lowWaterMarkM)  ? BufferOkE
                 : BufferLowE;
        }
        inline bool isHealthy()
        {
            return (size() < highWaterMarkM); 
        }
        inline bool empty() {return writeIndexM == readIndexM;}
        inline size_t size(){return (writeIndexM - readIndexM);}
        inline size_t unusedSize(){return sizeM - (writeIndexM - readIndexM);}

        size_t put(const char* const theBuffer, const size_t theLen);
        size_t get(char* const theBuffer, const size_t theLen);
        size_t peek(char* const theBuffer, const size_t theLen);
        size_t putn(const char* const theBuffer, const size_t theLen);
        size_t getn(char* const theBuffer, const size_t theLen);
        size_t peekn(char* const theBuffer, const size_t theLen);
        size_t commitRead(const size_t theLen);

    private:
        char* rawM;
        size_t sizeM;
        size_t maskM;
        mutable size_t readIndexM;
        mutable size_t writeIndexM;
        size_t highWaterMarkM;
        size_t lowWaterMarkM;
    };
}
}

#endif /* KFIFOBUFFER_H */

