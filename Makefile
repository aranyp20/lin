# Makefile
CPPFLAGS=-pthread
LDFLAGS=-pthread

objs = main.o ./General/Account.o ./DataLayer/DataAccess.o
objs = main.o   ./Network/Interpreter.o ./Network/CommandServer.o ./DataLayer/DataAccess.o   ./General/Gens.o ./General/Account.o 

prg: $(objs)
	g++ -g -o prg $(objs) -L/usr/include/mysql -lmysqlclient -I/usr/include/mysql
