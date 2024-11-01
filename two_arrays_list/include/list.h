#ifndef LIST_H
#define LIST_H

#include <stdio.h>

const ssize_t PREV_POISON_VALUE = -1;

typedef signed int elem_t;
#define ELEM_T_PRINTF "%d"

typedef signed int (*compare_t) (void* elem1, void* elem2);

typedef enum {
	OK                     = 0,
	MEM_ALLOCATON_ERROR    = 1,
	LIST_IS_FULL           = 2,
	INDEX_EXCEED_LIST_SIZE = 3,
	NULL_INDEX             = 4,
	INCORRECT_FREE_CELL    = 5,
	SIZE_EXCEED_CAPACITY   = 6,
	NULL_FREE_CELL         = 7,
} list_status_t;

typedef struct {
	elem_t data;
	size_t next;
	size_t prev;
} list_cell_t;

typedef enum {
    NO_ERRORS            = 0,
    FILE_OPEN_ERROR      = 1,
    IMAGE_CREATION_ERROR = 2,
} error_t;

class list_t {
public:
	list_t() = default;
    ~list_t() = default;

	list_status_t ctor();
	void dtor();
	void clear();

	ssize_t head();
	size_t tail();

	list_status_t verify();
	void set_dump_ostream(FILE* ostream);
	error_t dump();
	void print_list();

	list_status_t insert_after(size_t anchor, elem_t elem);
	list_status_t insert_before(size_t anchor, elem_t elem);
	list_status_t delete_at(size_t anchor);

	list_status_t push_front(elem_t elem);
	list_status_t push_back(elem_t elem);
	list_status_t pop_front();
	list_status_t pop_back();

	elem_t front();
	elem_t back();
	elem_t previos_elem(size_t pos);
	elem_t next_elem(size_t pos);

	size_t get_size();
	size_t is_empy();
	size_t max_size();

	size_t find_elem_by_value(elem_t elm, compare_t compare);
	size_t find_elem_by_index(size_t index);
private:
	void printf_graph_dot_file(FILE* graph_file);
	list_status_t insert_data_validation(size_t anchor);
	void initialize_cells();
	list_cell_t* cells;
	size_t free_cell;

	size_t size;
	size_t capacity;
};

#endif /* LIST_H */
