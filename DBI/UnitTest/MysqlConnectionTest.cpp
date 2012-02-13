#include "MysqlConnection.h"

using namespace DBI;

int main()
{
    MysqlConnection connection;
    connection.connect();
    return 0;
}

