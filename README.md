# server-client-encryption
Vanilla implementation of RSA Encryption in Client Server model in C++


## Features
1. The client is the sender of the secret message and server receives and decrypts the message.
2. The Encryption key length can be set in the server execution.
3. Used a open source library for Big Integers in C++.
4. Primes are randomely generated and tested using Miller-Rabin Primality test.
5. Public Key and Modulus is shared with the Client


## Usage
1. Clone the master git repository:
`git clone --recursive https://github.com/iamarshsingh/server-client-encryption`

2. Navigate to src directory and execute the makefile:
`../make`

3. Firstly execute:
`./server`
and then enter the desired encryption key primes length.

4. Open a second terminal in the same directory and execute:
`./client`
