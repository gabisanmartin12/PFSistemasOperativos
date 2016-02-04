#include <string>
#include <cstdio>
#include <iostream>
using namespace std;

#define MAX_INPUT_LENGTH 160

char prompt = '%';

void readCommand(char input[]);

int main() {
	char inputBuffer[MAX_INPUT_LENGTH];
	readCommand(inputBuffer);
	printf(inputBuffer);
	return 0;
}

/**
* Function to read command from terminal
*
* @param input Char array to storing entered commands.
*/
void readCommand(char *input) {
	printf("%c>", prompt);
	char *aux;
	aux = fgets(input, MAX_INPUT_LENGTH, stdin);
}
