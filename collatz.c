#include <stdio.h>
#include <stdlib.h>
#include "ll.h"
int main(int argc, char *argv[])
{
    int x = atoi(argv[1]);
    int i = x;
    printf("Collatz Conjecture for %d: \n", i);
    node* list = NULL;
    while (i > 1)
    {
        insert(&list, i);
        if ((i % 2) == 0)        
        {
            i = i / 2;
        }    
        else
            i = (i * 3) + 1;
    }
    insert(&list, 1);
    display(&list);
}