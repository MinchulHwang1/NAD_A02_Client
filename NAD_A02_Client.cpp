/*
* File Name         : NAD_A02_Client.cpp 
* Programmer        : Min Chul Hwang - 8818858
* Submission Date   : 2/24/2024
* Purpose           : This program is client side made as CPP file.
*                     This program can correspond with server side which is logging service through the TCP connection
*                     it can take command, and file name include IP and port in command line argument.
*                     and send them to server through TCP connection
* Function List     : FileOpen, generateUniqueID, globalUniqueID, SendTCPMessage.
*/

#include <iostream>
#include <string>
#include <fstream>
#include <WinSock2.h>
#include <WS2tcpip.h>
#include <chrono>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

void FileOpen(string command, string fileName);

string generateUniqueID();

string globalUniqueID = generateUniqueID();

//void SendUDPMessage(const char* ip, int port, const std::string& message);

void SendTCPMessage(const char* ip, int port, const std::string& message);


/*
* Main Function
* Purpose       : This function get IP and port number from command line argument.
*                 and also user prompt command and file name, and send it to SednTCPMessage function.
*/
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
    cout << "Your number : " << globalUniqueID << endl;
    while (true) {
        cin >> command >> fileName;

        if (command == "-q") {
            string message = command + " " + fileName;
            SendTCPMessage(ip, port, message);
            break;
        }
        else {
            FileOpen(command, fileName);
            string message = command + " " + fileName;

            // Send UDP message
            SendTCPMessage(ip, port, message);
        }
    }

	return 0;
}


/*
* Function Comment Header
* Function Name         : FileOpen
* Purpose               : This function can figure the file is exist or not.
*                         but also, it can translate what user input is and open, close, read, and write in the file.
* Input                 : command           string          the contents that contain information of command
*                         fileName          string          the contents that contain file name
* Output                : A proper message followed each command
* Return                : NONE
*/
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

/*
* Function Comment Header
* Function Name         : generateUniqueID
* Purpose               : This function generates unique number come up with current time to give unique number as clients when they connect with server
*                         and it can be distinguished as number
* Input                 : NONE
* Output                : NONE
* Return                : Unique number based on current time
*/
string generateUniqueID() {
    auto now = chrono::system_clock::now();
    auto duration = now.time_since_epoch();
    auto millis = chrono::duration_cast<chrono::milliseconds>(duration).count();
    return to_string(millis);
}


/*
* Function Comment Header
* Function Name         : SendTCPMessage
* Purpose               : This function send message what user write to server called logging service.
* Input                 : ip        const char*         the ip address written in command line arguments
*                         port      int                 the port number written in command line arguments
*                         message   string              the message contains information which user input
* Output                : The proper message what user input
* Return                : NONE
*/
void SendTCPMessage(const char* ip, int port, const std::string& message) {
    // Initialize Winsock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed\n";
        return;
    }

    // Create socket
    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return;
    }

    // Get Server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    inet_pton(AF_INET, ip, &serverAddr.sin_addr);

    // Connect to server
    if (connect(sockfd, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "Error connecting to server: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return;
    }

    // Apprend message and unique number
    string messageWithID = globalUniqueID + " " + message;

    // Send data
    int bytesSent = send(sockfd, messageWithID.c_str(), messageWithID.length(), 0);
    if (bytesSent == SOCKET_ERROR) {
        std::cerr << "Error sending message: " << WSAGetLastError() << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return;
    }

    std::cout << "Message sent successfully.\n";

    // Clean up socket
    closesocket(sockfd);
    WSACleanup();
}



//void SendUDPMessage(const char* ip, int port, const std::string& message) {
//    // Initialize Winsock
//    WSADATA wsaData;
//    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
//        std::cerr << "WSAStartup failed\n";
//        return;
//    }
//
//    // Create socket
//    SOCKET sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
//    if (sockfd == INVALID_SOCKET) {
//        std::cerr << "Error creating socket: " << WSAGetLastError() << std::endl;
//        WSACleanup();
//        return;
//    }
//
//    // Server address
//    sockaddr_in serverAddr;
//    serverAddr.sin_family = AF_INET;
//    serverAddr.sin_port = htons(port);
//    inet_pton(AF_INET, ip, &serverAddr.sin_addr);
//
//    string messageWithID = globalUniqueID + " " + message;
//
//    // Send data
//    int bytesSent = sendto(sockfd, message.c_str(), message.length(), 0, (sockaddr*)&serverAddr, sizeof(serverAddr));
//    if (bytesSent == SOCKET_ERROR) {
//        std::cerr << "Error sending message: " << WSAGetLastError() << std::endl;
//        closesocket(sockfd);
//        WSACleanup();
//        return;
//    }
//
//    std::cout << "Message sent successfully.\n";
//
//    // Clean up
//    closesocket(sockfd);
//    WSACleanup();
//}