/**
 * Tony Givargis
 * Copyright (C), 2023
 * University of California, Irvine
 *
 * CS 238P - Operating Systems
 * lexer.c
 */

#include "lexer.h"

struct lexer
{
	uint64_t size;
	struct lexer_token *tokens;
};

static struct lexer_token *mktoken(struct lexer *lexer, enum lexer_token_op op)
{
	struct lexer_token *token, *tokens;
	size_t n;

	if (0 == (lexer->size % 100)) // memory is not enough
	{
		n = (lexer->size + 100) * sizeof(tokens[0]);
		if (!(tokens = realloc(lexer->tokens, n))) // can not realloc
		{
			TRACE("out of memory");
			return NULL;
		}
		lexer->tokens = tokens; // realloc new memory success
	}
	token = &lexer->tokens[lexer->size++]; // assign token a memory address
	memset(token, 0, sizeof(struct lexer_token));
	token->op = op;
	return token;
}

static int tokenize(struct lexer *lexer, const char *s)
{
	const char *const OPERATORS = "+-*/()";
	struct lexer_token *token;
	size_t i;
	char *e;

	while (*s) // while not end of string
	{
		for (i = 0; i < safe_strlen(OPERATORS); ++i) // find which op
		{
			if (OPERATORS[i] == (*s))
			{
				if (!mktoken(lexer, LEXER_OP_ADD + i)) // i=0: OPERATORS[0]=='+' -> LEXER_OP_ADD
				{
					TRACE(0);
					return -1;
				}
				break;
			}
		}
		if (safe_strlen(OPERATORS) > i)
		{
			++s;
		}
		else if (isspace(*s))
		{
			++s;
		}
		else // token is a number
		{
			if (!(token = mktoken(lexer, LEXER_OP_VAL))) // get the new token memory space
			{
				TRACE(0);
				return -1;
			}
			// convert the string pointed to by the argument str to a floating-point number (type double)
			// e is the pointer to the first character after the number in the string s
			/*  example:
				char str[] = "11.03e 0mn";
				char* end;
				double number;

				number = strtod(str, &end); // number = 11.03, end = "e 0mn"
			*/
			token->val = strtod(s, &e);

			if (s == e) // the string do not contain double
			{
				TRACE("lexer");
				return -1;
			}
			s = e; // set s point to the first character after the number
		}
	}
	return 0;
}

struct lexer *lexer_open(const char *s)
{
	struct lexer *lexer;

	assert(safe_strlen(s)); // if return 0 (length of string is 0), then it is false

	if (!(lexer = malloc(sizeof(struct lexer))))
	{
		TRACE("out of memory"); // help debug
		return NULL;
	}
	memset(lexer, 0, sizeof(struct lexer));
	if (tokenize(lexer, s))
	{
		lexer_close(lexer);
		TRACE(0);
		return NULL;
	}
	return lexer;
}

void lexer_close(struct lexer *lexer)
{
	if (lexer)
	{
		FREE(lexer->tokens);
		memset(lexer, 0, sizeof(struct lexer));
	}
	FREE(lexer);
}

uint64_t lexer_size(const struct lexer *lexer)
{
	assert(lexer);

	return lexer->size;
}

const struct lexer_token *lexer_lookup(const struct lexer *lexer, uint64_t i)
{
	assert(lexer);
	assert(i < lexer->size);

	return &lexer->tokens[i];
}
