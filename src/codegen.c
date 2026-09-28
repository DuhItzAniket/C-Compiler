#include "codegen.h"
#include "parser.h"
#include <stdio.h>
#include <string.h>

#define MAX_LOCAL_VARS 32
static int var_offset[MAX_LOCAL_VARS];
static int var_count = 0;

static void codegen_init_offsets(void) {
    for (int i = 0; i < MAX_LOCAL_VARS; i++) {
        var_offset[i] = -(i + 1) * 8;
    }
}

static int codegen_alloc_var_offset(void) {
    if (var_count < MAX_LOCAL_VARS) {
        return var_offset[var_count++];
    }
    return -8;
}

static void codegen_prologue(FILE *out) {
    fprintf(out, ".file \"compiler.c\"\n");
    fprintf(out, ".intel_syntax noprefix\n");
    fprintf(out, ".globl main\n");
    fprintf(out, "main:\n");
    fprintf(out, "push rbp\n");
    fprintf(out, "mov rbp, rsp\n");
    fprintf(out, "sub rsp, 128\n");
    codegen_init_offsets();
}

static void codegen_epilogue(FILE *out) {
    fprintf(out, "leave\n");
    fprintf(out, "ret\n");
}

static void codegen_emit_mov_to_mem(FILE *out, int offset) {
    if (offset >= 0)
        fprintf(out, "mov [rbp + %d], rax\n", offset);
    else
        fprintf(out, "mov [rbp - %d], rax\n", -offset);
}

static void codegen_emit_mov_from_mem(FILE *out, int offset) {
    if (offset >= 0)
        fprintf(out, "mov rax, [rbp + %d]\n", offset);
    else
        fprintf(out, "mov rax, [rbp - %d]\n", -offset);
}

static void codegen_emit_expr(FILE *out, ast_node_t *node);

static void codegen_emit_integer(FILE *out, ast_node_t *node) {
    fprintf(out, "mov rax, %d\n", node->ival);
}

static void codegen_emit_id(FILE *out, ast_node_t *node) {
    /* For simplicity, assume variable at known offset */
    /* In a full compiler, we'd look up the name in a symbol table */
    if (var_count > 0) {
        codegen_emit_mov_from_mem(out, var_offset[0]);
    }
}

static void codegen_emit_unary(FILE *out, ast_node_t *node) {
    codegen_emit_expr(out, node->children[0]);
    fprintf(out, "neg rax\n");
}

static void codegen_emit_binary(FILE *out, ast_node_t *node) {
    /* Emit left, save to stack, emit right, add */
    if (node->children[0])
        codegen_emit_expr(out, node->children[0]);

    int tmp = codegen_alloc_var_offset();
    codegen_emit_mov_to_mem(out, tmp);

    if (node->children[1])
        codegen_emit_expr(out, node->children[1]);

    fprintf(out, "add rax, [rbp%s]\n",
            tmp >= 0 ? " + " : " - ",
            tmp >= 0 ? tmp : -tmp);
}

static void codegen_emit_call(FILE *out, ast_node_t *node) {
    if (node->name) {
        fprintf(out, "call %s\n", node->name);
    }
}

static void codegen_emit_return(FILE *out, ast_node_t *node) {
    if (node->children[0]) {
        codegen_emit_expr(out, node->children[0]);
    }
    fprintf(out, "leave\n");
    fprintf(out, "ret\n");
}

static void codegen_traverse_program(FILE *out, ast_node_t *prog) {
    /* prog->children[0] is a linked list: each node's children[0] = next decl */
    ast_node_t *curr = prog->children[0];
    while (curr) {
        if (curr->type == AST_FUNC) {
            if (curr->children[0]) {
                codegen_emit_expr(out, curr->children[0]);
            }
        } else if (curr->type == AST_DECL) {
            if (curr->children[0]) {
                codegen_emit_expr(out, curr->children[0]);
                int off = codegen_alloc_var_offset();
                codegen_emit_mov_to_mem(out, off);
            }
        }
        curr = curr->children[0];
    }
}

void codegen_generate(ast_node_t *prog, const char *output_file) {
    FILE *out = fopen(output_file, "w");
    if (!out) return;

    codegen_prologue(out);

    if (prog) {
        codegen_traverse_program(out, prog);
    }

    codegen_epilogue(out);
    fclose(out);
}