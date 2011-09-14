#ifndef KFIFOBUFFER_H
#define KFIFOBUFFER_H


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

        BufferStatus put(const char* const theBuffer, const size_t theLen);
        BufferStatus get(char* const theBuffer, const size_t theLen);
        BufferStatus peek(char* const theBuffer, const size_t theLen);

    private:
        char* rawM;
        size_t sizeM;
        size_t readIndexM;
        size_t writeIndexM;
        size_t highWaterMarkM
    };
}
}

#endif /* KFIFOBUFFER_H */

