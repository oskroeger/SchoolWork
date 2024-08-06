// C Program to design a shell in Linux
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<stdbool.h>
#include<sys/types.h>
#include<ctype.h>
#include<sys/wait.h>


#define MAXCOM 1000 // max number of letters to be supported
#define BUFFER 128

// Clearing the shell using escape sequences
#define clear() printf("\033[H\033[J")

void introduction();
void getCommand(char *str);
char **getFile(char *filename);
void exec_commands(char** parsed);
char *trimwhitespace(char *str);

// function to print welcome message
void introduction(){

    puts("\n-----Welcome to Command Line Interpreter-----\n"
        "\nSupports Basic User Commands:\n"
        "\nAlso Supports Batch Mode:\n"
        "\n(Enter 'quit' to Exit)\n"
        "\n---------------------------------------------\n");
    return;
}

// function to get a command from the user
void getCommand(char *str) {

    char *buf = NULL;
    int buffer = BUFFER;
    char **tokens = malloc(buffer * sizeof(char*));

    char **commands = malloc(buffer * sizeof(char*));

    char path[MAXCOM];
    getcwd(path, MAXCOM);

    // word is treated as a pointer
    // to the first element of array
    size_t size = 0;
    int readsize = 0;

    printf("\nuser~%s>>> ", path);
    readsize = getline(&buf, &size, stdin);

    char *token = NULL;

    char *command = strtok(buf, ";");

    int v = 0;

    while(command != NULL){

        commands[v] = command;
        command = strtok(NULL, ";");
        v++;
    }

    v = 0;

    int tok = 0;

    while(commands[v] != NULL) {

        token = strtok(commands[v], " ");

        int pos = 0;

        while (token != NULL) {

            tokens[pos] = trimwhitespace(token);
            pos++;
            token = strtok(NULL, " ");
        }

        exec_commands(tokens);
        v++;

        tok = 0;

        while(tokens[tok] != NULL){

            tokens[tok] = NULL;
            tok++;
        }

        printf("\n");
    }

    free(commands);
    free(tokens);
    if(buf)
        free(buf);

}

// trims whitespace from a buffer of characters
char *trimwhitespace(char *str) {
    char *end;

    // Trim leading space
    while(isspace((unsigned char)*str)) str++;

    if(*str == 0)  // All spaces?
        return str;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace((unsigned char)*end)) end--;

    // Write new null terminator character
    end[1] = '\0';

    return str;
}

// reads in a text file and runs each line as a command
char **getFile(char *filename) {

    int buffer = BUFFER;

    FILE *f;
    char *mode = "r";

    char *line = NULL;
    char *buf;
    size_t len = 0;
    ssize_t read;

    char **tokens = malloc(buffer * sizeof(char*));

    printf("\n-----BATCH MODE-----\n\n");

    // open the file
    f = fopen(filename, mode);

    // if the file is not found
    if(f == NULL){
        printf("Error: File not found.");
        exit(1);

    }

    // while the file has another line, read a line in
    while ((read = getline(&line, &len, f)) != -1) {

        printf("\nuser_>>> %s", line);

        // set pointer to line
        tokens[0] = trimwhitespace(line);

        // pass pointer into execute commands function
        exec_commands(tokens);

        printf("\n");

    }

    // close the file
    fclose(f);

    if (line)
        free(line);

    free(tokens);
    exit(EXIT_SUCCESS);
}

// display help menu
void openHelp() {

    puts("\n-----HELP-----\n"
        "\nCommands Supported:\n"
        "\n- ls (-l, -R, -la, -laF, -lh)"
        "\n- cd (.., [directory name], [directory path])"
        "\n- pwd (-L, -P)"
        "\n- mkdir [directory_name]"
        "\n- ps"
        "\n- date"
        "\n- whoami"
        "\n- help"
        "\n- top"
        "\n- grep (-r) (keyword)"
        "\n- sudo apt-get (update, upgrade, check, install)"
        "\n- df (-m, -k, -T)"
        "\n- head (filename)"
        "\n- tail (filename)"
        "\n- chmod (-c, -f, -v)"
        "\n- ping (hostname or ip address)"
        "\n- uname (-a, -s, -n)"
        "\n- quit");

    return;
}

// custom functions
int built_in(char** parsed) {


    char *token = trimwhitespace(parsed[0]);

    int num_commands = 4;
    int arg = 0;

    char* command_list[num_commands];

    command_list[0] = "quit";
    command_list[1] = "help";
    command_list[2] = "cd";

    // passed in string matches a command
    for(int i = 0; i < num_commands; i++){

        if (strcmp(token, command_list[i]) == 0){
            arg = i + 1;
            break;
        }
    }


    // switch to run correct command
    switch(arg) {
        case 1:
            exit(0);
        case 2:
            openHelp();
            return 1;
        case 3:
            chdir(parsed[1]);
            return 1;
        default:
            break;
    }

    return 0;
}


// Function where the system command is executed
void exec_commands(char** parsed) {

    // if command is built in, run and exit
    if (built_in(parsed)) {
        return;
    }

    // otherwise run as bash command
    else {

        // Forking a child
        pid_t pid = fork();

        // fork failed
        if (pid == -1) {
            printf("\nFailed forking child..");
            return;
        } else if (pid == 0) {
            // if fork is successful, execute command as bash command
            if (execvp(parsed[0], parsed) < 0) {
                printf("\nCould not execute command..%s\n", parsed[0]);
            }
            exit(0);
        } else {
            // waiting for child to terminate
            wait(NULL);
            return;
        }
    }
}

int main(int argc, char **argv)
{

    introduction();

    char inputString[MAXCOM];
    int execFlag = 1;

    char* filename;

    // if when called, there is a filename
    // run the file
    if(argc > 1){

        // expecting arguments to be filename
        filename = argv[1];
        getFile(filename);
    }

    else{

        while (1) {

            getCommand(inputString);
        }
    }

    return 0;
}
