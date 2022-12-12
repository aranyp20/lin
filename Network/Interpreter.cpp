#include "Interpreter.h"

#include <iostream>
request::request(const account& acc, char* buf, int length) : requester(acc)
{
    std::string current_word;
    for(int i=0;i<length;i++){
        if(buf[i]==' '){
            args.push_back(current_word);
            current_word.clear();
        }else{
            current_word.push_back(buf[i]);
        }
    }
    if(current_word.size()>0)args.push_back(current_word);

    if(args.size()>0)args[args.size()-1].pop_back();

}


interpreter::interpreter(data_accessor* const da) : accessor(da)
{

}

server_answer interpreter::interpret(const request& req) const
{
    switch(req.args.size()){
        case 1:
            if(req.args[0]=="logout") return r_logout();
        break;
        case 2:
            if(req.args[0]=="login") return r_login(req.args[1]);
        break;
    }

    return server_answer("Unknown command.",false);
}


server_answer interpreter::r_login(const std::string& username) const
{
    records q_res = accessor->get_user(username);
    if(q_res.get_row_count()==0)return server_answer("User not found!",false);
    server_answer res("Successful login.",q_res);
    res.int_code=server_answer::internal_code::LOGIN;
    return res;
}

server_answer interpreter::r_logout() const
{
    server_answer res("Logged out.");
    res.int_code=server_answer::internal_code::LOGOUT;
    return res;
}