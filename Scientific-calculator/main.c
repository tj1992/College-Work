//  main.c

#include <stdio.h>
#include <ctype.h>
#include "calc.h"

#define MAXLEN 100		///	Maximum lengh of array ( @attention )
#define PROMPT "\n>"

/**
 * @fn Getline : reads a string from 'stdin'
 * @param
 * 	<char*>s : storage location for input string
 * 	<unsigned>lim : max length of input string
 * 	<char>term : input terminator
 * @return length of string
 */
int Getline(char *s, unsigned lim, char term)
{
	char ch;
	unsigned i = 0;

	//	read until EOF or 'term' is encountered
	while (i < lim-1 && (ch = getchar()) != EOF && ch != term)
	//	if (!isspace(ch))		//	skip all whitespaces
			s[i++] = ch;

	s[i] = '\0';
	return i;	//	length of 's' (without '\0')
}

//	the MAIN function
int main(int argc, char** argv)
{
	char line[MAXLEN], *ptr;
	int len;
	Token temp, res;
	token_store *start;

	ptr = line;		//	ptr points to the array
	len = 0;
	start = NULL;

	init_func();

	printf("\nEnter a mathematical expression : ");
	while (1)
	{
		printf(PROMPT);
		if ((len = Getline(ptr, MAXLEN, '\n')) > 0)		//	get the input
		{
			while ((ptr = get_token(ptr, &temp)) != NULL)
			{
				if (temp.type != OTHER)		//	is not space, tab, newline etc.
				{
					append(&start,&temp);
				}
			}
			ptr = line;		//	reset to 'line'
		}
		else		//	no input
			break;	//	exit

		res = post_in(inf_post(start));		//	evaluate
		if (res.type == OTHER)
			fprintf(stderr, "Bad token!!");
		else
			printf("Result : %g", res.value);

		pop_all(&start);	//	free the memory
	}

	//printf("\nAborted.");
	return 0;
}
