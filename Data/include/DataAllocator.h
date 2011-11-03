#ifndef DATAALLOCATOR_H
#define DATAALLOCATOR_H

#include <glib.h>

namespace Data 
{
    template<typename DataType>
    class Allocator
    {
    public:        
        static inline DataType* newData()
        {
            DataType* data = g_slice_new(DataType);
            data = new(data) DataType;
            return data;
        }
        static inline void freeData(DataType* theData)
        {
            theData->~DataType();
            g_slice_free(DataType, theData);
        }
    };
}
#endif /* DATAALLOCATOR_H */

