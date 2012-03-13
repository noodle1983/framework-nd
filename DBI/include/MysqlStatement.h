#ifndef MYSQLSTATEMENT_H
#define MYSQLSTATEMENT_H

#include <string>
#include <mysql.h>

namespace DBI
{
    class MysqlStatement
    {
    public:
        virtual ~MysqlStatement(){};
        virtual int execute(MYSQL* theMysqlHandler) = 0;
    };
}

#endif /* MYSQLSTATEMENT_H */

