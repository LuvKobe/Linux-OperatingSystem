#include "udpClient.hpp"
#include <memory>

using namespace std;
using namespace Client;

static void Usage(string proc)
{
    cout << "\nUsage:\n\t" << proc << " server_ip server_port\n\t" << endl;
}

/*
./udpClient server_ip server_port
*/
int main(int argc, char* argv[])
{
    if (argc != 3)
    {
        Usage(argv[0]);
        exit(USAGE_ERR);
    }
    string serverIp = argv[1];
    uint16_t serverPort = atoi(argv[2]);

    unique_ptr<udpClient> ucli(new udpClient(serverIp, serverPort));

    ucli->initClient();
    ucli->runClient();

    return 0;
}