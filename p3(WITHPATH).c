#include <stdio.h> // for getline(), printf(), fopen()
#include <stdlib.h> // for exit(), free()
#include <string.h> // for strsep()
#include <unistd.h> // for fork(), execv(), getcwd(), chdir()
#include <sys/wait.h> // for wait(), waitpid()

#define MAX_LINE 255
#define MAX_ARG 16
#define SHELL_PROMPT "ccsh> "

char path[] = "/bin";
char *directory;
char* read_lineIM();
char** parse_line(char *);
void run_line(char **);
char error_message[30] = "An error has occurred\n"; 

// read line in interactive mode 
char* read_lineIM() {
    char *line = NULL;
    size_t len = 0; 
    getline(&line, &len, stdin);
    return line;
}

// read line in batch mode 
/* char *read_lineBM(char* filename){
    FILE *stream;
    char *line = NULL;
    size_t len = 0; 
    ssize_t nread;
    int i;
    stream = fopen(filename, "r");
    if (stream == NULL) {
        perror(argv[1]);
        exit(1));
    }
    return line;
} */


// parse line in interactive mode 
char** parse_line(char *line){
    int numTok = 0;
    char *p = line;
    char **tokens = malloc(MAX_ARG * sizeof(char*));
    char *token;
    
    while ((token = strsep(&p, " \t\v\f\r\n")) != NULL) {
        if (*token == ' '){
			numTok ++;
			continue;
		}
		tokens[numTok] = token;
        numTok++;
    }
    free(token);
    tokens[numTok-1] = '\0'; // strip newline character
    tokens[numTok] = NULL;
    return tokens;
}

void run_line(char **args) {
    	pid_t pid;


	if(!strcmp(args[0],"exit")){   //built-in exit
		exit(0);
	}

	if (!strcmp(args[0], "cd")){		
		directory = args[1];			
		if(chdir(directory) == -1 || strlen(directory) == 0){			
			write(STDERR_FILENO, error_message, strlen(error_message));			
			}
        } 

	if (!strcmp(args[0],"path")){
		path[0] = *args[1];
	}
	
	strcat(path,args[0]);
	
    	if ((pid = fork()) == 0) {
        	execvp(args[0], args);
    	} 
    	else if (pid > 0) {
        	waitpid(pid, NULL, 0);
    	} 
    	else{
        	write(STDERR_FILENO, error_message, strlen(error_message));
    	}
    
}


int main(int argc, char* argv[]) {
    char *line;
    char **args;

    if (argc <= 1){ // interactive mode
        while(1) {
            printf(SHELL_PROMPT); 
            fflush(stdout);  
            line = read_lineIM();   // read input 
	    	  args = parse_line(line);    // parse input
	    	  if(!strcmp(args[0],"")) continue;            
            run_line(args);
            free(line);
            free(args);
        } 
    } else { // batch mode
        printf("\n");
/*         fflush(stdout);  
        line = read_lineBM();   // read input 
        args = parse_line(line);    // parse input 
        run_line(args);
        free(line);
        free(args); */
    }
    return 0;
   
}
