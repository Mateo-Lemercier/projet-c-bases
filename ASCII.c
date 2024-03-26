#include <stdio.h>
#include <conio.h>

int main()
{
    char cUser;
    while ( 1 )
    {
        if ( kbhit() )
        {
            cUser = getch();
            printf( "%c : %d\n", cUser, cUser );
        }
    }
}