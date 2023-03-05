#include "List.h"

int main()
{
    List list;

    ListCtor(&list, 10);

    ListDump(&list);

    ListAdd(&list, 10);
    ListAdd(&list, 20);
    ListAdd(&list, 30);

    ListDump(&list);

    ListDelete(&list, 2);

    ListDump(&list);

     ListAdd(&list, 40);

    ListDump(&list);

    ListInsert(&list, 60, 1);

    ListDump(&list);

    return 0;
}