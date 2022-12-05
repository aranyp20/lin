# Makefile
CPPFLAGS=-pthread
LDFLAGS=-pthread

objs = main.o    ./Network/CommandServer.o ./DataLayer/DataAccess.o   ./General/Gens.o ./General/Account.o 

prg: $(objs)
	g++ -g -o prg $(objs) -L/usr/include/mysql -lmysqlclient -I/usr/include/mysql