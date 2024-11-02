#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "intrusive_list.h"

typedef struct {
    char name[10];
    int index;
} human_t;

typedef struct {
    human_t human;
    list_t list;
} item_t;

void human_printer(list_t* anchor);

int main() {
    list_t head = {};
    _HEAD_INIT(&head);

    item_t first_item = {};
    memcpy(first_item.human.name,  &"Alina", 6);
    first_item.human.index = 0xC0FFEE;
    list_add_after(&first_item.list, &head);

    print_list(&head, human_printer);

    item_t second_item = {};
    memcpy(second_item.human.name,  &"Mark", 5);
    second_item.human.index = 0xAD2AAA;
    list_add_after(&second_item.list, &head);

    print_list(&head, human_printer);

    item_t new_item = {};
    memcpy(new_item.human.name,  &"Part", 5);
    new_item.human.index = 0xB81222;

    list_replace(&second_item.list, &new_item.list);
    print_list(&head, human_printer);
}

void human_printer(list_t* anchor) {
    printf("index is %s\n", (container_of(anchor, item_t, list))->human.name);
}
