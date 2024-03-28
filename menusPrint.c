#include <stdio.h>
#include <stdlib.h>
#include "cvarTypes.h"
#include "bigCharacters.c"

typedef struct Vector2
{
    uint1 row;
    uint1 col;
} Vector2;

typedef struct Menu
{
    char table[ 15 ][ 119 ];
    uint1 iSelected;
    Vector2 *canBeSelected;
    uint1 canBeSelectedCount;
} Menu;





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





void PrintTitle( const char *const title )
{
    uint1 iSize = 0;
    for ( uint1 i = 0 ; title[ i ] != '\0' ; i++ )
    {
        iSize++;
        if ( title[ i ] == 'm' || title[ i ] == 'w' )
            iSize++;
    };
    
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

void PrintMenu( Menu *pMenu )
{
    for ( uint1 i = 0 ; i < 15 ; i++)
    {
        printf( " \033[0m" );
        for ( uint1 j = 0 ; j < 119 ; j++ )
        {
            if ( pMenu->canBeSelected[ pMenu->iSelected ].row == i && pMenu->canBeSelected[ pMenu->iSelected ].col == j )
                printf( "\033[4;36m" );
            
            printf( "%c", pMenu->table[ i ][ j ] );
        }
        printf( "\033[0;47m \n" );
    }
    
    PrintLoopWith( "\033[0;47m", " ", "\n", 120 );
}





void PlaceText( Menu *pMenu, char *sText, uint1 iRow, uint1 iColumn, bool bCanBeSelected )
{
    for ( uint1 i = 0 ; sText[ i ] != '\0' ; i++ )
        pMenu->table[ iRow ][ iColumn + i ] = sText[ i ];
    
    if ( bCanBeSelected )
    {
        pMenu->canBeSelectedCount++;
        pMenu->canBeSelected = ( Vector2* ) realloc( pMenu->canBeSelected, sizeof( Vector2 ) * pMenu->canBeSelectedCount );
        pMenu->canBeSelected[ pMenu->canBeSelectedCount - 1 ].row = iRow;
        pMenu->canBeSelected[ pMenu->canBeSelectedCount - 1 ].col = iColumn;
    }
}





void OpenMenu( const char *const sTitle )
{
    Menu menu;
    menu.iSelected = 0;
    menu.canBeSelected = ( Vector2* ) malloc( sizeof( Vector2 ) );
    menu.canBeSelectedCount = 0;
    for ( uint1 i = 0 ; i < 15 ; i++ )
    {
        for ( uint1 j = 0 ; j < 118 ; j++ )
            menu.table[ i ][ j ] = ' ';
        menu.table[ i ][ 118 ] = '\0';
    }
    
    PlaceText( &menu, "Start", 8, 12, true );
    PlaceText( &menu, "Noaleplusbo", 2, 74, true );
    
    while ( true )
    {
        PrintTitle( sTitle );
        PrintMenu( &menu );
        break;
    }
}





int main()
{
    // PrintTitle( "Minesweeper" );
    OpenMenu( "Minesweeper" );
    while( true );
    return 0;
}