#include <iostream>
#include <stdlib.h>
#include "varTypes.h"

// void Assert( bool bAssertion, const char* message, bool bQuit = true )
// {
//     if ( bAssertion )
//         return;

//     std::cerr << "Assertion failed: " << message << std::endl;

//     if ( bQuit )
//         exit( 1 );
// }

class String
{
public:
    
    String( const char *const sContent )
    {
        charListLength( sContent, &m_iLength );
        m_sContent = ( char* ) malloc( sizeof( char ) * m_iLength+1 );
        for ( int i = 0; i < m_iLength; i++ )
            m_sContent[ i ] = sContent[ i ];
        m_sContent[ m_iLength ] = '\0';
    }
    
    const char* c_str() const
    { return m_sContent; }
    
    const uint2 length() const
    { return m_iLength; }
    
    
    
    
    
    void substring( uint2 iStartIndex, uint2 iEndIndex )
    {
        if ( iStartIndex < 0 )
            iStartIndex = m_iLength + iStartIndex;
        // Assert( iStartIndex < 0 || iStartIndex > m_iLength, "Invalid Starting Index" );
        
        if ( iEndIndex < 0 )
            iEndIndex = m_iLength + iEndIndex;
        // Assert( iEndIndex < 0 || iEndIndex > m_iLength, "Invalid End Index" );
        
        // Assert( ( iStartIndex > iEndIndex ), "Invalid Index Range (Start > End)" );
        
        m_iLength = iEndIndex - iStartIndex + 1;
        for ( uint2 i = 0; i < m_iLength; i++ )
            m_sContent[ i ] = m_sContent[ iStartIndex + i ];
        m_sContent = ( char* ) realloc( m_sContent, sizeof( char ) * m_iLength+1 );
        m_sContent[ m_iLength ] = '\0';
    }
    
    void insert( const String *const sString, int2 iIndex ) //* Insert with a string
    {
        if ( iIndex < 0 )
            iIndex = m_iLength + iIndex + 1;
        
        // Assert( iIndex < 0 || iIndex > m_iLength, "Invalid Index" );
        
        char *sSavedContent = ( char* ) malloc( sizeof( char ) * ( m_iLength - iIndex ) );
        for ( int i = iIndex; i < m_iLength; i++ )
            sSavedContent[ i - iIndex ] = m_sContent[ i ];
        
        m_iLength += sString->length();
        m_sContent = ( char* ) realloc( m_sContent, sizeof( char ) * m_iLength+1 );
        
        for ( int i = 0; i < sString->length(); i++ )
            m_sContent[ iIndex + i ] = sString->c_str()[ i ];
        
        for ( int i = iIndex + sString->length(); i < m_iLength; i++ )
            m_sContent[ i ] = sSavedContent[ i - iIndex - sString->length() ];
        
        m_sContent[ m_iLength ] = '\0';
        free( sSavedContent );
    }
    
    void insert( const char *const str, int2 iIndex ) //* Insert with a list of characters
    {
        if ( iIndex < 0 )
            iIndex = m_iLength + iIndex + 1;
        // Assert( iIndex < 0 || iIndex > m_iLength, "Invalid Index" );
        
        char *sSavedContent = ( char* ) malloc( sizeof( char ) * ( m_iLength - iIndex ) );
        for ( int i = iIndex; i < m_iLength; i++ )
            sSavedContent[ i - iIndex ] = m_sContent[ i ];
        
        uint2 iStrLength = charListLength( str );
        m_iLength += iStrLength;
        m_sContent = ( char* ) realloc( m_sContent, sizeof( char ) * m_iLength+1 );
        
        for ( int i = 0; i < iStrLength; i++ )
            m_sContent[ iIndex + i ] = str[ i ];
        
        for ( int i = iIndex + iStrLength; i < m_iLength; i++ )
            m_sContent[ i ] = sSavedContent[ i - iIndex - iStrLength ];
        
        m_sContent[ m_iLength ] = '\0';
        free( sSavedContent );
    }
    
    void append( const String *const sString ) //* Append with a string
    { insert( sString , -1 ); }
    
    void append( const char *const str ) //* Append with a list of characters
    { insert( str , -1 ); }
    
    
    
    
    
    static String Substring( const String *sString, const uint2 iStartIndex, const uint2 iEndIndex )
    {
        String sResult( sString->c_str() );
        sResult.substring( iStartIndex, iEndIndex );
        return sResult;
    }
    
    static String Insert( String *sString1, String *sString2, int2 iIndex )
    {
        if ( iIndex < 0 )
            iIndex = sString1->length() - iIndex + 1;
        
        // Assert( iIndex < 0 || iIndex > sString1->length(), "Invalid Index" );
        
        String sResult( Substring( sString1, 0, iIndex ).c_str() );
        sResult.append( sString2 );
        String sEndSubstring = Substring( sString1, iIndex + 1, -1 );
        sResult.append( &sEndSubstring );
        return sResult;
    }
    
private:
    char *m_sContent;
    uint2 m_iLength;

    void charListLength( const char *const str, uint2* pLength ) const
    {
        for ( *pLength = 0; str[ *pLength ] != '\0'; (*pLength)++ );
    }
    
    const uint2 charListLength( const char *const str ) const
    {
        uint2 iLength;
        charListLength( str, &iLength );
        return iLength;
    }
};



int main()
{
    String s("015");
    String s2("234");
    printf( "%s\n", s.c_str() );
    
    s.insert( &s2, 2 );
    printf( "%s\n", s.c_str() );
    
    s.insert( "abc", 3 );
    printf( "%s\n", s.c_str() );
    
    s.substring( 0, -1 );
    printf( "%s\n", s.c_str() );
    
    printf( "%s\n",String::Insert( &s, &s2, 1 ).c_str() );
    
    return 0;
}