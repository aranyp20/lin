#include <iostream>

#include "DataLayer/DataAccess.h"







int main()
{
    
    account acc(account::reg_reqs("Arany Peti","1234"));

    data_accessor da;
    //da.insert_user(acc);
    da.list_users();

    return 0;
}