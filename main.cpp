#include <iostream>

#include "DataLayer/DataAccess.h"
#include "Network/CommandServer.h"





int main()
{
    
    data_accessor da;

    command_server cs(&da);
    cs.Run();

    return 0;
}