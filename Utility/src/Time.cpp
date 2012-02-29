#include "Time.h"
#include <stdio.h>
#include <string.h>

    void convert2SqlStr(struct timeval& theTime, char* theBuffer, const unsigned theBufferLen)
    {
        struct tm tmTime;
        localtime_r((time_t*)&theTime.tv_sec, &tmTime);

        snprintf(theBuffer, theBufferLen, 
                "%04d-%02d-%02d %02d:%02d:%02d.%06u",
                tmTime.tm_year + 1900,
                tmTime.tm_mon + 1,
                tmTime.tm_mday,
                tmTime.tm_hour,
                tmTime.tm_min,
                tmTime.tm_sec,
                (unsigned)theTime.tv_usec
                );
    }
    void convert2SqlStr(const unsigned theSec, const unsigned theUSec, char* theBuffer, const unsigned theBufferLen)
    {
        struct timeval tv;
        tv.tv_sec = theSec;
        tv.tv_usec = theUSec;
        convert2SqlStr(tv, theBuffer, theBufferLen);
    }

