#include <stdio.h>
#include <stdlib.h>
#include "varTypes.h"
#include "bigCharacters.c"





void PrintLoop( const char *const sStringToPrint, const uint2 iCount )
{
    for ( uint2 i = 0; i < iCount; i++ )
        printf( "%s", sStringToPrint );
}

void PrintLoopWith( const char *const sStringBefore, const char *const sStringToPrint, const char *const sStringAfter, const uint2 iCount )
{
    printf( "%s", sStringBefore );
    
    for ( uint2 i = 0; i < iCount; i++ )
        printf( "%s", sStringToPrint );
    
    printf( "%s", sStringAfter );
}





void PrintTitle( const char *const title, uint1 iSize )
{
    uint1 spaceCount;
    system( "cls" );
    
    PrintLoopWith( "\033[0;47m", " ", "\n", 120 );
    
    for (uint1 terminalRow = 0 ; terminalRow < 2 ; terminalRow++)
        PrintLoopWith( " \033[0m", " ", "\033[0;47m \n", 118 );
    
    for (uint1 characterRow = 0 ; characterRow < 6 ; characterRow++)
    {
        printf( " \033[0m" );
        spaceCount = (118 - 9*iSize)/2;
        for (uint1 space = 0 ; space < 118 - 9*iSize - spaceCount ; space++)
            printf( " " );
        
        PrintBigText( title, characterRow );
        
        for (uint1 space = 0 ; space < spaceCount ; space++)
            printf( " " );
        printf( "\033[0;47m \n" );
    }
    
    for (uint1 terminalRow = 8 ; terminalRow < 10 ; terminalRow++)
        PrintLoopWith( " \033[0m", " ", "\033[0;47m \n", 118 );
    
    PrintLoopWith( "", " ", "\n", 120 );
}


int main()
{
    PrintTitle( "Minesweeper", 12 );
    while( true );
    return 0;
}