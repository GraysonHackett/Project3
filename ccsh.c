#include <stdio.h> // for getline(), printf(), fopen()
#include <stdlib.h> // for exit(), free()
#include <string.h> // for strsep(), strcmp()
#include <unistd.h> // for fork(), execvp(), getcwd(), chdir()
#include <sys/wait.h> // for wait(), waitpid()

#define MAX_LINE 255
#define MAX_ARG 16

char* read_lineIM();
char** parse_line(char *);
void run_line(char **);

char error_message[30] = "An error has occurred\n"; 
char cwd[256];
char *set_path = "/bin/";
char *directory;

// read line in interactive mode 
char* read_lineIM() {
    char *line = NULL;
    size_t len = 0; 
    ssize_t nread;
    if ((nread = getline(&line, &len, stdin)) == -1) {
        exit(0); // EOF
    }
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

    while(isspace(*p)) p++; // trim leading whitepace

    while ((token = strsep(&p, " t\v\f\r\n")) != NULL) { 
        tokens[numTok] = token;
        numTok++;
    }

    free(p);
    free(token);
    tokens[numTok-1] = '\0'; // strip newline character
    tokens[numTok] = NULL;

   for (int i = 0; i < numTok; i++) {
        printf("Token[%d]: %s\n", i, tokens[i]);
    }

    return tokens;
}
  
void run_line(char **args) {
    pid_t pid;

	if(!strcmp(args[0],"exit")){   //built-in exit
		exit(0);
	}

	if (!strcmp(args[0], "cd")){	
        strcpy(set_path, cwd);
        strcat(set_path, args[0]);
        cd_cmd(args);
    }	


    if ((pid = fork()) == 0) {
        execvp(args[0], args);
        _exit(1);//temporary
    } else if (pid > 0) {
        waitpid(pid, NULL, 0);
    } else{
        write(STDERR_FILENO, error_message, strlen(error_message));
    }
    
}


int cd_cmd(char **args){
    int cd = 0;
    if (args[1]==NULL){
        write(STDERR_FILENO, error_message, strlen(error_message));
    } else {
        cd = chdir(args[1]);
        getcwd(cwd, sizeof(cwd));
    }
    return cd;
}

// int path_cmd(char **args) {

// }

int main(int argc, char* argv[]) {
    char *line;
    char **args;
    if (argc <= 1){ // interactive mode 
        while(1) {
            printf("ccsh> "); 
            fflush(stdout);  
            line = read_lineIM();   // read input 
            args = parse_line(line);    // parse input 
            if(!strcmp(args,"")) continue;	
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
