#include <iostream>
#include <string>
#include <fstream>
#include <WinSock2.h>
#include <WS2tcpip.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

void FileOpen(string command, string fileName);


void SendUDPMessage(const char* ip, int port, const std::string& message) {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return;
    }

    // Create socket
    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    // Server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serverAddr.sin_addr); // ipv4 주소 설정

    // Send data
    int bytesSent = sendto(sockfd, message.c_str(), message.length(), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Error sending message: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return;
    }

    std::cout << "Message sent successfully.\n";

    // Clean up
    closesocket(sockfd);
    WSACleanup();
}


int main(int argc, char* argv[]) {
	
	if (argc < 3) {
		cerr << "Use : -command fileName" << endl;
		cerr << "Command : -o : Open, -c : close, -r : read, -w : write, -q : quit" << endl;
		return 1;
	}

	// Get command line argument
	const char* ip = argv[1];
    const int port = stoi(argv[2]);

	// Get command from user
	string command;
	string fileName;

    while (true) {
        cin >> command >> fileName;

        if (command == "-q") {
            break;
        }
        else {
            FileOpen(command, fileName);
            string message = command + " " + fileName;

            // Send UDP message
            SendUDPMessage(ip, port, message);
        }
    }



	return 0;

}

void FileOpen(string command, string fileName) {
    ifstream inFile;
    ofstream outFile;

    if (command == "-o") {
        inFile.open(fileName);
        if (!inFile) {
            cerr << "The file does not exist" << endl;
        }
        else {
            printf("File opened.\n");
        }
        inFile.close();
    }
    else if (command == "-c") {
        inFile.open(fileName);
        if (!inFile) {
            cerr << "The file does not exist" << endl;
        }
        else {
            inFile.close();
            cout << "File closed." << endl;
        }
    }
    else if (command == "-r") {
        inFile.open(fileName);
        if (!inFile) {
            cerr << "The file does not exist" << endl;
        }
        cout << "File contents : " << endl;
        string line;
        while (getline(inFile, line)) {
            cout << line << endl;
        }
        inFile.close();
    }
    else if (command == "-w") {
        ofstream outFile(fileName);
            if (!outFile) {
                cerr << "The file does not exist." << endl;
            }
            //cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            cout << "Enter what you want to write to the file. Press Ctrl+Z to complete your entry." << endl;
            string input;
            while (getline(cin, input)) {
                outFile << input << endl;
            }
            cout << "Content is written successfully" << endl;
            
            outFile.close();
            cin.clear();
    }
   
    else {
        cerr << "Use : -command fileName" << endl;
        cerr << "Command : -o : Open, -c : close, -r : read, -w : write, -q : quit" << endl;
    }

}