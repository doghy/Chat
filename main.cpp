#include <iostream>
#include "server/Server.hpp"
int main(int argc, char* argv[])
{
    server::Server srv(5000);
    srv.dealWithIncommingConnections();

    return 0;
}
