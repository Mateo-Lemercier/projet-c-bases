#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
//dynamic array

typedef struct IntArray
{
    int* pContent;

    int iSize;
    int iCapacity;

} IntArray;

void Init( IntArray*pIntArray)
{
    pIntArray -> iSize = 0;
    pIntArray -> iCapacity = 1;
    pIntArray -> pContent = (int*)malloc(sizeof(int) * pIntArray -> iCapacity);
}

void Add(IntArray* pIntArray, int iValue)
{
    if (pIntArray -> iCapacity <= pIntArray -> iSize)
    {
        pIntArray -> iCapacity *= 2;
        pIntArray -> pContent = (int*)realloc(pIntArray -> pContent, sizeof(int) * pIntArray -> iCapacity);
    }
    pIntArray->pContent[pIntArray ->iSize] = iValue;
    pIntArray ->iSize++;
}
void Insert(IntArray* pIntArray, int iValue, int iIndex)
{
    if( iIndex<0 || iIndex > pIntArray -> iSize)
        return;

    if (pIntArray -> iCapacity <= pIntArray -> iSize)
    {
        pIntArray -> iCapacity *= 2;
        pIntArray -> pContent = (int*)realloc(pIntArray -> pContent, sizeof(int) * pIntArray -> iCapacity);
    }
    pIntArray ->iSize++;
    for (int i = pIntArray -> iSize; i > iIndex; i--)
    {
        pIntArray -> pContent[i] = pIntArray -> pContent[i-1];
    }
    pIntArray->pContent[iIndex] = iValue;    

}

void Remove(IntArray* pIntArray, int iIndex)
{   
    if (iIndex < 0 || iIndex >= pIntArray -> iSize)
        return;
    for (int i = iIndex; i < pIntArray -> iSize; i++)
    {
        pIntArray -> pContent[i] = pIntArray -> pContent[i+1];
    } 
    pIntArray -> iSize--;

    if (pIntArray -> iSize < pIntArray -> iCapacity / 4)
    {
        pIntArray -> iCapacity /= 2;
        pIntArray -> pContent = (int*)realloc(pIntArray -> pContent, sizeof(int) * pIntArray -> iCapacity);
    }
}

int Get(IntArray* pIntArray, int iIndex)
{
    if (iIndex >= 0 && iIndex < pIntArray -> iSize)
        return pIntArray ->pContent[iIndex];
}

void Print(IntArray* pIntArray)
{   
    printf("{");
    if(pIntArray->iSize > 0)
        printf(" %d", pIntArray->pContent[0]);

    for(int i = 1; i < pIntArray->iSize; i++)
        printf(", %d", pIntArray->pContent[i]);

    printf(" }");
}

void Destroy(IntArray* pIntArray)
{
    free(pIntArray->pContent);
}

int main()
{
    IntArray oArray;
    Init(&oArray);
    Add(&oArray, -1);
    Add(&oArray, 2);
    Insert(&oArray,12,0);
    Remove(&oArray,0);
    Get(&oArray,0);
    Print(&oArray);
    Destroy(&oArray);

    return 0;
}
