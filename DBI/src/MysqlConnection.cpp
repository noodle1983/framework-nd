#include "MysqlConnection.h"
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
        LOG_ERROR("mysql_stmt_init failed!");
        return -1;
    }

    std::string sql = "select name, owner, species, sex from pet";
    if (0 != mysql_stmt_prepare(stmt, sql.c_str(), sql.length()))
    {
        LOG_ERROR("mysql_stmt_prepare failed. errmsg:" << mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return -1;
    }

    if (0 != mysql_stmt_execute(stmt))
    {
        LOG_ERROR("mysql_stmt_execute failed. errmsg:" << mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return -1;
    }

    MYSQL_BIND    bind[4];
    unsigned long length[4];
    my_bool       is_null[4];
    char outBuffer[4][16];

    memset(bind, 0, sizeof(bind));
     
    bind[0].buffer_type= MYSQL_TYPE_STRING;
    bind[0].buffer= outBuffer[0];
    bind[0].buffer_length= 16;
    bind[0].is_null= &is_null[0];
    bind[0].length= &length[0];
     
    bind[1].buffer_type= MYSQL_TYPE_STRING;
    bind[1].buffer= outBuffer[1];
    bind[1].buffer_length= 16;
    bind[1].is_null= &is_null[1];
    bind[1].length= &length[1];
     
    bind[2].buffer_type= MYSQL_TYPE_STRING;
    bind[2].buffer= outBuffer[2];
    bind[2].buffer_length= 16;
    bind[2].is_null= &is_null[2];
    bind[2].length= &length[2];
     
    bind[3].buffer_type= MYSQL_TYPE_STRING;
    bind[3].buffer= outBuffer[3];
    bind[3].buffer_length= 16;
    bind[3].is_null= &is_null[3];
    bind[3].length= &length[3];

    /* Bind the result buffers */
    if (0 != mysql_stmt_bind_result(stmt, bind))
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
        LOG_ERROR("get:" << outBuffer[0] << "\t"
                <<outBuffer[1] << "\t"
                <<outBuffer[2] << "\t"
                <<outBuffer[3] );
    }
    mysql_stmt_close(stmt);
    return 0;

}

//-----------------------------------------------------------------------------

