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
    fprintf(ostream, "[size]: %zu\n\n", size);
    static size_t graph_cnt = 0;

    char graph_filename[MAX_NAME_LEN] = {};
    char image_filename[MAX_NAME_LEN] = {};

    snprintf(graph_filename, MAX_NAME_LEN, "logs/images2/%s%zu.dot", FILENAME, graph_cnt);
    snprintf(image_filename, MAX_NAME_LEN, "logs/images2/%s%zu.png", FILENAME, graph_cnt);

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

void list_t::printf_graph_dot_file(FILE* graph_file) {
    fprintf(graph_file, "digraph G {\n\t"
                        "rankdir=LR;\n\t"
                        "bgcolor=\"#20B2AA\";\n\t"
                        "splines=true;\n\t"
                        "node [shape=box, width=1, height=0.5, style=filled, fillcolor=\"#7B68EE\"];\n\t");

    fprintf(graph_file, "subgraph cluster_head_tail {\n\t"
                        "color=transparent;\n"
                        "style=invis;\n"
                        "label=\"\";\n"
                        "head [label=<<table border='0' cellspacing='0' cellpadding='1'>"
                        "<tr><td>head</td></tr></table>>];\n\t"
                        "tail [label=<<table border='0' cellspacing='0' cellpadding='1'>"
                        "<tr><td>tail</td></tr></table>>];\n\t"
                        "rank=same;\n"
                        "}\n");

    fprintf(graph_file, "head -> tail [weight=1000,color=\"#20B2AA\"];\n\t");

    fprintf(graph_file, "subgraph cluster_nodes {\n\t"
                        "color=transparent;\n"
                        "rankdir=LR;\n\t"
                        "label=\"\";\n"
                        "rank=same;\n");

    node_t* current_node = tail;
    for (size_t j = 0; (j < size) && (current_node != nullptr); j++) {
        fprintf(graph_file, "node%03zu [label=<<table border='0' cellspacing='0' cellpadding='1'>"
                            "<tr><td>%03zu</td></tr>"
                            "<tr><td bgcolor='black' height='1'></td></tr>"
                            "<tr><td>data = " ELEM_T_PRINTF "</td></tr>"
                            "<tr><td bgcolor='black' height='1'></td></tr>"
                            "<tr><td>next = %p</td></tr>"
                            "<tr><td bgcolor='black' height='1'></td></tr>"
                            "<tr><td>prev = %p</td></tr></table>>];\n\t"
                            "rank=same;\n",
                            j, j, current_node->data, current_node->next, current_node->prev);
        current_node = current_node->next;
    }

    for (size_t i = 0; i < size - 1; i++) {
        fprintf(graph_file, "node%03zu -> node%03zu [weight=1000,color=\"#20B2AA\"];\n\t", i, i + 1);
        fprintf(graph_file, "node%03zu -> node%03zu;\n\t", i, i + 1);
    }

    fprintf(graph_file, "}\n");

    for (size_t i = 0; i < size - 1; i++) {
        fprintf(graph_file, "node%03zu -> node%03zu [color=red];\n\t",  i, i + 1);
        fprintf(graph_file, "node%03zu -> node%03zu [color=blue];\n\t", i + 1, i);
    }

    fprintf(graph_file, "\n\t}");
}
