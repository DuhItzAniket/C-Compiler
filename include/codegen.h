#ifndef CODEGEN_H
#define CODEGEN_H

#include "parser.h"

void codegen_emit(ast_node_t *node);
void codegen_generate(ast_node_t *prog, const char *output_file);

#endif