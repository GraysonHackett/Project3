#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define	MAX_SIZE_CMD	256
#define	MAX_SIZE_ARG	16
#define TAM 1000

char cmd[MAX_SIZE_CMD];				// string holder for the command
char *argv[MAX_SIZE_ARG];			// an array for command and arguments
pid_t pid;					// global variable for the child process ID
char i;						// global for loop counter
char *directory;			// path for the directory 

void get_cmd();					// get command string from the user
void convert_cmd();				// convert the command string to the required format by execvp()
void c_shell();					// to start the shell

char error_message[30] = "An error has occurred\n"; // error message

int main(){
	// tie the handler to the SGNCHLD signal

	// start the shell
	c_shell();

	return 0;
}

void c_shell(){
	while(1){
		// get the command from user
		get_cmd();
		// put the command into argv
		convert_cmd();

		// bypass empty commands
		if(!strcmp(argv[0],"")){
			continue;
		}

		// check for "exit" command
        if(!strcmp(argv[0],"exit")){
            for(int j = 0; j < 5; j++){
				exit(0);					// temporary loop to force an exit because sometimes exit doesnt work
				j++
			}
        }

		// built in cd command
		if (!strcmp(argv[0], "cd")){
			// takes the dir from stdin for chdir()
			directory = argv[1];

			// prints error messafe if args were empty (instead of seg fault) or if dir not foudn
			if(chdir(directory) == -1 || strlen(directory) == 0){
				write(STDERR_FILENO, error_message, strlen(error_message));			
				}
			continue;
			// if you don't continue, chdir() will get stuck parsing 
        }  


		// fork and execute the command
		pid = fork();
		if(-1 == pid){
			write(STDERR_FILENO, error_message, strlen(error_message));			
		}
		else if(0 == pid){
			// execute command
			execvp(argv[0], argv);
		}
		else{
			// wait for the command to finish if "&" is not present
			if(NULL == argv[i]) waitpid(pid, NULL, 0);
		}
	}
}


void get_cmd(){
	printf("ccsh> ");
	fgets(cmd, MAX_SIZE_CMD, stdin);				// gets command from user

	if ((strlen(cmd) > 0) && (cmd[strlen (cmd) - 1] == '\n')) 		// removes the trailing newline
        	cmd[strlen (cmd) - 1] = '\0';
}

void convert_cmd(){
	char *ptr;				// split the string into argv
	i = 0;
	ptr = strtok(cmd, " ");
	while(ptr != NULL){
		argv[i] = ptr;
		i++;
		ptr = strtok(NULL, " ");
	}

	// check for "&"
	if(!strcmp("&", argv[i-1])){
	argv[i-1] = NULL;
	argv[i] = "&";
	}else{
	argv[i] = NULL;
	}
}
