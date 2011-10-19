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
    lowWaterMarkM = sizeM >> 1;    // 50%
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
    rawM = NULL;
    readIndexM = 0;
    writeIndexM = 0;
}

//-----------------------------------------------------------------------------

size_t KfifoBuffer::put(const char* const theBuffer, const size_t theLen)
{
    size_t leftSize = sizeM - writeIndexM + readIndexM;
    size_t putLen = (theLen < leftSize) ? theLen : leftSize;

    size_t leftEnd = sizeM - (writeIndexM & maskM);
    size_t firstPartLen = (putLen < leftEnd) ? putLen : leftEnd;
    memcpy(rawM + (writeIndexM & maskM), theBuffer, firstPartLen);
    memcpy(rawM, theBuffer + firstPartLen, putLen - firstPartLen);
    writeIndexM += putLen;
    return putLen;
}

//-----------------------------------------------------------------------------

size_t KfifoBuffer::get(char* const theBuffer, const size_t theLen)
{
    size_t usedSize = writeIndexM - readIndexM;
    size_t getLen = (theLen < usedSize) ? theLen : usedSize;

    size_t readIndexToEnd = sizeM - (readIndexM & maskM);
    size_t firstPartLen = (getLen < readIndexToEnd) ? getLen : readIndexToEnd;
    memcpy(theBuffer, rawM + (readIndexM & maskM), firstPartLen);
    memcpy(theBuffer + firstPartLen, rawM, getLen - firstPartLen);
    readIndexM += getLen;
    return getLen;
}

//-----------------------------------------------------------------------------

size_t KfifoBuffer::peek(char* const theBuffer, const size_t theLen)
{
    size_t usedSize = writeIndexM - readIndexM;
    size_t getLen = (theLen < usedSize) ? theLen : usedSize;

    size_t readIndexToEnd = sizeM - (readIndexM & maskM);
    size_t firstPartLen = (getLen < readIndexToEnd) ? getLen : readIndexToEnd;
    memcpy(theBuffer, rawM + (readIndexM & maskM), firstPartLen);
    memcpy(theBuffer + firstPartLen, rawM, getLen - firstPartLen);
    return getLen;
}

//-----------------------------------------------------------------------------

size_t KfifoBuffer::putn(const char* const theBuffer, const size_t theLen)
{
    size_t leftSize = sizeM - writeIndexM + readIndexM;
    if (theLen > leftSize)
        return 0;
    size_t putLen = theLen;

    size_t leftEnd = sizeM - (writeIndexM & maskM);
    size_t firstPartLen = (putLen < leftEnd) ? putLen : leftEnd;
    memcpy(rawM + (writeIndexM & maskM), theBuffer, firstPartLen);
    memcpy(rawM, theBuffer + firstPartLen, putLen - firstPartLen);
    writeIndexM += putLen;
    return putLen;
}

//-----------------------------------------------------------------------------

size_t KfifoBuffer::getn(char* const theBuffer, const size_t theLen)
{
    size_t usedSize = writeIndexM - readIndexM;
    if (theLen > usedSize)
        return 0;
    size_t getLen = theLen;

    size_t readIndexToEnd = sizeM - (readIndexM & maskM);
    size_t firstPartLen = (getLen < readIndexToEnd) ? getLen : readIndexToEnd;
    memcpy(theBuffer, rawM + (readIndexM & maskM), firstPartLen);
    memcpy(theBuffer + firstPartLen, rawM, getLen - firstPartLen);
    readIndexM += getLen;
    return getLen;
}

//-----------------------------------------------------------------------------

size_t KfifoBuffer::peekn(char* const theBuffer, const size_t theLen)
{
    size_t usedSize = writeIndexM - readIndexM;
    if (theLen > usedSize)
        return 0;
    size_t getLen = theLen;

    size_t readIndexToEnd = sizeM - (readIndexM & maskM);
    size_t firstPartLen = (getLen < readIndexToEnd) ? getLen : readIndexToEnd;
    memcpy(theBuffer, rawM + (readIndexM & maskM), firstPartLen);
    memcpy(theBuffer + firstPartLen, rawM, getLen - firstPartLen);
    return getLen;
}

//-----------------------------------------------------------------------------

size_t KfifoBuffer::commitRead(const size_t theLen)
{
    readIndexM += theLen;
    return theLen;
}


