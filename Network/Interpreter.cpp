#include <iostream>
#include <unistd.h>

#include "Interpreter.h"

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

server_answer interpreter::interpret(const request& req)
{
    if(st==CAN_RESERVE){
        if(req.args.size()==2){
            if(req.args[0]=="reserve"){
                int num = std::stoi(req.args[1]);
                if(num>=0&&num<cache.get_row_count()){
                   return r_task_reserve(cache.get_data_parsed()[num][0],req.requester.get_username());
                }
            }
        }
    }
    if(st==CAN_UPLOAD){
        if(req.args.size()==3){
            if(req.args[0]=="upload"){
                int num = std::stoi(req.args[1]);
                if(num>=0&&num<cache.get_row_count()){
                    server_answer answ;
                    answ.int_help = std::atoi(cache.get_data_parsed()[num][0].c_str());
                    answ.int_code=server_answer::internal_code::WAIT_FILE_S;
                    accessor->ready_task(cache.get_data_parsed()[num][0]);
                    return answ;
                }
            }
        }
    }
    if(st==CAN_CHECK){
        if(req.args.size()==2){
            if(req.args[0]=="check"){
                int num = std::stoi(req.args[1]);
                if(num>=0&&num<cache.get_row_count()){
                    server_answer answ;
                    answ.int_help = std::atoi(cache.get_data_parsed()[num][0].c_str());
                    answ.int_code=server_answer::internal_code::SEND_FILE_S;
                    return answ;
                }
            }
        }

    }

    st = DEF;

    switch(req.args.size()){
        case 1:
            if(!req.requester.get_logged_in())return server_answer("Need to sign in.",false);
            if(req.args[0]=="logout") return r_logout();
        break;
        case 2:
            if(req.args[0]=="login") return r_login(req.args[1]);
            else if(req.args[0]=="register") return r_register(req.args[1]);
        break;
        case 3:
            if(!req.requester.get_logged_in())return server_answer("Need to sign in.",false);
            if(req.args[0]=="task"){
                if(req.args[1]=="list"){
                    if(req.args[2]=="available"){
                        st=CAN_RESERVE;
                        server_answer t1 = r_task_list_available(req.requester.get_username());
                        cache = t1.recs;
                        return t1;
                    }
                    if(req.args[2]=="reserved"){
                        st=CAN_UPLOAD;
                        server_answer t1 = r_task_list_reserved(req.requester.get_username());
                        cache = t1.recs;
                        return t1;
                    }
                    if(req.args[2]=="finished"){
                        st=CAN_CHECK;
                        server_answer t1 = r_task_list_finished(req.requester.get_username());
                        cache = t1.recs;
                        return t1;
                    }
                }
                else if(req.args[1]=="create"){
                    return r_task_create(task(req.requester,10,"xx4"));
                }
            } 
        break;
    }

    return server_answer("Unknown command.",false);
}

server_answer interpreter::r_task_create(const task& t) const
{
    server_answer answ("Uploaded.");

    answ.int_help = accessor->insert_task(t);
    answ.int_code=server_answer::internal_code::WAIT_FILE;
    return answ;

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

server_answer interpreter::r_register(const std::string& username) const
{
    accessor->insert_user(account(username));

    return server_answer("Registered.");
}


server_answer interpreter::r_task_list_available(const std::string& username) const
{
    return accessor->get_tasks_available(username);
}

server_answer interpreter::r_task_reserve(const std::string& id,const std::string& username) const
{
    accessor->assign_slave_to_task(id, username);
    server_answer answ("Reserved.");
    answ.int_code=server_answer::internal_code::SEND_FILE;
    answ.int_help = std::atoi(id.c_str());
    return answ;
}


server_answer interpreter::r_task_list_reserved(const std::string& username) const
{
    return accessor->get_tasks_reserved(username);
}


server_answer interpreter::r_task_list_finished(const std::string& username) const
{
    return accessor->show_finisheds(username);
}


