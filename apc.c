/***********************************************************************
 *  File Name   : apc.c
 *  Description : Utility source file for the Arbitrary Precision Calculator (APC).
 *                Contains implementations for input validation, 
 *                operand conversion into doubly linked lists, 
 *                operation type determination, and exponent validation.
 *
 *                Functions:
 *                - validate_input_args()
 *                - digit_to_list()
 *                - check_operation_type()
 *                - check_exponent()
 ***********************************************************************/

#include "apc.h"
#include "list.h"

/* 
 * Function: validate_input_args
 * -----------------------------
 * Validates whether the operands passed through argv are valid numbers.
 * Accepts optional '+' or '-' sign, but ensures the rest are digits.
 *
 * argv : command-line arguments
 *
 * returns:
 *   SUCCESS if both operands are valid numbers
 *   FAILURE otherwise
 */
int validate_input_args(char **argv)
{
	int i = 0;

	// Validate first operand (argv[1])
	if (argv[1][i] == '-' || argv[1][i] == '+')  // skip sign
		i++;
	while (argv[1][i])   // check remaining characters
	{
		if (isdigit(argv[1][i++]) == 0)  // not a digit
			return FAILURE;
	}

	i = 0;
	// Validate second operand (argv[3])
	if (argv[3][i] == '-' || argv[3][i] == '+')  // skip sign
		i++;
	while (argv[3][i])   // check remaining characters
	{
		if (isdigit(argv[3][i++]) == 0)  // not a digit
			return FAILURE;
	}
	return SUCCESS;
}

/* 
 * Function: digit_to_list
 * -----------------------
 * Converts string operands into doubly linked lists.
 * Each digit is stored as a separate node (MSB at head).
 *
 * head1, tail1 : pointers for first operand list
 * head2, tail2 : pointers for second operand list
 * argv         : command-line arguments containing operands
 */
void digit_to_list(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, char *argv[])
{
	int i = 0;

	/* Insert digits of operand1 */
	if (argv[1][i] == '-' || argv[1][0] == '+') // skip sign
		i++;
	while (argv[1][i])  
		dl_insert_last(head1, tail1, argv[1][i++] - '0'); // convert char → int

	i = 0;
	/* Insert digits of operand2 */
	if (argv[3][i] == '-' || argv[3][0] == '+') // skip sign
		i++;
	while (argv[3][i])  
		dl_insert_last(head2, tail2, argv[3][i++] - '0'); // convert char → int
}

/* 
 * Function: check_operation_type
 * ------------------------------
 * Determines the type of arithmetic operation (+, -, x, /, %)
 * and sets the correct result sign depending on operands.
 *
 * argv      : command-line arguments
 * head1     : head of first operand list
 * head2     : head of second operand list
 * sign_flag : pointer to store result sign (1 = positive, -1 = negative)
 *
 * returns:
 *   operation character ('+', '-', 'x', '/', '%')
 */
char check_operation_type(char **argv, Dlist *head1, Dlist *head2, int *sign_flag)
{
	// Case: Addition
	if (argv[2][0] == '+')
	{
		// (-a) + (-b) → negative result
		if (argv[1][0] == '-' && argv[3][0] == '-')
		{
			*sign_flag = -1;
			return '+';
		}
		// (-a) + b → subtraction (sign depends on larger number)
		else if (argv[1][0] == '-')
		{
			if (compare_list(head1, head2) > 0)
				*sign_flag = -1;
			return '-';
		}
		// a + (-b) → subtraction (sign depends on larger number)
		else if (argv[3][0] == '-')
		{
			if (compare_list(head1, head2) < 0)
				*sign_flag = -1;
			return '-';
		}
		// a + b → always positive
		else
			return '+';
	}

	// Case: Subtraction
	else if (argv[2][0] == '-')
	{
		// (-a) - (-b) → compare and decide sign
		if (argv[1][0] == '-' && argv[3][0] == '-')
		{
			if (compare_list(head1, head2) > 0)
				*sign_flag = -1;
			return '-';
		}
		// (-a) - b → always negative
		else if (argv[1][0] == '-')
		{
			*sign_flag = -1;
			return '+';
		}
		// a - (-b) → becomes addition
		else if (argv[3][0] == '-')
		{
			return '+';
		}
		// a - b → normal subtraction, but sign depends on comparison
		else
		{
			if (compare_list(head1, head2) < 0)
				*sign_flag = -1;
			return '-';
		}
	}

	// Case: Multiplication
	else if (argv[2][0] == 'x')
	{
		// If exactly one operand is negative → result is negative
		if ((argv[1][0] == '-' && argv[3][0] != '-') || (argv[1][0] != '-' && argv[3][0] == '-'))
			*sign_flag = -1;
		return 'x';
	}

	// Case: Division
	else if (argv[2][0] == '/')
	{
		// Same sign logic as multiplication
		if ((argv[1][0] == '-' && argv[3][0] != '-') || (argv[1][0] != '-' && argv[3][0] == '-'))
			*sign_flag = -1;
		return '/';
	}

	// Case: Modulus
	else if (argv[2][0] == '%')
	{
		// Modulus result sign depends only on dividend
		if (argv[1][0] == '-')
			*sign_flag = -1;
		return '%';
	}

	// Case: Power
	else if (argv[2][0] == '^')
	{
		// Modulus result sign depends only on dividend
		int len = strlen(argv[3]) - 1;
		if (argv[1][0] == '-' && argv[3][len] % 2 != 0)
			*sign_flag = -1;
		return '^';
	}

	// Default case → return as-is
	else
		return argv[2][0];
}

/*
 * Function: check_exponent
 * -----------------------
 * Checks whether the exponent provided is valid for computation.
 * 
 * Parameters:
 *  exp : string representation of the exponent
 *
 * Returns:
 *  SUCCESS if exponent is valid,
 *  FAILURE if exponent is negative or too large.
 *
 * Notes:
 *  - Negative exponents are not supported in this integer-only APC.
 *  - Exponents longer than 4 digits are considered too large for practical computation.
 */
int check_exponent(char * exp)
{
    // Check for negative exponent
	if(exp[0] == '-')
	{
		fprintf(stderr, "❌ ERROR: Negative exponent not supported !\n");
		return FAILURE;
	}
    // Check for exponent length > 4 digits (arbitrary limit for performance)
	if(strlen(exp) > 4)
	{
		fprintf(stderr, "❌ ERROR: Exponent too large to compute !\n");
		return FAILURE;
	}
	return SUCCESS; // exponent is valid
}