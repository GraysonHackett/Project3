#include <stdio.h>
#include <stdlib.h>
//#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

void read_command(char cmd[], char *par[]){

    char line[1024];
    int count = 0, i = 0, j = 0;
    char *array[100], *pch;

    //reads just one line 
    for ( ;; ){
        int c = fgetc(stdin);
        line[count++] = (char)c;
        if (c == '\n') break;
    }
    if (count == 1) return;
    pch = strtok (line," \n");

    // parses the line into words
    while (pch != NULL){
        array[i++] = strdup(pch);
        pch = strtok(NULL, " \n");
    }
    // gets the command
    strcpy (cmd,array[0]);

    // gets the other parameters (words)
    for (int j = 0; j < i; j++){
        par[j] = array[j];
    par[i] = NULL; // it will terminate the parameter list 
    }
}

int main(){

    char cmd[100];
    char command[100];
    char *parameters[20];

    char *envp[] = {(char*) "PATH=/bin",0};

    // init loop to run the shell forever
    while(1) { 

        printf("ccsh> ");
        read_command(command,parameters); // reads the user input from terminal 

        if(fork() != 0){ // init parent
            wait (NULL); // waits for the child
        }
        else{
            strcpy(cmd, "/bin");
            strcat(cmd, command);
            execve(cmd,parameters,envp); //execute the command
        }
        if (strcmp (command,"exit") == 0){
            break;
        }
    }
    return 0; 

}
