//List

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <math.h>

typedef struct Node Node;


struct Node
{
    Node* pNext;
    Node* pPrevious;

    int iValue;
};

typedef struct List
{
    Node* pFirst;
    int iCount;

}List;

void Init(List*pList)
{
    pList->pFirst = NULL;
    pList->iCount = 0;
}

void AddFirst(List* pList, int iValue)
{
    Node* pNode = (Node*)malloc(sizeof(Node));
    pNode->iValue = iValue;
    pNode->pNext = pList->pFirst;
    pNode->pPrevious = NULL;
}

void AddLast(List* pList, int iValue)
{
    Node* pNode = (Node*)malloc(sizeof(Node));
    pNode->iValue = iValue;
    pNode->pNext = NULL;
    pNode->pPrevious = pList->pFirst;
}

void AddBeforeNode(List* pList, Node* pNode, int iValue)
{
    Node* pNewNode = (Node*)malloc(sizeof(Node));
    pNewNode->iValue = iValue;
    pNewNode->pNext = pNode;
    pNewNode->pPrevious = pNode->pPrevious;
    pNode->pPrevious->pNext = pNewNode;
    pNode->pPrevious = pNewNode;
}

void AddAfterNode(List* pList, Node* pNode, int iValue)
{
    Node* pNewNode = (Node*)malloc(sizeof(Node));
    pNewNode->iValue = iValue;
    pNewNode->pNext = pNode->pNext;
    pNewNode->pPrevious = pNode;
    pNode->pNext->pPrevious = pNewNode;
    pNode->pNext = pNewNode;
}

void Insert(List* pList, int iIndex, int iValue)
{
    if (iIndex == 0)
    {
        AddFirst(pList, iValue);
    }
    else if (iIndex == pList->iCount)
    {
        AddLast(pList, iValue);
    }
    else
    {
        Node* pNode = pList->pFirst;
        for (int i = 0; i < iIndex - 1; i++)
        {
            pNode = pNode->pNext;
        }
        Node* pNewNode = (Node*)malloc(sizeof(Node));
        pNewNode->iValue = iValue;
        pNewNode->pNext = pNode->pNext;
        pNewNode->pPrevious = pNode;
        pNode->pNext->pPrevious = pNewNode;
        pNode->pNext = pNewNode;
    }
    pList->iCount++;
}

void Remove(List* pList, int iIndex)
{
    if (iIndex == 0)
    {
        Node* pNode = pList->pFirst;
        pList->pFirst = pList->pFirst->pNext;
        pList->pFirst->pPrevious = NULL;
        free(pNode);
    }
    else if (iIndex == pList->iCount - 1)
    {
        Node* pNode = pList->pFirst;
        for (int i = 0; i < iIndex - 1; i++)
        {
            pNode = pNode->pNext;
        }
        pNode->pNext = pNode->pNext->pNext;
        pNode->pNext->pPrevious = pNode;
        free(pNode);
    }
    else{
        Node* pNode = pList->pFirst;
        for (int i = 0; i < iIndex - 1; i++)
        {
            pNode = pNode->pNext;
        }
        pNode->pNext = pNode->pNext->pNext;
        pNode->pNext->pPrevious = pNode;
        free(pNode);
    }
}

void RemoveNode(List* pList, int iIndex)
{
    Node* pNode = pList->pFirst;
    for (int i = 0; i < iIndex - 1; i++)
    {
        pNode = pNode->pNext;
    }
    pNode->pNext = pNode->pNext->pNext;
    pNode->pNext->pPrevious = pNode;
    free(pNode);
}

Node* GetNode(List* pList, int iIndex)
{
    Node* pNode = pList->pFirst;
    for (int i = 0; i < iIndex - 1; i++)
    {
        pNode = pNode->pNext;
    }
    return pNode;
}

void Print(List* pList)
{
        printf("{");
    if(pList->iCount > 0)
        printf(" %d", pList->pFirst[0]);

    for(int i = 1; i < pList->iCount; i++)
        printf(", %d", pList->pFirst[i]);

    printf(" }");
}

void Destroy(List* pList)
{
    while (pList->pFirst!= NULL)
    {
        Node* pNode = pList->pFirst;
        pList->pFirst = pList->pFirst->pNext;
        free(pNode);
    }
}

int main()
{
    List oList;
    Init(&oList);
    Print(&oList);
    AddLast(&oList, 1);
    AddLast(&oList, 2);
    AddLast(&oList, 3);
    AddFirst(&oList, 0);
    Destroy(&oList);

}