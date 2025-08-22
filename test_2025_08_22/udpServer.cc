#include "udpServer.hpp"
#include <memory>

using namespace std;

int main()
{
    unique_ptr<udpServer> usvr(new udpServer());
    return 0;
}