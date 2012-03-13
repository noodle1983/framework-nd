#include "MysqlConnection.h"
#include "MysqlStatementExample.h"

using namespace DBI;

int main()
{
    MysqlConnection connection;
    connection.connect();
    MysqlStatementExample statementFindBy("a", 1);
    connection.processStatement(&statementFindBy);
    return 0;
}

