#ifndef MYSQLSTATEMENTSTRUCT_H
#define MYSQLSTATEMENTSTRUCT_H

#include <string>
#include <string.h>

namespace DBI
{
namespace Mysql
{
    template<typename IntType, enum_field_types MysqlTypeId>
    class Integer
    {
    public:

        Integer()
            : valueM(0)
            , isNullM(0)
        {}
        Integer(const IntType theValue)
            : valueM(theValue)
            , isNullM(0)
        {}
        Integer(MYSQL_BIND* theBindHandler)
            : valueM(0)
            , isNullM(0)
        {
            bind(theBindHandler);
        }
        Integer(MYSQL_BIND* theBindHandler, const IntType theValue)
            : valueM(theValue)
            , isNullM(0)
        {
            bind(theBindHandler);
        }

        ~Integer(){};

        operator IntType()
        { return isNullM ? 0 : valueM; }

        const IntType get(const IntType theDefault = 0)
        { return isNullM ? theDefault : valueM; }

        void set(const IntType theValue, const char theNullFlag = 0)
        { valueM = theValue; isNullM = theNullFlag;}

        void setNull()
        { valueM = 0; isNullM = 1;}

        void bind(MYSQL_BIND* theMysqlStruct)
        {
            ::memset((void*)theMysqlStruct, 0, sizeof(MYSQL_BIND));
            theMysqlStruct->buffer_type= MysqlTypeId;
            theMysqlStruct->buffer= (char*) &valueM;
            theMysqlStruct->is_null= &isNullM;
        }

    private:
        IntType valueM;
        my_bool isNullM;
    };

    typedef Integer<char,  MYSQL_TYPE_TINY>  Char;
    typedef Integer<short, MYSQL_TYPE_SHORT> ShortInt;
    typedef Integer<long,  MYSQL_TYPE_LONG>  Long;
    typedef Integer<long long, MYSQL_TYPE_LONGLONG>  LongLong;
    typedef Integer<float, MYSQL_TYPE_FLOAT> Float;
    typedef Integer<double, MYSQL_TYPE_DOUBLE> Double;

    class String
    {
    public:
        enum{PRE_ALLOC_LENGTH = 64};

//        String(const unsigned theFieldLen)
//            : resultLengthM(0)
//            , isNullM(0)
//        {
//            init(NULL, theFieldLen);
//        }
//
//        String(const char* theValue, const unsigned theFieldLen)
//            : resultLengthM(0)
//            , isNullM(0)
//        {
//            init(theValue, theFieldLen);
//        }

        String(MYSQL_BIND* theBindHandler, const unsigned theFieldLen)
        {
            init(NULL, theFieldLen);
            bind(theBindHandler);
        }

        String(MYSQL_BIND* theBindHandler, const char* theFieldValue, const unsigned theFieldLen)
            : resultLengthM(0)
            , isNullM(0)
        {
            init(theFieldValue, theFieldLen);
            bind(theBindHandler);
        }

        ~String()
        {
            if (bufferLengthM > PRE_ALLOC_LENGTH)
            { delete[] pBufferM; }
        }

        void init(const char* theValue, const unsigned theFieldLen)
        {
            if (theFieldLen > PRE_ALLOC_LENGTH)
            {
                bufferLengthM = theFieldLen;
                pBufferM = new char[bufferLengthM];
            }
            else
            {
                bufferLengthM = theFieldLen;
                pBufferM = bufferM;
            }

            if (NULL != theValue)
            {
                strncpy(pBufferM, theValue, bufferLengthM);
            }
            else
            {
                memset(pBufferM, 0, bufferLengthM);
            }
        }

        operator const char*()
        { return isNullM ? "" : pBufferM; }

        const char* get(const char* theDefault = "")
        { return isNullM ? theDefault : pBufferM; }

        const std::string getString(const char* theDefault = "")
        { return isNullM ? theDefault : std::string(pBufferM, resultLengthM); }

        void set(const char* theValue, const char theNullFlag = 0)
        { 
            if (NULL != theValue)
            {
                strncpy(pBufferM, theValue, bufferLengthM);
            }
            else
            {
                memset(pBufferM, 0, bufferLengthM);
            }

            isNullM = theNullFlag;
        }

        void setNull()
        { 
            memset(pBufferM, 0, bufferLengthM);
            isNullM = 1;
        }

        void bind(MYSQL_BIND* theMysqlStruct)
        {
            memset((void*)theMysqlStruct, 0, sizeof(MYSQL_BIND));
            theMysqlStruct->buffer_type= MYSQL_TYPE_STRING;
            theMysqlStruct->buffer= pBufferM;
            theMysqlStruct->buffer_length= bufferLengthM;
            theMysqlStruct->is_null= &isNullM;
            theMysqlStruct->length= (*pBufferM) ? NULL : &resultLengthM;
        }

    private:
        char bufferM[PRE_ALLOC_LENGTH];
        char* pBufferM;
        unsigned long bufferLengthM;
        unsigned long resultLengthM;
        my_bool isNullM;
    };

    
}
}
#endif /* MYSQLSTATEMENTSTRUCT_H */

