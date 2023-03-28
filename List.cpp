#include "List.h"

FILE* LogList = StartLog();

int ListCtor(List* list, size_t capacity)
{
    if (list == NULL) return -1;

    list->capacity = capacity;
    list->buf = (Elem*)calloc(list->capacity + 1, sizeof(Elem));

    if (list->buf == NULL) 
    {
        list->status |= CAN_NOT_ALLOCATE_MEMORY;
        ListDump(list);
        return -2;
    }

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

    list->number_elem = 0;

    return 0; 
}

int ListAdd(List* list, Elem_t value)
{
    ASSERT_OK(list, BAD_ADD);
        
    if (list->tail == -1) 
    {
        if (ListResizeUp(list)) return ListResizeUp(list);
    }

    int last_elem = list->buf->prev;

    list->buf[last_elem].next  = list->tail;
    list->buf[list->tail].prev =  last_elem;

    last_elem  =                 list->tail;
    list->tail =  list->buf[last_elem].next;

    list->buf[last_elem].data =       value;
    list->buf[last_elem].next =           0;

    list->buf->prev = last_elem;
    
    list->number_elem ++;
    
    return last_elem;
}

Elem_t ListDelete(List* list, size_t index)
{
    ASSERT_OK(list, BAD_DELETE);

    if (index == 0)                             return -1;  
    if (index > list->capacity)                 return -2;
    if (list->buf[index].prev == -1)            return -3;

    list->buf[list->buf[index].prev].next = list->buf[index].next;
    list->buf[list->buf[index].next].prev = list->buf[index].prev;

    list->buf[index].prev   =         -1;
    list->buf[index].next   = list->tail;
    list->tail              = (int)index;

    list->number_elem --;

    return list->buf[index].data;
}

int ListInsert(List* list, Elem_t value, size_t index)
{
    ASSERT_OK(list, BAD_INSERT);

    if (index > list->capacity)                 return -3;
    if (list->buf[index].prev == -1)            return -4;

    if (list->tail == -1) 
    {
        if (ListResizeUp(list)) return ListResizeUp(list);
    }

    int elem_index  =                 list->tail;
    list->tail      = list->buf[elem_index].next;

    list->buf[elem_index].next              =   list->buf[index].next;
    list->buf[list->buf[index].next].prev   =              elem_index;
    list->buf[index].next                   =              elem_index;
    list->buf[elem_index].prev              =              (int)index;
    list->buf[elem_index].data              =                   value;

    list->number_elem ++;

    return elem_index;

}

int ListLogic_number(List* list, size_t index)
{
    ASSERT_OK(list, BAD_LOGIC);

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

    if (search == 0) 
    {
        list->status |= LIST_DAMEGED;
        ListDump(list);
        return -4;
    }

    return logic_number;
}

int ListPhysic_number(List* list, size_t logic_num)
{
    ASSERT_OK(list, BAD_PHYSIC);

    if (logic_num > list->number_elem) return -1;

    size_t physic_num = 0;

    for (size_t counter = 0; counter < logic_num; counter++)
    {
        physic_num = (size_t)list->buf[physic_num].next;
    }

    return (int)physic_num;
}

size_t* ListPhysic_Logic_Array(List* list)
{
    if (ListCheck(list))
    {
        list->status |= BAD_ARRAY;
        ListDump(list);
        return (size_t*)LIST_POISON_PTR;
    }

    size_t* array = (size_t*)calloc(list->number_elem + 1, sizeof(size_t));
    if (array == NULL) return array;

    size_t phusic_num = 0;
    size_t number_elem = list->number_elem;

    for (size_t logic_num = 0; logic_num <= number_elem; logic_num++)
    {
        array[logic_num] = phusic_num;
        phusic_num = (size_t)list->buf[phusic_num].next;
    }

    return array;
}

int ListLineariz(List* list)
{
    ASSERT_OK(list, BAD_LINEAR);

    Elem_t* arr_value = (Elem_t*)calloc(list->number_elem + 1, sizeof(Elem_t));
    if (arr_value == NULL) return -1;

    size_t number_elem = list->number_elem;
    size_t physic_index = 0;

    for (size_t logic_num = 0; logic_num <= number_elem; logic_num++)
    {
        arr_value[logic_num] = list->buf[physic_index].data;
        physic_index = (size_t)list->buf[physic_index].next;
    }

    list->buf->data =     arr_value[0];
    list->buf->next =                1;
    list->buf->prev = (int)number_elem;

    for (size_t index = 1; index < number_elem; index++)
    {
        list->buf[index].data = arr_value[index];
        list->buf[index].next =   (int)index + 1;
        list->buf[index].prev =   (int)index - 1;
    }

    list->buf[number_elem].data =    arr_value[number_elem];
    list->buf[number_elem].next =                         0;
    list->buf[number_elem].prev =      (int)number_elem - 1;

    size_t capacity = list->capacity;

    for (size_t index = number_elem + 1; index < capacity; index++)
    {
        list->buf[index].next = (int)index + 1;
        list->buf[index].prev =             -1;
    }

    list->buf[capacity].next   =  -1;
    list->buf[capacity].prev   =  -1;

    list->tail = (int)number_elem + 1;

    return 0;
}

int ListResizeUp(List* list)
{
    ASSERT_OK(list, BAD_RESIZE_UP);

    if (list->number_elem != list->capacity) 
    {
        list->status |= LIST_DAMEGED;
        ListDump(list);
        return -2;
    }

    size_t new_capacity = list->capacity * 2;

    Elem* temp_buf = (Elem*)realloc(list->buf, (new_capacity + 1)*sizeof(Elem));
    if (temp_buf == NULL) 
    {
        list->status |= CAN_NOT_ALLOCATE_MEMORY | BAD_RESIZE_UP;
        ListDump(list);
        return -1;
    }

    list->buf = temp_buf;

    for (int index = (int)list->capacity + 1; index < (int)new_capacity; index++)
    {
        list->buf[index].next = index + 1;
        list->buf[index].prev =        -1;
    }

    list->buf[new_capacity].next   =  -1;
    list->buf[new_capacity].prev   =  -1;
    list->tail = (int)list->capacity + 1;

    list->capacity = new_capacity;

    return 0;
}

int ListResizeDown(List* list)
{
    ASSERT_OK(list, BAD_RESIZE_DOWN);

    if (list->number_elem == list->capacity) return -1;

    if (ListLineariz(list)) return ListLineariz(list);

    Elem* temp_buf = (Elem*)realloc(list->buf, (list->number_elem + 1)*sizeof(Elem));
    if (temp_buf == NULL) 
    {
        list->status |= CAN_NOT_ALLOCATE_MEMORY | BAD_RESIZE_DOWN;
        ListDump(list);
        return -2;
    }

    list->capacity = list->number_elem;
    list->tail = -1;
    list->buf = temp_buf;

    return 0;
}

int ListDtor(List* list)
{
    if (list == NULL) return -1;

    list->capacity = LIST_DES_POISON;
    list->number_elem = LIST_DES_POISON;
    list->tail = LIST_DES_POISON;

    free(list->buf);
    list->buf = NULL;

    return 0;
    
}

int ListCheck(List* list)
{
    if (list == NULL)           
        return LIST_NULL_PTR;

    if (list->capacity == LIST_DES_POISON    || 
        list->number_elem == LIST_DES_POISON || 
        list->tail == LIST_DES_POISON)  
    {
        list->status |= LIST_IS_DESTRUCTED;
        return LIST_IS_DESTRUCTED;
    }

    int status = LIST_STATUS_OK;

    if (list->capacity <= 0)
        status |= BAD_CAPACITY;

    if (list->number_elem > list->capacity)
        status |= SIZE_MORETHAN_CAPACITY;

    if (list->tail > (int)list->capacity)
        status |= TOO_LONG_TAIL;

    if (list->tail < 0 && list->tail != -1)
        status |= RUINED_TAIL;

    if (list->buf == NULL) 
        status |= LIST_DATA_NULL_PTR;
    
    list->status |= status;
    return status;
}

void ListDump(List* list)
{
    if (list == NULL) 
    {
        fputs("!!!__list == nullptr__!!!", LogList);
        return;
    }

    ListCheck(list);

    fprintf(LogList, "\n<pre>\n----------------------------------ListDump----------------------------------\n\n");

    fprintf(LogList, "-----------------------------------\n");
    if (list->status == 0)
        fprintf(LogList, "\t>>>List is OK\n");
    else 
    {   
        StatPrint_(LIST_DAMEGED,             >>>List is dameged!!!!!);
        StatPrint_(LIST_DATA_NULL_PTR,       >>>List is dameged!!!!!);
        StatPrint_(BAD_LOGIC,                >>>Logic_number operation troubles);
        StatPrint_(BAD_ADD,                  >>>Add operation troubles);
        StatPrint_(LIST_IS_DESTRUCTED,       >>>List is destructed);
        StatPrint_(BAD_RESIZE_UP,            >>>List has resize problem);
        StatPrint_(CAN_NOT_ALLOCATE_MEMORY,  >>>Allocate memory problems);
        StatPrint_(SIZE_MORETHAN_CAPACITY,   >>>List size more than capacity);
        StatPrint_(LIST_DATA_NULL_PTR,       >>>List data pointer is null);
        StatPrint_(BAD_CAPACITY,             >>>Incorrect list capacity);
        StatPrint_(BAD_INSERT,               >>>Insert operation troubles);
        StatPrint_(TOO_LONG_TAIL,            >>>Tail is more than capacity);
        StatPrint_(RUINED_TAIL,              >>>Ruined tail);
        StatPrint_(BAD_INDEX,                >>>Troubles in list index);
        StatPrint_(BAD_LOGIC,                >>>Troubles in list index);
        StatPrint_(BAD_PHYSIC,               >>>Troubles in list index);
        StatPrint_(BAD_ARRAY,                >>>Troubles in list index);
    }

    fprintf(LogList, "\n    data pointer         = %p\n", list->buf);    
    fprintf(LogList, "    number_elem          = %lu\n", list->number_elem);
    fprintf(LogList, "    capacity             = %lu\n", list->capacity); 
    fprintf(LogList, "    tail                 = %d\n", list->tail);


    fprintf(LogList, "    ---------------------------------------\n\n");

    if (list == NULL) 
    {
        fputs("!!!__list->buf == nullptr__!!!", LogList);
        return;
    }

    fputs("\t index | data | next | prev |\n", LogList);
    for (size_t index = 0; index <= list->capacity; index++)
    {
        fprintf(LogList, "\t[%-2lu] = | %-4d | %-4d | %-4d |", index, list->buf[index].data, 
                                         list->buf[index].next, list->buf[index].prev);
        fputs((int)index == list->tail ? "    <--tail\n": "\n", LogList);
    }

    fprintf(LogList, "\n---------------------------------------------------------------------------\n<pre>\n");

    FILE* DumpFile = fopen("dump.dot", "w+");

    fprintf(DumpFile, "digraph G{\n");
    
    fprintf(DumpFile, "node [color=black, shape=box, style=\"rounded, filled\"];\n");
    fprintf(DumpFile, "rankdir=LR;\n");


    fprintf(DumpFile, "tail  [fillcolor=\"#FFFEB6\", "
                     "label=\"TAIL = %d\"];\n",
                      list->tail);

 
    fprintf(DumpFile, "capacity [fillcolor=\"#FFFEB6\", "
                     "label=\"CAPACITY = %lu\"];\n",
                      list->capacity);

   fprintf(DumpFile, "size [fillcolor=\"#FFFEB6\", "
                     "label=\"NAMBER OF ELEM = %lu\"];\n",
                      list->number_elem);
    

    fprintf(DumpFile, "node [color=black, shape=record, style=\"rounded, filled\"];\n");
    fprintf(DumpFile, "\n");
    fprintf(DumpFile, "edge [style=invis, constraint=true];\n");

    for (size_t index = 0; index <= list->capacity; ++index)
    {
		if ((int)index == list->tail) 
			fprintf(DumpFile, "node%lu [fillcolor=\"#C1AED1\",", index);
		else if ((int)list->buf[index].prev == -1)
			fprintf(DumpFile, "node%lu [fillcolor=\"#98FF98\",", index);
		else
			fprintf(DumpFile, "node%lu [fillcolor=\"#FFB2D0\",", index); 

        fprintf(DumpFile, "label=\" <i> Node %lu | Val. %d | <n> Next %d | <p> Prev.", 
			    index, list->buf[index].data, list->buf[index].next);
		
        if (list->buf[index].prev == -1)
        {
            fprintf(DumpFile, " Free node \"];\n");
        }
        else
        {
            fprintf(DumpFile, " %d \"];\n", list->buf[index].prev);
        }
        
        if (index != 0) 
        {
           fprintf(DumpFile, "    node%lu -> node%lu ;\n", index - 1, index);
        }
    }

    fprintf(DumpFile, "\n    edge [style=solid, constraint=false];\n");

    for (size_t index = 0; index <= list->capacity; ++index)
    {
        if (list->buf[index].next != -1)
            fprintf(DumpFile, "node%lu: <n> -> node%d: <i> ;\n", index, list->buf[index].next);
        if (list->buf[index].prev != -1)
            fprintf(DumpFile, "node%lu: <p> -> node%d: <p> ;\n", index, list->buf[index].prev);
        fprintf(DumpFile, "\n");
    }

    fprintf(DumpFile, "\n edge [style=bold, constraint=false];\n");

    if (list->tail >= 0) fprintf(DumpFile, "tail     -> node%d; \n", list->tail );

    fprintf(DumpFile, "}\n");
    
    fclose(DumpFile);

    static char pngIndex    = 1;
    char dumpName[40] = "";
    MakePngName(dumpName, pngIndex);

    char buff[100] = "";
    sprintf(buff, "dot -Tsvg -o %s dump.dot", dumpName);
    system(buff);

    fprintf(LogList, "<img src = %s>\n", dumpName);
    ++pngIndex;


}

FILE* StartLog(void)
{
    FILE* logfile = fopen(NAME_LOG_FILE, "w");
    fprintf(logfile, "--------------------------------Start Logging--------------------------------\n");
    return logfile;
}

void MakePngName(char* name, char num)
{
	
	sprintf(name, "dot_files/dump%03d.svg", num);
} 