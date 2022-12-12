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
}


interpreter::interpreter(data_accessor* const da) : accessor(da)
{

}


server_answer interpreter::interpret(const request& req) const
{

}
