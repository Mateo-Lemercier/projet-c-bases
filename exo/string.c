// String

#ifndef __stdio__stdlib__
#include <stdio.h>
#include <stdlib.h>
#endif

typedef struct String
{
    char* pContent;
    int iLength;
} String;

int StrLength( const char* str )
{
    int iLength;
    for ( iLength = 0; str[ iLength ] != '\0'; iLength++ );
    return iLength;
}

String Create( const char* str )
{
    String pStr;
    
    pStr.iLength = StrLength( str );
    pStr.pContent = ( char* ) malloc( sizeof( char ) * (pStr.iLength) );
    
    for ( int i = 0; i < pStr.iLength; i++ )
        pStr.pContent[ i ] = str[ i ];
    
    return pStr;
}

void Print( const String* pStr )
{
    for ( int i = 0; i < pStr->iLength; i++ )
        printf( "%c", pStr->pContent[ i ] );
    printf( "\n" );
}

String Concatenate1( const String* pStr1, const String* pStr2 )
{
    String pStr3;
    
    pStr3.iLength = pStr1->iLength + pStr2->iLength;
    pStr3.pContent = ( char* ) malloc( sizeof( char ) * pStr3.iLength );
    
    for ( int i = 0; i < pStr1->iLength; i++ )
        pStr3.pContent[ i ] = pStr1->pContent[ i ];
    
    for ( int i = 0; i < pStr2->iLength; i++ )
        pStr3.pContent[ pStr1->iLength + i ] = pStr2->pContent[ i ];
    
    return pStr3;
}

String Concatenate2( const char* str1, const char* str2 )
{
    String pStr;
    
    pStr.iLength = StrLength( str1 ) + StrLength( str2 );
    pStr.pContent = ( char* ) malloc( sizeof( char ) * pStr.iLength );
    
    for ( int i = 0; i < StrLength( str1 ); i++ )
        pStr.pContent[ i ] = str1[ i ];
    
    for ( int i = 0; i < StrLength( str2 ); i++ )
        pStr.pContent[ StrLength( str1 ) + i ] = str2[ i ];
    
    return pStr;
}

String Substring( const String* pStr1, int iStartIndex, int iEndIndex )
{
    String pStr2;
    
    pStr2.iLength = iEndIndex - iStartIndex + 1;
    pStr2.pContent = ( char* ) malloc( sizeof( char ) * pStr2.iLength );
    
    for ( int i = iStartIndex; i <= iEndIndex; i++ )
        pStr2.pContent[ i - iStartIndex ] = pStr1->pContent[ i ];
    
    return pStr2;
}

String Insert( const String* pStr1, const String* pStr2, int iIndex )
{
    String pStr3;
    
    pStr3.iLength = pStr1->iLength + pStr2->iLength;
    pStr3.pContent = ( char* ) malloc( sizeof( char ) * pStr3.iLength );

    for ( int i = 0; i < iIndex; i++ )
        pStr3.pContent[ i ] = pStr1->pContent[ i ];
    
    for ( int i = 0; i < pStr2->iLength; i++ )
        pStr3.pContent[ iIndex + i ] = pStr2->pContent[ i ];
    
    for ( int i = iIndex; i < pStr1->iLength; i++ )
        pStr3.pContent[ pStr2->iLength + i ] = pStr1->pContent[ i ];
    
    return pStr3;
}

int AreEquals( const String* pStr1, const String* pStr2 )
{
    if ( pStr1->iLength != pStr2->iLength )
        return 0;
    
    for ( int i = 0; i < pStr1->iLength; i++ )
        if ( pStr1->pContent[ i ] != pStr2->pContent[ i ] )
            return 0;
    
    return 1;
}

int TryCastToInt( const String* pStr, int* pResult )
{
    for ( int i = 0; i < pStr->iLength; i++ )
    {
        *pResult = pow(10, pStr->iLength - 1 - i) * pStr->pContent[ i ] - '0';
        if ( !( '0' < pStr->pContent[ i ] && pStr->pContent[ i ] < '9' ) )
            return 0;
    }
    return 1;
}

void Destroy( String* pStr )
{
    pStr->iLength = 0;
    free( pStr->pContent );
}

int main()
{
    String str1 = Create( "Bonjour" );
    
    Print( &str1 );
    /*
    String str2 = Create( "AuRevoir" );
    
    String str3 = Concatenate1( &str1, &str2 );
    
    Print( &str3 );
    
    Destroy( &str1 );
    Destroy( &str2 );
    Destroy( &str3 );
    */
    return 0;
}