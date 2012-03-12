#include "MysqlConnection.h"
#include "MysqlStatementStruct.h"
#include "ConfigCenter.h"
#include "Log.h"

using namespace DBI;
using namespace Config;
using namespace std;

//-----------------------------------------------------------------------------

MysqlLibHandler MysqlConnection::mysqlLibHandlerM;

//-----------------------------------------------------------------------------

MysqlConnection::MysqlConnection(const std::string& theCfgGroup)
    : mysqlM(NULL)
    , cfgGroupM(theCfgGroup)
{
}

//-----------------------------------------------------------------------------

MysqlConnection::~MysqlConnection()
{
    close();
}

//-----------------------------------------------------------------------------

int MysqlConnection::connect()
{
    if(NULL == (mysqlM = mysql_init(NULL)))
    {
        LOG_ERROR("Mysql_init fails!");
        return -1;
    }
    
    std::string host = ConfigCenter::instance()->get(cfgGroupM+".host", "127.0.0.1");
    std::string user = ConfigCenter::instance()->get(cfgGroupM+".user", "test");
    std::string pswd = ConfigCenter::instance()->get(cfgGroupM+".pswd", "test");
    std::string dbname = ConfigCenter::instance()->get(cfgGroupM+".db", "test");
    int dbport = ConfigCenter::instance()->get(cfgGroupM+".port", 3306);
    if (!mysql_real_connect(mysqlM, host.c_str(), user.c_str(), pswd.c_str(), 
                dbname.c_str(), dbport, NULL, 0))
    {
        LOG_ERROR("fail to connect mysql:" << user << "@" << host << ":" << dbport << " " << dbname
                << ". errmsg:" << mysql_error(mysqlM));
        close();
        return -1;
    }
    return 0;
}

//-----------------------------------------------------------------------------

void MysqlConnection::close()
{
    if (mysqlM)
    {
        mysql_close(mysqlM);
        mysqlM = NULL;
    }
}

//-----------------------------------------------------------------------------

int MysqlConnection::processStatement()
{
    if (NULL == mysqlM)
    {
        LOG_WARN("Connection is not connected!");
        return -1;
    }

    MYSQL_STMT* stmt = mysql_stmt_init(mysqlM);
    if (NULL == stmt)
    {
        LOG_ERROR("mysql_stmt_init failed. errmsg:" << mysql_stmt_error(stmt));
        return -1;
    }

    std::string sql = "select name, owner, species, sex, age from pet where name = ? and age = ?"; 
    if (0 != mysql_stmt_prepare(stmt, sql.c_str(), sql.length()))
    {
        LOG_ERROR("mysql_stmt_prepare failed. errmsg:" << mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return -1;
    }

    LOG_ERROR("param cout:"  << mysql_stmt_param_count(stmt));

    MYSQL_BIND    bindParam[2];
    memset(bindParam, 0, sizeof(bindParam));
    
    DBI::Mysql::String param0(&bindParam[0], "a", 1);
    DBI::Mysql::Long param1(&bindParam[1], 1);

    if (mysql_stmt_bind_param(stmt, bindParam))
    {
        LOG_ERROR("mysql_stmt_bind_param failed. errmsg:" << mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return -1;
    }

    if (0 != mysql_stmt_execute(stmt))
    {
        LOG_ERROR("mysql_stmt_execute failed. errmsg:" << mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return -1;
    }

    MYSQL_BIND    bindResult[4];
    memset(bindResult, 0, sizeof(bindResult));
    DBI::Mysql::String field0(&bindResult[0], 16);
    DBI::Mysql::String field1(&bindResult[1], 16);
    DBI::Mysql::String field2(&bindResult[2], 16);
    DBI::Mysql::String field3(&bindResult[3], 16);
    DBI::Mysql::Long field4(&bindResult[4]);
     

    /* Bind the result buffers */
    if (0 != mysql_stmt_bind_result(stmt, bindResult))
    {
        LOG_ERROR("mysql_stmt_bind_result failed. errmsg:" << mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return -1;
    }

    /* Now buffer all results to client */
    if (0 != mysql_stmt_store_result(stmt))
    {
        LOG_ERROR("mysql_stmt_store_result failed. errmsg:" << mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return -1;
    }

    while (!mysql_stmt_fetch(stmt))
    {
        LOG_ERROR("get:" << field0 << "\t"
                << field1 << "\t"
                << field2 << "\t"
                << field3 << "\t"
                << field4 << "\t"
                );
    }
    mysql_stmt_close(stmt);
    return 0;

}

//-----------------------------------------------------------------------------

