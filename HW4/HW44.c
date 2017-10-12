#include <unistd.h>
#include <stdio.h>
#include <string.h>
//define the line size
#define LINE_LEN 80
//read in characters passed down by HW43
//print out as 80-character-long strings
int main(int argc, char const *argv[]) {

    char c;
    char str[LINE_LEN];
    int i = 0;
    //set string to 0
    memset(str,0,LINE_LEN);
    //if return value of read is 0, the pipe is closed
    //read while there is available character
    while(read(STDIN_FILENO, &c, sizeof(char)) > 0) {
        str[i++] = c;
        //print if the array is filled
        if(i == LINE_LEN-1) {
            printf("%s\n", str);
            //set array back to 0
            memset(str,0,LINE_LEN);
            i = 0;
        }
    }
    return 0;
}
