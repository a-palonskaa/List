// insertion

// list_status_t list_t::insert_after(size_t anchor_index, elem_t elem) {
// 	list_status_t list_status = insert_data_validation(anchor_index);
// 	if (list_status != OK) {
// 		return list_status;
// 	}
//
// 	size_t next_free_cell = cells[free_cell].next;
// 	cells[free_cell].data = elem;
// 	size++;
//
// 	if (size == 1) {
// 		cells[1].prev = 0;
// 		cells[1].next = 0;
// 		head = 1;
// 		tail = 1;
// 		free_cell = next_free_cell;
// 		return OK;
// 	}
//
// 	if (anchor_index == 0) {
// 		cells[free_cell].next = tail;
// 		cells[free_cell].prev = 0;
// 		tail = free_cell;
// 		free_cell = next_free_cell;
// 		return OK;
// 	}
//
// 	if ((anchor_index == head) && (size > 1)) {
// 		cells[free_cell].prev = (ssize_t) anchor_index;
// 		cells[head].next = free_cell;
// 		cells[cells[head].next].next = 0;
// 		head = cells[head].next;
// 		free_cell = next_free_cell;
// 		return OK;
// 	}
// 	else {
// 		cells[free_cell].prev = (ssize_t) anchor_index;
// 		cells[free_cell].next = cells[anchor_index].next;
//
// 		cells[cells[anchor_index].next].prev = (ssize_t) free_cell;
// 		if (cells[anchor_index].next == 0) {
// 			cells[0].prev= 0;
// 		}
// 		cells[anchor_index].next = free_cell;
// 	}
//
// 	free_cell = next_free_cell;
// 	return OK;
// }

//============================================================================================

// deletion

// list_status_t list_t::delete_element(size_t anchor) {
// 	if (anchor > size) {
// 		LOG(ERROR, "Index %zu is greater than the size %zu of the list\n", anchor, size);
// 		return INDEX_EXCEED_LIST_SIZE;
// 	}
//
// 	if (size == 0) {
// 		LOG(ERROR, "List is empy, cannot delete an element\n", capacity);
// 		return LIST_IS_FULL;
// 	}
//
// 	if (anchor == 0) {
// 		LOG(ERROR, "Cannot delete a 0 element\n", capacity);
// 		return NULL_INDEX;
// 	}
//
// 	size--;
//
// 	if (anchor == head) {
// 		size_t first_elem = head;
// 		head = cells[first_elem].next;
// 		cells[head].prev = 0;
//
// 		cells[first_elem].prev = PREV_POISON_VALUE;
// 		cells[first_elem].data = -cells[first_elem].data;
//
// 		cells[first_elem].next = free_cell;
// 		free_cell = first_elem;
// 		return OK;
//
// 	}
//
// 	if (anchor == (size_t) cells[tail].prev) {
// 		cells[anchor].next = 0;
// 		cells[tail].data = -cells[tail].data;
// 		cells[tail].prev = PREV_POISON_VALUE;
//
// 		cells[tail].next = free_cell;
// 		free_cell = tail;
//
// 		tail = anchor;
// 		return OK;
// 	}
//
// 	if (anchor == tail) {
// 		cells[tail].data = -cells[tail].data;
// 		cells[cells[tail].prev].next = 0;
// 		cells[tail].prev = PREV_POISON_VALUE;
//
// 		cells[tail].next = free_cell;
// 		free_cell = tail;
// 		return OK;
// 	}
//
// 	size_t next_cell = cells[anchor].next;
// 	cells[next_cell].data = -cells[next_cell].data;
// 	cells[next_cell].prev = PREV_POISON_VALUE;
//
// 	cells[cells[next_cell].next].prev = cells[next_cell].prev;
// 	cells[anchor].next = cells[next_cell].next;
//
// 	cells[next_cell].next = free_cell;
// 	free_cell = next_cell;
// 	return OK;
// }
