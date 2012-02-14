#include "MysqlConnection.h"

using namespace DBI;

int main()
{
    MysqlConnection connection;
    connection.connect();
    connection.processStatement();
    return 0;
}

