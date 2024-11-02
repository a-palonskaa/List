#include <string.h>
#include <errno.h>
#include <cstdlib>

#include "list.h"
#include "logger.h"

const size_t MAX_NAME_LEN = 40;
const size_t COMMAND_SIZE = 100;

const char* FILENAME = "graph";

static FILE** get_dump_ostream();

static FILE** get_dump_ostream() {
    static FILE* file = nullptr;
    return &file;
}

void list_t::set_dump_ostream(FILE* ostream) {
    *get_dump_ostream() = ostream;
}

bool list_t::close_dump_ostream() {
    return fclose(*get_dump_ostream()) != EOF;
}

error_t list_t::dump() {
    FILE* ostream = *get_dump_ostream();

    fprintf(ostream, "<pre>");
    fprintf(ostream, "[size]: %zu\n[capacity]: %zu\n[head]: %zu\n[tail]: %zu\n", size, capacity, head(), tail());
    static size_t graph_cnt = 0;

    char graph_filename[MAX_NAME_LEN] = {};
    char image_filename[MAX_NAME_LEN] = {};

    snprintf(graph_filename, MAX_NAME_LEN, "logs/images/%s%zu.dot", FILENAME, graph_cnt);
    snprintf(image_filename, MAX_NAME_LEN, "logs/images/%s%zu.png", FILENAME, graph_cnt);

    FILE* graph_file = fopen(graph_filename, "wb");
    if (graph_file == nullptr) {
        LOG(ERROR, "Failed to open an output file\n");
        return FILE_OPEN_ERROR;
    }

    printf_graph_dot_file(graph_file);

    if (fclose(graph_file) == EOF) {
        LOG(ERROR, "Failed to close a %s file\n" STRERROR(errno), graph_filename);
    }

    char command[COMMAND_SIZE] = "";

    snprintf(command, sizeof(command), "dot -Tpng %s -o %s", graph_filename, image_filename);

    if(system(command) != 0) {
        return IMAGE_CREATION_ERROR;
    }

    fprintf(ostream, "\n<img src = \"../%s\" width = 50%%>\n", image_filename);
    graph_cnt++;
    return NO_ERRORS;
}


void list_t::print_list() {
	size_t current_pos = (size_t) head();
	for (size_t i = 1; i <= size; i++) {
		printf("[%zu]: %5d\n", i, nodes[current_pos].data);
		current_pos = nodes[current_pos].next;
	}
}

list_status_t list_t::verify() {
    if (size >= capacity) {
        LOG(ERROR, "Size %zu exceeds capacity %zu\n", size, capacity);
        return SIZE_EXCEED_CAPACITY;
    }

    if (free_node== 0) {
        LOG(ERROR, "Free node is 0\n", size, capacity);
        return NULL_FREE_node;
    }

    size_t next_free_node = free_node;
    for (size_t i = size; i < capacity; i++) {
        if ((nodes[next_free_node].next == 0) ||
            (nodes[next_free_node].prev != PREV_POISON_VALUE)) {
            LOG(ERROR, "Free node previous value must be %zd: its prev is %zd, next is %zu\n",
                   PREV_POISON_VALUE, nodes[next_free_node].prev, nodes[next_free_node].next);
            return INCORRECT_FREE_node;
        }
        next_free_node = nodes[next_free_node].next;
    }
    return OK;
}
void list_t::printf_graph_dot_file(FILE* graph_file) {
    fprintf(graph_file, "digraph G {\n\t"
                        "rankdir=LR;\n\t"
                        "bgcolor=\"#DDA0DD\";\n\t"
                        "splines=true;\n\t"
                        "node [shape=box, width=1, height=0.5, style=filled, fillcolor=\"#7FFFD4\"];\n\t");

    fprintf(graph_file, "subgraph cluster_head_tail {\n\t"
                        "color=transparent;\n"
                        "style=invis;\n"
                        "label=\"\";\n"
                        "head [label=<<table border='0'>"
                        "<tr><td>head</td></tr></table>>];\n\t"
                        "tail [label=<<table border='0'>"
                        "<tr><td>tail</td></tr></table>>];\n\t"
                        "rank=same;\n"
                        "}\n");

    fprintf(graph_file, "head -> tail [weight=1000,color=\"#DDA0DD\"];\n\t");

    fprintf(graph_file, "subgraph cluster_nodes {\n\t"
                        "color=transparent;\n"
                        "rankdir=LR;\n\t"
                        "label=\"\";\n"
                        "rank=same;\n");

    size_t used_nodes_amount = get_used_size();

    for (size_t j = 0; j <= used_nodes_amount; j++) {
        fprintf(graph_file, "node%03zu [label=<<table border='0'>"
                            "<tr><td>%03zu</td></tr>"
                            "<tr><td bgcolor='black' height='1'></td></tr>"
                            "<tr><td>data = " ELEM_T_PRINTF "</td></tr>"
                            "<tr><td bgcolor='black' height='1'></td></tr>"
                            "<tr><td>next = %zu</td></tr>"
                            "<tr><td bgcolor='black' height='1'></td></tr>"
                            "<tr><td>prev = %zd</td></tr></table>>];\n\t"
                            "rank=same;\n",
                            j, j, nodes[j].data, nodes[j].next, nodes[j].prev);
    }

    for (size_t i = 0; i < used_nodes_amount; i++) {
        fprintf(graph_file, "node%03zu -> node%03zu [weight=1000,color=\"#DDA0DD\"];\n\t", i, i + 1);
        fprintf(graph_file, "node%03zu -> node%03zu;\n\t", i, i + 1);
    }

    fprintf(graph_file, "}\n");

    for (size_t i = 1, begin = tail(); i <= size; i++) {
        fprintf(graph_file, "node%03zu -> node%03zu [color=red];\n\t", begin, nodes[begin].next);
        begin = nodes[begin].next;
    }

    fprintf(graph_file, "head -> node%03zu [color=blue];\n\t", head());
    fprintf(graph_file, "tail -> node%03zu [color=blue];\n\t", tail());

    fprintf(graph_file, "node000 -> node000 [label=\"self-loop\" tailport=s headport=s];\n\t}");
}

