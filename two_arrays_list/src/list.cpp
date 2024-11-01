#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "list.h"
#include "logger.h"

//=============================================================================================

const size_t LIST_SIZE = 100;

//============================================================================================

list_status_t list_t::ctor() {
	dtor();

	cells = (list_cell_t*) calloc(sizeof(list_cell_t), LIST_SIZE);
	if (cells == nullptr) {
		LOG(ERROR, "Memory allocation error\n" STRERROR(errno));
		return MEM_ALLOCATON_ERROR;
	}

	capacity = LIST_SIZE;
	size = 0;
	free_cell = 1;

	initialize_cells();
	return OK;
}

void list_t::dtor() {
	free(cells);
	cells = nullptr;

	capacity = 0;
	size = 0;
	free_cell = 0;

	if (close_dump_ostream() == false) {
		LOG(ERROR, "Failed to close a logger output file\n" STRERROR(errno));
	}
}

void list_t::initialize_cells() {
	cells[0].next = 0;
	cells[0].prev = 0;

	for (size_t i = 1; i < LIST_SIZE; i++) {
		cells[i].next = i + 1;
		cells[i].prev = PREV_POISON_VALUE;
	}
}

void list_t::clear() {
	initialize_cells();

	for (size_t i = 1; i < size; i++) {
		cells[i].data = -cells[i].data;
	}
}

//=============================================================================================

list_status_t list_t::insert_after(size_t anchor, elem_t elem) {
	list_status_t list_status = insert_data_validation(anchor);
	if (list_status != OK) {
		return list_status;
	}

	size_t current_free_cell = free_cell;
	free_cell = cells[free_cell].next;

	cells[current_free_cell].data = elem;

	cells[current_free_cell].prev = (ssize_t) anchor;
	cells[current_free_cell].next = cells[anchor].next;

	cells[cells[anchor].next].prev = (ssize_t) current_free_cell;
	cells[anchor].next = current_free_cell;

	size++;
	return OK;
}

list_status_t list_t::insert_before(size_t anchor, elem_t elem) {
	if (anchor == 0) {
		LOG(ERROR, "Cannot push before the 0 position\n", anchor, size);
		return INDEX_EXCEED_LIST_SIZE;
	}
	return insert_after((size_t) cells[anchor].prev, elem);
}

list_status_t list_t::insert_data_validation(size_t anchor) {
	if (anchor > capacity) {
		LOG(ERROR, "Index %zu is greater than the size %zu of the list\n", anchor, size);
		return INDEX_EXCEED_LIST_SIZE;
	}

	if (size == capacity - 1) {
		LOG(ERROR, "List is full(cpaacity is %zu), cannot push an element\n", capacity);
		return LIST_IS_FULL;
	}
	return OK;
}

//---------------------------------------------------------------------------------------------

list_status_t list_t::delete_at(size_t anchor) {
	if (anchor > capacity) {
		LOG(ERROR, "Index %zu is greater than the size %zu of the list\n", anchor, size);
		return INDEX_EXCEED_LIST_SIZE;
	}

	if (size == 0) {
		LOG(ERROR, "List is empy, cannot delete an element\n", capacity);
		return LIST_IS_FULL;
	}

	if (anchor == 0) {
		LOG(ERROR, "Cannot delete a 0 element\n", capacity);
		return NULL_INDEX;
	}

	cells[cells[anchor].next].prev = cells[anchor].prev;
	cells[cells[anchor].prev].next = (size_t) cells[anchor].next;

	cells[anchor].data = -cells[anchor].data;

	cells[anchor].next = free_cell;
	free_cell = anchor;

	cells[anchor].prev = PREV_POISON_VALUE;
	size--;

	return OK;
}

//=============================================================================================

list_status_t list_t::push_front(elem_t elem) {
	return insert_before((size_t) tail(), elem);
}

list_status_t list_t::push_back(elem_t elem) {
	return insert_after((size_t) head(), elem);
}

list_status_t list_t::pop_front() {
	return delete_at((size_t) head());
}

list_status_t list_t::pop_back() {
	return delete_at((size_t) tail());
}

//=============================================================================================

elem_t list_t::front() {
	return cells[head()].data;
}

elem_t list_t::back() {
	return cells[tail()].data;
}

elem_t list_t::previos_elem(size_t pos) {
	if (pos > size) {
		LOG(ERROR, "Position %zu is greater than the list size %zu\n", pos, size);
		return NULL;
	}
	if (pos <= 1) {
		LOG(ERROR, "Element %zu does not have a previous one\n", pos);
		return NULL;
	}
	return cells[cells[pos].prev].data;
}

elem_t list_t::next_elem(size_t pos) {
	if (pos > size) {
		LOG(ERROR, "Position %zu is greater than the list size %zu\n", pos, size);
		return NULL;
	}
	return (pos == 0) ? cells[head()].data : cells[cells[pos].next].data;
}

//=============================================================================================

ssize_t list_t::head() {
	return cells[0].prev;
}

size_t list_t::tail() {
	return cells[0].next;
}

//=============================================================================================

size_t list_t::get_size() {
	return size;
}

size_t list_t::is_empy() {
	return size > 0;
}

size_t list_t::max_size() {
	return capacity;
}

//=============================================================================================

size_t list_t::find_elem_by_index(size_t index) {
	size_t current_pos = cells[0].next;
	for (int i = 0; i < (int) index - 1; i++) {
		current_pos = cells[current_pos].next;
	}
	return index ? current_pos : 0;
}


size_t list_t::find_elem_by_value(elem_t elm, compare_t compare) {
	for (size_t i = 1; i <= size; i++) {
		if (compare(&elm, &cells[i].data) == 0) {
			return i;
		}
	}
	return 0;
}

