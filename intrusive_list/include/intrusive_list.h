#ifndef INTRUSIVE_LIST_H
#define INTRUSIVE_LIST_H

typedef struct list {
    struct list* next;
    struct list* prev;
} list_t;

//=======================================================================================

#define NEW_HEAD(name) list_t name = {&(name), &(name)}

#define HEAD_INIT(head) {&(head), &(head)}

void _HEAD_INIT(list_t* head);

//=======================================================================================

#define offset_of(type, member)    \
    (size_t)&(((type*) 0)->member)

#define container_of(ptr, type, member)             \
    ((type*)((uintptr_t)(ptr) - offset_of(type, member)))

//=======================================================================================

void list_add_before(list_t* entry, list_t* anchor);
void list_add_after(list_t* entry, list_t* anchor);
void list_delete_at(list_t* anchor);
void list_replace(list_t* old_entry, list_t* new_entry);

//=======================================================================================

bool is_list_empty(list_t* head);

void print_list(list_t* head, void (*printer)(list_t* anchor));

#endif /* INTRUSIVE_LIST_H */
