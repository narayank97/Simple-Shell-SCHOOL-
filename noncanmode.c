#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <ctype.h>
#include <sys/types.h>
#include <dirent.h>
#include <libgen.h>


void ResetCanonicalMode(int fd, struct termios *savedattributes){
    tcsetattr(fd, TCSANOW, savedattributes);
}

void SetNonCanonicalMode(int fd, struct termios *savedattributes){
    struct termios TermAttributes;
    char *name;

    // Make sure stdin is a terminal.
    if(!isatty(fd)){
        fprintf (stderr, "Not a terminal.\n");
        exit(0);
    }

    // Save the terminal attributes so we can restore them later.
    tcgetattr(fd, savedattributes);

    // Set the funny terminal modes.
    tcgetattr (fd, &TermAttributes);
    TermAttributes.c_lflag &= ~(ICANON | ECHO); // Clear ICANON and ECHO.
    TermAttributes.c_cc[VMIN] = 1;
    TermAttributes.c_cc[VTIME] = 0;
    tcsetattr(fd, TCSAFLUSH, &TermAttributes);
}



int main(int argc, char *argv[]){
    struct termios SavedTermAttributes;
    char RXChar;
    char input[1000];
    int pos = 0;

    SetNonCanonicalMode(STDIN_FILENO, &SavedTermAttributes);


    while(input[0]!='e' && input[1]!='x' && input[2]!='i' && input[3]!='t')
    {
    while(1){
        read(STDIN_FILENO, &RXChar, 1);
        if(0x04 == RXChar){ // C-d
            break;
        }
        if(0x0A == RXChar){ // hit enter stops reading input
            break;
        }

        else{

            if(isprint(RXChar)){
                printf("RX: '%c' 0x%02X\n",RXChar, RXChar);
                input[pos] = RXChar;// adds the character in that position
                pos++;
            }

            else{

                printf("RX: ' ' 0x%02X\n",RXChar);
            }

        }
        /*if(0x0A == RXChar){ // hit enter stops reading input
            break;
        }*/

    }


    //printf("%s\n", input);

    if(input[0] == 'c' && input[1] == 'd' && input[2] == ' ') // checks cd
    {
        int j = 0;
        char cd_arg[1000];
        for(int i = 3; input[i] != '\0'; i++)
        {
            cd_arg[j] = input[i];
            j++;
        }
        cd_arg[j+1] = '\0';
        DIR *dir = opendir(cd_arg);
        /*if(dir==NULL)
        {
            perror("cant open");
            exit (1);
        }*/



    }
    if(input[0] == 'l' && input[1] == 's' && input[2] == ' ')// checks ls
    {
        int j = 0;
        char ls_arg[1000];
        for(int i = 3; input[i] != '\0'; i++)
        {
            ls_arg[j] = input[i];
            j++;
        }

    }
    if(input[0] == 'p' && input[1] == 'w' && input[2] == 'd')// checks pwd
    {
        int PATH_MAX = 16;

        int j = 0;
        char pwd_arg[1000];
        for(int i = 4; input[i] != '\0'; i++)
        {
            pwd_arg[j] = input[i];
            j++;
        }
        char *buf;
        char *ptr;
        long size = pathconf(".", _PC_PATH_MAX);
        buf = (char *)malloc((size_t)size);
        if( buf != NULL)
        {
            ptr = getcwd(buf,(size_t)size);
        }

        printf("This is the the cwd: %s\n", buf);



    }
}





    ResetCanonicalMode(STDIN_FILENO, &SavedTermAttributes);
    return 0;
}
