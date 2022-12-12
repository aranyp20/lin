#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <sys/sendfile.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <vector>
#include <string>
#include "ClientApp.h"


client::client()
{
  connect_to_server();
}

void client::connect_to_server()
{
  struct addrinfo hints;
  struct addrinfo* res;
  int err;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  err = getaddrinfo("localhost", PORT, &hints, &res);
  if(err != 0)
  {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
    exit(1);
  }
  if(res == NULL)
  {
    exit(1);
  }


  csock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if(csock < 0)
  {
    perror("socket");
    exit(1);
  }

  if(connect(csock, res->ai_addr, res->ai_addrlen) < 0)
  {
    perror("connect");
    exit(1);
  }
  freeaddrinfo(res);
}

void client::send_even_file(char* buf,int length) const
{
  std::vector<std::string> parsed;
   std::string current_word;
    for(int i=0;i<length;i++){
        if(buf[i]==' '){
            parsed.push_back(current_word);
            current_word.clear();
        }else{
            current_word.push_back(buf[i]);
        }
    }
    if(current_word.size()>0)parsed.push_back(current_word);

    if(parsed.size()>0)parsed[parsed.size()-1].pop_back();


    if(parsed.size()==3&&parsed[0]=="task"&&parsed[1]=="create"){
      send(csock, buf, length, 0);
      send_file(parsed[2]);
    }else{
      send(csock, buf, length, 0);
    }
}


void client::run()
{
    char buf[1024];
    char read_buf[1024];
    int len;
    int read_len;
    
    std::cout<<ask_for_username(); fflush(stdout);
    
    while((len = read(STDIN_FILENO, buf, sizeof(buf))) > 0)
    {
      
      send_even_file(buf,len);
      std::cout<<"------------------"<<std::endl;
      bool end = false;
      while(!end&&(read_len = recv(csock, read_buf, 1024, 0)) > 0){
        for(int i=0;i<read_len;i++){
          if(read_buf[i] == '#'){
            end = true;
            break;
          }
          printf("%c",read_buf[i]);
        }
        
      }
      std::cout<<"------------------"<<std::endl<<std::endl;

      std::cout<<ask_for_username(); fflush(stdout);

    }

}

void client::send_file(const std::string& filename) const
{
  std::cout<<"Küldés kezdete..."<<std::endl;
  int sendable_fd;
  struct stat sendable_stat;
  off_t offset = 0;

  std::string t_fname = "./Descriptions/"+filename+".txt";
  sendable_fd = open(t_fname.c_str(), O_RDONLY);
  fstat(sendable_fd,&sendable_stat);

  sendfile(csock,sendable_fd,&offset,sendable_stat.st_size);
  write(csock,"#",2);



  close(sendable_fd);
  std::cout<<"Küldés vége."<<std::endl;
}

client::~client()
{
  close(csock);
}

std::string client::ask_for_username() const
{
  const char* msg = "user";
  write(csock,msg,5);

  std::string res;
  int read_len;
  char read_buf[200];
  //TODO: kulon fuggvenybe!!
  bool end = false;
      while(!end&&(read_len = recv(csock, read_buf, 200, 0)) > 0){
        for(int i=0;i<read_len;i++){
          if(read_buf[i] == '#'){
            end = true;
            break;
          }
          res.push_back(read_buf[i]);
        }
        
      }
      res.pop_back();
      res[res.size()-1]='>';
      res.push_back(' ');
      return res;
}


int main()
{

  client fav_client;
  fav_client.run();




  return 0;
}