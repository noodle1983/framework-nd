#ifndef MYSQLCONNECTION_H
#define MYSQLCONNECTION_H

#include "MysqlStatement.h"

#include <string>
#include <mysql.h>

namespace DBI
{
    class MysqlLibHandler
    {
    public:
        MysqlLibHandler(){mysql_library_init(-1, NULL, NULL);}
        ~MysqlLibHandler(){mysql_library_end();}
    };

    class MysqlConnection
    {
    public:
        MysqlConnection(const std::string& theCfgGroup = "mysql");
        ~MysqlConnection();

        int connect();
        void close();

        int processStatement(MysqlStatement* theStatement);

    private:
        MYSQL* mysqlM;
        std::string cfgGroupM;

        static MysqlLibHandler mysqlLibHandlerM;
    };
}

#endif /* MYSQLCONNECTION_H */

