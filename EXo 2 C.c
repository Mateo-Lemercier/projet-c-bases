//string
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>
#include <math.h>

typedef struct String
{
    char* pContent;
    int iLength;
}String;


String Create(const char* str)
{
    int len=0;
    for (size_t i = 0; i < sizeof str; ++i) 
    {
        if(str[i] == '\0')
            break;
        len++;
    }
    return (String){str, len};
}

void Print(const String* pStr)
{
    printf("{");
    if(pStr->iLength > 0)
        printf(" %c", pStr -> pContent[0]);
    for(int i = 1; i < pStr -> iLength; i++)
        printf("%c", pStr -> pContent[i]);

    printf(" }");
    printf("\n");

}

String Concatenate1(const String* pStr1, const String* pStr2)
{
    String str;
    str.iLength = pStr1 -> iLength + pStr2 -> iLength;
    str.pContent = (char*)malloc(sizeof(char) * str.iLength);
    for(int i = 0; i < pStr1 -> iLength; i++)
        str.pContent[i] = pStr1 -> pContent[i];
    for(int i = 0; i < pStr2 -> iLength; i++)
        str.pContent[i + pStr1 -> iLength] = pStr2 -> pContent[i];
    return str;
}

String Concatenate2(const String* str1, const String* str2)
{
    String str;
    str.iLength = str1 -> iLength + str2 -> iLength;
    str.pContent = (char*)malloc(sizeof(char) * str.iLength);
    for(int i = 0; i < str1 -> iLength; i++)
        str.pContent[i] = str1 -> pContent[i];
    for(int i = 0; i < str2 -> iLength; i++)
        str.pContent[i + str1 -> iLength] = str2 -> pContent[i];
    return str;
}

String Substring(const String* pStr1, int iStartIndex, int iLength)
{
    String str;
    str.iLength = iLength;
    str.pContent = (char*)malloc(sizeof(char) * str.iLength);
    for(int i = 0; i < iLength; i++)
        str.pContent[i] = pStr1 -> pContent[i + iStartIndex];
    return str;
}

String Insert(const String* pStr1, const String* pStr2, int iIndex)
{
    String str;
    str.iLength = pStr1 -> iLength + pStr2 -> iLength;
    str.pContent = (char*)malloc(sizeof(char) * str.iLength);
    for(int i = 0; i < iIndex; i++)
        str.pContent[i] = pStr1 -> pContent[i];
    for(int i = 0; i < pStr2 -> iLength; i++)
        str.pContent[i + iIndex] = pStr2 -> pContent[i];
    for(int i = iIndex; i != pStr1 -> iLength ; i++)
        str.pContent[i + pStr2 -> iLength] = pStr1 -> pContent[i];
    return str;
}

int AreEqual(const String* pStr1, const String* pStr2)
{
    if(pStr1 -> iLength!= pStr2 -> iLength)
        return 0;
    for(int i = 0; i < pStr1 -> iLength; i++)
        if(pStr1 -> pContent[i]!= pStr2 -> pContent[i])
            return 0;
    return 1;
}

int TryCastToInt(const String* pStr, int pResult)
{
    for(int i = 0; i < pStr -> iLength; i++)
    {
        if(pStr -> pContent [i] < '0' || pStr -> pContent [i] > '9')
            return 0;
        pResult = (pResult + pStr -> pContent [i] - '0')*10;
    }
    pResult=pResult/10;
    return 1;
}

void Destroy(String* pStr)
{
    free(pStr->pContent);
    free(pStr);
}

int main()
{
    String str1 = Create("Bonjour");
    Print(&str1);
    String str2 = Create("Bye");
    String str3 = Concatenate1(&str1, &str2);
    Print(&str3);
    String str4 = Insert(&str1, &str2, 3);
    Print(&str4);
    String str5 = Create("1321679874321685789");
    TryCastToInt(&str5, 0);
}