/**
 * Tony Givargis
 * Copyright (C), 2023
 * University of California, Irvine
 *
 * CS 238P - Operating Systems
 * lexer.h -> string to array of tokens
 */

#ifndef _LEXER_H_
#define _LEXER_H_

#include "system.h"

struct lexer_token
{
    enum lexer_token_op
    {
        LEXER_OP_,      /* avoid 0 (just a decision can change) maybe for wrong input token */
        LEXER_OP_VAL,   /* value */
        LEXER_OP_ADD,   /* '+' */
        LEXER_OP_SUB,   /* '-' */
        LEXER_OP_MUL,   /* '*' */
        LEXER_OP_DIV,   /* '/' */
        LEXER_OP_OPEN,  /* '(' */
        LEXER_OP_CLOSE, /* ')' */
        // LEXER_OP_SIGM,  /* sigmoid 簡稱 s */
    } op;
    double val;
};

struct lexer;

struct lexer *lexer_open(const char *s); // put the input string into lexer

void lexer_close(struct lexer *lexer); // end of lexing and free the memory

uint64_t lexer_size(const struct lexer *lexer); // how many tokens

const struct lexer_token *lexer_lookup(const struct lexer *lexer, uint64_t i); // return the token at i

#endif /* _LEXER_H_ */

/**
string="1+2"

lexer{
    size: 3 (how many tokens)
    tokens: lexer_token{LEXER_OP_VAL, 1} -> lexer_token{LEXER_OP_ADD, nil} -> lexer_token{LEXER_OP_VAL, 2}
}

*/