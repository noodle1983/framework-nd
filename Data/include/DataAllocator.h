#ifndef DATAALLOCATOR_H
#define DATAALLOCATOR_H

#include <boost/pool/pool.hpp>

namespace Data 
{
    template<typename DataType>
    class Allocator
    {
    public:        
        Allocator()
            : poolM(sizeof(DataType))
        {}
        ~Allocator()
        {}

        inline DataType* newData()
        {
            void* data = poolM.malloc();
            return new(data) DataType;
        }
        template<typename ParamType1>
        inline DataType* newData(ParamType1& param1)
        {
            void* data = poolM.malloc();
            return new(data) DataType(param1);
        }
        inline void freeData(DataType* theData)
        {
            theData->~DataType();
            poolM.free((void*)theData);
        }
    private:
        boost::pool<> poolM;
    };
}
#endif /* DATAALLOCATOR_H */
