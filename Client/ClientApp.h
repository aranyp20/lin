#pragma once

#define PORT "1111"


class client{


    int csock;
    void connect_to_server();
    void send_file();

    std::string ask_for_username() const;

public:
    client();
    ~client();

    void run();

};