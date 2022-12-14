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

    data_accessor();
    ~data_accessor();

    void insert_user(const account&);
    int insert_task(const task&);
    void assign_slave_to_task(const std::string& id,const std::string& username);
    void ready_task(const std::string& id);

    records get_users();
    records get_user(const std::string&);
    records get_tasks_available(const std::string&);
    records get_tasks_reserved(const std::string&);
    records show_finisheds(const std::string&);


};