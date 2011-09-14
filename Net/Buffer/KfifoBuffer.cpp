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
    readIndexM = 0;
    writeIndexM = 0;
}

//-----------------------------------------------------------------------------

BufferStatus KfifoBuffer::getStatus()
{
	return (writeIndexM - readIndexM > highWaterMarkM) ? BufferHighE : BufferOkE;
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

size_t KfifoBuffer::commitRead(const size_t theLen)
{
    readIndexM += theLen;
    return theLen;
}


