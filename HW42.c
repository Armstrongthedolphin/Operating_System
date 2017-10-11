#include <unistd.h>
#include <stdio.h>
//read in characters passed down by HW41 and change new line into blank
int main(int argc, char const *argv[]) {
    char c;
    //If the return value of read is 0, pipe closes
    //read in while there is character available
    while(read(STDIN_FILENO, &c, sizeof(char)) > 0) {
    	//when there is a new line character, pass it down as blank
        if(c == '\n') {
            c = ' ';
        }
        write(STDOUT_FILENO, &c, sizeof(char));
    }

    return 0;
}
