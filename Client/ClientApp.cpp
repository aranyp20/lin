#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>

#define PORT "1111"


int main()
{

  struct addrinfo hints;
  struct addrinfo* res;
  int err;
  int csock;
  char buf[1024];
  char read_buf[1024];
  int len;
  int read_len;
  


  /* kitöltjük a hints struktúrát */
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  /* végezzük el a névfeloldást */
  err = getaddrinfo("localhost", PORT, &hints, &res);
  if(err != 0)
  {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
    return -1;
  }
  if(res == NULL)
  {
    return -1;
  }
  
  /* létrehozzuk a kliens socketet */
  csock = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
  if(csock < 0)
  {
    perror("socket");
    return -1;
  }
    
  /* Kapcsolodunk a szerverhez. */
  if(connect(csock, res->ai_addr, res->ai_addrlen) < 0)
  {
    perror("connect");
    return -1;
  }
  
  /* az STDIN_FILENO-n érkező adatokat elküldjük a socketen keresztül */
  while((len = read(STDIN_FILENO, buf, sizeof(buf))) > 0)
  {
    send(csock, buf, len, 0);
    
    while((read_len = recv(csock, read_buf, 1024, 0)) > 0){
      bool end = false;
      for(int i=0;i<read_len;i++){
        if(read_buf[i] == '#'){
          end = true;
          break;
        }
        printf("%c",read_buf[i]);
      }
      
      if(end)break;
    }
    std::cout<<"Küldve."<<std::endl;
  }
  
  /* lezárjuk a szerver socketet */
  close(csock);

  /* szabadítsuk fel a láncolt listát */
  freeaddrinfo(res);




    return 0;
}