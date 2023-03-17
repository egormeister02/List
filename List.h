#include <stdio.h>
#include <stdlib.h>

 #define ASSERT_OK(list, BAD_FUNC)                      \
        if (ListCheck(list))                            \
        {                                               \
            list->status |= BAD_FUNC;                   \
            ListDump(list);                             \
            return -5;                                  \
        } 

#define StatPrint_(STATUS,text)         \
if (list->status & STATUS)              \
{                                       \
 fprintf(LogList, "\t"#text "\n");          \
}
                                

typedef int Elem_t;

enum ListStatus
{
    LIST_STATUS_OK          = 0 << 0,
    LIST_NULL_PTR           = 1 << 0,
    LIST_DATA_NULL_PTR      = 1 << 1,
    CAN_NOT_ALLOCATE_MEMORY = 1 << 2,
    LIST_IS_DESTRUCTED      = 1 << 3,
    BAD_RESIZE              = 1 << 4,
    BAD_CAPACITY            = 1 << 5,
    BAD_INSERT              = 1 << 6,
    BAD_DELETE              = 1 << 7,
    SIZE_MORETHAN_CAPACITY  = 1 << 8,
    TOO_LONG_TAIL           = 1 << 9,
    RUINED_TAIL             = 1 << 10,
    BAD_INDEX               = 1 << 11, 
    LIST_DAMEGED            = 1 << 12,
    BAD_ADD                 = 1 << 13,
    BAD_LOGIC               = 1 << 14
};

struct Elem
{    
    Elem_t      data     = 0;
    int         next     = 0;
    int         prev     = 0;
};

struct List 
{
    int         status   =    0;
    Elem*  buf           = NULL;
    int    tail          =    0;
    size_t namber_elem   =    0;
    size_t capacity      =    0;
};


const char NAME_LOG_FILE[] =   "LogList.log";
const int  LIST_DES_POISON =     -0xCFDEADFC;

int ListCtor(List*, size_t);                    // return   0 if OK
                                                // return  -1 if List* == NULL
                                                // return  -2 if could not allocate memory

int ListAdd(List*, Elem_t);                     // return index of added element if OK
                                                // return  -1 if could not reallocate memory
                                                // return  -2 if number of elems != capacity (unexpected error, list damaged)
                                                // return  -5 if the list is dameged, check LogFile

Elem_t ListDelete(List*, size_t index);         // return "data" of deleted element if OK
                                                // return  -1 if try to delete "null" element
                                                // return  -2 if (index > capacity)
                                                // return  -3 if try to delete "empty" element 
                                                // return  -5 if the list is dameged, check LogFile

int ListInsert(List*, Elem_t, size_t);          // return index of inserted element if OK
                                                // return  -1 if could not reallocate memory
                                                // return  -2 if number of elems != capacity (unexpected error, list damaged)
                                                // return  -3 if (index > capacity)
                                                // return  -4 if try to insert after "empty" element 
                                                // return  -5 if the list is dameged, check LogFile
                        
int ListLogic_number(List*, size_t index);      // return logical number of element at index "index" if OK (1 <= logic_number <= capacity)
                                                // return  -1 if try to search "null" element
                                                // return  -2 if (index > capacity)
                                                // return  -3 if try to search "empty" element 
                                                // return  -4 if element not found (unexpected error, list damaged)
                                                // return  -5 if the list is dameged, check LogFile

int ListDtor(List*);                            // return   0 if OK
                                                // return  -1 if List* == NULL

int ListResize(List*);                          // return   0 if OK
                                                // return  -1 if could not allocate memory
                                                // return  -2 if list->namber_elem != list->capacity (unexpected error, list damaged)
                                                // return  -5 if the list is dameged, check LogFile

int ListCheck(List*);

void ListDump(List*);                           

FILE* StartLog(void);

void MakePngName(char* name, char num);
