#include <iostream>
#include "CommandServer.h"
#include "../General/Gens.h"
#include "../DataLayer/DataAccess.h"

command_server::command_server(data_accessor* const _da) : my_accessor(_da)
{}

void assistant::send_records(const records& r, int socket) const
{
    std::vector<char*> res = r.get_data_raw();
    for(int i=0;i<res.size();i++){
        write(socket, res[i], r.get_row_bytes(i));
    }
    write(socket,"#",2);
    

}



int command_server::create_socket(const char* port)
{
    struct addrinfo hints;
    struct addrinfo* res;
    int err;
    int created_sock;
    

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    err = getaddrinfo("localhost", port, &hints, &res);
    if(err != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
        return -1;
    }
    if(res == NULL)
    {
        return -1;
    }
  
    created_sock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if(created_sock < 0)
    {
        perror("socket");
        return -1;
    }

    int reuse = 1;
    setsockopt(created_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));
  

    if(bind(created_sock, res->ai_addr, res->ai_addrlen) < 0)
    {
        perror("bind");    
        return 1;
    }

    freeaddrinfo(res);


    return created_sock;
}


void command_server::Run()
{
    std::cout<<"kurva"<<std::endl;

    int local_sock = create_socket("1111");
    if(local_sock<0)return;

    struct sockaddr_in6 addr;
    socklen_t addrlen;
    char ips[NI_MAXHOST];
    char servs[NI_MAXSERV];
    int csock;
    char buf[256];
    int len;

    if(listen(local_sock, 5) < 0)
    {
        perror("listen");
        return;
    }

    addrlen = sizeof(addr);

    while((csock = accept(local_sock, (struct sockaddr*)&addr, &addrlen)) >= 0)
    {
        if(getnameinfo((struct sockaddr*)&addr, addrlen, ips, sizeof(ips), servs, sizeof(servs), 0) == 0)
    {
      assistant* ass = new assistant(my_accessor,csock);
      std::cout<<"Kapcsolódás.."<<std::endl;
      ass->start();
      assistants.push_back(ass);
      
      for(int i=assistants.size()-1;i>=0;i--){
        if(assistants[i]->is_dead()){assistants[i]->join();
          delete assistants[i];
          assistants.erase(assistants.begin()+i);
        }
      }
    }
        
  }
 
    
   
}





void MyThread::start()
{
  if(pthread_create(&_th, NULL, thread_entry, (void*)this))
  {
    std::cerr << "pthread_create: " << strerror(errno) << std::endl;
    exit(1);
  }
}

void* MyThread::thread_entry(void* pobj)
{
  MyThread* pthis = (MyThread*)pobj;
  pthis->on_run();
  return NULL;
}

void MyThread::join()
{
  pthread_join(_th, NULL);
}

assistant::assistant(data_accessor* const da,int socket) : my_accessor(da),socket(socket)
{

}

void assistant::on_run()
{
  std::cout<<"Assistent elkezdett dolgozni."<<std::endl;
  char buf[1024];
  int len;
  while((len = recv(socket, buf, sizeof(buf), 0)) > 0)
    {
        send_records(my_accessor->get_users(),socket);
    }
    
    printf("Kapcsolat zárása.\n");
    close(socket);
    dead = true;
}
