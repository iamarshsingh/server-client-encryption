#include <iostream>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "RSAClass.h"

static void usage();

int main(int argc, char *argv[])
{
    if (argc > 1 && *(argv[1]) == '-')
    {
        usage(); exit(1);
    }

    int listenPort = 1234;
    if (argc > 1)
        listenPort = atoi(argv[1]);

    //Asking for the length of keys
    int key_len;
    cout<<"Please select keys encryption length (64 recommended estimated wait time: 1 minute): ";
    cin>>key_len;

	//Creating the RSAClass instance
	RSAClass rsa(key_len);
	string public_key = rsa.e.to_string();

    std::cout<<"RSA keys are ready"<<std::endl;
    std::cout<<"The modulus: "<<rsa.n<<std::endl;
    std::cout<<"Prime p: "<<rsa.p<<std::endl;
    std::cout<<"Prime q: "<<rsa.q<<std::endl;
	std::cout<<"Public Key: "<<rsa.e<<std::endl;
	std::cout<<"Private Key: "<<rsa.d<<std::endl;

    // Create a socket
    int s0 = socket(AF_INET, SOCK_STREAM, 0);
    if (s0 < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }
	std::cout<<"Socket Created!"<<std::endl;

    // Fill in the address structure containing self address
    struct sockaddr_in myaddr;
    memset(&myaddr, 0, sizeof(struct sockaddr_in));
    myaddr.sin_family = AF_INET;
    myaddr.sin_port = htons(listenPort);        // Port to listen
    myaddr.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind a socket to the address
    int res = bind(s0, (struct sockaddr*) &myaddr, sizeof(myaddr));
    if (res < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }

	std::cout<<"Socket Binded to localhost"<<std::endl;

    // Set the "LINGER" timeout to zero, to close the listen socket
    // immediately at program termination.
    struct linger linger_opt = { 1, 0 }; // Linger active, timeout 0
    setsockopt(s0, SOL_SOCKET, SO_LINGER, &linger_opt, sizeof(linger_opt));

	std::cout<<"Listening"<<std::endl;

    // Now, listen for a connection
    res = listen(s0, 1);    // "1" is the maximal length of the queue
    if (res < 0)
    {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }


    // Accept a connection (the "accept" command waits for a connection with
    // no timeout limit...)
    struct sockaddr_in peeraddr;
    socklen_t peeraddr_len;
    int s1 = accept(s0, (struct sockaddr*) &peeraddr, &peeraddr_len);
    if (s1 < 0)
    {
		std::cout<<"here1"<<endl;
        std::cerr << "Error: " << errno << std::endl;
        exit(1);
    }

    // A connection is accepted. The new socket "s1" is created
    // for data input/output. The peeraddr structure is filled in with
    // the address of connected entity, print it.
    std::cout << "Connection from IP "
              << ( ( ntohl(peeraddr.sin_addr.s_addr) >> 24) & 0xff ) << "."  // High byte of address
              << ( ( ntohl(peeraddr.sin_addr.s_addr) >> 16) & 0xff ) << "."
              << ( ( ntohl(peeraddr.sin_addr.s_addr) >> 8) & 0xff )  << "."
              <<   ( ntohl(peeraddr.sin_addr.s_addr) & 0xff ) << ", port "   // Low byte of addr
              << ntohs(peeraddr.sin_port)
			  << std::endl;

    res = close(s0);    // Close the listen socket

	//write(s1, "gell", 5);
    write(s1, public_key.data() , public_key.length()+1);
	write(s1, rsa.n.to_string().data() , rsa.n.to_string().length()+1);

    char buffer[1024];
    res = read(s1, buffer, 1023);
    if (res < 0) {
        std::cerr << "Error: " << strerror(errno) << std::endl;
        exit(1);
    }
    buffer[res] = 0;
    std::cout << "Received Encrypted Message of " << res << " bytes:\n" << buffer << std::endl;

	string decrepted_message = rsa.decrypt(buffer);
	std::cout << "decrypted Message: " << decrepted_message << std::endl;

    close(s1);
    return 0;
}

static void usage() {
    std::cout << "A simple Internet server application.\n"
              << "It listens to the port written in command line (default 1234),\n"
              << "accepts a connection, and sends the \"Hello!\" message to a client.\n"
              << "Then it receives the answer from a client and terminates.\n\n"
              << "Usage:\n"
              << "     server [port_to_listen]\n"
              << "Default is the port 1234.\n";
}
