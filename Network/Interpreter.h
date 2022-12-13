#pragma once
#include "../DataLayer/DataAccess.h"
#include "../General/Gens.h"

struct request{
    std::vector<std::string> args;
    account requester;

    request(const account&, char* buf, int length);
};

enum state{
    DEF = 0,
    CAN_RESERVE = 1,
    CAN_UPLOAD = 2
};

class interpreter{
    records cache;

    state st = DEF;

    data_accessor* accessor;

    server_answer r_login(const std::string& username) const;
    server_answer r_logout() const;
    server_answer r_register(const std::string& username) const;
    server_answer r_task_list_available(const std::string& username) const;
    server_answer r_task_list_reserved(const std::string& username) const;
    server_answer r_task_list_finished(const std::string& username) const;
    server_answer r_task_create(const task&) const;
    server_answer r_task_reserve(const std::string& task_id,const std::string& username) const;

public:

    interpreter(data_accessor* const);

    server_answer interpret(const request&);
};