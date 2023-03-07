#include "List.h"

int main()
{
    List list;

    ListCtor(&list, 10);

    ListDump(&list);

    for (int i = 0; i <12; i++)
    {
        
        printf("%d\n", ListAdd(&list, 123));
    }

    ListDump(&list);

    printf("%d\n", ListDelete(&list, 5));

    ListDump(&list);

    printf("%d\n", ListLogic_number(&list, 6));


    return 0;
}