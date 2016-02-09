/**
 *   shell.cpp
 *   Purpose: Simulate a linux terminal
 *
 *   @author Gabriel San Martín
 *   @version 1.0 05/02/2016
 */
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <cstdio>
#include <iostream>
#include <cstring>
#include <vector>
#include <cstdlib>
using namespace std;

#define MAX_INPUT_LENGTH 160

/**
* Contain the welcome message
*
* @var string
*/
string welcome  = string("TRABAJO PRACTICO FINAL DE SISTEMAS OPERATIVOS\n")
				+ string("Objetivo: Desarrollo de un shell\n")
				+ string("Alumno: San Martín, Gabriel\n")
				+ string("Profesores:\n")
				+ string("\tProdolliet, Jorge\n\tMannarino, Gabriela\n");
/**
 * Contain the prompt
 *
 * @var string
 */
string prompt = "%> ";
/**
 * Define a instruction how an struct with a command and arguments
 *
 * @var struct
 */
struct instruction {
	string command;
	vector<string> arguments;
};

/**
 * Function to read command from terminal
 *
 * @return String that was read from stdin
 */
string readCommands();
/**
 * Function to get the vector of instructions to execute
 *
 * @param  input String to split by pipe
 * @return Vector of instructions to execute
 */
vector<instruction> separateInstructions(string input);
/**
 * Function to get the instruction
 *
 * @param  command String that contain the command and his arguments.
 * @return Instruction to execute
 */
instruction getInstruction(string command);
/**
 * Function to convert an instruction to the params type needed to pass execvp
 *
 * @param com The instruction to convert
 * @param comand The char array where I will return the command to execute
 * @return A char 2D array with the arguments of the command to execute
 */
char** convertInstruction(instruction com, char* &command);
/**
 * Function to run instruction
 *
 * @param com   The command to execute
 * @param args  The arguments to the command
 * @param error Variable where I will return the error messages
 * 
 * @return A integer that represents if the execution was successfully or not
 */
int run(char *com, char **args, string *error);
/**
 * Function to show an instruction
 *
 * @param com The instruction to show
 */
void showInstruction(instruction com);
/**
* Function to add the cwd to the PATH environment variable
*/
void addPath();
/**
* Function to remove the cwd to the PATH environment variable
*/
void removePath();
/**
* Function to show the content of the PATH environment varible
*/
void showPath();
	
int main() {
	// Print welcome message
	cout << welcome;
	// Read input
	string input;
	input = readCommands();
	// Check if command is not exit
	while (input.compare("exit\n") != 0) {
		// Create vector of instructions to storing instructions separated by pipe
		vector<instruction> instructions;
		// Separate the instructions from the input string
		instructions = separateInstructions(input);
		// Execute each instruction
		vector<instruction>::iterator it = instructions.begin();
		for (it; it != instructions.end(); it++) {
			// Check if command is minishell
			if ((*it).command.compare("minishell") == 0) {
				// It's then check the argument
				string arg = (*it).arguments.back();
				if (arg.compare("+") == 0) {
					addPath();
				} else {
					removePath();
				}
			} else {
				char *com, **args;
				// Convert instruction to the params needed by exec
				args = convertInstruction((*it), com);
				string error;
				// Execute command
				if (run(com, args, &error) == -1) {
					// If there are errors, show them
					cout << error << endl;
				};
			}
		}
		// Read input again
		input = readCommands();
	}
	return 0;
}

// Function to read command from terminal
string readCommands() {
	char inputAux[MAX_INPUT_LENGTH];
	char *aux;
	cout << prompt;
	aux = fgets(inputAux, MAX_INPUT_LENGTH, stdin);
	string input(aux);
	return input;
}

// Function to get the vector of instructions to execute
vector<instruction> separateInstructions(string input) {
	int pos    = 0,
		posAux = 0;
	vector<string> commands;
	// Find pipes positions
	while ((pos = input.find("|", pos)) != string::npos) {
		string sub = input.substr(posAux, pos-posAux);
		commands.push_back(sub);
		pos    += 2;
		posAux  = pos;
	}
	// After last pipe or there isn't pipe
	commands.push_back(input.substr(posAux, input.length()-posAux));
	// Define vector of instructions
	vector<instruction> instructions;
	// Get the instructions to execute
	for (vector<string>::iterator it = commands.begin(); it != commands.end(); it++) {
		instruction com = getInstruction(*it);
		instructions.push_back(com);
	}
	return instructions;
}

// Function to get the instruction
instruction getInstruction(string command) {
	instruction com;
	// Find blank space
	int blankSpacePos = command.find(" ",0);
	if (blankSpacePos != -1) {
		// Extract command and the arguments.
		com.command = command.substr(0, blankSpacePos);
		com.arguments.push_back(com.command);
		blankSpacePos++;
		int blankSpacePosAux  = blankSpacePos;
		while ((blankSpacePos = command.find(" ", blankSpacePos)) != string::npos) {
			string sub = command.substr(blankSpacePosAux, blankSpacePos-blankSpacePosAux);
			if (!sub.empty()) {
				com.arguments.push_back(sub);
			}
			blankSpacePos++;
			blankSpacePosAux  = blankSpacePos;
		}
		// After last pipe or there isn't pipe
		string sub = command.substr(blankSpacePosAux, command.length()-blankSpacePosAux-1);
		if (!sub.empty()) {
			com.arguments.push_back(sub);
		}
	} else {
		// If there isn't blank space is beacause of the command has not arguments
		com.command = command.substr(0, command.length()-1);
		com.arguments.push_back(com.command);
	}
	return com;
}

// Function to convert an instruction to the params type needed to pass execvp
char** convertInstruction(instruction com, char* &command) {
	command = new char[com.command.length()];
	std::copy(com.command.begin(), com.command.end(), command);
	command[com.command.length()] = '\0';
	char **args = new char* [com.arguments.size()+1];
	args[0] = new char[strlen(command)];
	args[0] = command;
	vector<string>::iterator it = com.arguments.begin()+1;
	for (int i=1; i<com.arguments.size(); i++) {
		args[i] = new char[(*it).length()];
		std::copy((*it).begin(), (*it).end(), args[i]);
		args[i][(*it).length()] = '\0';
		it++;
	}
	args[com.arguments.size()] = new char[1];
	args[com.arguments.size()] = (char *)0;
	return args;
}

// Function to run instruction
int run(char *com, char **args, string *error) {
	int ret;
	// Create a new process
	pid_t pid = fork();
	if (pid < 0) {
		*error += "It could not create the process.\n";
		exit(-1);
	}
	// If pid is 0 then it is the child process
	if (pid == 0) {
		if ((ret = execvp(args[0],args)) == -1) {
			*error += "Error: Fail to execute the command.\n";
			exit(-1);
		}
		exit(1);
	} else {
		sleep(1);
	}
	return ret;
}

// Function to show an instruction
void showInstruction(instruction com) {
	cout << "COMMAND  : " << com.command << endl;
	cout << "ARGUMENTS: " << endl;
	int num = 0;
	for (vector<string>::iterator it = com.arguments.begin(); it != com.arguments.end(); it++) {
		// Show number of argument and the argument
		cout << "[" << num << "] " << *it << endl;
		num++;
	}
}

// Function to add the cwd to the PATH environment variable
void addPath(){
	int	ret = system("sh addPath.sh");
	if (ret == -1) {
		cout << "Failed to add path to path environment variable." << endl;
	}
}

// Function to remove the cwd to the PATH environment variable
void removePath() {
	int ret = system("sh removePath.sh");
	if (ret == -1) {
		cout << "Failed to add path to path environment variable." << endl;
	}
}

// Function to show the content of the PATH environment varible
void showPath() {
	char* path = getenv("PATH");
	printf("%s\n", path);
}
