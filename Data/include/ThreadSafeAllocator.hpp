#ifndef THREADSAFEALLOCATOR_HPP
#define THREADSAFEALLOCATOR_HPP

#include "DataAllocator.hpp"

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

		unsigned long long getSize()
		{
			return allocatorM.getSize();
		}

		unsigned long long getUsed()
		{
			return allocatorM.getUsed();
		}
    private:
        Allocator<DataType, batchCount> allocatorM;
        boost::mutex allocatorMutexM;
    };
}
#endif /* THREADSAFEALLOCATOR_HPP */

