#ifndef TIME_H
#define TIME_H

#include <unistd.h>
#include <sys/time.h>
#include <time.h>

void convert2SqlStr(struct timeval& theTime, char* theBuffer, const unsigned theBufferLen);
void convert2SqlStr(const unsigned theSec, const unsigned theUSec, char* theBuffer, const unsigned theBufferLen);

#endif /* TIME_H */

