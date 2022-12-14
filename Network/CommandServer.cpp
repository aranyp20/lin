#include <iostream>
#include "CommandServer.h"

command_server::command_server(data_accessor* const _da) : my_accessor(_da)
{}

void assistant::send_records(const records& r) const
{
    std::vector<char*> res = r.get_data_raw();
    for(int i=0;i<res.size();i++){
        write(socket, res[i], r.get_row_bytes(i));
    }
    
}

void assistant::send_answer(const server_answer& answer) const
{ 
  std::string sendable_msg = answer.message;
  sendable_msg.push_back('\n');  sendable_msg.push_back('\n');
  write(socket,sendable_msg.c_str(),sendable_msg.size());
  send_records(answer.recs);
  write(socket,"#",2);
}

void assistant::recieve_file(const std::string& filename, bool description) const
{
  std::cout<<"A file is comming..."<<std::endl;

  int read_len;
  char read_buf[1024];
  int write_fd;
  std::string t_fname ;
  remove(t_fname.c_str());
  if(description)t_fname = "./Files/Descr/"+filename+".txt";
  else t_fname = "./Files/Sol/"+filename+".txt";

  write_fd = open(t_fname.c_str(),O_RDWR | O_CREAT, 0777);
  bool end = false;
   while(!end&&(read_len = recv(socket, read_buf, 1024, 0)) > 0){
      for(int i=0;i<read_len;i++){
          if(read_buf[i] == '#'){
            end = true;
            break;
          }
          write(write_fd,&read_buf[i],1);
      }
   }
   close(write_fd);    
  std::cout<<"File arrived."<<std::endl;
}

void assistant::send_file(const std::string& filename, bool description) const
{
  std::cout<<"Sending file..."<<std::endl;
  sleep(2);
  int sendable_fd;
  struct stat sendable_stat;
  off_t offset = 0;

  std::string t_fname;
  if(description)t_fname = "./Files/Descr/"+filename+".txt";
  else t_fname = "./Files/Sol/"+filename+".txt";

  sendable_fd = open(t_fname.c_str(), O_RDONLY);
  fstat(sendable_fd,&sendable_stat);

  sendfile(socket,sendable_fd,&offset,sendable_stat.st_size);
  write(socket,"#",2);



  close(sendable_fd);
  std::cout<<"File was sent."<<std::endl;
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
      std::cout<<"Connecting.."<<std::endl;
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

assistant::assistant(data_accessor* const da,int socket) : inter(da),socket(socket)
{

}

void assistant::on_run()
{
  std::cout<<"Assistent assigned to client."<<std::endl;
  char buf[1024];
  int len;
  while((len = recv(socket, buf, sizeof(buf), 0)) > 0)
    {
      request req(connected_acc,buf,len);
      if(req.args[0]=="user"){
        if(connected_acc.get_logged_in())send_answer(server_answer(connected_acc.get_username()));
        else send_answer(server_answer("Unknown"));
        
      }else{
        server_answer answ = inter.interpret(req);
        if(answ.int_code==server_answer::internal_code::LOGIN){
          connected_acc = account(answ.recs.get_data_parsed()[0]);
        }else if(answ.int_code==server_answer::internal_code::LOGOUT){
          connected_acc = account();
        }else if(answ.int_code==server_answer::internal_code::WAIT_FILE){
          recieve_file(std::to_string(answ.int_help));
        }else if(answ.int_code==server_answer::internal_code::SEND_FILE){
          send_file(std::to_string(answ.int_help));
        }else if(answ.int_code==server_answer::internal_code::WAIT_FILE_S){
          recieve_file(std::to_string(answ.int_help),false);
        }else if(answ.int_code==server_answer::internal_code::SEND_FILE_S){
          send_file(std::to_string(answ.int_help),false);
        }
        sleep(1);
        send_answer(answ);
      }
      
    }
    
    printf("Connection closed.\n");
    close(socket);
    dead = true;
}
