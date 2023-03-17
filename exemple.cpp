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


    return 0;
}