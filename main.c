/***********************************************************************
 *  File Name   : main.c
 *  Description : Entry point for the Arbitrary Precision Calculator (APC).
 *                Handles command-line parsing, input validation, 
 *                operator recognition, and calls the respective 
 *                arithmetic functions for computation using 
 *                doubly linked lists.
 *
 *                Functions:
 *                - main()
 ***********************************************************************/

#include "apc.h"
#include "list.h"

int main(int argc, char **argv)
{
	/* Declare doubly linked list pointers for operands and result */
	Dlist *head1 = NULL, *tail1 = NULL;   // Operand1
	Dlist *head2 = NULL, *tail2 = NULL;   // Operand2
	Dlist *headR = NULL, *tailR = NULL;   // Result

	// Check if minimum required arguments are passed
    if (argc < 2)
    {
        fprintf(stderr, "For help: %s --help\n", argv[0]);
        return 0;
    }
	/* Handle --help option */
	if (argc == 2 && (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0))
	{
		printf(
			"Arbitrary Precision Calculator (APC)\n"
			"-------------------------------------\n"
			"Usage:\n"
			"  ./a.out <operand1> <operator> <operand2>\n"
			"\n"
			"Operators:\n"
			"  +   Addition\n"
			"  -   Subtraction\n"
			"  *   Multiplication\n"
			"  /   Division\n"
			"  %%   Modulus\n"
			"  ^   Power\n"
			"\n"
			"Examples:\n"
			"  ./a.out 12345678901234567890 + 98765432109876543210\n"
			"  ./a.out 55555 * 99999\n"
			"\n");
		return SUCCESS; /* Exit after printing help */
	}

	/* Check for valid syntax: needs exactly 3 arguments */
	if (argc != 4)
	{
		fprintf(stderr, "❌ ERROR: Invalid Syntax :-( Try again...\n");
		return FAILURE;
	}

	/* Validate that both operands contain only digits */
	if (validate_input_args(argv) == FAILURE)
	{
		fprintf(stderr, "❌ Error: Invalid Operand :-( Try again...\n");
		return FAILURE;
	}

	/* Validate operator (must be a single character) */
	if (strlen(argv[2]) > 1)
	{
        printf("❌ Error: Invalid operator '%s'\nAllowed: + - x / %% ^\n", argv[2]);
		return FAILURE;
	}

	/* Convert input string digits into doubly linked lists */
	digit_to_list(&head1, &tail1, &head2, &tail2, argv);

	/* Perform operation based on operator */
	delete_leading_zero(&head1, &tail1);
	delete_leading_zero(&head2, &tail2);

	/* Extract operator */
	int sign_flag = 1;

	/* check operation type and also update sign flag based on operationa and operands */
	char operator = check_operation_type(argv, head1, head2, &sign_flag);
	int status = FAILURE;  // store operation result status
	int sub_flag = 1;          // used for subtraction sign handling

	switch (operator)
	{
		case '+':
			/* Perform addition */
			status = addition(&head1, &tail1, &head2, &tail2, &headR, &tailR);
			break;
		case '-':
			/* Perform subtraction (requires operand comparison for sign) */
			sub_flag = compare_list(head1, head2);
			if (sub_flag == 1) // Operand1 >= Operand2
				status = subtraction(&head1, &tail1, &head2, &tail2, &headR, &tailR);
			else if (sub_flag == -1) // Operand2 > Operand1 → result negative
				status = subtraction(&head2, &tail2, &head1, &tail1, &headR, &tailR);
			else
				status = SUCCESS;
			break;
		case 'x': // (you can also use '*' if implemented that way)
			/* Perform multiplication */
			status = multiplication(&head1, &tail1, &head2, &tail2, &headR, &tailR);
			break;
		case '/':
			/* Perform division */
			status = division(&head1, &tail1, &head2, &tail2, &headR, &tailR);
			break;
		case '%':
			/* Perform modulo */
			status = modulus(&head1, &tail1, &head2, &tail2, &headR, &tailR);
			break;
		case '^':
			/* Perform power */
			/* check for valid exponent */
			if(check_exponent(argv[3])  == FAILURE)
				break;
			status = power(&head1, &tail1, &head2, &tail2, &headR, &tailR);
			break;
		default:

			/* Invalid operator */
			printf("❌ ERROR: Invalid Input :-( Try again...\n");
			return -1;
	}

	if (status == FAILURE)
    {
        fprintf(stderr, "❌ ERROR: Operation failed.\n");
        return FAILURE;
    }

    /* ---------- OUTPUT SECTION ---------- */
    printf("------------------------------\n");
    printf("Input:    %s\n", argv[1]);
    printf("Operator: %c\n", argv[2][0]);
    printf("Input:    %s\n", argv[3]);
    printf("------------------------------\n");

    printf("Result:   ");
    if (sign_flag == -1)
        printf("-");
    print_list(headR);
    printf("\n");
    printf("------------------------------\n");

	/* Free memory for all lists */
	dl_delete_list(&head1, &tail1);
	dl_delete_list(&head2, &tail2);
	dl_delete_list(&headR, &tailR);
	return 0;
}