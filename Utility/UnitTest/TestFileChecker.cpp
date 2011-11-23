#include "FileWatcher.h"
#include "Log.h"

#include <iostream>
#include <event.h>
#include <event2/thread.h>
#include <unistd.h>
#include <signal.h>
#include <boost/thread.hpp>

using namespace std;

static int closed = false;
static boost::mutex closedMutexM;
static boost::condition_variable closedCondM;
void sig_stop(int sig)
{
    LOG_DEBUG("receive signal " << sig << ". stopping...");
    boost::lock_guard<boost::mutex> lock(closedMutexM);
    closed = true;
    closedCondM.notify_one();
}

void printFile(const std::string& theFile)
{
    cout << "reload "  << theFile  << endl; 
}

int main()
{
    evthread_use_pthreads();
    Utility::FileWatcher fileWatcher = Utility::FileWatcher(
                    "checker.txt", 
                    5, 
                    Utility::FileWatcherCallback(printFile));

    boost::unique_lock<boost::mutex> lock(closedMutexM);
    while(!closed)
    {
        closedCondM.wait(lock);
    }

    
}


