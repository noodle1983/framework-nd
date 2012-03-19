#include "MysqlStatementExample.h"
#include "Log.h"

using namespace DBI;

//-----------------------------------------------------------------------------

const std::string MysqlStatementExample::sqlM = 
    "select name, owner, species, sex, age, bit from pet where name = ? and age = ?"; 

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

    if (0 != mysql_stmt_prepare(stmt, sqlM.c_str(), sqlM.length()))
    {
        LOG_ERROR("mysql_stmt_prepare failed. errmsg:" << mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return -1;
    }

    MYSQL_BIND    bindParam[2];
    memset(bindParam, 0, sizeof(bindParam));
    paramNameM.bind(&bindParam[0]);
    paramAgeM.bind(&bindParam[1]);

#ifdef DEBUG
    if (sizeof(bindParam)/sizeof(MYSQL_BIND) != mysql_stmt_param_count(stmt))
    {
        LOG_ERROR("invalid input parameter count. "
                << "expect:" << sizeof(bindParam)/sizeof(MYSQL_BIND)
                << ", real:" << mysql_stmt_param_count(stmt));
        mysql_stmt_close(stmt);
        return -1;
    }
#endif
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

    MYSQL_BIND    bindResult[6];
    memset(bindResult, 0, sizeof(bindResult));
    resultNameM.bind(&bindResult[0]);
    resultOwnerM.bind(&bindResult[1]);
    resultSpeciesM.bind(&bindResult[2]);
    resultSexM.bind(&bindResult[3]);
    resultAgeM.bind(&bindResult[4]);
    resultBitM.bind(&bindResult[5]);
    
#ifdef DEBUG
    /* Fetch result set meta information */
    MYSQL_RES     *prepareMetaResult;
    prepareMetaResult = mysql_stmt_result_metadata(stmt);
    if (!prepareMetaResult)
    {
        LOG_ERROR("mysql_stmt_result_metadata failed. errmsg:" << mysql_stmt_error(stmt));
        mysql_stmt_close(stmt);
        return -1;
    }
    int columnCount= mysql_num_fields(prepareMetaResult);
    /* Free the prepared result metadata */
    mysql_free_result(prepareMetaResult);
    if (sizeof(bindResult)/sizeof(MYSQL_BIND) != columnCount)
    {
        LOG_ERROR("invalid output parameter count. "
                << "expect:" << sizeof(bindResult)/sizeof(MYSQL_BIND)
                << ", real:" << columnCount);
        mysql_stmt_close(stmt);
        return -1;
    }
#endif

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
                         << int(resultBitM) << "\t"
                );
    }
    mysql_stmt_close(stmt);
    return 0;
}

//-----------------------------------------------------------------------------


