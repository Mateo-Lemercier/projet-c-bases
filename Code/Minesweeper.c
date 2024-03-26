#include <stdio.h>
#include <stdlib.h>
#include "genericFunctions.c"

// Black \033[0;30m
// Red \033[0;31m
// Green \033[0;32m
// Yellow \033[0;33m
// Blue \033[0;34m
// Magenta \033[0;35m
// Cyan \033[0;36m
// White \033[0;37m
// Bold \033[1;??m


typedef struct Cell
{
    unsigned short Value;
    unsigned short isBombed;
    unsigned short isBroken;
    unsigned short isFlagged;
} Cell;

typedef struct Row
{
    Cell* column;
} Row;

typedef struct GameBoard
{
    Row* row;
    short selectedCell[2];
    unsigned short flaggedCells;
    unsigned short brokenCells;
} GameBoard;



typedef struct Borders
{
    short rowStart;
    unsigned short rowEnd;
    short colStart;
    unsigned short colEnd;
} Borders;

typedef struct Settings
{
    unsigned short rowCount;
    unsigned short colCount;
    unsigned short bombsCount;
} Settings;





void SetBoundaries( Borders* const borders, const Settings* const settings, const unsigned short row, const unsigned short col )
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
}





void PlaceBombs( GameBoard* const board, const Settings* const settings )
{
    Borders borders; unsigned short row; unsigned short col;
    for ( unsigned short si = 0 ; si < settings->bombsCount; si++ )
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
}





unsigned short BreakCell( GameBoard* const board, const Settings* const settings, const unsigned short row, const unsigned short col )
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
    unsigned short iUserRow; unsigned short iUserCol;

    while ( 1 )
    {
        printf( "\nSelect the row you want to play in : " );
        iUserRow = AskIntBetween( 1, settings->rowCount, 2 ) - 1;
        printf( "Select the column you want to play in : " );
        iUserCol = AskIntBetween( 1, settings->colCount, 2 ) - 1;
        
        if ( board->row[ iUserRow ].column[ iUserCol ].isBroken == 0 )
        {
            board->selectedCell[ 0 ] = iUserRow;
            board->selectedCell[ 1 ] = iUserCol;
            break;
        }

        printf( "Please, select a cell that is not already broken\n" );
    }
}





void PrintGameBoard( const GameBoard* const board, const Settings* const settings )
{
    printf( "      " );
    for ( unsigned short i = 0 ; i < settings->colCount ; i++ )
        printf( "%02d ", i+1 );
    printf( "\n\n" );

    for ( unsigned short i = 0; i < settings->rowCount; i++ )
    {
        printf( "%02d    ", i+1 );
        for ( unsigned short j = 0; j < settings->colCount; j++ )
        {
            if ( board->row[ i ].column[ j ].isBroken == 1 )
            {
                if ( board->row[ i ].column[ j ].isBombed == 1 )
                    printf( "\033[0;31m■" );
                else if ( board->row[ i ].column[ j ].Value == 0 )
                    printf( "-" );
                else
                    printf( "%d", board->row[ i ].column[ j ].Value );
            }
            else if ( i == board->selectedCell[ 0 ] && j == board->selectedCell[ 1 ] )
                printf( "\033[0;36m■" );
            else if ( board->row[ i ].column[ j ].isFlagged == 1 )
                printf( "\033[0;32m■" );
            else
                printf( "■" );
            printf( "\033[0m  " );
        }
        printf( "\n" );
    }
}





void ChangeGameSettings( Settings* const settings )
{
    printf( "Enter how many rows the board will have : " );
    settings->rowCount = AskIntBetween( 1, 99, 2 );

    printf( "Enter how many columns the board will have : " );
    settings->colCount = AskIntBetween( 1, 99, 2 );

    printf( "Enter how many bombs are going to be placed on the board : " );
    settings->bombsCount = AskIntBetween( 0, settings->rowCount * settings->colCount, 2 );
}





void StartGame( unsigned short rowCount, unsigned short colCount, unsigned short bombsCount )
{
    Settings settings;
    settings.rowCount = rowCount;
    settings.colCount = colCount;
    settings.bombsCount = bombsCount;
    
    if ( !( ( 1 < settings.rowCount && settings.rowCount < 99) || ( 1 < settings.colCount && settings.colCount < 99) || (0 < settings.bombsCount && settings.bombsCount < settings.rowCount * settings.colCount) ) )
        return;
    
    char placeOrBreak; unsigned short endgame; unsigned short isFlagged;
    const char* statement = "";

    GameBoard board;

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

            for ( unsigned short i = 0; i < settings.rowCount; i++ )
                free( board.row[ i ].column );
            free( board.row );
            
            board.row = ( Row* ) malloc( settings.rowCount * sizeof( Row ) );
            for( unsigned short i = 0; i < settings.rowCount; i++ )
                board.row[ i ].column = ( Cell* ) malloc( settings.colCount * sizeof( Cell ) );
        }
        
        for( unsigned short i = 0; i < settings.rowCount; i++ )
            for ( unsigned short j = 0; j < settings.colCount; j++ )
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
            printf( "\n   %d \033[0;32m■\033[0m left\n", (settings.bombsCount - board.flaggedCells) );

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
                isFlagged = board.row[ board.selectedCell[ 0 ] ].column[ board.selectedCell[ 1 ] ].isFlagged;
                
                if ( board.flaggedCells < settings.bombsCount || isFlagged == 1 )
                {
                    board.row[ board.selectedCell[ 0 ] ].column[ board.selectedCell[ 1 ] ].isFlagged = !isFlagged;
                    board.flaggedCells += 1-2*isFlagged;
                }

                else
                    statement = "\nYou don't have any flag left, remove a flag if you want to place one here";
            }
            else if ( placeOrBreak == 'b' )
                endgame = BreakCell( &board, &settings, board.selectedCell[ 0 ], board.selectedCell[ 1 ] );
            
            board.selectedCell[ 0 ] = -1;
            board.selectedCell[ 1 ] = -1;
        }

        if ( !AskYesOrNo( "Do you want to play again ? (y/n)\n" ) )
            break;
    }

    for ( unsigned short i = 0; i < settings.rowCount; i++ )
        free( board.row[ i ].column );
    free( board.row );
}





int main()
{
    srand( time( NULL ) );
    StartGame( 14, 18, 40 );
    return 0;
}