#ifndef MYSQLSTATEMENTSTRUCT_H
#define MYSQLSTATEMENTSTRUCT_H

#include <string>
#include <string.h>
#include <mysql.h>

namespace DBI
{
namespace Mysql
{
    template<typename IntType, enum_field_types MysqlTypeId>
    class Number
    {
    public:

        Number()
            : valueM(0)
            , isNullM(0)
        {}
        Number(const IntType theValue)
            : valueM(theValue)
            , isNullM(0)
        {}
        Number(MYSQL_BIND* theBindHandler)
            : valueM(0)
            , isNullM(0)
        {
            bind(theBindHandler);
        }
        Number(MYSQL_BIND* theBindHandler, const IntType theValue)
            : valueM(theValue)
            , isNullM(0)
        {
            bind(theBindHandler);
        }

        ~Number(){};

        operator IntType()
        { return isNullM ? 0 : valueM; }

        const IntType get(const IntType theDefault = 0)
        { return isNullM ? theDefault : valueM; }

        void set(const IntType theValue)
        { valueM = theValue; isNullM = false;}

        void setNull()
        { valueM = 0; isNullM = 1;}

        void bind(MYSQL_BIND* theMysqlStruct)
        {
            ::memset((void*)theMysqlStruct, 0, sizeof(MYSQL_BIND));
            theMysqlStruct->buffer_type= MysqlTypeId;
            theMysqlStruct->buffer= (char*) &valueM;
            theMysqlStruct->buffer_length= sizeof(valueM);
            theMysqlStruct->is_null= &isNullM;
        }

    private:
        IntType valueM;
        my_bool isNullM;
    };

    typedef Number<char,  MYSQL_TYPE_BIT>           BitParam;
    typedef Number<char,  MYSQL_TYPE_TINY>          CharParam;
    typedef Number<short, MYSQL_TYPE_SHORT>         ShortIntParam;
    typedef Number<long,  MYSQL_TYPE_LONG>          LongParam;
    typedef Number<long long, MYSQL_TYPE_LONGLONG>  LongLongParam;
    typedef Number<float, MYSQL_TYPE_FLOAT>         FloatParam;
    typedef Number<double, MYSQL_TYPE_DOUBLE>       DoubleParam;

    typedef Number<char,  MYSQL_TYPE_BIT>           BitResult;
    typedef Number<char,  MYSQL_TYPE_TINY>          CharResult;
    typedef Number<short, MYSQL_TYPE_SHORT>         ShortIntResult;
    typedef Number<long,  MYSQL_TYPE_LONG>          LongResult;
    typedef Number<long long, MYSQL_TYPE_LONGLONG>  LongLongResult;
    typedef Number<float, MYSQL_TYPE_FLOAT>         FloatResult;
    typedef Number<double, MYSQL_TYPE_DOUBLE>       DoubleResult;

    class StringParam
    {
    public:
        enum { PRE_ALLOC_LENGTH = 32 };

        StringParam()
        { init(NULL, 0); }
        StringParam(const char* theValue)
        { init(theValue); }
        StringParam(const char* theValue, const unsigned theFieldLen)
        { init(theValue, theFieldLen); }

        StringParam(MYSQL_BIND* theBindHandler)
        { init(NULL, 0); bind(theBindHandler); }
        StringParam(MYSQL_BIND* theBindHandler, const char* theFieldValue)
        { init(theFieldValue); bind(theBindHandler); }
        StringParam(MYSQL_BIND* theBindHandler, const char* theFieldValue, const unsigned theFieldLen)
        { init(theFieldValue, theFieldLen); bind(theBindHandler); }

        ~StringParam()
        {
            if (bufferLengthM > PRE_ALLOC_LENGTH)
            { delete[] pBufferM; }
        }

        void init(const char* theValue){init(theValue, strlen(theValue));}
        void init(const char* theValue, const unsigned theFieldLen)
        {
            isNullM = 0;      //set to not NULL by default
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
        { return isNullM ? theDefault : std::string(pBufferM, bufferLengthM); }

        void set(const char* theValue)
        { set(theValue, strlen(theValue));}
        void set(const char* theValue, const unsigned theFieldLen)
        { 
            if (bufferLengthM > PRE_ALLOC_LENGTH)
            { 
                delete[] pBufferM; 
            }
            init(theValue, theFieldLen);
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
        }

    private:
        char bufferM[PRE_ALLOC_LENGTH];
        char* pBufferM;
        unsigned long bufferLengthM;
        my_bool isNullM;
    };

    template<unsigned TheFieldLen>
    class StringResult
    {
    public:
        enum 
        { FIXED_LENGTH = TheFieldLen };

        StringResult()
        { init(); }

        StringResult(MYSQL_BIND* theBindHandler)
        { init(); bind(theBindHandler); }

        ~StringResult() {}

        void init()
        { 
            memset(bufferM, 0, sizeof(bufferM)); 
            resultLengthM = 0;
            isNullM = 0;
        }

        operator const char*()
        { return isNullM ? "" : bufferM; }

        const char* get(const char* theDefault = "")
        { return isNullM ? theDefault : bufferM; }

        const std::string getString(const char* theDefault = "")
        { return isNullM ? theDefault : std::string(bufferM, resultLengthM); }

        void bind(MYSQL_BIND* theMysqlStruct)
        {
            memset((void*)theMysqlStruct, 0, sizeof(MYSQL_BIND));
            theMysqlStruct->buffer_type= MYSQL_TYPE_STRING;
            theMysqlStruct->buffer= bufferM;
            theMysqlStruct->buffer_length= FIXED_LENGTH;
            theMysqlStruct->is_null= &isNullM;
            theMysqlStruct->length= &resultLengthM;
        }

    private:
        char bufferM[FIXED_LENGTH + 1];
        my_bool isNullM;
        unsigned long resultLengthM;
    };

    
}
}
#endif /* MYSQLSTATEMENTSTRUCT_H */

