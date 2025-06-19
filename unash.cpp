//unashSkeleton.cpp -- the basic skeleton for a unash (UNA shell) program

#include <iostream>
#include <string>

//libraries added
#include <sstream>
#include <vector>
#include <unistd.h>
#include <sys/wait.h>

/* Sources: https://www.geeksforgeeks.org/stringstream-c-applications/
 *			https://www.geeksforgeeks.org/vector-in-cpp-stl/
 *			https://cplusplus.com/reference/vector/vector/
 *			https://cplusplus.com/reference/string/string/c_str/
*/

using namespace std;

string readLine();
char** tokenize(string);
void printTest(char**);
bool execute(char**);

int main()
{
	bool flag=1;
	string line;
	char** argList;

	while(flag)
	{
		cout<<"csis> ";

		line = readLine();
		argList = tokenize(line);
		//IMPORTANT: comment the next line out when done testing tokenize().
		//printTest(argList);
		//***
		flag = execute(argList);
	}

}
/*
char** tokenize(string)
	This function is responsible for accepting the command string as a parameter.
	It returns a char** pointer.  The function dynamically allocates an array of 
	char* pointers equal to the number of (space delimited) words in the command string.
	Each char* pointer is dynamically allocated to exactly store one word from the command.
	Make the string pointed to by each char* pointer is NULL terminated (C-strings).
	Make sure the array of char* pointers pointed to by the char** pointer is NULL terminated.
	(This is what will be expected by the execvp function.)	
*/
char** tokenize(string line)
{
	//create stringstream to be used to get each token
	stringstream line_stream(line);

	//create vector to store each token
	vector<string> lines;

	//store each token in the vector using stringstream
	string current_line = "";
	while(line_stream >> current_line){
		lines.push_back(current_line);
	}

	//create char** from vector
	int size = lines.size();
	char** result = (char**)malloc(size + 1);
		
	//allocate each char* from vector
	for(int i = 0; i < size; ++i){
		int size = lines[i].size();
		result[i] = (char*)malloc(size + 1);
		strncpy(result[i], lines[i].c_str(), size + 1);
	}
		
	//add the null pointer at the end of the char**
	result[size] = (char*)'\0';

	//return result
	return result;
}

/*
bool execute(char** args)
	This function accepts the char** that points to the array of pointers to C-strings.  
	This function uses the fork() command to fork a brand new copy of itself.
	Then, for the child process, it makes use of execvp() to overwrite itself
	with a new program.  Call execvp properly making use of the char**.
	For the parent process (still running the shell program), it waits until
	the child process is terminated.  Make use of waitpid for this. 
*/
bool execute(char** args)
{	
	//added the exit cmd
	if(strncmp(args[0], "exit\0", 4) == 0){
		//deallocate each char* in the char**
		for(int i = 0; args[i] != (char*)'\0'; ++i){
			if(args[i][0] == '\0'){
				break;
			}
			free(args[i]);
		}

		//deallocate the char**
		free(args);

		return false;	
	}

	//fork the process
	pid_t process = fork();
	
	//use execvp
	if(process == 0){
		int error = execvp(args[0], args);	
		
		//if the program does not exist/there is an error, exit
		if(error){
			//deallocate each char* in the char**
			for(int i = 0; args[i] != (char*)'\0'; ++i){
				if(args[i][0] == '\0'){
					break;
				}
				free(args[i]);
			}

			//deallocate the char**
			free(args);
			return false;
		}
	} else {
		//the parent is waiting on child
		waitpid(WAIT_MYPGRP, 0, 0);
	}
		
	//deallocate each char* in the char**
	for(int i = 0; args[i] != (char*)'\0'; ++i){
		if(args[i][0] == '\0'){
			break;
		}
		free(args[i]);
	}

	//deallocate the char**
	free(args);

	//go to next itteration of the loop
	return true;
}
string readLine()
{
	string line;
	getline(cin, line);
	return line;
}
void printTest(char** temp)
{
	int counter = 0;
	while(temp[counter]!=nullptr)
	{
		cout<<temp[counter]<<endl;
		counter++;
	}
}