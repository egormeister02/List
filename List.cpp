#include "List.h"

FILE* LogList = StartLog();

int ListCtor(List* list, size_t capacity)
{
    if (list == NULL) return -1;

    list->capacity = capacity;
    list->buf = (Elem*)calloc(list->capacity + 1, sizeof(Elem));

    if (list->buf == NULL) return -2;

    list->tail = 1;
    list->buf->next = 0;
    list->buf->prev = 0;

    for (int index = 1; index < (int)list->capacity; index++)
    {
        list->buf[index].next = index + 1;
        list->buf[index].prev =        -1;
    }

    list->buf[list->capacity].next = -1;
    list->buf[list->capacity].prev = -1;

    return 0; 
}

int ListAdd(List* list, Elem_t value)
{
    if (list->tail == -1) return -1;

    int last_elem = list->buf->prev;

    list->buf[last_elem].next  = list->tail;
    list->buf[list->tail].prev =  last_elem;

    last_elem  =                 list->tail;
    list->tail =  list->buf[last_elem].next;

    list->buf[last_elem].data =       value;
    list->buf[last_elem].next =           0;

    list->buf->prev = last_elem;
    
    list->size++;
    
    return last_elem;
}

Elem_t ListDelete(List* list, size_t index)
{
    if (index == 0)                     return -1;  
    if (index > list->capacity)         return -2;
    if (list->buf[index].prev == -1)    return -3;

    list->buf[list->buf[index].prev].next = list->buf[index].next;
    list->buf[list->buf[index].next].prev = list->buf[index].prev;

    list->buf[index].prev   =         -1;
    list->buf[index].next   = list->tail;
    list->tail              = (int)index;

    list->size--;

    return list->buf[index].data;
}

int ListInsert(List* list, Elem_t value, size_t index)
{
    if (list->tail == -1)               return -1;
    if (index > list->capacity)         return -2;
    if (list->buf[index].prev == -1)    return -3;

    int elem_index  =                 list->tail;
    list->tail      = list->buf[elem_index].next;

    list->buf[elem_index].next              =   list->buf[index].next;
    list->buf[list->buf[index].next].prev   =              elem_index;
    list->buf[index].next                   =              elem_index;
    list->buf[elem_index].prev              =              (int)index;
    list->buf[elem_index].data              =                   value;

    list->size++;

    return elem_index;

}

int ListDtor(List* list)
{
    if (list == NULL) return -1;
    
}

void ListDump(List* list)
{
    if (list == NULL) fputs("!!!__list == nullptr__!!!", LogList);

    fprintf(LogList, "\n----------------------------------ListDump----------------------------------\n\n");
    

    fprintf(LogList, "    data pointer         = %p\n", list->buf);    
    fprintf(LogList, "    size                 = %lu\n", list->size);
    fprintf(LogList, "    capacity             = %lu\n", list->capacity); 
    fprintf(LogList, "    tail                 = %d\n", list->tail);


    fprintf(LogList, "    ---------------------------------------\n\n");

    fputs("\t index | data | next | prev |\n", LogList);
    for (size_t index = 0; index <= list->capacity; index++)
    {
        fprintf(LogList, "\t[%-2lu] = | %-4d | %-4d | %-4d |", index, list->buf[index].data, 
                                         list->buf[index].next, list->buf[index].prev);
        fputs((int)index == list->tail ? "    <--tail\n": "\n", LogList);
    }

    fprintf(LogList, "\n---------------------------------------------------------------------------\n");

}

FILE* StartLog(void)
{
    FILE* logfile = fopen(NAME_LOG_FILE, "w");
    fprintf(logfile, "--------------------------------Start Logging--------------------------------\n");
    return logfile;
}
