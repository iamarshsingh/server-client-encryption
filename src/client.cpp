#include <iostream>
#include <sstream>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "bigint.h"

static void usage();
std::string encrypt(std::string message, bigint public_key, bigint modulus);
bigint power(bigint a, bigint b, bigint m);


int main(int argc, char *argv[])
{
    if (argc > 1 && *(argv[1]) == '-')
    {
        usage(); exit(1);
    }

    // Create socket
    int s0 = socket(AF_INET, SOCK_STREAM, 0);
    if (s0 < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    // Fill in server IP address
    struct sockaddr_in server;
    int serverAddrLen;
    bzero( &server, sizeof( server ) );


    char* peerHost = "localhost";
    if (argc > 1)
        peerHost = argv[1];

    // Resolve server address (convert from symbolic name to IP number)
    struct hostent *host = gethostbyname(peerHost);
    if (host == NULL)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    server.sin_family = AF_INET;
    short peerPort = 1234;
    if (argc >= 3)
        peerPort = (short) atoi(argv[2]);
    server.sin_port = htons(peerPort);

    // Print a resolved address of server (the first IP of the host)
    std::cout << "server address = " << (host->h_addr_list[0][0] & 0xff) << "." <<
                                        (host->h_addr_list[0][1] & 0xff) << "." <<
                                        (host->h_addr_list[0][2] & 0xff) << "." <<
                                        (host->h_addr_list[0][3] & 0xff) << ", port " <<
                                        static_cast<int>(peerPort) << std::endl;

    // Write resolved IP address of a server to the address structure
    memmove(&(server.sin_addr.s_addr), host->h_addr_list[0], 4);

    // Connect to the remote server
    int res = connect(s0, (struct sockaddr*) &server, sizeof(server));
    if (res < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    std::cout << "Connected. Reading a server message" << std::endl;

    char buffer[1024];
    res = read(s0, buffer, 1024);
    if (res < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    std::cout << "Received Public Key:" << "\n" << buffer << std::endl;

    bigint public_key(buffer);

    res = read(s0, buffer, 1024);
    if (res < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

    std::cout << "Received Modulus:" << "\n" << buffer << std::endl;

    bigint modulus(buffer);

    // std::cout<<public_key<<" "<<modulus<<std::endl;

    string msg;
    std::cout<<"Enter the message here: "<< endl;
    getline(cin, msg);
    std::string cryptic_msg = encrypt(msg, public_key, modulus);
    std::cout<<"Encrypted Message senting to Server: "<<cryptic_msg<<std::endl;
    write(s0, cryptic_msg.data(), cryptic_msg.length()+1);

    close(s0);
    return 0;
}

static void usage()
{
    std::cout << "A simple Internet client application.\n"
              << "Usage:\n"
              << "         client [IP_address_of_server [port_of_server]]\n"
              << "     where IP_address_of_server is either IP number of server\n"
              << "     or a symbolic Internet name, default is \"localhost\";\n"
              << "     port_of_server is a port number, default is 1234.\n"
              << "The client connects to a server which address is given in a\n"
              << "command line, receives a message from a server, sends the message\n"
              << "\"Thanks! Bye-bye...\", and terminates.\n";
}

bigint power(bigint a, bigint b, bigint m){
	bigint ans = 1;
	while (b > 0)
	{
		if (b % 2 == 1)
			ans = (ans*a)%m;

		b /= 2;
		a = (a*a)%m;
	}
	return ans;
}

std::string encrypt(std::string msg, bigint public_key, bigint n){
    std::string num_message="";

	//Coverting the message to a number
	for (size_t i = 0 ; i < msg.length(); ++i){
		std::string curr = to_string(static_cast<int>(static_cast<unsigned char>(msg[i])));
		if(curr.length()==2) curr = "0"+curr;
	    num_message += curr;
	}
	std::cout<<"Text encoded to String: "<<num_message<<std::endl;

	bigint msg_in_num;
	msg_in_num.read(num_message);
	bigint crypt = power(msg_in_num, public_key, n);
	return crypt.to_string();
}
