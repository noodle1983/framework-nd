#include "Buffer/KfifoBuffer.h"
#include "string.h"

using namespace Net::Buffer;

//-----------------------------------------------------------------------------

KfifoBuffer::KfifoBuffer()
{
    sizeM = 1 << 20; //1MB
    maskM = sizeM - 1;
    rawM = new char[sizeM];
    readIndexM = 0;
    writeIndexM = 0;
    highWaterMarkM = sizeM *4 / 5; // 80%
}

//-----------------------------------------------------------------------------

KfifoBuffer::~KfifoBuffer()
{
    release();
}

//-----------------------------------------------------------------------------

void KfifoBuffer::init()
{
}

//-----------------------------------------------------------------------------

void KfifoBuffer::release()
{
    delete[] rawM;
    readIndexM = 0;
    writeIndexM = 0;
}

//-----------------------------------------------------------------------------

int KfifoBuffer::put(const char* const theBuffer, const size_t theLen)
{
    size_t leftSize = sizeM - writeIndexM + readIndexM;
    if (theLen > leftSize)
        return BufferNotEnoughE;
    
    size_t leftEnd = sizeM - (writeIndexM & maskM);
    size_t firstPartLen = (theLen < leftEnd) ? theLen : leftEnd;
    memcpy(rawM + (writeIndexM & maskM), theBuffer, firstPartLen);
    memcpy(rawM, theBuffer + firstPartLen, theLen - firstPartLen);
    writeIndexM += theLen;
    return (writeIndexM - readIndexM > highWaterMarkM) ? BufferHighE : BufferOkE;
}

//-----------------------------------------------------------------------------

int KfifoBuffer::get(char* const theBuffer, const size_t theLen)
{
    size_t usedSize = writeIndexM - readIndexM;
    if (theLen > usedSize)
        return BufferNotEnoughE;
    
    size_t readIndexToEnd = sizeM - (readIndexM & maskM);
    size_t firstPartLen = (theLen < readIndexToEnd) ? theLen : readIndexToEnd;
    memcpy(theBuffer, rawM + (readIndexM & maskM), firstPartLen);
    memcpy(theBuffer + firstPartLen, rawM, theLen - firstPartLen);
    readIndexToEnd += theLen;
    return (writeIndexM - readIndexM > highWaterMarkM) ? BufferHighE : BufferOkE;
}

//-----------------------------------------------------------------------------

int KfifoBuffer::peek(char* const theBuffer, const size_t theLen)
{
    size_t usedSize = writeIndexM - readIndexM;
    if (theLen > usedSize)
        return -1;
    
    size_t readIndexToEnd = sizeM - (readIndexM & maskM);
    size_t firstPartLen = (theLen < readIndexToEnd) ? theLen : readIndexToEnd;
    memcpy(theBuffer, rawM + (readIndexM & maskM), firstPartLen);
    memcpy(theBuffer + firstPartLen, rawM, theLen - firstPartLen);
    return (writeIndexM - readIndexM > highWaterMarkM) ? BufferHighE : BufferOkE;
}

