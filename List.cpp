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

    list->namber_elem = 0;

    return 0; 
}

int ListAdd(List* list, Elem_t value)
{
    if (list->capacity == LIST_DES_POISON    || 
        list->namber_elem == LIST_DES_POISON || 
        list->tail == LIST_DES_POISON)          return -5;
        
    if (list->tail == -1) 
    {
        if (ListResize(list)) return ListResize(list);
    }

    int last_elem = list->buf->prev;

    list->buf[last_elem].next  = list->tail;
    list->buf[list->tail].prev =  last_elem;

    last_elem  =                 list->tail;
    list->tail =  list->buf[last_elem].next;

    list->buf[last_elem].data =       value;
    list->buf[last_elem].next =           0;

    list->buf->prev = last_elem;
    
    list->namber_elem ++;
    
    return last_elem;
}

Elem_t ListDelete(List* list, size_t index)
{
    if (list->capacity == LIST_DES_POISON    || 
        list->namber_elem == LIST_DES_POISON || 
        list->tail == LIST_DES_POISON)          return -5;

    if (index == 0)                             return -1;  
    if (index > list->capacity)                 return -2;
    if (list->buf[index].prev == -1)            return -3;

    list->buf[list->buf[index].prev].next = list->buf[index].next;
    list->buf[list->buf[index].next].prev = list->buf[index].prev;

    list->buf[index].prev   =         -1;
    list->buf[index].next   = list->tail;
    list->tail              = (int)index;

    list->namber_elem --;

    return list->buf[index].data;
}

int ListInsert(List* list, Elem_t value, size_t index)
{
    if (list->capacity == LIST_DES_POISON    || 
        list->namber_elem == LIST_DES_POISON || 
        list->tail == LIST_DES_POISON)          return -5;

    if (index > list->capacity)                 return -3;
    if (list->buf[index].prev == -1)            return -4;

    if (list->tail == -1) 
    {
        if (ListResize(list)) return ListResize(list);
    }

    int elem_index  =                 list->tail;
    list->tail      = list->buf[elem_index].next;

    list->buf[elem_index].next              =   list->buf[index].next;
    list->buf[list->buf[index].next].prev   =              elem_index;
    list->buf[index].next                   =              elem_index;
    list->buf[elem_index].prev              =              (int)index;
    list->buf[elem_index].data              =                   value;

    list->namber_elem ++;

    return elem_index;

}

int ListLogic_number(List* list, size_t index)
{
    if (list->capacity == LIST_DES_POISON    || 
        list->namber_elem == LIST_DES_POISON || 
        list->tail == LIST_DES_POISON)          return -5;

    if (index == 0)                             return -1;  
    if (index > list->capacity)                 return -2;
    if (list->buf[index].prev == -1)            return -3;

    int search = 0;
    int logic_number = 0;

    do
    {
        search = list->buf[search].next;
        logic_number ++;
    } while (search != (int)index && search != 0);

    if (search == 0) return -4;

    return logic_number;
}

int ListResize(List* list)
{
    if (list->namber_elem != list->capacity) return -2;

    size_t new_capacity = list->capacity * 2;

    Elem* temp_buf = (Elem*)realloc(list->buf, (new_capacity + 1)*sizeof(Elem));
    if (temp_buf == NULL) return -1;

    list->buf = temp_buf;

    for (int index = (int)list->capacity + 1; index < (int)new_capacity; index++)
    {
        list->buf[index].next = index + 1;
        list->buf[index].prev =        -1;
    }

    list->buf[new_capacity].next = -1;
    list->buf[new_capacity].prev = -1;
    list->tail  =  list->capacity + 1;

    list->capacity = new_capacity;

    return 0;
}

int ListDtor(List* list)
{
    if (list == NULL) return -1;

    list->capacity = LIST_DES_POISON;
    list->namber_elem = LIST_DES_POISON;
    list->tail = LIST_DES_POISON;

    free(list->buf);
    list->buf = NULL;

    return 0;
    
}

void ListDump(List* list)
{
    if (list == NULL) fputs("!!!__list == nullptr__!!!", LogList);

    fprintf(LogList, "\n----------------------------------ListDump----------------------------------\n\n");
    

    fprintf(LogList, "    data pointer         = %p\n", list->buf);    
    fprintf(LogList, "    namber_elem                = %lu\n", list->namber_elem);
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

