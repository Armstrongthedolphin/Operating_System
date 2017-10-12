#include <unistd.h>
#include <stdio.h>
//read in characters passed down by HW42 and change '**' into '^'
int main(int argc, char const *argv[]) {

    char c;

    int fst_star = 0;
    //we set the constants so that we could pass it to write()
    char up = '^', star = '*';
    //when read() returns 0, the pipe is closed
    while(read(STDIN_FILENO, &c, sizeof(char)) > 0) {
        if(c == '*') {
        	//if the previous character is "*"
            if(fst_star == 1) {
            	//two "*" in a row, write "^"
            	write(STDOUT_FILENO, &up, sizeof(char));
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
                write(STDOUT_FILENO, &star, sizeof(char));
            }
            write(STDOUT_FILENO, &c, sizeof(char));
        }
    }
    //if last character read in is '*', pass it down
    if (fst_star){
    	write(STDOUT_FILENO, &star, sizeof(char));
    }
    return 0;
}
