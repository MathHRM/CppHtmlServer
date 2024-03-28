// Imports
#include <iostream>
#include <tchar.h>

// API imports
#include <WinSock2.h>
#include <WS2tcpip.h>

// A Pragma comment. Loading Library 
// about pragma: https://learn.microsoft.com/pt-br/cpp/preprocessor/comment-c-cpp?view=msvc-170
#pragma comment(lib, "ws2_32.lib")


int main() {
	// variables
	const int BUFFER = 1500;
	const int PORT = 80;

	struct sockaddr_in server;
	SOCKET server_socket;
	SOCKET client_socket;

	WSADATA wsa; // WSA( Windows Socket Api )
	int result;

	const char message[BUFFER] = "Message to be displayed\r";
	char reply[BUFFER] = { 0 };

	

	// About WSAStartup
	// It allows an application or DLL to specify the version of Windows Sockets required 
	// and retrieve details of the specific Windows Sockets implementation. 
	// The application or DLL can only issue further Windows Sockets functions after successfully calling WSAStartup.
	// See more in: https://learn.microsoft.com/en-us/windows/win32/api/winsock/nf-winsock-wsastartup
	result = WSAStartup(MAKEWORD(2, 2), &wsa);
	if (result != 0) {
		std::cerr << "WinsocK Failed" << std::endl;
		getchar();
		return 1;
	}


	// See about socket in: https://www.geeksforgeeks.org/socket-programming-in-cpp
	server_socket = socket(AF_INET, SOCK_STREAM, NULL);

	if (server_socket == SOCKET_ERROR)
	{
		std::cerr << "Socket Error : " << WSAGetLastError() << std::endl;
		getchar();
		return 1;
	}
	if (server_socket == INVALID_SOCKET) {
		std::cerr << "Socket Error : " << WSAGetLastError() << std::endl;
		getchar();
		return 1;
	}


	server.sin_family = AF_INET;
	server.sin_port = htons( PORT ); // Defining PORT
	InetPton(AF_INET, TEXT( "0.0.0.0" ), &server.sin_addr); // Defining The Network Address to Run the Server on

	result = bind(server_socket, (struct sockaddr*)&server, sizeof(server)); // binding the Host Address and Port Number
	
	if (result == SOCKET_ERROR)
	{
		std::cerr << "Bind Error " << WSAGetLastError() << std::endl;
		getchar();
		return 1;
	}

	std::cout << "Listening on : https://localhost:" << PORT << std::endl; // Tell the User we Started Listening.
	result = listen(server_socket, AF_INET); // Then Actually Start Listening for incoming Connections.


	client_socket = accept(server_socket, NULL, NULL);

	if (client_socket == SOCKET_ERROR) { // if Accepting Connection is a Error
		std::cerr << "Accept FAiled!" << WSAGetLastError() << std::endl;
		getchar(); 
		return 1;
	}

	std::cout << "A Client Connected. Sending a Message and closing Connection" << std::endl; // notify user that someone connected
	send(client_socket, message, strlen(message), NULL); // Send Client a Message

	recv(client_socket, reply, sizeof(reply), NULL); // Just in case if the Client sends something, We Receive it.

	closesocket(client_socket); // close the Client Socket now that our Work is Complete.
	WSACleanup(); // Clean Windows Socket API.

	getchar();
	return 0;
}