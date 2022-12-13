#pragma once

#define PORT "1111"


class client{


    int csock;
    void connect_to_server();
    void send_file(const std::string& filename) const;
    void recieve_file() const;


    std::string ask_for_username() const;

    void send_even_file(char*,int) const;

public:
    client();
    ~client();

    void run();

};