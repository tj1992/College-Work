//  symbol.c

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "calc.h"

#define HASHSIZE	5		//value of hashsize

//------------------------------------------------------------------------------

enum{ VARIABLE, FUNCTION, OTHERS };		//types of identifier

/**
 * @struct 'Symbol'
 * @brief
 * Represents individual unit i.e VARIABLE, FUNCTION , OTHERS
 * Store type ,name and value or definition of function in Symbol
 */
typedef struct
{
	char type;			//type of identifier
	char name[MAX_ID];	//name of identifier

	union
    {
		double val;					//value contained
//		struct token_store *def;	//	definition of a function
//		double (*fptr)(double);		//	C library's in-built function
	};
} Symbol;

//-------------------------------------------------------------------


typedef struct node_
{
	Symbol sym;			//object of symbol
	struct node_ *next;	//a pointer pointing to next node
} sym_node;

sym_node *hash_table[HASHSIZE];		//	the symbol table

//--------------------------------------------------------------------------

/**
 * @fn hash
 * @brief
   first convert the string to integer and then decreses its range to HASHSIZE-1
   by using modulus function
   @param takes the name of identifier as argument and convert it integer
*/
int hash(char *key)		//key is the character that is input
{
	int index = 0;
	while (*key!='\0')	//read till end of key
		index += *key++;

	return index % HASHSIZE;//using modulus operation to limit the value of hash
}

//-----------------------------------------------------------------------------

/**
 * @fn htable_resolve
 * @brief
    solves the problem of hash clashes using chaining
 * @param
 *loc is the index in the hash table determined by the hash function
 *str is  the string
 */
sym_node* htable_resolve(int loc, char *str)
{
    sym_node *temp;
	temp = (*hash_table) + loc;	//temp storing the loc

     while(temp != NULL)	//traverses till hash table is not empty at loc
          temp = temp->next;

	if ((temp = (sym_node *)malloc(sizeof(sym_node))) == NULL)	//allocation of memory
    {
    	//	 exception handling
    	fprintf(stderr, "Memory  allocation failed!!");
    	return NULL;
    }

      strcpy(temp->sym.name, str);//copy of str
      temp->next->next = NULL;
      return temp;
}
//-----------------------------------------------------------------------------

/**
 * @fn	insert
 * Insert the value in the hash table.
 * if the hash table is not empty at that value it will resolve using htable_resolve
 *
 */
void htable_insert(char *str)
{
    int index;

	index = hash(str);

	if(hash_table[index] != NULL)
	{
	// collision occurs - resolve by chaining
		htable_resolve(index, str);
	}
	else
	{
		strcpy(hash_table[index]->sym.name, str);	//copy the data from str to hashtable
	}
}

//----------------------------------------------------------------------------

/**
 * @fn	search
 * @brief	*search for name str in symbol table.
 * first convert str into its hash value and then compare the names in
 * symbol table with str
 */
sym_node* search(char *str)
{
	sym_node *i;
	int index;

	if(hash_table[index = hash(str)] == NULL)
	{
		return NULL;
	}

	i = hash_table[index];
	while(i!=NULL)
    {
		if (!strcmp(i->sym.name,str))
			break;
		i=i->next;
	}
	return i;
}

//-------------------------------------------------------------------
//-------------------------------------------------------------------

#define FUNC_SIZE 5
typedef struct
{
	char name[MAX_ID];
	double (*fptr)(double);
} Function;

static Function func[FUNC_SIZE];
void init_func()
{
	strcpy(func[0].name, "sin");
	func[0].fptr = sin;

	strcpy(func[1].name, "cos");
	func[1].fptr = cos;

	strcpy(func[2].name, "tan");
	func[2].fptr = tan;

	strcpy(func[3].name, "exp");
	func[3].fptr = exp;

	strcpy(func[4].name, "sqrt");
	func[4].fptr = sqrt;
}

//-------------------------------------------------------------------------

int is_defined(char* str)
{
	int i;
	for (i = 0; i < FUNC_SIZE; ++i)
	{
		if (!strcmp(str, func[i].name))
			return i;
	}
	return -1;
}

double get_val(char* str, double d)
{
	int i;
	if ((i = is_defined(str)) >= 0)
	{
		if (i < 3)		//	for trigonometric functions
			return func[i].fptr(d*3.14159265/180.0);
		else
		return func[i].fptr(d);
	}
	else
	{
		fprintf(stderr, "Error : %s not defined!! Ignoring!!", str);
		return d;
	}
}

//-------------------------------------------------------------------
