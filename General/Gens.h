#pragma once

#include <string>
#include <vector>
#include <mysql/mysql.h>


class records{

    std::vector<char*> data_raw;
    std::vector<std::vector<std::string>> data_parsed;
    char* parse(const std::vector<std::string>&) const;


public:

    records(MYSQL_RES*,unsigned long);
    ~records(){}

    const std::vector<char*> get_data_raw() const;
    const std::vector<std::vector<std::string>> get_data_parsed() const;
    const int get_row_bytes(size_t) const;

    void print() const;

};


struct date{

    int year;
    int mounth;
    int day;

};