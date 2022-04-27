#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <signal.h>

#define	MAX_SIZE_CMD	256			// sets the max size that a command can be
#define	MAX_SIZE_ARG	64			// '' arguments can be 

char cmd[MAX_SIZE_CMD];				// string holder for the command
char *argv[MAX_SIZE_ARG];			// an array for command and arguments
pid_t pid;							// global variable for the child process ID
char i;								// global for loop counter
char *directory;					// path for the directory 

void get_cmd();						// get command string from the user
void convert_cmd();					// convert the command string to the required format by execvp()
void cssh_shell();						// to start the shell

char error_message[30] = "An error has occurred\n"; // error message

int main(){						
	cssh_shell();						// start the shell

	return 0;
}

void cssh_shell(){
	while(1){
		get_cmd();					// get the command from user
		convert_cmd(); 				// put the command into argv

		if(!strcmp(argv[0],"")){	// bypass empty commands
			continue;
		}

        if(!strcmp(argv[0],"exit")){		// check for "exit" command (built-in)
            for(int j = 0; j < 5; j++){
				exit(0);					// temporary loop to force an exit because sometimes exit doesnt work
				j++;
			}
        }

		if (!strcmp(argv[0], "cd")){		// built in cd command
			directory = argv[1];			// takes the dir from stdin for chdir()

			if(chdir(directory) == -1 || strlen(directory) == 0){			// prints error messafe if args were empty (instead of seg fault) or if dir not found
				write(STDERR_FILENO, error_message, strlen(error_message));			
				}
			continue;			// if you don't continue, chdir() will get stuck parsing 

        }  


		pid = fork();		// fork and execute the command
		if(-1 == pid){
			write(STDERR_FILENO, error_message, strlen(error_message));			
		}
		else if(0 == pid){
			execvp(argv[0], argv);		// execute command

		}
		else{
			if(NULL == argv[i]) waitpid(pid, NULL, 0);			// wait for the command to finish if "&" is not present
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

	if(!strcmp("&", argv[i-1])){	// check for "&" to allow for parallel commands
	argv[i-1] = NULL;
	argv[i] = "&";
	}else{
	argv[i] = NULL;
	}
}
