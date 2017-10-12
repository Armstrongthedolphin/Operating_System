#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
//define line size
#define LINE_LEN 80
//construct a data type of three pipes
//used as parameters for functions
typedef struct {
    int *pipe_1;
    int *pipe_2;
    int *pipe_3;
} PipeSet;
//read in characters
void input_func(void* v_pipes) {

	PipeSet *pipes = (PipeSet*)v_pipes;

    char c;
    //define the file descriptors and closed the unused pipe
    int out_fd = pipes->pipe_1[1];
    close(pipes->pipe_1[0]);
    close(pipes->pipe_2[0]);
    close(pipes->pipe_2[1]);
    close(pipes->pipe_3[0]);
    close(pipes->pipe_3[1]);

    c = getchar();
    //looping until read in EOOF
    while(c != EOF) {
    	//write character into the pipe
    	write(out_fd,&c,sizeof(char));
        c = getchar();
    }
    //close pipe
    close(out_fd);
}
// change new line into blank
void r_b_func(void* v_pipes) {

	PipeSet *pipes = (PipeSet*)v_pipes;

    char c;
    //define the file descriptors and closed the unused pipe
    int in_fd = pipes->pipe_1[0];
    int out_fd = pipes->pipe_2[1];
    close(pipes->pipe_1[1]);
    close(pipes->pipe_2[0]);
    close(pipes->pipe_3[1]);
    close(pipes->pipe_3[0]);
    //If the return value of read is 0, pipe closes
    //read while there is character available
    while(read(in_fd, &c, sizeof(char)) > 0) {
    	//change new line into blank
        if(c == '\n' ) {
            c = ' ';
        }
        write(out_fd, &c, sizeof(char));

    }
    //close pipe
    close(in_fd);
    close(out_fd);
}
//change '**' into '^'
void squash_func(void* v_pipes) {

	PipeSet *pipes = (PipeSet*)v_pipes;

    char c;

    int fst_star = 0;
    //we set the constants so that we could pass it to write()
    char up = '^', star = '*';
    //define the file descriptors and closed the unused pipe
    int in_fd = pipes->pipe_2[0];
    int out_fd = pipes->pipe_3[1];
    close(pipes->pipe_1[0]);
    close(pipes->pipe_1[1]);
    close(pipes->pipe_2[1]);
    close(pipes->pipe_3[0]);
    //If the return value of read is 0, pipe closes
    //read while there is character available
    while(read(in_fd, &c, sizeof(char)) > 0) {
        if(c == '*') {
        	//if the previous character is "*"
            if(fst_star == 1) {
            	//two "*" in a row, write "^"
            	write(out_fd, &up, sizeof(char));
                //set flag back into 0
                fst_star = 0;
            } else {
            	//first "*" read in, set flag to 1
                fst_star = 1;
            }
        } else {
        	//if the previous character is "*", set flag back to 0
            if(fst_star) {
                fst_star = 0;
                write(out_fd, &star, sizeof(char));
            }
            write(out_fd, &c, sizeof(char));
        }
    }
    //if last character read in is '*', pass it down
    if (fst_star){
    	write(out_fd, &star, sizeof(char));
    }
    //close pipe
    close(in_fd);
    close(out_fd);
}
//read in characters passed down by HW43
//print out as 80-character-long strings
void out_func(void* v_pipes) {
	PipeSet *pipes = (PipeSet*)v_pipes;

    char c;
    //define the file descriptors and closed the unused pipe
    int in_fd = pipes->pipe_3[0];
    close(pipes->pipe_1[0]);
    close(pipes->pipe_1[1]);
    close(pipes->pipe_2[0]);
    close(pipes->pipe_2[1]);
    close(pipes->pipe_3[1]);

    char str[LINE_LEN];
    int i = 0;
    //set array to 0
    memset(str,0,LINE_LEN);
    //If the return value of read is 0, pipe closes
    //read while there is character available
    while(read(in_fd, &c, sizeof(char)) > 0) {
        str[i++] = c;
        //print if the array is filled
        if(i == LINE_LEN-1) {
            printf("%s\n", str);
            //set array back to 0
            memset(str,0,LINE_LEN);
            i = 0;
        }
    }
    //close pipe
    close(in_fd);
}
//this function is used to fork a process and use it to call function in parameter
//return if it is the parent process
void fork_func(void (*fn)(void*), void *arg) {
    pid_t pid = fork();
    if(pid == -1){
        perror("failed to perform fork");
        exit(EXIT_FAILURE);
    } else if(pid == 0){
    	//call function
        fn(arg);
        exit(EXIT_SUCCESS);
    }
}

int main (int argc, char const *argv[]) {

    // Setup three pipes,
	//pipe_1 connect input function and return to blank function
	//pipe_2 connect return to blank function and squash function
	//pipe_1 connect squash function and output function
    int pipe_1[2];
    int pipe_2[2];
    int pipe_3[2];

    PipeSet pipes = {pipe_1, pipe_2, pipe_3};

    //create three pipes
    //exit if error occur
    if(pipe(pipe_1) == -1
       || pipe(pipe_2) == -1
       || pipe(pipe_3) == -1){
        perror("pipe creation failed");
        exit(EXIT_FAILURE);
    }

    //create child processes and call input, r_b and squash functions
    fork_func(input_func, (void*)(&pipes));
    fork_func(r_b_func, (void*)(&pipes));
    fork_func(squash_func, (void*)(&pipes));
    //perform output function
    out_func((void*)(&pipes));

    return 0;
}





