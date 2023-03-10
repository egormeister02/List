#include <stdio.h>
#include <stdlib.h>

typedef int Elem_t;

struct Elem
{
    Elem_t data     = 0;
    int    next     = 0;
    int    prev     = 0;
};

struct List 
{
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
                                                // return  -2 if number of elems != capacity (unexpected error, list corrupted)
                                                // return  -5 if the List is destructed

Elem_t ListDelete(List*, size_t index);         // return "data" of deleted element if OK
                                                // return  -1 if try to delete "null" element
                                                // return  -2 if (index > capacity)
                                                // return  -3 if try to delete "empty" element 
                                                // return  -5 if the List is destructed

int ListInsert(List*, Elem_t, size_t);          // return index of inserted element if OK
                                                // return  -1 if could not reallocate memory
                                                // return  -2 if number of elems != capacity (unexpected error, list corrupted)
                                                // return  -3 if (index > capacity)
                                                // return  -4 if try to insert after "empty" element 
                                                // return  -5 if the List is destructed
                        
int ListLogic_number(List*, size_t index);      // return logical number of element at index "index" if OK (1 <= logic_number <= capacity)
                                                // return  -1 if try to search "null" element
                                                // return  -2 if (index > capacity)
                                                // return  -3 if try to search "empty" element 
                                                // return  -4 if element not found (unexpected error, list corrupted)
                                                // return  -5 if the List is destructed

int ListDtor(List*);                            // return   0 if OK
                                                // return  -1 if List* == NULL

int ListResize(List*);

void ListDump(List*);                           

FILE* StartLog(void);
