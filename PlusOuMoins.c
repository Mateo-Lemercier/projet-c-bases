#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "genericFunctions.c"





void ChangeGameSettings(int* settings)
{
    printf( "Enter the minimum value : " );
    settings[ 0 ] = AskInt();

    printf( "\nEnter the maximum value : " );
    settings[ 1 ] = AskIntBetween( settings[ 0 ], 0, 0 );

    printf( "Enter the maximum amount of tries allowed : " );
    settings[ 2 ] = AskIntBetween( 1, 0, 0 );
}





void StartGame( int min, int max, int maxTries )
{
    int iUser; int iMach;
    int winStreak = 0;
    int tries = 0;

    while ( 1 )
    {
        printf( "Right now, the minimum value is %d, the maximum value is %d and the maximum amount of tries is %d\n", min, max, maxTries );
        if ( AskYesOrNo( "Do you want to change the game settings ? (y/n)\n" ) )
        {
            int* settings = ( int* ) malloc( sizeof( int ) * 3 );
            ChangeGameSettings( settings );
            min = settings[ 0 ];
            max = settings[ 1 ];
            maxTries = settings[ 2 ];
            free( settings );
        }

        iMach = min + ( rand() % ( max - min + 1 ) );

        printf( "\nFind my number" );
        while ( 1 )
        {

            printf( " (%d tries left)\n", maxTries - tries );
            iUser = AskInt();
            tries++;

            if ( tries >= maxTries )
            {
                printf( " You don't have any tries left... My number was %d\n", iMach );
                break;
            }

            if ( iMach < iUser )
            {
                printf( "My Number is... lower than %d", iUser );
                continue;
            }

            if ( iMach > iUser )
            {
                printf( "My Number is... higher than %d", iUser );
                continue;
            }

            printf( "You found my number !\n" );
            winStreak++;
            break;
        }
        
        if ( !AskYesOrNo( "Do you want to play again ? (y/n)\n" ) )
            break;
    }
}





int main()
{
    srand( time( NULL ) );
    StartGame( 0, 100, 7 );
    return 0;
}