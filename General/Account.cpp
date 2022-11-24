#include "Account.h"


account::account(const reg_reqs& rr) : username(rr.username), password(rr.password), validation_points(0), reward(0)
{
}

account::reg_reqs::reg_reqs(const std::string& _username, const std::string& _password) : username(_username), password(_password)
{
}




