//  post_in.c

#include "calc.h"
#include <stdio.h>
#include <math.h>

//--------------------------------------------------------------------

/**
 * @fn operate : operates the expression 't1' 'opr' 't2'
 * @param
 * <Token*> t1 : first operand
 * <Token*> t2 : second operand
 * <Token*> opr : operator
 * @return <Token> : result of 't1' 'opr' 't2'
 */
Token operate(Token* t1, Token* t2, Token* opr)
{
	if (t1->type == NUMBER && t2 == NULL)
	{
		Token ret;
		double d = get_val(opr->name, t1->value);
		ret.type = NUMBER;
		ret.value = d;
		return ret;
	}
	else if (t1->type == NUMBER && t2->type == NUMBER)	//	NUMBERs
	{
		double d;
		Token ret;			//	for @return

		switch (opr->op)	//	which operator??
		{
			case '+' :
				d = t1->value + t2->value;
				break;

			case '-' :
				d = t1->value - t2->value;
				break;

			case '*' :
				d = t1->value * t2->value;
				break;

			case '/' :
				if (t2->value == 0.0)
				{
					fprintf(stderr, "Divide by zero error");
					return bad_token();
				}
				d = t1->value / t2->value;
				break;

			case '%' :
				if (t2->value == 0.0)
				{
					fprintf(stderr, "Divide by zero error");
					return bad_token();
				}
				d = fmod(t1->value, t2->value);		//	remainder function
				break;

			case '^' :
				d = pow(t1->value, t2->value);
				break;

			default :
				fprintf(stderr, "Operator error!! Operator not defined!!");
				return bad_token();
		}
		ret.type = NUMBER;
		ret.value = d;		//	resulting value
		return ret;
	}
	else
	{
		fprintf(stderr, "Illegal operator \'");
		opr->type == OPERATOR ? fprintf(stderr, "%c\'", opr->op) : fprintf(stderr, "%s\'", opr->name);
		return bad_token();
	}
}		//	end of @fn 'operate'

//--------------------------------------------------------------------

/**
 * @fn post_in : converts the post_fix expression 'pf' to infix (evaluates it)
 * @param <token_store*> pf : the postfix expression in list form
 * @return <Token> the converted expression
 */
Token post_in(token_store *pf)
{
	token_store* stack;
	Token pop1, pop2;		//	some dummy variables
	Token dummy;

	stack = NULL;		//	initialise 'stack'
	for ( ; pf != NULL; pf = pf->next)				//	loop for reading 'pf'
	{
		switch (pf->T.type)		//	type of <Token>
		{
			case OPERATOR :
				//	pop out the top two <Token>
				if ((pop1 = pop(&stack)).type !=  OTHER
					&& (pop2 = pop(&stack)).type != OTHER)	///	@attention OTHER used as bad <Token> type
					if ((dummy = operate(&pop2, &pop1, &pf->T)).type != OTHER
						&& push(&stack, &dummy) != NULL)	//	pop2 OP pop1
						break;		//	success
				//	failure
				///	@attention handles exception here
				pop_all(&stack);
				return bad_token();

			case IDENTIFIER :
				
				if ((pop1 = pop(&stack)).type != OTHER)		///	@attention OTHER used as bad <Token> type
					if ((dummy = operate(&pop1, NULL, &pf->T)).type != OTHER
						&& push(&stack, &dummy) != NULL)
						break;		//	success
				//	failure
				///	@attention handles exception here
				pop_all(&stack);
				return bad_token();
				
			case NUMBER :
				if (push(&stack, &pf->T) != NULL)	//	push into the stack
					break;
				//	push failed!!
				///	@attention handles exception here
				pop_all(&stack);
				return bad_token();

			default :
				///	@attention handles exception here
				fprintf(stderr, "Bad Token in post_in!! Aborting!!");
				pop_all(&stack);
				return bad_token();
				break;
		}
	}
	//	perform a check for valid expression
	dummy = pop(&stack);
	if (pop(&stack).type != OTHER)
	{	///	@attention handles exception here
		pop_all(&stack);
		return bad_token();
	}
	return dummy;
}		//	end of @fn 'post_in'

//--------------------------------------------------------------------

/**
 * @fn prcd : checks the precedence of input <Token>s
 * @param	<Token> t1 : first <Token>
 * 			<Token> t2 : second <Token>
 * @brief 	checks the precedence of <Token> t1 and <Token> t2 and
 * @return	positive, if precedence of t1 is greater than t2
 * 			negative, if precedence of t1 is less than t2
 * 			zero, if input <Token> are of illegal types(not OPERATOR or SEPARATOR)
 */
int prcd(Token t1, Token t2)
{
	if (t1.type == OPERATOR && t2.type == OPERATOR)
	{
		if (t1.op == '^' && t2.op == '^')
			return -1;
		else if (is_operator(t1.op) >= is_operator(t2.op))	//	is_operator() returns precedence number
			return 1;
		else
			return -1;
	}
	else if ((t1.type == OPERATOR || t1.type == IDENTIFIER) && t2.type == SEPARATOR)
	{
		if (is_separator(t2.op) % 2)		//	closing SEPARATOR
			return 1;
		else		//	opening SEPARATOR
			return -1;
	}
	else if (t1.type == SEPARATOR)
	{
		if (is_separator(t1.op) % 2)		//	closing SEPARATOR
			return 0;		///	@attention error
		else	//	opening SEPARATOR
			return -1;
	}
	else if (t2.type == IDENTIFIER)
	{
		return -1;
	}
	else if (t1.type == IDENTIFIER)
	{
			return 1;
	}
	else
	{
		fprintf(stderr, "prcd : Illegal input!!");
		return 0;	///	@attention error
	}
}

//--------------------------------------------------------------------

/**
 * @fn inf_post : converts the input infix expression to postfix
 * @param <token_store*> inf : the infix expression
 * @return <token_store*> the postfix expression if infix expression was valid
 * and NULL if expression was valid
 */
token_store* inf_post(token_store* inf)
{
	token_store* eval;		//	the converted postfix expression
	token_store* stack;		//	stack for holding the OPERATORs and SEPARATORs
	Token dummy;

	eval = stack = NULL;

	for ( ; inf != NULL; inf = inf->next)	//	traverse the expression
	{
		switch (inf->T.type)
		{
			case NUMBER :
				if (append(&eval, &inf->T) != NULL)
					break;
				//	failure of 'push()'
				pop_all(&stack);
				pop_all(&eval);
				return NULL;		///	@attention handle errors

			case IDENTIFIER :
				if (is_defined(inf->T.name) < 0)	//	a function??
				{
					pop_all(&stack);
					pop_all(&eval);
					return NULL;
				}
				else
				{
				case OPERATOR : case SEPARATOR :	///	@attention
					while (stack != NULL && prcd(stacktop(stack), inf->T) > 0)
					{
						if ((dummy = pop(&stack)).type != OTHER && append(&eval, &dummy) != NULL)
							;
						else	///	@attention handle errors
						{
							pop_all(&stack);
							pop_all(&eval);
							return NULL;
						}
					}
					if (stack == NULL ||
						(inf->T.type != SEPARATOR || !(is_separator(inf->T.op) % 2)))	//	not a closing SEPARATOR
					{
						if (push(&stack, &inf->T) == NULL)
						{
							pop_all(&stack);
							pop_all(&eval);
							return NULL;		///	@attention handle errors
						}
					}
					else
						pop(&stack);
					break;
				}

			default :
				///	@attention handles exception here
				fprintf(stderr, "Bad Token in post_in!! Aborting!!");
				pop_all(&stack);
				pop_all(&eval);
				return NULL;
				break;
		}
	}
	while (stack != NULL)
	{
		if ((dummy = pop(&stack)).type != OTHER && append(&eval, &dummy) != NULL)
			;
		else	///	@attention handle errors
		{
			pop_all(&stack);
			pop_all(&eval);
			return NULL;
		}
	}
	return eval;
}
