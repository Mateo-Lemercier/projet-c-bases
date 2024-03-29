#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

void scanf_Clear()
{
	while (getchar() != '\n') {}
}

int table(float* t)
{
    for (int i = 0; i < 10; i++)
    {
        t[i] = 5;
        //printf("%f\n", t[i]);
    }
}

int ask_int()
{
    int deci;
    printf("please choose a number :  \n");
        while(scanf_s("%d", &deci)==0)
        { 
            scanf_Clear();
            printf("please choose a decimal number \n");
        }
        scanf_Clear();
    return deci;
}

int ask_intP()
{
    int deci;
    printf("please choose a number :  \n");
    while(scanf_s("%d", &deci)==0 && deci<0)
    { 
        scanf_Clear();
        printf("please choose a decimal number \n");
        
    }
    scanf_Clear();
    return deci;
}

int main()
{
    //1)
    float f1 = 5;
    float* p1 = &f1;
    *p1 =10;
    float f2 = 0;
    if(f2 ==0)
    {
        float* p1 = &f2;
        *p1 = 20;
        //printf("%f\n",f2);
    }
    
    //printf("%f\n",f1);

    
    //2)
    float*pf1 = (float*)malloc(sizeof(float));
    
    if( pf1 == NULL)
        exit(1);
    
    *pf1 = 5;

    float*pf2 = (float*)malloc(sizeof(float));

    if( pf2 == NULL)
        exit(1);
    
    *pf2 = 10;
    float *temp=pf1;
    pf1 = pf2;
    pf2 = temp;

    //printf("%p\n",pf1);
    //printf("%p\n",&pf1);
    //printf("%p\n",pf2);
    //printf("%p\n",&pf2);
    free(pf1);
    free(pf2);

    //3)
    

    float* t = (float*)malloc(sizeof(float)*10);
    for (int i = 0; i < 10; i++)
    {
        t[i] = 0;
        //printf("%f\n",t[i]);
    }
    table(t);
    free(t);

    //4)

    int tf[10];
    for (int i = 0; i < 10; i++)
    {
        tf[i] = 0;
        //printf("%f\n",tf[i]);
    }
    //table(tf);

    //5)
    
    /*
    int deci = ask_int();
    int* tf1 = (int*)malloc(sizeof(int)*deci);
    for (int i = 0; i < deci; i++)
    {
        tf1[i] = ask_int();
    }

    for (int i = 0; i < deci; i++)
    {   
        printf("\n[%d]\n",tf1[i]);
    }
    */

    //6)
    
    int deci = ask_intP();
    int* tf1 = (int*)malloc(sizeof(int)*deci);
    for (int i = 0; i < deci; i++)
    {
        tf1[i] = ask_intP();
        if (tf1[i] == -1)
            break;
    }

    for (int i = 0; i < deci; i++)
    {   
        printf("\n[%d]\n",tf1[i]);
        if (tf1[i] == -1)
            break;
    }


    return 0;
}

