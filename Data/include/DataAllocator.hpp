#ifndef DATAALLOCATOR_HPP
#define DATAALLOCATOR_HPP

#include <vector>
#include <boost/thread.hpp>
#include <assert.h>

namespace Data 
{
    /*
     * implicit interface
     *as it is a template parameter in Allocator
     *
public:
    typedef Data::Allocator<..., ...> Allocator;
    typedef ImbededClass DataType;
    union
    {
        Allocator* allocatorM; 
        DataType* nextFreeM;
    };
    */

    class NullMutex
    {
    public:
        inline void lock(){}
        inline bool try_lock(){return true;}
        inline void unlock(){}
    };

    template<typename DataType, 
        unsigned long long batchCount,
        typename NewMutex = NullMutex,
        typename FreeMutex = NullMutex>
    class Allocator
    {
    public:        
        enum{ BATCH_COUNT = batchCount};
        typedef std::vector<DataType*> DataCacheVector;
        
        Allocator()
            : freeHeaderM(NULL)
            , freeTailerM(NULL)
			, sizeM(0)
			, newCounterM(0)
			, freeCounterM(0)
        {}
        ~Allocator()
        {
			sizeM = 0;
            int vectorSize = dataCachesM.size();
            for (int i = 0; i < vectorSize; i++)
            {
                delete[] (dataCachesM[i]);
            }
        }

        DataType* newData()
        {
            boost::lock_guard<NewMutex> lock(newMutexM);
            newCounterM++;
            if (freeHeaderM != freeTailerM)
            {
                DataType* res = freeHeaderM;
                freeHeaderM = res->nextFreeM;
                res->allocatorM = this;
                return res;
            }
            else
            {
                DataType* newDataCache = new DataType[BATCH_COUNT]; 
				sizeM += BATCH_COUNT;
                for (unsigned long long i = 0; i < BATCH_COUNT - 1; i++)
                {
                    newDataCache[i].nextFreeM = &newDataCache[i+1];
                }
                newDataCache[BATCH_COUNT-1].nextFreeM = freeHeaderM;
                dataCachesM.push_back(newDataCache);

                if (NULL == freeHeaderM)
                {
                    freeTailerM = &newDataCache[BATCH_COUNT-1];
                }
                freeHeaderM = &newDataCache[1];
                newDataCache[0].allocatorM = this;
                return newDataCache;
            }
        }

        void freeData(DataType* theData)
        {
            assert(theData->allocatorM == this);
            boost::lock_guard<FreeMutex> lock(freeMutexM);
            freeCounterM++;
            freeTailerM->nextFreeM = theData;
            freeTailerM = theData;
            freeTailerM->nextFreeM = NULL;
        }

		unsigned long long getSize()
		{
			return sizeM;
		}

		unsigned long long getUsed()
		{
			return newCounterM - freeCounterM;
		}
    private:
        DataCacheVector dataCachesM;
        DataType* freeHeaderM;
        DataType* freeTailerM;
		unsigned long long sizeM;
		unsigned long long newCounterM;
		unsigned long long freeCounterM;
        NewMutex newMutexM;
        FreeMutex freeMutexM;
    };
}
#endif /* DATAALLOCATOR_HPP */

