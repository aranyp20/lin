#include <string.h>
#include <iostream>
#include "Gens.h"

char* records::parse(const std::vector<std::string>& s) const
{
    char *temp = new char[256];
    int i = 0;
    for(const auto& a: s){
        strcpy(temp+i,a.c_str());
        i+=a.size();
        temp[i++]='-';
    }
    temp[--i]= '\n';


    return temp;
}

size_t records::get_row_count() const
{
    return data_parsed.size();
}

void records::print() const
{
    for(const auto& a : data_parsed){
        for(const auto& b : a){
            std::cout<<b<<' ';
        }
        std::cout<<std::endl;
    }
}

records::records(MYSQL_RES* res, unsigned long field_count)
{
    MYSQL_ROW row;
    
    while((row = mysql_fetch_row(res))!=NULL){
        if(field_count==0)continue;
        std::vector<std::string> row_s;
        for(unsigned long i = 0;i<field_count;i++){
            row_s.push_back(row[i]);
        }
        data_parsed.push_back(row_s);
        char *temp = parse(row_s);        
        data_raw.push_back(temp);
    }
}

const std::vector<char*> records::get_data_raw() const{ return data_raw;}

const std::vector<std::vector<std::string>> records::get_data_parsed() const{return data_parsed;}

const int records::get_row_bytes(size_t index) const
{
    int result = 0;
    for(const auto& a : data_parsed[index]){
        result += a.size()+1;
    }
    return result;
}


server_answer::server_answer(const std::string& mes) : message(mes)
{
    success = true;

}

server_answer::server_answer(const records& rec) : recs(rec)
{
    success = true;
}
server_answer::server_answer(const std::string& mes, const records& rec) : message(mes), recs(rec)
{
    success = true;
}

