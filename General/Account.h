#pragma once

#include <string>
#include <vector>

class account{

    std::string username;
    std::string password;
    float validation_points;    
    unsigned int reward;
    bool logged_in = false;

public:

    struct reg_reqs{
        std::string username;
        std::string password;
        reg_reqs(const std::string&, const std::string&);   
    };

    account(const std::string&);
    account(const reg_reqs&);
    account(const std::vector<std::string>&);
    account(){}


    inline std::string get_username() const {return username;}
    inline float get_reputation() const {return validation_points;}
    inline unsigned int get_reward() const {return reward;}
    inline bool get_logged_in() const {return logged_in;}
};
