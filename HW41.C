#include <unistd.h>
#include <stdio.h>
//read in characters and pass it down
int main(int argc, char const *argv[]) {

    char c;
    c = getchar();
    //if he character is not EOF, pass it down
    while(c != EOF) {
        write(STDOUT_FILENO, &c, sizeof(char));
        c = getchar();
    }

    return 0;
}
