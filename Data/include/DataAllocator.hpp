#ifndef DATAALLOCATOR_HPP
#define DATAALLOCATOR_HPP

#include <vector>

namespace Data 
{
    //implicit interface
    //as it is a template parameter in Allocator
    struct ICacheable
    {
        ICacheable* nextFreeM;
    };

    template<typename DataType, unsigned long long batchCount>
    class Allocator
    {
    public:        
        enum{ BATCH_COUNT = batchCount};
        typedef std::vector<DataType*> DataCacheVector;
        
        Allocator()
            : freeHeaderM(NULL)
			, sizeM(0)
			, usedM(0)
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
			usedM++;
            if (freeHeaderM)
            {
                DataType* res = freeHeaderM;
                freeHeaderM = res->nextFreeM;
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
                newDataCache[BATCH_COUNT-1].nextFreeM = NULL;
                dataCachesM.push_back(newDataCache);

                freeHeaderM = &newDataCache[1];
                return newDataCache;
            }
        }

        void freeData(DataType* theData)
        {
			usedM--;
            theData->nextFreeM = freeHeaderM;
            freeHeaderM = theData;
        }

		unsigned long long getSize()
		{
			return sizeM;
		}

		unsigned long long getUsed()
		{
			return usedM;
		}
    private:
        DataCacheVector dataCachesM;
        DataType* freeHeaderM;
		unsigned long long sizeM;
		unsigned long long usedM;
    };
}
#endif /* DATAALLOCATOR_HPP */

