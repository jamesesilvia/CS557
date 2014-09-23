/* Tyler Carroll - Password Wallet - tcc_pw_wallet.cpp */
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <list>
#include <sstream>
#include <fstream>
#include <errno.h>
#include <termios.h>

#define WALLET "/home/tcarroll/cs557/proj1/wallet.txt"

using namespace std;

bool add_password(string name, string password);
bool remove_password(string name);
bool list_passwords(void);
bool check_master_password(string password);

int main(int argc, char *argv[])
{
	string master, command, tmp, name, pw;
	bool login = false;
    bool first_time = true;
 
    /* set tty flags to hide input */
    termios oldt;
    termios newt;
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;
    newt.c_lflag &= ~ECHO;
   
    /* open password wallet file */
    ifstream wallet_in;
    ofstream wallet_out;
    wallet_in.open(WALLET, fstream::in | fstream::app);
    wallet_out.open(WALLET, fstream::out | fstream::app);

    /* find master password in wallet */
    while (getline(wallet_in, command)) {
        if (strstr(command.c_str(),"master")) {
            master = command.substr(command.find(":")+1,string::npos);
            first_time = false;
            break;
        }
    }

    /* enter master password for first time login */
    if (first_time) {
        cout << "First time login, enter your master password: ";
        cout.flush();
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        getline(cin, master);
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        cout << endl;

        /* 
         * wipe out file, maybe someone hacked the file
         * and cleared the master so they could put in their own
         */
        wallet_out.close();
        wallet_out.open(WALLET, fstream::out | fstream::trunc);

        wallet_out << "master:" << master << endl;
    }
	
    while (!login) {
        
        cout << "Enter Password: ";
        cout.flush();
        tcsetattr(STDIN_FILENO, TCSANOW, &newt);
        getline(cin, command);
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        cout << endl;

        if ((strcmp(command.c_str(), master.c_str())) == 0) {
            cout << "Login Successful!" << endl;
            login = true;
        } else {
            cout << "Login Failure" << endl;
        }
    }
	
	/* Send/Recieve commands */
	while (login) {
		
        /* Get the first word and see what command it is */
        cout << "-> ";
        cout.flush();
        getline(cin, command);

        stringstream temp(command);

        if (!(temp >> tmp)) {
            continue;
        } else if (!( (tmp == "add")||(tmp == "remove")||(tmp == "list")||(tmp == "quit") )) {
            cout << "Valid commands are: add remove list quit" << endl;
            continue;
        } else if(tmp == "add") {
            cout << "Please enter the name to associate with the password: ";
            cout.flush();
            getline(cin, name);

            cout << "Please enter the password: ";
            cout.flush();
            tcsetattr(STDIN_FILENO, TCSANOW, &newt);
            getline(cin, pw);
            tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
            cout << endl;

            wallet_out << name << ":" << pw << endl;

            cout << "Stored in password wallet!" << endl;
            
        } else if(tmp == "remove") {

        } else if(tmp == "list") {
            cout << "Please enter the name of the password you are looking for: ";
            cout.flush();
            getline(cin, name);

            while (getline(wallet_in, command)) {
                if (strstr(command.c_str(), name.c_str())) {
                    pw = command.substr(command.find(":")+1,string::npos);
                    cout << "The password for " << name << " is: " << pw << endl; 
                }
            }

        } else if (tmp == "quit") {
            cout << "Logging out" << endl;
            login = false;
            break;
        }	

	}

    wallet_in.close();
    wallet_out.close();

	exit(0);
}

bool add_password(string name, string password) {
    return true;
}
bool remove_password(string name) {
    return true;
}
bool list_passwords(void) {
    return true;
}
