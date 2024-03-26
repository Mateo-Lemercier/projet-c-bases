#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include "genericFunctions.c"

#define KEY_UP_ARROW 72
#define KEY_LEFT_ARROW 75
#define KEY_DOWN_ARROW 80
#define KEY_RIGHT_ARROW 77
#define KEY_SPACE 32
#define KEY_ENTER 10
#define KEY_ZERO 48

typedef unsigned short uint2;
typedef char bool;
#define true 1
#define false 0

// Bold \033[1;??m

// Black Foreground - \033[0;30m
// Red Foreground - \033[0;31m
// Green Foreground - \033[0;32m
// Yellow Foreground - \033[0;33m
// Blue Foreground - \033[0;34m
// Magenta Foreground - \033[0;35m
// Cyan Foreground - \033[0;36m
// White Foreground - \033[0;37m

// Black Background - \033[40m
// Red Background - \033[41m
// Green Background - \033[42m
// Yellow Background - \033[43m
// Blue Background - \033[44m
// Magenta Background - \033[45m
// Cyan Background - \033[46m
// White Background - \033[47m


typedef struct Cell
{
    uint2 Value;
    uint2 isBombed;
    uint2 isBroken;
    uint2 isFlagged;
} Cell;

typedef struct Row
{
    Cell* column;
} Row;

typedef struct GameBoard
{
    Row* row;
    short selectedCell[2]; //*V1 (var type could be unsigned short in V2)
    uint2 flaggedCells;
    uint2 brokenCells;
} GameBoard;



typedef struct Borders
{
    short rowStart;
    uint2 rowEnd;
    short colStart;
    uint2 colEnd;
} Borders;

typedef struct Settings
{
    uint2 rowCount;
    uint2 colCount;
    uint2 bombsCount;
} Settings;





/*void SetBoundaries( Borders* const borders, const Settings* const settings, const uint2 row, const uint2 col )
{
    borders->rowStart = row - 1;
    borders->rowEnd = row + 1;
    borders->colStart = col - 1;
    borders->colEnd = col + 1;

    if ( borders->rowStart < 0 )
        borders->rowStart += 1;
    
    if ( borders->rowEnd > settings->rowCount - 1 )
        borders->rowEnd -= 1;
    
    if ( borders->colStart < 0 )
        borders->colStart += 1;
    
    if ( borders->colEnd > settings->colCount - 1 )
        borders->colEnd -= 1;
}*/





/*void PlaceBombs( GameBoard* const board, const Settings* const settings )
{
    Borders borders; uint2 row; uint2 col;
    for ( uint2 si = 0 ; si < settings->bombsCount; si++ )
    {
        while ( 1 )
        {
            row = rand() % settings->rowCount;
            col = rand() % settings->colCount;
            
            if ( board->row[ row ].column[ col ].isBombed == 0 )
            {
                board->row[ row ].column[ col ].isBombed = 1;
                SetBoundaries( &borders, settings, row, col );

                for ( short j = borders.rowStart ; j < borders.rowEnd+1 ; j++ )
                    for ( short k = borders.colStart ; k < borders.colEnd+1 ; k++ )
                        board->row[ j ].column[ k ].Value++;
                
                break;
            }
        }
    }
}*/





unsigned short BreakCell( GameBoard* const board, const Settings* const settings, const uint2 row, const uint2 col )
{
    if ( ( board->row[ row ].column[ col ].isBroken == 1 ) || ( board->row[ row ].column[ col ].isFlagged == 1 ) )
        return 0;
    
    board->row[ row ].column[ col ].isBroken = 1;
    board->brokenCells++;

    if ( board->row[ row ].column[ col ].isBombed == 1 )
        return 1;

    if ( board->row[ row ].column[ col ].Value != 0 )
        return 0;

    Borders borders;
    SetBoundaries( &borders, settings, row, col );
    
    for ( short i = borders.rowStart ; i < borders.rowEnd+1 ; i++ )
        for ( short j = borders.colStart ; j < borders.colEnd+1 ; j++ )
            BreakCell( board, settings, i, j );
    
    return 0;
}





void SelectCell( GameBoard* const board, const Settings* const settings )
{
    uint2 iUserRow; uint2 iUserCol;

    while ( 1 )
    {
        printf( "\nSelect the row you want to play in : " );
        iUserRow = AskIntBetween( 1, settings->rowCount, 2 ) - 1;
        printf( "Select the column you want to play in : " );
        iUserCol = AskIntBetween( 1, settings->colCount, 2 ) - 1;
        
        if ( board->row[ iUserRow ].column[ iUserCol ].isBroken == 0 )
        {
            board->selectedCell[ 0 ] = iUserCol;
            board->selectedCell[ 1 ] = iUserRow;
            break;
        }

        printf( "Please, select a cell that is not already broken\n" );
    }
}





int AskKeyFromList( const short* const iList, short iSize )
{
    char cUser; int cUserIndex;
    char sList[ iSize ]; // ! Comment c'est possible ? (iSize)
    for ( int i = 0 ; i < iSize ; i++ )
        sList[ i ] = iList[ i ];

    while ( 1 )
    {
        if ( kbhit() )
        {
            cUser = getch();
            cUserIndex = CharInString( cUser, sList );
            if ( cUserIndex != -1 )
                return iList[ cUserIndex ];
        }
    }
}





/*void PrintGameBoard( const GameBoard* const board, const Settings* const settings )
{
    system( "cls" );

    for ( uint2 i = 0; i < settings->rowCount; i++ )
    {
        for ( uint2 j = 0; j < settings->colCount; j++ )
        {
            if ( board->row[ i ].column[ j ].isBroken == 1 )
            {   
                if ( board->row[ i ].column[ j ].isBombed == 1 )
                    printf( "\033[4;41;30m  " );
                else if ( board->row[ i ].column[ j ].Value == 0 )
                    printf( "- " );
                else
                    printf( "%d ", board->row[ i ].column[ j ].Value );
            }
            else if ( i == board->selectedCell[ 1 ] && j == board->selectedCell[ 0 ] )
                printf( "\033[4;46;30m  " );
            else if ( board->row[ i ].column[ j ].isFlagged == 1 )
                printf( "\033[4;42;30m  " );
            else
                printf( "\033[4;47;30m  " );
            printf( "\033[0m " );
        }
        printf( "\n" );
    }

    printf( "\n   %d \033[0;42m  \033[0m left\n", (settings->bombsCount - board->flaggedCells) );
}*/





void ChangeGameSettings( Settings* const settings )
{
    printf( "Enter how many rows the board will have : " );
    settings->rowCount = AskIntBetween( 1, 99, 2 );

    printf( "Enter how many columns the board will have : " );
    settings->colCount = AskIntBetween( 1, 99, 2 );

    printf( "Enter how many bombs are going to be placed on the board : " );
    settings->bombsCount = AskIntBetween( 0, settings->rowCount * settings->colCount, 2 );
}





void StartGame( uint2 rowCount, uint2 colCount, uint2 bombsCount )
{
    if ( !( ( 1 < rowCount && rowCount < 99) || ( 1 < colCount && colCount < 99) || (0 < bombsCount && bombsCount < rowCount * colCount) ) )
        return;
    
    Settings settings;
    settings.rowCount = rowCount;
    settings.colCount = colCount;
    settings.bombsCount = bombsCount;
    
    char placeOrBreak; uint2 endgame; uint2 isFlagged;
    const char* statement = "";

    GameBoard board;
    board.row = ( Row* ) malloc( settings.rowCount * sizeof( Row ) );
    for( uint2 i = 0; i < settings.rowCount; i++ )
        board.row[ i ].column = ( Cell* ) malloc( settings.colCount * sizeof( Cell ) );

    while ( 1 )
    {
        board.selectedCell[ 0 ] = -1;
        board.selectedCell[ 1 ] = -1;
        board.flaggedCells = 0;
        board.brokenCells = 0;
        endgame = 0;

        printf( "Right now, there are %d rows, %d columns and %d bombs to find\n", settings.rowCount, settings.colCount, settings.bombsCount );
        if ( AskYesOrNo( "Do you want to change the game settings ? (y/n)\n" ) )
        {
            ChangeGameSettings( &settings );

            for ( uint2 i = 0; i < settings.rowCount; i++ )
                free( board.row[ i ].column );
            free( board.row );
            
            board.row = ( Row* ) malloc( settings.rowCount * sizeof( Row ) );
            for( uint2 i = 0; i < settings.rowCount; i++ )
                board.row[ i ].column = ( Cell* ) malloc( settings.colCount * sizeof( Cell ) );
        }
        
        for( uint2 i = 0; i < settings.rowCount; i++ )
            for ( uint2 j = 0; j < settings.colCount; j++ )
            {
                board.row[ i ].column[ j ].Value = 0;
                board.row[ i ].column[ j ].isBombed = 0;
                board.row[ i ].column[ j ].isBroken = 0;
                board.row[ i ].column[ j ].isFlagged = 0;
            }

        PlaceBombs( &board, &settings );

        while ( 1 )
        {
            PrintGameBoard( &board, &settings );

            if ( endgame == 1 )
            {
                printf( "\nYou've lost !\n" );
                break;
            }
            
            if ( board.brokenCells == (settings.rowCount * settings.colCount) - settings.bombsCount )
            {
                printf( "\nYou won !\n" );
                break;
            }
            
            if (board.selectedCell[ 0 ] == -1 && -1 == board.selectedCell[ 1 ])
            {
                printf( statement );
                statement = "";
                SelectCell( &board, &settings );
                continue;
            }

            printf( "\nYou've selected this cell, do you want to place a flag on the cell, break it or cancel the selection ? (f/b/c)\n" );
            placeOrBreak = AskCharFromList( "fbc" );
            
            if ( placeOrBreak == 'f' )
            {
                isFlagged = board.row[ board.selectedCell[ 1 ] ].column[ board.selectedCell[ 0 ] ].isFlagged;
                
                if ( board.flaggedCells < settings.bombsCount || isFlagged == 1 )
                {
                    board.row[ board.selectedCell[ 1 ] ].column[ board.selectedCell[ 0 ] ].isFlagged = !isFlagged;
                    board.flaggedCells += 1-2*isFlagged;
                }

                else
                    statement = "\nYou don't have any flag left, remove a flag if you want to place one here";
            }
            else if ( placeOrBreak == 'b' )
                endgame = BreakCell( &board, &settings, board.selectedCell[ 1 ], board.selectedCell[ 0 ] );
            
            board.selectedCell[ 0 ] = -1;
            board.selectedCell[ 1 ] = -1;
        }

        if ( !AskYesOrNo( "Do you want to play again ? (y/n)\n" ) )
            break;
    }

    for ( uint2 i = 0; i < settings.rowCount; i++ )
        free( board.row[ i ].column );
    free( board.row );
}





void StartGameV2( uint2 rowCount, uint2 colCount, uint2 bombsCount )
{
    if ( !( ( 1 < rowCount && rowCount < 99) || ( 1 < colCount && colCount < 99) || (0 < bombsCount && bombsCount < rowCount * colCount) ) )
        return;
    
    Settings settings;
    settings.rowCount = rowCount;
    settings.colCount = colCount;
    settings.bombsCount = bombsCount;

    uint2 endgame; uint2 isFlagged; short keyPressed;
    const short keysList[7] = { KEY_UP_ARROW, KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_RIGHT_ARROW, KEY_SPACE, KEY_ENTER, KEY_ZERO };

    GameBoard board;
    board.row = ( Row* ) malloc( settings.rowCount * sizeof( Row ) );
    for( uint2 i = 0; i < settings.rowCount; i++ )
        board.row[ i ].column = ( Cell* ) malloc( settings.colCount * sizeof( Cell ) );

    while ( 1 )
    {
        board.selectedCell[ 0 ] = 0;
        board.selectedCell[ 1 ] = 0;
        board.flaggedCells = 0;
        board.brokenCells = 0;
        endgame = 0;
        
        for( uint2 i = 0; i < settings.rowCount; i++ )
            for ( uint2 j = 0; j < settings.colCount; j++ )
            {
                board.row[ i ].column[ j ].Value = 0;
                board.row[ i ].column[ j ].isBombed = 0;
                board.row[ i ].column[ j ].isBroken = 0;
                board.row[ i ].column[ j ].isFlagged = 0;
            }

        PlaceBombs( &board, &settings );

        while ( 1 )
        {
            PrintGameBoard( &board, &settings );
            printf( "\n" );

            if ( endgame == 1 )
            {
                printf( "\nYou've lost !\n" );
                break;
            }
            
            if ( board.brokenCells == (settings.rowCount * settings.colCount) - settings.bombsCount )
            {
                printf( "\nYou won !\n" );
                break;
            }
            
            keyPressed = AskKeyFromList( keysList, 7 );
            
            if ( keyPressed == KEY_UP_ARROW )
            {
                if ( board.selectedCell[ 1 ] > 0 )
                    board.selectedCell[ 1 ] -= 1;
            }
            
            else if ( keyPressed == KEY_LEFT_ARROW )
            {
                if ( board.selectedCell[ 0 ] > 0 )
                    board.selectedCell[ 0 ] -= 1;
            }
            
            else if ( keyPressed == KEY_DOWN_ARROW )
            {
                if ( board.selectedCell[ 1 ] < settings.rowCount - 1 )
                    board.selectedCell[ 1 ] += 1;
            }
            
            else if ( keyPressed == KEY_RIGHT_ARROW )
            {
                if ( board.selectedCell[ 0 ] < settings.colCount - 1 )
                    board.selectedCell[ 0 ] += 1;
            }

            else if ( keyPressed == KEY_SPACE )
            {
                endgame = BreakCell( &board, &settings, board.selectedCell[ 1 ], board.selectedCell[ 0 ] );
            }

            else if ( keyPressed == KEY_ZERO )
            {
                if ( board.row[ board.selectedCell[ 1 ] ].column[ board.selectedCell[ 0 ] ].isBroken == 0)
                {
                    isFlagged = board.row[ board.selectedCell[ 1 ] ].column[ board.selectedCell[ 0 ] ].isFlagged;
                    
                    if ( board.flaggedCells < settings.bombsCount || isFlagged == 1 )
                    {
                        board.row[ board.selectedCell[ 1 ] ].column[ board.selectedCell[ 0 ] ].isFlagged = !isFlagged;
                        board.flaggedCells += 1-2*isFlagged;
                    }
                }
            }

            else if ( keyPressed == KEY_ENTER )
            {
                break;
            }
        }

        if ( !AskYesOrNo( "Do you want to play again ? (y/n)\n" ) )
            break;
    }

    for ( uint2 i = 0; i < settings.rowCount; i++ )
        free( board.row[ i ].column );
    free( board.row );
}





int main()
{
    srand( time( NULL ) );
    //StartGame( 14, 18, 40 );
    StartGameV2( 14, 18, 40 );
    return 0;
}