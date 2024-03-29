#ifndef __genericFunctions_c__
#define __genericFunctions_c__
#include <stdio.h>
#include <stdlib.h>





void BufferClear()
{
    while (getchar() != '\n');
}





int AskInt()
{
    int iUser;
    
    while ( scanf_s( "%d", &iUser ) == 0 )
    {
        printf( "Please, enter a valid whole number\n" );
        BufferClear();
    }

    BufferClear();
    return iUser;
}





int AskIntBetween( const int iMin, const int iMax, const int iWhich )
{
    int iUser; int iCompare;
    const char* sWhich;
    if ( 2-iWhich )
    {
        sWhich = ( iWhich == 0 )? "above" : "under";
        iCompare = ( iWhich == 0 )? iMin : iMax;
    }

    while ( 1 )
    {
        iUser = AskInt();
        if ( ( iWhich%2 || iMin <= iUser ) && ( iUser <= iMax || !iWhich ) )
            return iUser;

        if ( 2-iWhich )
            printf( "Please, enter a number %s %d\n", sWhich, iCompare );
        else
            printf( "Please, enter a number between %d and %d\n", iMin, iMax );
    }
}





int AskIntStrictlyBetween( const int iMin, const int iMax, const int iWhich )
{
    int iUser; int iCompare;
    const char* sWhich;
    if ( 2-iWhich )
    {
        sWhich = ( iWhich == 0 )? "above" : "under";
        iCompare = ( iWhich == 0 )? iMin : iMax;
    }

    while ( 1 )
    {
        iUser = AskInt();
        if ( ( iWhich%2 || iMin < iUser ) && ( iUser < iMax || !iWhich ) )
        {
            return iUser;
        }

        if ( 2-iWhich )
            printf( "Please, enter a number %s %d\n", sWhich, iCompare );
        else
            printf( "Please, enter a number between %d and %d\n", iMin, iMax );
    }
}





int CharInString( const char cChar, const char* const sString )
{
    for (int i = 0; sString[ i ] != '\0'; i++)
        if ( sString[ i ] == cChar )
            return i;
    return -1;
}





char AskChar()
{
    char cUser;

    while ( scanf_s( "%c", &cUser, 1 ) == 0 )
    {
        printf( "Please, enter a single valid character\n" );
        BufferClear();
    }

    BufferClear();
    return cUser;
}





char AskCharFromList( const char* const sList )
{
    char cUser = AskChar();

    while ( CharInString( cUser, sList ) == -1 )
    {
        printf( "Please, enter one of these characters : %c", sList[ 0 ] );

        for ( int i = 1; sList[ i ] != '\0'; i++ )
            printf( ", %c", sList[ i ] );

        printf( "\n" );
        
        cUser = AskChar();
    }

    return cUser;
}





int AskYesOrNo( const char* const question )
{
    printf(question);
    char cUser = AskCharFromList("yYnN\0");

    if ( cUser == 'y' || cUser == 'Y' )
        return 1;

    return 0;
}





int GetLength( const char* sString )
{
    int iLength;
    for ( iLength = 0; sString[ iLength ] != '\0'; iLength++ );
    return iLength;
}

#endif