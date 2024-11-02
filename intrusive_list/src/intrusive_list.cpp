#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include "intrusive_list.h"

// -------- NAMING moment -------------------------------
// __func_name - static func
// _name_func - here is a macros with similar fucn & name

static void inline __list_add(list_t* entry, list_t* prev, list_t* next);
static void inline __list_del(list_t* prev, list_t* next);

//=======================================================================================

void _HEAD_INIT(list_t* head) {
    assert(head != nullptr);

    head->next = head;
    head->prev = head;
}

//=======================================================================================

static void __list_add(list_t* entry, list_t* prev, list_t* next) {
    assert(entry != nullptr);
    assert(prev != nullptr);
    assert(next != nullptr);

    entry->next = next;
    entry->prev = prev;
    next->prev = entry;
    prev->next = entry;
}

static void __list_del(list_t* prev, list_t* next) {
    assert(prev != nullptr);
    assert(next != nullptr);

    prev->next = next;
    next->prev = prev;
}

void list_add_before(list_t* entry, list_t* anchor) {
    assert(entry != nullptr);
    assert(anchor != nullptr);

    __list_add(entry, anchor->prev, anchor);
}

void list_add_after(list_t* entry, list_t* anchor) {
    assert(entry != nullptr);
    assert(anchor != nullptr);

    __list_add(entry, anchor, anchor->next);
}

void list_delete_at(list_t* anchor) {
    assert(anchor != nullptr);

    __list_del(anchor->prev, anchor->next);
    anchor->prev = nullptr;
    anchor->next = nullptr;
}

void list_replace(list_t* old_entry, list_t* new_entry) {
    new_entry->next = old_entry->next;
    new_entry->prev = old_entry->prev;
    old_entry->prev->next = new_entry;
    old_entry->next->prev = new_entry;
}


//=======================================================================================

bool is_list_empty(list_t* head) {
    assert(head != nullptr);

    return head == head_next;
}

//=======================================================================================

void print_list(list_t* head, void (*printer) (list_t* anchor)) {
    assert(head != nullptr);

    list_t* iterator = head->next;
    while (iterator != head) {
        printer(iterator);
        iterator = iterator->next;
    }
}
