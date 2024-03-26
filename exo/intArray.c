// IntArray

#ifndef __stdio__stdlib__
#include <stdio.h>
#include <stdlib.h>
#endif

typedef struct IntArray
{
    int* pContent;
    
    int iSize;
    int iCapacity;
    
} IntArray;

void Init( IntArray* pIntArray )
{
    pIntArray->iSize = 0;
    pIntArray->iCapacity = 1;
    pIntArray->pContent = ( int* ) malloc( sizeof( int ) * pIntArray->iCapacity );
}

void Add( IntArray* pIntArray, int iValue )
{
    if ( pIntArray->iSize == pIntArray->iCapacity )
    {
        pIntArray->iCapacity *= 2;
        pIntArray->pContent = ( int* ) realloc( pIntArray->pContent, sizeof( int ) * pIntArray->iCapacity );
    }
    
    pIntArray->pContent[ pIntArray->iSize ] = iValue;
    pIntArray->iSize++;
}

void Insert( IntArray* pIntArray, int iValue, int iIndex )
{
    if ( iIndex < 0 )
        iIndex += pIntArray->iSize;
    
    if ( !( 0 < iIndex && iIndex <= pIntArray->iSize ) )
        return;
    
    if ( pIntArray->iSize == pIntArray->iCapacity )
    {
        pIntArray->iCapacity *= 2;
        pIntArray->pContent = ( int* ) realloc( pIntArray->pContent, sizeof( int ) * pIntArray->iCapacity );
    }
    
    for ( int i = pIntArray->iSize; i > iIndex; i-- )
        pIntArray->pContent[ i ] = pIntArray->pContent[ i - 1 ];
    pIntArray->pContent[ iIndex ] = iValue;
    pIntArray->iSize++;
}

void Remove( IntArray* pIntArray, int iIndex )
{
    if ( iIndex < 0 )
        iIndex += pIntArray->iSize;
    
    if ( !( 0 < iIndex && iIndex < pIntArray->iSize ) )
        return;
    
    for ( int i = iIndex; i < pIntArray->iSize - 1; i++ )
        pIntArray->pContent[ i ] = pIntArray->pContent[ i + 1 ];
    pIntArray->iSize--;
    
    if ( pIntArray->iSize <= pIntArray->iCapacity / 2 )
    {
        pIntArray->iCapacity /= 2;
        pIntArray->pContent = ( int* ) realloc( pIntArray->pContent, sizeof( int ) * pIntArray->iCapacity );
    }
}

int Get( IntArray* pIntArray, int iIndex )
{
    if ( iIndex < 0 )
        iIndex += pIntArray->iSize;
    
    if ( 0 < iIndex && iIndex < pIntArray->iSize )
        return pIntArray->pContent[ iIndex ];
}

void Print( IntArray* pIntArray )
{
    printf( "{" );
    if ( pIntArray->iSize > 0 )
    {
        printf( " %d", pIntArray->pContent[ 0 ] );
        for ( int i = 1; i < pIntArray->iSize; i++ )
            printf( " ; %d", pIntArray->pContent[ i ] );
    }
    printf( " }\n" );
}

void Destroy( IntArray* pIntArray )
{
    pIntArray->iSize = 0;
    pIntArray->iCapacity = 0;
    free( pIntArray->pContent );
}

int main()
{
    IntArray oArray;
    Init( &oArray );
    
    Print( &oArray );
    for ( int i = 0; i < 10; i++ )
        Add( &oArray, i );
    Print( &oArray );
    
    for ( int i = 9; i >= 0; i-- )
        Insert( &oArray, 0, i );
    Print( &oArray );
    
    printf( "%d\n", Get( &oArray, 5 ) );
    
    for ( int i = 0; i < 10; i++ )
        Remove( &oArray, i );
    Print( &oArray );
    
    Destroy( &oArray );
    Print( &oArray );
    
    return 0;
}