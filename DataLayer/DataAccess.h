#pragma once

#include <mysql/mysql.h>

#include "../General/Account.h"
#include "../General/Task.h"


class sql_connection{

    MYSQL *con;


public:
    struct connection_details{
        const char *server, *user, *password, *database;
    };


    sql_connection(connection_details&);
    ~sql_connection();

    records execute_query(const char*);

};

class data_accessor{

    sql_connection create_connection();

public:

    void insert_user(const account&);
    void insert_task(const task&);

    records get_users();
    records get_user(const std::string&);


};