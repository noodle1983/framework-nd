#ifndef MYSQLSTATEMENTEXAMPLE_H
#define MYSQLSTATEMENTEXAMPLE_H

#include "MysqlStatementStruct.h"
#include "MysqlStatement.h"
#include <string>

namespace DBI
{
    class MysqlStatementExample: public MysqlStatement
    {
    public:
        MysqlStatementExample(const char* theName, const int theAge);
        virtual ~MysqlStatementExample();
        virtual int execute(MYSQL* theMysqlHandler);

    private:
        static const std::string sqlM;

        DBI::Mysql::StringParam paramNameM;
        DBI::Mysql::LongParam   paramAgeM;

        DBI::Mysql::StringResult<16> resultNameM;
        DBI::Mysql::StringResult<16> resultOwnerM;
        DBI::Mysql::StringResult<16> resultSpeciesM;
        DBI::Mysql::StringResult<16> resultSexM;
        DBI::Mysql::LongResult       resultAgeM;
    };
}

#endif /* MYSQLSTATEMENTEXAMPLE_H */

