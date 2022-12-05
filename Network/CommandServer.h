#pragma once

#include "../General/Gens.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <poll.h>


class data_accessor;

#include <pthread.h>
#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <string.h>
#include <errno.h>

class MyThread
{
public:
  MyThread() {}
  virtual ~MyThread() { std::cout<<"Thread done"<<std::endl;join(); }
  
  void start();
  void join();
  
protected:
  virtual void on_run() = 0;
  
private:
  static void* thread_entry(void* pobj);
  
protected:
  pthread_t _th;
};


class assistant : public MyThread{
  int socket;
  bool dead = false;

  data_accessor* my_accessor;

  void send_records(const records&, int socket) const;
protected:

  virtual void on_run();
public:

  assistant(data_accessor* const, int socket);

  inline bool is_dead() const {return dead;}

};


class command_server{

  std::vector<assistant*> assistants;

  

private:

    data_accessor* my_accessor;

    int create_socket(const char* port);


public:
  
    command_server(data_accessor* const);
    void Run();

};






