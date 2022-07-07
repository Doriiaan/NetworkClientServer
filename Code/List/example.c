#include <stdio.h>
#include "list.h"



int main(int argc, char const *argv[])
{
    List *l = list_create();


    char elem[20] = "Hello ";
    l = list_push_back(l, elem);
    char elem2[20] = "à tous !\n";
    l = list_push_back(l, elem2);
    char elem3[30] = "Comment allez vous ?\n";
    l = list_push_back(l, elem3);

    for (int i = 0; i < 3; i++)
    {
        printf("%s", (char*)list_front(l));
        list_pop_front(l);
    }

    list_delete(&l);

    return 0;
}
