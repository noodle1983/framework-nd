#include "MysqlStatementExample.h"
#include "Log.h"

using namespace DBI;

//-----------------------------------------------------------------------------

const std::string MysqlStatementExample::sqlM = 
    "select name, owner, species, sex, age from pet where name = ? and age = ?"; 

//-----------------------------------------------------------------------------

MysqlStatementExample::MysqlStatementExample(const char* theName, const int theAge)
    : paramNameM(theName)
    , paramAgeM(theAge)
{
}

//-----------------------------------------------------------------------------

MysqlStatementExample::~MysqlStatementExample()
{
}

//-----------------------------------------------------------------------------

int MysqlStatementExample::execute(MYSQL* theMysqlHandler)
{
    MYSQL_STMT* stmt = mysql_stmt_init(theMysqlHandler);
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

    MYSQL_BIND    bindParam[2];
    memset(bindParam, 0, sizeof(bindParam));
    paramNameM.bind(&bindParam[0]);
    paramAgeM.bind(&bindParam[1]);

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
    resultNameM.bind(&bindResult[0]);
    resultOwnerM.bind(&bindResult[1]);
    resultSpeciesM.bind(&bindResult[2]);
    resultSexM.bind(&bindResult[3]);
    resultAgeM.bind(&bindResult[4]);

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
        LOG_ERROR("get:" << resultNameM << "\t"
                         << resultOwnerM << "\t"
                         << resultSpeciesM << "\t"
                         << resultSexM << "\t"
                         << resultAgeM << "\t"
                );
    }
    mysql_stmt_close(stmt);
    return 0;
}

//-----------------------------------------------------------------------------


