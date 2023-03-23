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

    size_t* arr = ListPhysic_Logic_Array(&list);
    for (int i = 0; i <= list.namber_elem; i++)
    {
        printf("[%02d] = %lu\n", i, arr[i]);
    };


    return 0;
}