#include <iostream>
#include <string>
#include <fstream>

using namespace std;

bool FileOpen(string command, string fileName);
// 추후에 IP와 포트를 1, 2번으로 넣고, 명령어, 파일이름, 내용은 뒤에다가 넣는걸로
int main(int argc, char* argv[]) {
	
	if (argc < 3) {
		cerr << "Use : -command fileName" << endl;
		cerr << "Command : -o : Open, -c : close, -r : read, -w : write, -q : quit" << endl;
		return 1;
	}

	// Get command line argument
	//const char* ip = argv[1];
	//const char* port = argv[2];


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
        }
    }



	return 0;

}

bool FileOpen(string command, string fileName) {
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
                return true;
            }
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

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

    return true;
}