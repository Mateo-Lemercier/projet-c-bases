#include <stdio.h>
#define __stdio_h__
#include <stdlib.h>
#define __stdlib_h__
#include <time.h>
#define __time_h__
#include <conio.h>
#define __conio_h__
#include "genericFunctions.c"
#define __genericFunctions_c__
#include "bigCharacters.c"
#define __bigCharacters_c__

#define KEY_SPACE 32
#define KEY_ENTER 13

#define KEY_ZERO 48

#define KEY_UP_ARROW 72
#define KEY_LEFT_ARROW 75
#define KEY_RIGHT_ARROW 77
#define KEY_DOWN_ARROW 80

typedef int int4;
typedef unsigned int uint4;

typedef short int2;
typedef unsigned short uint2;

typedef char int1;
typedef unsigned char uint1;

typedef char bool;
#define true 1
#define false 0

// Style : \033[?m

// Normal - 0
// Bold - 1
// Underline - 4

// Black Foreground - 30
// Red Foreground - 31
// Green Foreground - 32
// Yellow Foreground - 33
// Blue Foreground - 34
// Magenta Foreground - 35
// Cyan Foreground - 36
// White Foreground - 37

// Black Background - 40
// Red Background - 41
// Green Background - 42
// Yellow Background - 43
// Blue Background - 44
// Magenta Background - 45
// Cyan Background - 46
// White Background - 47

typedef struct Coords
{
    uint1 row;
    uint1 col;
} Coords;

typedef struct Tile
{
    uint1 Value; // Max = 9
    bool isBombed;
    bool isRevealed;
    bool isFlagged;
} Tile;

typedef struct Row
{
    Tile *column;
} Row;

typedef struct Board
{
    uint1 rowCount; // Max = 255
    uint1 colCount; // Max = 255
    uint2 bombsCount; // Max = 255*255 = 65_025
    uint2 revealedCount; // Max = 255*255 = 65_025
    uint2 flaggedCount; // Max = 255*255 = 65_025
    
    Coords selectedCell;
    bool isGameOver;
    
    Row *row;
} Board;

typedef struct Boundaries
{
    int2 rowStart; // Min = -1 | Max = 255
    uint2 rowEnd; // Max = 256
    int2 colStart; // Min = -1 | Max = 255
    uint2 colEnd; // Max = 256
} Boundaries;





int AskKeyFromList( const short *const iList, short iSize )
{
    char cUser; int cUserIndex;
    char *const sList = ( char* ) malloc( iSize * sizeof( char ) );
    for ( int i = 0 ; i < iSize ; i++ )
        sList[ i ] = iList[ i ];

    while ( true )
    {
        if ( kbhit() )
        {
            cUser = getch();
            cUserIndex = CharInString( cUser, sList );
            if ( cUserIndex != -1 )
                return iList[ cUserIndex ];
        }
    }
    
    free( sList );
}





void SetValuesInBoundaries( const Board *const board, Boundaries *const boundaries, const uint1 row, const uint1 col )
{
    boundaries->rowStart = row - 1;
    boundaries->rowEnd = row + 1;
    boundaries->colStart = col - 1;
    boundaries->colEnd = col + 1;

    if ( boundaries->rowStart < 0 )
        boundaries->rowStart++;
    
    if ( boundaries->rowEnd > board->rowCount - 1 )
        boundaries->rowEnd--;
    
    if ( boundaries->colStart < 0 )
        boundaries->colStart++;
    
    if ( boundaries->colEnd > board->colCount - 1 )
        boundaries->colEnd--;
}





void PlaceBombs( Board *const board )
{
    Coords *availableTiles = ( Coords* ) malloc( sizeof( Coords ) * ( board->rowCount * board->colCount ) );
    uint1 x = 0;
    
    for ( uint1 rowIndex = 0; rowIndex < board->rowCount; rowIndex++ )
    {
        for ( uint1 colIndex = 0; colIndex < board->colCount; colIndex++ )
        {
            if ( ( board->selectedCell.row - 1 <= rowIndex && rowIndex <= board->selectedCell.row + 1 ) && ( board->selectedCell.col - 1 <= colIndex && colIndex <= board->selectedCell.col + 1 ) )
            {
                availableTiles[ ( board->rowCount * board->colCount ) - 1 - x ].row = rowIndex;
                availableTiles[ ( board->rowCount * board->colCount ) - 1 - x ].col = colIndex;
                x++;
            }
            else
            {
                availableTiles[ ( rowIndex * board->colCount ) + colIndex - x ].row = rowIndex;
                availableTiles[ ( rowIndex * board->colCount ) + colIndex - x ].col = colIndex;
            }
        }
    }

    uint2 tileIndex;
    Boundaries boundaries;
    Coords transitionTile;

    for ( uint2 placedBombs = 0 ; placedBombs < board->bombsCount; placedBombs++ )
    {
        tileIndex = rand() % ( ( board->rowCount * board->colCount ) - placedBombs - x );
        board->row[ availableTiles[ tileIndex ].row ].column[ availableTiles[ tileIndex ].col ].isBombed = true;
        
        SetValuesInBoundaries( board, &boundaries, availableTiles[ tileIndex ].row, availableTiles[ tileIndex ].col );
        for (
                int2 rowIndex = boundaries.rowStart;
                rowIndex < boundaries.rowEnd + 1;
                rowIndex++
            )
            for (
                    int2 colIndex = boundaries.colStart;
                    colIndex < boundaries.colEnd + 1;
                    colIndex++
                )
                board->row[ rowIndex ].column[ colIndex ].Value++;
        
        transitionTile = availableTiles[ tileIndex ];
        availableTiles[ tileIndex ] = availableTiles[ ( board->rowCount * board->colCount ) - 1 - placedBombs - x ];
        availableTiles[ ( board->rowCount * board->colCount ) - 1 - placedBombs - x ] = transitionTile;
    }

    free ( availableTiles );
}





void RevealTile( Board *const board, const uint1 row, const uint1 col )
{
    if ( board->row[ row ].column [ col ].isRevealed || board->row[ row ].column [ col ].isFlagged )
        return;
    
    board->row[ row ].column [ col ].isRevealed = true;
    board->revealedCount++;
    if ( board->revealedCount == 1 )
        PlaceBombs( board );

    if ( board->row[ row ].column[ col ].isBombed )
    {
        board->isGameOver = true;
        return;
    }
    
    if ( board->row[ row ].column[ col ].Value != 0 )
        return;
    
    Boundaries boundaries;
    SetValuesInBoundaries( board, &boundaries, row, col );
    
    for (
            int2 rowIndex = boundaries.rowStart;
            rowIndex < boundaries.rowEnd + 1;
            rowIndex++
        )
        for (
                int2 colIndex = boundaries.colStart;
                colIndex < boundaries.colEnd + 1;
                colIndex++
            )
            RevealTile( board, rowIndex, colIndex );
}





void FlagSelected( Board *const board )
{
    if ( board->row[ board->selectedCell.row ].column[ board->selectedCell.col ].isRevealed )
        return;
    
    bool isFlagged = board->row[ board->selectedCell.row ].column[ board->selectedCell.col ].isFlagged;
    
    if ( !( board->flaggedCount < board->bombsCount || isFlagged ) )
        return;
    
    board->row[ board->selectedCell.row ].column[ board->selectedCell.col ].isFlagged = !isFlagged;
    board->flaggedCount += 1 - 2*isFlagged;
}





void PrintBoard( const Board *const board )
{
    char symbol;
    system( "cls" );
    for ( uint1 rowIndex = 0; rowIndex < board->rowCount; rowIndex++ )
    {
        for ( uint1 colIndex = 0; colIndex < board->colCount; colIndex++ )
        {
            printf( "\033[4m" );
            symbol = ' ';
            
            if ( board->row[ rowIndex ].column[ colIndex ].isRevealed )
            {
                printf( "\033[0m" );
                
                if ( board->row[ rowIndex ].column[ colIndex ].Value == 0 )
                    symbol = '-';
                else
                    symbol = board->row[ rowIndex ].column[ colIndex ].Value + '0';
            }
            
            else if ( board->row[ rowIndex ].column[ colIndex ].isFlagged )
                printf( "\033[41;30m" );
            
            else
                printf( "\033[42;30m" );
            
            
            if ( board->isGameOver )
            {
                if ( board->row[ rowIndex ].column[ colIndex ].isBombed )
                {
                    printf( "\033[4;45;30m" );
                    symbol = ' ';
                }
            }
            
            else if ( rowIndex == board->selectedCell.row && colIndex == board->selectedCell.col )
                printf( "\033[44;30m");
            
            
            printf( "%c \033[0m ", symbol );
        }
        
        printf( "\n" );
    }
    
    printf( "\n%d \033[0;41m  \033[0m left", ( board->bombsCount - board->flaggedCount ) );
}





void StartGame( Board *const board )
{
    const int2 keysList[] = { KEY_SPACE, KEY_ENTER, KEY_ZERO, KEY_UP_ARROW, KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_RIGHT_ARROW };
    int2 keyPressed;
    
    board->row = ( Row* ) malloc( sizeof( Row ) * board->rowCount );
    for ( uint1 rowIndex = 0; rowIndex < board->rowCount; rowIndex++ )
        board->row[ rowIndex ].column = ( Tile* ) malloc( sizeof( Tile ) * board->colCount );
    
    while ( true )
    {
        board->revealedCount = 0;
        board->flaggedCount = 0;
        board->selectedCell.row = 0;
        board->selectedCell.col = 0;
        board->isGameOver = false;
        
        for ( uint1 rowIndex = 0; rowIndex < board->rowCount; rowIndex++ )
        {
            for ( uint1 colIndex = 0; colIndex < board->colCount; colIndex++ )
            {
                board->row[ rowIndex ].column[ colIndex ].Value = 0;
                board->row[ rowIndex ].column[ colIndex ].isBombed = false;
                board->row[ rowIndex ].column[ colIndex ].isRevealed = false;
                board->row[ rowIndex ].column[ colIndex ].isFlagged = false;
            }
        }

        PrintBoard( board );
        
        while ( !( board->isGameOver || board->revealedCount == (board->rowCount * board->colCount) - board->bombsCount ) )
        {
            keyPressed = AskKeyFromList( keysList, 7 );
            
            if ( keyPressed == KEY_UP_ARROW )
            {
                if ( board->selectedCell.row > 0 )
                    board->selectedCell.row--;
            }
            
            else if ( keyPressed == KEY_LEFT_ARROW )
            {
                if ( board->selectedCell.col > 0 )
                    board->selectedCell.col--;
            }
            
            else if ( keyPressed == KEY_DOWN_ARROW )
            {
                if ( board->selectedCell.row < board->rowCount - 1 )
                    board->selectedCell.row++;
            }
            
            else if ( keyPressed == KEY_RIGHT_ARROW )
            {
                if ( board->selectedCell.col< board->colCount - 1 )
                    board->selectedCell.col++;
            }

            else if ( keyPressed == KEY_SPACE || keyPressed == KEY_ENTER )
            {
                RevealTile( board, board->selectedCell.row, board->selectedCell.col );
            }

            else if ( keyPressed == KEY_ZERO )
            {
                FlagSelected( board );
            }

            else if ( keyPressed == KEY_ENTER )
            {
                break;
            }
            
            PrintBoard( board );
        }
        
        if ( board->isGameOver )
            printf( "\nYou've lost!\n" );
        else
            printf( "\nYou won !\n" );
        
        if ( !AskYesOrNo( "Do you want to play again ? (y/n)\n" ) )
            break;
    }
    
    for ( uint2 i = 0; i < board->rowCount; i++ )
        free( board->row[ i ].column );
    free( board->row );
}





void PrintTitle( const char *const title, uint1 iSize )
{
    uint1 spaceCount;
    system( "cls" );
    
    printf( "\033[0;47m                                                                                                                       \n" );
    
    for (uint1 terminalRow = 0 ; terminalRow < 2 ; terminalRow++)
        printf( " \033[0m                                                                                                                     \033[0;47m \n" );
    
    for (uint1 characterRow = 0 ; characterRow < 6 ; characterRow++)
    {
        printf( " \033[0m" );
        spaceCount = (117 - 9*iSize)/2;
        for (uint1 space = 0 ; space < 117 - 9*iSize - spaceCount ; space++)
            printf( " " );
        
        PrintBigText( title, characterRow );
        
        for (uint1 space = 0 ; space < spaceCount ; space++)
            printf( " " );
        printf( "\033[0;47m \n" );
    }
    
    for (uint1 terminalRow = 8 ; terminalRow < 10 ; terminalRow++)
        printf( " \033[0m                                                                                                                     \033[0;47m \n" );
    
    printf( "                                                                                                                       \n" );
}





void PrintSettings( const Board *const board, const uint1 selectedOption )
{
    PrintTitle( "Settings", 8 );
    
    for (uint1 terminalRow = 11 ; terminalRow < 14 ; terminalRow++)
        printf( " \033[0m                                                                                                                     \033[0;47m \n" );
    
    printf( " \033[0m                                                        Rows                                                         \033[0;47m \n" );
    printf( " \033[0m                                                       < " );
    if ( selectedOption == 1 )
        printf( "\033[4;36m" );
    printf( "%02d\033[0m >                                                        \033[0;47m \n", board->rowCount );
    
    for (uint1 terminalRow = 16 ; terminalRow < 17 ; terminalRow++)
        printf( " \033[0m                                                                                                                     \033[0;47m \n" );
    
    printf( " \033[0m                                                       Columns                                                       \033[0;47m \n" );
    printf( " \033[0m                                                       < " );
    if ( selectedOption == 2 )
        printf( "\033[4;36m" );
    printf( "%02d\033[0m >                                                        \033[0;47m \n", board->colCount );
    
    for (uint1 terminalRow = 19 ; terminalRow < 20 ; terminalRow++)
        printf( " \033[0m                                                                                                                     \033[0;47m \n" );
    
    printf( " \033[0m                                                        Bombs                                                        \033[0;47m \n" );
    printf( " \033[0m                                                       < " );
    if ( selectedOption == 3 )
        printf( "\033[4;36m" );
    printf( "%03d\033[0m >                                                       \033[0;47m \n", board->bombsCount );
    
    for (uint1 terminalRow = 22 ; terminalRow < 26 ; terminalRow++)
        printf( " \033[0m                                                                                                                     \033[0;47m \n" );
    
    printf( "                                                                                                                       \033[0m\n" );
}





void PrintMainMenu( const uint1 selectedOption )
{
    PrintTitle( "Minesweeper", 12 );
    
    for (uint1 terminalRow = 11 ; terminalRow < 15 ; terminalRow++)
        printf( " \033[0m                                                                                                                     \033[0;47m \n" );
    
    printf( " \033[0m                                                        " );
    if ( selectedOption == 1 )
        printf( "\033[4;36m" );
    printf( "Start\033[0m                                                        \033[0;47m \n" );
    
    for (uint1 terminalRow = 16 ; terminalRow < 18 ; terminalRow++)
        printf( " \033[0m                                                                                                                     \033[0;47m \n" );
    
    printf( " \033[0m                                                       " );
    if ( selectedOption == 2 )
        printf( "\033[4;36m" );
    printf( "Settings\033[0m                                                      \033[0;47m \n" );
    
    for (uint1 terminalRow = 19 ; terminalRow < 21 ; terminalRow++)
        printf( " \033[0m                                                                                                                     \033[0;47m \n" );
    
    printf( " \033[0m                                                        " );
    if ( selectedOption == 3 )
        printf( "\033[4;36m" );
    printf( "Quit\033[0m                                                         \033[0;47m \n" );
    
    for (uint1 terminalRow = 22 ; terminalRow < 26 ; terminalRow++)
        printf( " \033[0m                                                                                                                     \033[0;47m \n" );
    
    printf( "                                                                                                                       \033[0m\n" );
}





void OpenSettings( Board *const board )
{
    const int2 keysList[] = { KEY_ENTER, KEY_UP_ARROW, KEY_LEFT_ARROW, KEY_DOWN_ARROW, KEY_RIGHT_ARROW };
    int2 keyPressed;
    
    uint1 selectedOption = 1;
    
    while ( true )
    {
        PrintSettings( board, selectedOption );
        
        keyPressed = AskKeyFromList( keysList, 5 );
        
        if ( keyPressed == KEY_ENTER )
        {
            return;
        }
        
        else if ( keyPressed == KEY_UP_ARROW )
        {
            selectedOption--;
            if ( selectedOption < 1 )
                selectedOption = 3;
        }
        
        else if ( keyPressed == KEY_DOWN_ARROW )
        {
            selectedOption++;
            if ( selectedOption > 3 )
                selectedOption = 1;
        }
        
        else if ( keyPressed == KEY_LEFT_ARROW )
        {
            if ( selectedOption == 1 )
            {
                if ( board->rowCount > 3 )
                    board->rowCount--;
            }
            
            else if ( selectedOption == 2 )
            {
                if ( board->colCount > 3 )
                    board->colCount--;
            }
            
            else if ( selectedOption == 3 )
            {
                if ( board->bombsCount > 0 )
                    board->bombsCount--;
            }
            
            if ( ( board->rowCount * board->colCount ) - 9 < board->bombsCount )
                board->bombsCount = ( board->rowCount * board->colCount ) - 9;
        }
        
        else if ( keyPressed == KEY_RIGHT_ARROW )
        {
            if ( selectedOption == 1 )
            {
                if ( board->rowCount < 24 )
                    board->rowCount++;
            }
            
            else if ( selectedOption == 2 )
            {
                if ( board->colCount < 32 )
                    board->colCount++;
            }
            
            else if ( selectedOption == 3 )
            {
                if ( board->bombsCount < ( board->rowCount * board->colCount ) - 9 )
                    board->bombsCount++;
            }
        }
    }
}





void OpenMainMenu( Board *const board )
{
    const int2 keysList[] = { KEY_ENTER, KEY_UP_ARROW, KEY_DOWN_ARROW };
    int2 keyPressed;
    
    uint1 selectedOption = 1;
    
    while ( true )
    {
        PrintMainMenu( selectedOption );
        
        keyPressed = AskKeyFromList( keysList, 3 );
        
        if ( keyPressed == KEY_ENTER )
        {
            if ( selectedOption == 1 )
                StartGame( board );
            
            else if ( selectedOption == 2 )
                OpenSettings( board );
            
            else if ( selectedOption == 3 )
                return;
        }
        
        else if ( keyPressed == KEY_UP_ARROW )
        {
            selectedOption--;
            if ( selectedOption < 1 )
                selectedOption = 3;
        }
        
        else if ( keyPressed == KEY_DOWN_ARROW )
        {
            selectedOption++;
            if ( selectedOption > 3 )
                selectedOption = 1;
        }
    }
}





int main()
{
    srand( time( NULL ) );
    Board board = { 14, 18, 40 };
    
    OpenMainMenu( &board );
    
    return 0;
}