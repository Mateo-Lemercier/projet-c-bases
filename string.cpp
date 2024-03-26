#include <iostream>
#include <stdlib.h>
#include "varTypes.h"

class String
{
public:
    uint2 length;

    String( const char *const sContent )
    {
        for ( this->length = 0; sContent[ this->length ] != '\0'; this->length++ );
        this->sContent = ( char* ) malloc( sizeof( char ) * this->length+1 );
        int i;
        for ( int i = 0; i < this->length; i++ )
            this->sContent[ i ] = sContent[ i ];
    }

    void insert( String *sString, int2 iIndex )
    {
        this->length += sString->length;
    }

    void print()
    {
        for ( int i = 0; i < this->length; i++ )
            printf( "%c", this->sContent[ i ] );
        printf( "\n" );
    }

    const char* c_str() const
    {
        return sContent;
    }

private:
    char* sContent;

};

int main()
{
    String s("aaa");

    printf( "%s", s.c_str() );

    
    //printf( "Your string is %d characters long\n", s.length );
    return 0;
}