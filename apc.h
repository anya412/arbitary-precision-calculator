/***********************************************************************
 *  File Name   : apc.h
 *  Description : Header file for the Arbitrary Precision Calculator (APC).
 *                Contains data structure definitions, macro constants,
 *                and function prototypes for performing arithmetic
 *                operations (+, -, *, /, %, ^) on large numbers
 *                using doubly linked lists.
 *
 *                Functions:
 *                - validate_input_args()
 *                - digit_to_list()
 *                - addition()
 *                - subtraction()
 *                - multiplication()
 *                - division()
 *                - modulus()
 *                - power()
 *                - print_list()
 *                - check_operation_type()
 *                - check_exponent()
 ***********************************************************************/

#ifndef APC_H
#define APC_H

#define SUCCESS 0
#define FAILURE -1

#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>

typedef struct node
{
	struct node *prev;
	int data;
	struct node *next;
} Dlist;

/* Include the prototypes here */

/* Input Validation*/
int validate_input_args(char **argv);

/* Store the operands into the list */
void digit_to_list(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, char *argv[]);

/* Addition */
int addition(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR);

/* Subtraction */
int subtraction(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR);

/* Multiplication */
int multiplication(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR);

/* Division */
int division(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR);

/* Modulus */
int modulus(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR);

/* power */
int power(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR);

/* Print List */
void print_list(Dlist *head);

/* to check operation type and sign flag */
char check_operation_type(char ** argv, Dlist *head1, Dlist *head2, int * sign_flag);

/* To check valid exponent for power operation */
int check_exponent(char * exp);

#endif