#pragma once

#include <string>
#include <vector>

class account{

    std::string username;
    std::string password;
    float validation_points;    
    unsigned int reward;

public:

    struct reg_reqs{
        std::string username;
        std::string password;
        reg_reqs(const std::string&, const std::string&);   
    };

    account(const reg_reqs&);


    inline std::string get_username() const {return username;}
    inline float get_reputation() const {return validation_points;}
    inline unsigned int get_reward() const {return reward;}
};
