#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include "list.h"
#include "logger.h"

//============================================================================================

const size_t LIST_SIZE = 3;
const size_t CAPACITY_COEFF = 2;

//============================================================================================

list_status_t list_t::ctor() {
	dtor();

	nodes = (list_node_t*) calloc(sizeof(list_node_t), LIST_SIZE + 1);
	if (nodes == nullptr) {
		LOG(ERROR, "Memory allocation error\n" STRERROR(errno));
		return MEM_ALLOCATON_ERROR;
	}

	capacity = LIST_SIZE;
	size = 0;
	free_node = 1;

	initialize_nodes();
	return OK;
}

void list_t::dtor() {
	free(nodes);
	nodes = nullptr;

	capacity = 0;
	size = 0;
	free_node = 0;
}

void list_t::initialize_nodes() {
	nodes[0].next = 0;
	nodes[0].prev = 0;

	for (size_t i = 1; i < LIST_SIZE; i++) {
		nodes[i].next = i + 1;
		nodes[i].prev = PREV_POISON_VALUE;
	}
}

void list_t::clear() {
	initialize_nodes();

	for (size_t i = 1; i < size; i++) {
		nodes[i].data = -nodes[i].data;
	}
}

//=============================================================================================

bool list_t::expand_if_full() {
	return (size < capacity - 1) ? true : resize_up();
}

bool list_t::compress_if_underused() {
	return (get_used_size() + 1 > capacity / (CAPACITY_COEFF * CAPACITY_COEFF)) ? true : resize_down();
}

bool list_t::resize_down() {
	capacity /= (CAPACITY_COEFF * CAPACITY_COEFF);

	list_node_t* new_nodes = (list_node_t*) realloc(nodes, capacity * sizeof(list_node_t) + 1);
	if (new_nodes == nullptr) {
		LOG(ERROR, "Memory allocation error\n" STRERROR(errno));
		return false;
	}

	nodes = new_nodes;
	return true;
}

bool list_t::resize_up() {
	size_t prev_capacity = capacity;

	capacity *= CAPACITY_COEFF;

	list_node_t* new_nodes = (list_node_t*) realloc(nodes, capacity * sizeof(list_node_t) + 1);
	if (new_nodes == nullptr) {
		LOG(ERROR, "Memory allocation error\n" STRERROR(errno));
		return false;
	}
	nodes = new_nodes;

	nodes[free_node].next = prev_capacity;
	for (size_t i = prev_capacity; i < capacity; i++) {
		nodes[i].next = i + 1;
		nodes[i].prev = PREV_POISON_VALUE;
	}
	return true;
}

//=============================================================================================

list_status_t list_t::insert_after(size_t anchor, elem_t elem) {
	list_status_t list_status = insert_data_validation(anchor);
	if (list_status != OK) {
		return list_status;
	}

	size++;

	size_t current_free_node = free_node;
	free_node = nodes[free_node].next;

	nodes[current_free_node].data = elem;

	nodes[current_free_node].prev = (ssize_t) anchor;
	nodes[current_free_node].next = nodes[anchor].next;

	nodes[nodes[anchor].next].prev = (ssize_t) current_free_node;
	nodes[anchor].next = current_free_node;
	return OK;
}

list_status_t list_t::insert_before(size_t anchor, elem_t elem) {
	if (anchor == 0) {
		LOG(ERROR, "Cannot push before the 0 position\n", anchor, size);
		return INDEX_EXCEED_LIST_SIZE;
	}
	return insert_after((size_t) nodes[anchor].prev, elem);
}

list_status_t list_t::insert_data_validation(size_t anchor) {
	if (anchor > capacity) {
		LOG(ERROR, "Index %zu is greater than the size %zu of the list\n", anchor, size);
		return INDEX_EXCEED_LIST_SIZE;
	}

	if (expand_if_full() == false) {
		return MEM_ALLOCATON_ERROR;
	}

	return OK;
}

//---------------------------------------------------------------------------------------------

list_status_t list_t::delete_at(size_t anchor) {
	list_status_t list_status = delete_data_validation(anchor);
	if (list_status != OK) {
		return list_status;
	}

	nodes[nodes[anchor].next].prev = nodes[anchor].prev;
	nodes[nodes[anchor].prev].next = (size_t) nodes[anchor].next;

	nodes[anchor].data = -nodes[anchor].data;

	nodes[anchor].next = free_node;
	free_node = anchor;

	nodes[anchor].prev = PREV_POISON_VALUE;
	size--;

	return OK;
}

list_status_t list_t::delete_data_validation(size_t anchor) {
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

	compress_if_underused();
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
	return nodes[head()].data;
}

elem_t list_t::back() {
	return nodes[tail()].data;
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
	return nodes[nodes[pos].prev].data;
}

elem_t list_t::next_elem(size_t pos) {
	if (pos > size) {
		LOG(ERROR, "Position %zu is greater than the list size %zu\n", pos, size);
		return NULL;
	}
	return (pos == 0) ? nodes[head()].data : nodes[nodes[pos].next].data;
}

//=============================================================================================

ssize_t list_t::head() {
	return nodes[0].prev;
}

size_t list_t::tail() {
	return nodes[0].next;
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

size_t list_t::get_used_size() {
	size_t k = 0;
    size_t used_nodes_amount = 0;

    do {
        k = nodes[k].next;
        used_nodes_amount = (used_nodes_amount > k) ? used_nodes_amount : k;
    } while (k != 0);
	return used_nodes_amount;
}

//=============================================================================================

size_t list_t::find_elem_by_index(size_t index) {
	size_t current_pos = nodes[0].next;
	for (int i = 0; i < (int) index - 1; i++) {
		current_pos = nodes[current_pos].next;
	}
	return index ? current_pos : 0;
}

size_t list_t::find_elem_by_value(elem_t elm, compare_t compare) {
	for (size_t i = 1; i <= size; i++) {
		if (compare(&elm, &nodes[i].data) == 0) {
			return i;
		}
	}
	return 0;
}

