#include <stdio.h>
#include <string.h>
#define SIZE 1000

typedef struct Metadata
{
    int size;
    struct Metadata *next;
    struct Metadata* prev;
}Metadata;

//declaring all global variables
char heap[SIZE+ sizeof(Metadata)];
char *flptr;


void InitializeMeta( Metadata *m, int size)
{
    m->size = size + sizeof(Metadata);
    m->next = m->prev = NULL;
}

void InitializeHeap()
{
    flptr = heap;
    Metadata *m;
    m = (Metadata *)heap;
    InitializeMeta(m, SIZE);
}


void *alloc(int s)
{
    void *ptr;//return ptr
    char *temp = flptr;
    //finding block to allocate memory from using first fit
    while( temp != NULL && ((Metadata *)temp)->size < s + sizeof(Metadata))
    {
       temp =(char *)((Metadata *)temp)->next;
    }
    if(temp == NULL)//heap memory empty 
    {
        printf("Memory cannot be allocated\n");
        ptr = NULL;
    }
    else
    {
        if (((Metadata *)temp)->size > s + sizeof(Metadata))
        {
            Metadata *m = (Metadata *)temp;
            Metadata *a = (Metadata *)(temp+(m->size -1)-(s + sizeof(Metadata)-1));
            InitializeMeta(a, s);
            m->size = m->size - (a->size);
            a++;
            ptr = (void *)a;
        } 
        else
        {
            Metadata *m = (Metadata *)temp;
            if( m->prev != NULL) (m->prev)->next = m->next;
            else
            {
                flptr = (char *)m->next;
            }
            if(m->next != NULL) (m->next)->prev = m->prev;  
            Metadata *a = (Metadata *)temp;
            InitializeMeta(a, s);
            a++;
            ptr = (void *)a;
            
        }
    }
    return ptr;

}

void my_free(void *ptr)
{
    ptr = ptr - sizeof(Metadata);
    Metadata *a_temp = (Metadata *)ptr;
    if(flptr == NULL) //No memory in heap
    {
        flptr = (char *)ptr;
        Metadata *m_temp = ptr;
        InitializeMeta(m_temp, a_temp->size-sizeof(Metadata));
    }
    else
    {
        Metadata *prev = NULL;
        Metadata *next = ((Metadata *)flptr);
        while(next != NULL && !((char *)prev < (char *)a_temp && (char *)next > (char *)a_temp )) //findind right spot to fit freed memory
        {
            prev = next;
            next = next->next;
        }
        if(next == NULL) //freed memory is at last
        {
            if(((char *)prev + prev->size) == (char *)a_temp) //merging with prev
            {
                prev->size = prev->size + a_temp->size;
            }
            else
            {
                Metadata *m_new = ptr;
                InitializeMeta(m_new, a_temp->size-sizeof(Metadata));
                m_new->prev = prev;
                prev->next = m_new;

            }
        }
        else
        {
            if(prev == NULL) //freed memory is first memory in heap
            {
                if((char *)a_temp + a_temp->size == (char *)next) //merging memory with next
                {
                    flptr = (char *)a_temp;
                    Metadata *m = (Metadata *)a_temp;
                    InitializeMeta(m, a_temp->size + next->size - sizeof(Metadata));
                    m->next = next->next;
                    if(next->next != NULL)next->next->prev = m;
                }
                else
                {
                    flptr = (char *)a_temp;
                    Metadata *m = (Metadata *)a_temp;
                    InitializeMeta(m, a_temp->size - sizeof(Metadata));
                    m->next = next;
                    next->prev = m;
                }
            }
            else
                if(((char *)prev + prev->size)== (char *)a_temp && (char *)a_temp + a_temp->size == (char *)next)
                {
                    prev->size = prev->size + a_temp->size + next->size;
                    prev->next = next->next;
                    if(next->next != NULL) next->next->prev = prev;
                }
                else if((char *)prev + prev->size == (char *)a_temp)
                {
                    prev->size = prev->size + a_temp->size;
                }
                else if( (char *)a_temp + a_temp->size == (char *)next)
                {
                    Metadata *m = (Metadata *)a_temp;
                    InitializeMeta(m, a_temp->size + next->size - sizeof(Metadata));
                    m->prev = prev;
                    prev->next = m;
                    m->next = next->next;
                    if(next->next != NULL)(next->next)->prev = m;
                }
                else
                {
                    Metadata *m = (Metadata *)a_temp;
                    InitializeMeta(m, a_temp->size - sizeof(Metadata));
                    prev->next = m;
                    m->prev = prev;
                    m->next = next;
                    next->prev = m;
                }
        }
    }
    ptr = NULL;
}


void PrintHeap()
{
    Metadata *m = (Metadata *)flptr;
    while(m != NULL)
    {
        printf("%d->", m->size);
        m = m->next;
    }
    printf("\n");

}
void main()
{
    InitializeHeap();
    int *a1 = alloc(sizeof(int));
    int *a2 = alloc(sizeof(int));
    int *a3 = alloc(sizeof(int));
    int *a4 = alloc(sizeof(int));
    PrintHeap();
    my_free(a1);
    PrintHeap();
    my_free(a3);
    PrintHeap();
    my_free(a4);
    PrintHeap();
}





