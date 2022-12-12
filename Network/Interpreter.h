#pragma once
#include "../DataLayer/DataAccess.h"
#include "../General/Gens.h"

struct request{
    std::vector<std::string> args;
    account requester;

    request(const account&, char* buf, int length);
};

class interpreter{

    data_accessor* accessor;

public:

    interpreter(data_accessor* const);

    server_answer interpret(const request&) const;
};