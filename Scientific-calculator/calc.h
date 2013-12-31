//  calc.h

#ifndef CALC_H
#define CALC_H

#define SIZEOF(n) sizeof(n)/sizeof(n[0])		///	array version of 'sizeof'

#define MAX_ID 10		///	Maximum length of any IDENTIFIER @attention

enum { NUMBER, IDENTIFIER, OPERATOR, SEPARATOR, OTHER };		///	types of 'Token'

/**
 * @struct 'Token'
 * @brief
 * Represents individual unit i.e NUMBER, IDENTIFIER, OPERATOR, SEPARATOR, OTHER(else)
 * 'Token' can have different data according to its type (uses 'union')
 */
typedef struct
{
	char type;		//	type of 'Token'
	union		//	data according to 'type'
	{
		double value;		//	value of 'Token' (for NUMBERs)
		char op;		//	symbol of 'Token' (for OPERATORs and SEPARATORs)
		char name[MAX_ID];		//	name of 'Token' (for IDENTIFIERs)
	};
} Token;

//--------------------------------------------------------------------

/**
 * @fn get_token : get a <Token> by traversing string 'p'
 * @brief
 * 'p' is traversed until either a <Token> is found or '\0' is encountered
 * the immediate next postion to the <Token> is returned
 * @param
 * <char*>p : source string expected to contain <Token>s
 * <Token*>t : pass-by-reference variable for storing the extracted <Token>
 * @return <char*> updated postion of 'p' or NULL if 'p' is empty ("\0")
 */
char* get_token(char*, Token*);

//--------------------------------------------------------------------

/**
 * @fn bad_token : creates a bad <Token> i.e. <Token> having OTHER value
 * @return the bad <Token>
 */
Token bad_token();

//--------------------------------------------------------------------

/**
 * @fn is_operator : checks whether the passed character is an OPERATOR or not
 * @param <char>c : character to be tested
 * @return <int> index of OPERATOR(zero-positive) if @param is OPERATOR else negative
 */
int is_operator(char c);

//--------------------------------------------------------------------

/**
 * @fn is_separator : SEPARATOR version of 'is_operator'; checks for SEPARATORs
 * @param <char>c : character to be tested
 * @return <int> index of SEPARATOR(zero-positive) if @param is SEPARATOR else negative
 */
int is_separator(char c);

//----------------------------------------------------------------------------

/**
 * @struct 'token_store'
 * @brief
 * 'token_store' acts like a stream of token
 * Creates a linked list of Token defined above.
 */
typedef struct t_s
{
	Token T;	// token created above
	struct t_s *next;
} token_store;

//----------------------------------------------------------------------------

/**
 * @fn create: created a new token_sream to store token.
 * @param <Token>*tk: token to be stored in new token_stream.
 * @return <token_store>
 */
token_store *create(Token *tk);

//----------------------------------------------------------------------------

/**
 * @fn append : appends the token at the end of token_stream
 * @param <token_store>**start : takes pointer to the pinter to token_stream list,
 * 		  <Token>*tk : <Token> to be added/inserted.
 * @return <token_store> created from <Token> or NULL if operation failed
 */
token_store* append(token_store **start, Token *tk);

//--------------------------------------------------------------------

/**
 * @fn pop : deletes token_stream from the front of the list.
 * @param  <token_store>**start : takes pointer to the pinter to token_stream list.
 * @return <Token> or bad_token() if operation failed
 */
Token pop(token_store **start);

//--------------------------------------------------------------------

/**
 * @fn stacktop : return the top <Token> from <token_store> without popping it
 * @param  <token_store>*top : top node of <token_store>
 * @return <Token>
 */
Token stacktop(token_store *top);

//--------------------------------------------------------------------

/**
 * @fn pop_all : deletes all the nodes of <token_store>
 * @param <token_store**> start : the first node
 */
void pop_all(token_store **start);

//--------------------------------------------------------------------

/**
 * @fn push : push the token at the top of <token_store>
 * @param <token_store**> top : top node of <token_store>
 * 		  <Token*> tk : <Token> to be pushed
 * @return <token_store> created from <Token> or NULL if operation failed
 */
token_store* push(token_store **top, Token *t);

//--------------------------------------------------------------------

/**
 * @fn post_in : converts the post_fix expression 'pf' to infix (evaluates it)
 * @param <token_store*> pf : the postfix expression in list form
 * @return <Token> the converted expression
 */
///	@attention @todo convert the return type to <token_store> to facilitate symbols and functions
Token post_in(token_store *pf);

//--------------------------------------------------------------------

/**
 * @fn inf_post : converts the input infix expression to postfix
 * @param <token_store*> inf : the infix expression
 * @return <token_store*> the postfix expression if infix expression was valid
 * and NULL if expression was valid
 */
token_store* inf_post(token_store* inf);

//--------------------------------------------------------------------

int is_defined(char* str);

double get_val(char* str, double d);

void init_func();
#endif
//	EOF
