#ifndef WINDOWS_PORT_HPP
#define WINDOWS_PORT_HPP

#include <assert.h>

/****************
 * int relate
 ****************/
#include <stdint.h>
#define MAX_PATH 250
#define _MAX_PATH 250
#define TRUE 1
#define FALSE 0
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef bool BOOL;

#define _atoi64 atoll
static inline void _i64toa(__int64 i, char* const a, int base)
{
    assert(base == 10);
    sprintf(a, "%llu", (long long unsigned int)i);
}

/****************
 * mem/str relate
 ****************/
#include <string.h>
#define ZeroMemory(str, len) memset(str, 0, len)

/****************
 * mutex
 ****************/
#include <pthread.h>
typedef pthread_mutex_t* LPCRITICAL_SECTION;
#define CRITICAL_SECTION pthread_mutex_t 
#define EnterCriticalSection  pthread_mutex_lock
#define LeaveCriticalSection  pthread_mutex_unlock
#define InitializeCriticalSection(m) pthread_mutex_init(m, NULL)
#define DeleteCriticalSection pthread_mutex_destroy

/****************
 * file relate
 ****************/
#include <stdio.h>
#define MoveFile(oldFile, newFile) (!rename(oldFile, newFile))

/****************
 * socket
 ****************/
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>



#endif /* WINDOWS_PORT_HPP */
