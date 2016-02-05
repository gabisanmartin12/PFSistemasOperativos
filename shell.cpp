#include <string>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <vector>
using namespace std;

#define MAX_INPUT_LENGTH 160

string prompt = "%> ";

struct command {
	string command;
	string args;
};

// Prototype functions
string         readCommands();
vector<string> separateCommands(string input);
string getCommandAndArguments(string command, string *arguments);

int main() {
	vector<string> commands;
	string input;
	input = readCommands();
	while (input.compare("exit\n") != 0) {
		commands = separateCommands(input);
		string arguments;
		string command = getCommandAndArguments(*(commands.begin()), &arguments);
		cout << command << " + args = " << arguments << endl;
		input = readCommands();
	}
	return 0;
}

/**
* Function to read command from terminal
*
* @return String that was read from stdin
*/
string readCommands() {
	char inputAux[MAX_INPUT_LENGTH];
	char *aux;
	cout << prompt;
	aux = fgets(inputAux, MAX_INPUT_LENGTH, stdin);
	string input(aux);
	return input;
}

/**
* Function to separate commands from input
*
* @param  input String to split by pipe
* @return Vector of strings where each string is a command
*/
vector<string> separateCommands(string input) {
	int pos = 0, posAux = 0;
	vector<string> commands;
	while ((pos = input.find("|", pos)) != string::npos) {
		commands.push_back(input.substr(posAux, pos-posAux-1));
		pos += 2;
		posAux = pos;
	}
	// After last pipe or there isn't pipe
	commands.push_back(input.substr(posAux,input.length()-posAux));
	return commands;
}

/**
* Function to get command and his arguments
*
* @param  command   String that contain the command and his arguments.
* @param  arguments String where returned the arguments
* @return String that contain the command to execute
*/
string getCommandAndArguments(string command, string *arguments) {
	// Find blank space
	int blankSpacePos = command.find(" ",0);
	// Extract command
	string commandAux = command.substr(0,blankSpacePos);
	// Extract arguments
	*arguments        = command.substr(blankSpacePos+1,command.length()-blankSpacePos-1);
	return commandAux;
}
