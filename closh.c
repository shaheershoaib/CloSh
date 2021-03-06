// closh.c - COSC 315, Winter 2020
// YOUR NAME HERE

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <math.h>
#include <sys/wait.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

// tokenize the command string into arguments - do not modify
void readCmdTokens(char* cmd, char** cmdTokens) {
    cmd[strlen(cmd) - 1] = '\0'; // drop trailing newline
    int i = 0;
    cmdTokens[i] = strtok(cmd, " "); // tokenize on spaces
    while (cmdTokens[i++] && i < sizeof(cmdTokens)) {
        cmdTokens[i] = strtok(NULL, " ");
    }
}

// read one character of input, then discard up to the newline - do not modify
char readChar() {
    char c = getchar();
    while (getchar() != '\n');
    return c;
}

// main method - program entry point
int main() {
    char cmd[81]; // array of chars (a string)
    char* cmdTokens[20]; // array of strings
    int count; // number of times to execute command
    int parallel; // whether to run in parallel or sequentially
    int timeout; // max seconds to run set of commands (parallel) or each command (sequentially)
    
    while (TRUE) { // main shell input loop
        
        // begin parsing code - do not modify
        printf("closh> ");
        fgets(cmd, sizeof(cmd), stdin);
        if (cmd[0] == '\n') continue;
        readCmdTokens(cmd, cmdTokens);
        do {
            printf("  count> ");
            count = readChar() - '0';
        } while (count <= 0 || count > 9);
        
        printf("  [p]arallel or [s]equential> ");
        parallel = (readChar() == 'p') ? TRUE : FALSE;
        do {
            printf("  timeout> ");
            timeout = readChar() - '0';
        } while (timeout < 0 || timeout > 9);
        // end parsing code
        
        


        ////////////////////////////////////////////////////////
        //                                                    //
        // TODO: use cmdTokens, count, parallel, and timeout  //
        // to implement the rest of closh                     //
        //                                                    //
        // /////////////////////////////////////////////////////
        /***** BEGIN TODO *****/
        if(parallel){  // Running the programs in parallel
            for(int i=0; i<count; i++)   { 
                int id = fork();
                time_t now = time(0);
                if(id == 0){   // call fork() and check if we are currently in the child process
                    printf("Child process ID - %d\n", getpid());
                    execvp(cmdTokens[0], cmdTokens); // Call execvp() to run the process in the child process
                    exit(0);  // Exit out of the child process
                }
                else if (id > 0) {
                    printf("Parent process ID - %d\n", getpid());
                    double elapsedTime = difftime(time(0), now); 
                    // calculate timeout and kill process if expired
                    if (timeout != 0 && elapsedTime > timeout) {
                            printf("Timeout exceeded - %ds", timeout);
                            kill(id, SIGKILL);

                            break;
                        }
                }
            } 
         
        }   

        else if(!parallel){ // Run sequentially 
            for(int i=0; i<count; i++)   {
                int id = fork();
                time_t now = time(0);
                if(id == 0){   // call fork() and check if we are currently in the child process
                    printf("Child process ID - %d\n", getpid());
                    execvp(cmdTokens[0], cmdTokens); // Call execvp() to run the process in the child process
                    exit(0);
                }
                else if (id > 0) {
                    int pid = getpid();
                    printf("Parent process ID - %d\n", pid);
                    // while process is still running we wait until it finishes
                    while (waitpid(id, NULL, WNOHANG) == 0) {
                        double elapsedTime = difftime(time(0), now); 
                        // calculate timeout and kill process if expired
                        if (timeout != 0 && elapsedTime > timeout) {
                            printf("Timeout exceeded - %ds", timeout);
                            kill(id, SIGKILL);

                            break;
                        }
                    }
                }
            } 
        }
        exit(0);    // Exiting out of the shell

       // execvp(cmdTokens[0], cmdTokens);
        // just executes the given command once - REPLACE THIS CODE WITH YOUR OWN
         // replaces the current process with the given program
        // doesn't return unless the calling failed
      //  printf("Can't execute %s\n", cmdTokens[0]); // only reached if running the program failed
            
    }
}
