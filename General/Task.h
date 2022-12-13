#pragma once

#include "Account.h"
#include "Gens.h"

struct task{

    account master;
    account slave;
    date due;
    unsigned int reward;
    std::string code;

    task(const account& _master, int _reward, const std::string& _code) : master(_master), reward(_reward), code(_code){}

};