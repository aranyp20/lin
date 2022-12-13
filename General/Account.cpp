#include "Account.h"


account::account(const reg_reqs& rr) : username(rr.username), password(rr.password), validation_points(0), reward(0)
{
    logged_in = true;
}

account::reg_reqs::reg_reqs(const std::string& _username, const std::string& _password) : username(_username), password(_password)
{

}

account::account(const std::vector<std::string>& record)
{
    if(record.size()!=4)return;
    logged_in = true;
    username = record[1];
}

account::account(const std::string& usr) :username(usr)
{

}




