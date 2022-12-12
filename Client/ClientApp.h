#pragma once

#define PORT "1111"


class client{

    int csock;
    void connect_to_server();
    void send_file();

public:
    client();
    ~client();

    void run();

};