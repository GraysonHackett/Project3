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
		convert_cmd();

		// bypass empty commands
		if(!strcmp(argv[0],"")){
			continue;
		}

		// check for "exit" commandf
        if(!strcmp(argv[0],"exit")){
            exit(0);
        }

		// built in cd command
		if (!strcmp(argv[0], "cd")){
			directory = argv[1];
			// prints error messafe if args were empty (instead of seg fault) or if dir not foudn
			if(chdir(directory) == -1 || strlen(directory) == 0){
				write(STDERR_FILENO, error_message, strlen(error_message));			
				}
			continue;
        }  

		// fit the command into *argv[]

		// fork and execute the command
		pid = fork();
		if(-1 == pid){
			printf("failed to create a child\n");
		}
		else if(0 == pid){
			// printf("hello from child\n");
			// execute a command
			execvp(argv[0], argv);
		}
		else{
			// printf("hello from parent\n");
			// wait for the command to finish if "&" is not present
			if(NULL == argv[i]) waitpid(pid, NULL, 0);
		}
	}
}

// change-directory built in command



void get_cmd(){
	// get command from user
	printf("ccsh> ");
	fgets(cmd, MAX_SIZE_CMD, stdin);
	// remove trailing newline
	if ((strlen(cmd) > 0) && (cmd[strlen (cmd) - 1] == '\n'))
        	cmd[strlen (cmd) - 1] = '\0';
}

void convert_cmd(){
	// split string into argv
	char *ptr;
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
