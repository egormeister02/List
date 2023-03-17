#include "List.h"

int main()
{
    List list;

    ListCtor(&list, 10);

    for (int i = 0; i < 15; i++)
    {  
        printf("%d\n", ListAdd(&list, 123));
    }

    ListDump(&list);
    
    ListDelete(&list, 4);
    ListDelete(&list, 7);
    ListInsert(&list, 43, 12);

    ListDump(&list);


    return 0;
}