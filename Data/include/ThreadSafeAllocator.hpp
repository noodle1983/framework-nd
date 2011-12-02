#ifndef THREADSAFEALLOCATOR_HPP
#define THREADSAFEALLOCATOR_HPP

#include "DataAllocator.hpp"

#include <boost/thread.hpp>

namespace Data 
{
    template<typename DataType, unsigned long long batchCount>
    class ThreadSafeAllocator
    {
    public:        
        enum{ALLOC_COUNT = 10};
        typedef Allocator<DataType, batchCount/ALLOC_COUNT, boost::mutex, boost::mutex> AllocatorType;

        
        ThreadSafeAllocator()
        {}
        ~ThreadSafeAllocator()
        {}

        DataType* newData(uint64_t theRandId)
        {
            return allocatorM[theRandId%ALLOC_COUNT].newData();
        }

        void freeData(DataType* theData)
        {
            theData->allocatorM->freeData(theData);
        }

		unsigned long long getSize()
		{
            unsigned long long size = 0;
			for (int i = 0; i < ALLOC_COUNT; i++)
            {
                size += allocatorM[i].getSize();
            }
            return size;
		}

		unsigned long long getUsed()
		{
            unsigned long long used = 0;
			for (int i = 0; i < ALLOC_COUNT; i++)
            {
                used += allocatorM[i].getUsed();
            }
            return used;
		}
    private:
        AllocatorType allocatorM[ALLOC_COUNT];
    };

}
#endif /* THREADSAFEALLOCATOR_HPP */

