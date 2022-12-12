#include <iostream>
#include "DataAccess.h"

sql_connection::sql_connection(connection_details& mysql_details)
{
    MYSQL *connection = mysql_init(NULL);

    if(!mysql_real_connect(connection,mysql_details.server,mysql_details.user,mysql_details.password,mysql_details.database,0,NULL,0)){
        std::cout<<"Connection error: "<<mysql_error(connection)<<std::endl;
        exit(1);
    }

    con = connection;
}

sql_connection::~sql_connection()
{
    mysql_close(con);
}

records sql_connection::execute_query(const char *sql_query){
    if(mysql_query(con,sql_query)){
        std::cout<<"Query error: "<<mysql_error(con) << std::endl;
        exit(1);
    }

    MYSQL_RES* res = mysql_use_result(con);
    unsigned long row_length = mysql_field_count(con); 
    if(row_length>0){
        records result(res,row_length);

        mysql_free_result(res);
        return result;
    }
    return records();
}

//TODO: close connection
sql_connection data_accessor::create_connection()
{
    sql_connection::connection_details mysqlD;
    mysqlD.server = "localhost";
    mysqlD.user = "root";
    mysqlD.password = "12365r20";
    mysqlD.database = "dbtest";

    return sql_connection(mysqlD);
}

records data_accessor::get_users()
{

    sql_connection connection = create_connection();
    return connection.execute_query("SELECT * FROM Users;");
}

void data_accessor::insert_user(const account& account)
{
    sql_connection connection = create_connection();
  
    std::string query = "INSERT INTO Users(username) VALUES('" + account.get_username() + "');";

    connection.execute_query(query.c_str());
}
int data_accessor::insert_task(const task& task)
{
    sql_connection connection = create_connection();
    std::string query = "insert into Tasks(code,reward,master_id) (select '"+task.code+"',"+std::to_string(task.reward)+",min(user_id) from Users where username='"+task.master.get_username()+"');";
    connection.execute_query(query.c_str());
    query = "select max(task_id) from Tasks;";
    records r = connection.execute_query(query.c_str());
    return std::stoi(r.get_data_parsed()[0][0]);
}

records data_accessor::get_user(const std::string& username)
{
    sql_connection connection = create_connection();
    std::string query = "SELECT * FROM Users WHERE username ='"+ username +"';";

    return connection.execute_query(query.c_str());

}

records data_accessor::get_tasks_available(const std::string& username)
{
    sql_connection connection = create_connection();
    std::string query = "SELECT * FROM Tasks WHERE master_id NOT IN(select user_id from Users where username='"+username+"');";
    records res = connection.execute_query(query.c_str());

    return res;

}

void data_accessor::assign_slave_to_task(const std::string& id, const std::string& username)
{
    sql_connection connection = create_connection();
    std::string query = "update Tasks set slave_id = (select user_id from Users where username='"+username+"') where task_id ='"+id+"';";
    connection.execute_query(query.c_str());
}


