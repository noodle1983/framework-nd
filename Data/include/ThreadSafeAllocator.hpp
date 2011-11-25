#ifndef THREADSAFEALLOCATOR_HPP
#define THREADSAFEALLOCATOR_HPP

#include <vector>
#include <boost/thread.hpp>

namespace Data 
{
    template<typename DataType, unsigned long long batchCount>
    class ThreadSafeAllocator
    {
    public:        
        
        ThreadSafeAllocator()
        {}
        ~ThreadSafeAllocator()
        {}

        DataType* newData()
        {
            boost::lock_guard<boost::mutex> lock(allocatorMutexM);
            return allocatorM.newData();
        }

        void freeData(DataType* theData)
        {
            boost::lock_guard<boost::mutex> lock(allocatorMutexM);
            allocatorM.freeData(theData);
        }

    private:
        Allocator<DataType, batchCount> allocatorM;
        boost::mutex allocatorMutexM;
    };
}
#endif /* THREADSAFEALLOCATOR_HPP */

