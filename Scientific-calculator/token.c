//  token.c

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "calc.h"

//--------------------------------------------------------------------

static char operators[] = { '+', '-', '*', '/', '%', '^' };		///	list of OPERATORs

/**
 * @fn is_operator : checks whether the passed character is an OPERATOR or not
 * @param <char>c : character to be tested
 * @return <int> index of OPERATOR(zero-positive) if @param is OPERATOR else negative
 */
int is_operator(char c)
{
	int i;
	for (i = 0; i < SIZEOF(operators); ++i)		//	traverse the operators list
		if (c == operators[i])		//	is it OPERATOR??
			return i;		//	yes
	return -1;		//	no
}

//--------------------------------------------------------------------

static char separators[] = { '{', '}', '[', ']', '(', ')' };		///	list of SEPARATORs
///	@attention opening SEPARATORs are positioned at even indexes (0,2,4,...) and closing ones are positioned
///	at odd indexes(1,3,5,...)

/**
 * @fn is_separator : SEPARATOR version of 'is_operator'; checks for SEPARATORs
 * @param <char>c : character to be tested
 * @return <int> index of SEPARATOR(zero-positive) if @param is SEPARATOR else negative
 */
int is_separator(char c)
{
	int i;
	for (i = 0; i < SIZEOF(separators); ++i)		//	traverse the separators list
		if (c == separators[i])		//	is it SEPARATOR??
			return i;		//	yes
	return -1;		//	no
}

//--------------------------------------------------------------------

/**
 * @fn token : creates <Token> according to 'type'
 * @param
 * <char> type : 'type' of <Token>
 * <char*> valbeg : pointer to beginning of 'value' of <Token>
 * <char*> valend : pointer to end of 'value' of <Token>
 * @note the 'value' string has to be extracted using the above pointers
 * @return <Token>
 */
Token token(char type, char* valbeg, char* valend)
{
	Token t;
	t.type = type;		//	set <Token> 'type'

	switch (type)		//	set data of <Token> according to 'type'
	{
		case NUMBER :
		{
			char* ptr = (char*)malloc(sizeof(char)*(int)(valend-valbeg+1));		//	get the required space
			strncpy(ptr, valbeg, (int)(valend-valbeg));		//	convert to a valid string
			ptr[valend-valbeg] = '\0';		//	add '\0'
			t.value = strtod(ptr, NULL);		//	save as <double>
			free(ptr);
			break;
		}

		case OPERATOR : case SEPARATOR :
			t.op = valbeg[0];		//	symbol of OPERATOR/SEPARATOR
			break;

		case IDENTIFIER :
		{
			int n = (int)(valend-valbeg) >= MAX_ID ? (MAX_ID-1) : (valend-valbeg);
			//	to avoid overflow only MAX_ID-1 (last for '\0') characters are allowed
			strncpy(t.name, valbeg, n);
			t.name[n] = '\0';
			break;
		}

		default :
		{
		/// @todo handle OTHER case here
			int n = (int)(valend-valbeg) >= MAX_ID ? (MAX_ID) : (valend-valbeg);
			strncpy(t.name, valbeg, n);
			t.name[n] = '\0';
			break;
		}
	}
	return t;		// return the <Token>
}

//--------------------------------------------------------------------

/**
 * @fn bad_token : creates a bad <Token>!! i.e. <Token> having illegal value
 * @return the bad <Token>
 */
Token bad_token()
{
	return token(OTHER, "BAD_TOKEN", "BAD_TOKEN"+ 9);	//	"BAD_TOKEN"+9 = last character
}

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
char* get_token(char* p, Token* t)
{
	static char last = '\0';		//	last valid <Token> character read by 'get_token'
	char flag = OTHER;		//	the type flag of <Token>
	char* var = p;		//	remember the intial positon

	if (*p == '\0')		//	'p' is empty
	{
		last = *p;
		return NULL;
	}

	//	find type of <Token>
	if (isalpha(*p))		//	alphabetic character ?? IDENTIFIER??
		flag = IDENTIFIER;

	else if (isdigit(*p) || *p == '.')		//	NUMBER??
		flag = NUMBER;

	else if (*p == '-')		// -ve NUMBER??
	{
	///	@attention most probable source of error
	///	@attention '-' must be part of 'operators' else things will go wrong from here
		if ((isdigit(*(p+1)) || *(p+1) == '.') && !isdigit(last))		//	'*(p+1)' & 'last' can be '\0'
		//	(operator/separator/identifier)-(digit)??
			flag = NUMBER;
		else		//	not a digit??
			flag = OPERATOR;		//	must be binary '-' OPERATOR
	}
	else if (is_operator(*p) >= 0)		//	an OPERATOR??
		flag = OPERATOR;
	else if (is_separator(*p) >= 0)		//	a SEPARATOR??
		flag = SEPARATOR;
	else		//	OTHER case
		;		//	flag is already OTHER, so no change
	///	@todo handle other cases here

	// loop for traversing 'p' until '\0' is reached
	while (*(++p) != '\0')
	{
		if (flag == IDENTIFIER)		//	last character was part of IDENTIFIER
		{
			if (!isalpha(*p))		//	stop reading if '*p' is not an IDENTIFIER
				break;
		}
		else if (flag == NUMBER)		//	last character was part of NUMBER
		{
			if (!isdigit(*p) && *p != '.')		//	stop reading if '*p' is not NUMBER
				break;
		}
		else		//	OPERATOR/SEPARATOR do not have more than one character (+, /, })
			break;
	}
	*t = token(flag, var, p);		//	create the <Token>
	last = *(p-1);		//	update last valid character
	return p;		//	return the updated position
}


//	end of file 'token.c'
//--------------------------------------------------------------------
