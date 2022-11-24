# Makefile

objs = main.o ./General/Account.o ./DataLayer/DataAccess.o

prg: $(objs)
	g++ -g -o prg $(objs) -L/usr/include/mysql -lmysqlclient -I/usr/include/mysql