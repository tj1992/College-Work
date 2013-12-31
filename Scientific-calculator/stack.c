//  stack.c

#include <stdlib.h>		//	for malloc
#include <stdio.h>		//	for NULL
#include "calc.h"

//----------------------------------------------------------------------------

/**
 * @fn create: created a new token_sream to store token.
 * @param <Token>*tk: token to be stored in new token_stream.
 * @return <token_store>
 */
token_store *create(Token *tk)
{
	token_store *ts;
	//	allocate the memory
	if ((ts = (token_store*)malloc(sizeof(token_store))) == NULL)	//	check for exceptions
	{
		///	@todo handle exception
		fprintf(stderr, "Memory  allocation failed!!");
		return NULL;
	}
	//	everything is fine, intialize the members
	ts->T = *tk;
	ts->next = NULL;
	return ts;		//	return new <token_store>
}

//----------------------------------------------------------------------------

/**
 * @fn append : appends the token at the end of 'token_store'
 * @param <token_store**>start : takes pointer to the pinter to token_stream list,
 * 		  <Token*>t : <Token> to be added/inserted.
 * @return <token_store> created from <Token> or NULL if operation failed
 */
token_store* append(token_store **start, Token *t)
{
	token_store* tk;

	if ((tk = create(t)) == NULL)
	{
		///	@todo handle exception
		return NULL;
	}

	if (*start == NULL)
		*start = tk;
	else
	{
		token_store* temp;

		for (temp = (*start); temp->next != NULL; temp = temp->next)
			;
		temp->next = tk;
	}
	return tk;
}

//--------------------------------------------------------------------

/**
 * @fn push : push the token at the top of <token_store>
 * @param <token_store**> top : top node of <token_store>
 * 		  <Token*> tk : <Token> to be pushed
 * @return <token_store> created from <Token> or NULL if operation failed
 */
token_store* push(token_store **top, Token *t)
{
	token_store* tk;

	if ((tk = create(t)) == NULL)
	{
		///	@todo handle exception
		return NULL;
	}

	if (*top == NULL)
		*top = tk;
	else
	{
		tk->next = *top;
		*top = tk;
	}

	return tk;		//	return the newly created <token_store>
}

//--------------------------------------------------------------------

/**
 * @fn pop : pop out the top <Token> from <token_store>
 * @param  <token_store>**top : top node of <token_store>
 * @return <Token>
 */
Token pop(token_store **top)
{
	if (*top == NULL)
	{
		///	@todo raise exception here
		return bad_token();
	}
	else
	{
		Token p;
		token_store *temp;

		temp = *top;
		*top = (*top)->next;
		p = temp->T;
		free(temp);

		return p;
	}
}

//--------------------------------------------------------------------

/**
 * @fn stacktop : return the top <Token> from <token_store> without popping it
 * @param  <token_store>*top : top node of <token_store>
 * @return <Token>
 */
Token stacktop(token_store *top)
{
	if (top == NULL)	///	@attention handle errors
		return bad_token();
	else
		return top->T;
}

/**
 * @fn pop_all : deletes all the nodes of <token_store>
 * @param <token_store**> start : the first node
 */
void pop_all(token_store **start)
{
	while (*start != NULL)
		pop(start);
}
