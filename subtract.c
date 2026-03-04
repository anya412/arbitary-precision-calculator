/***********************************************************************
 *  File Name   : subtract.c
 *  Description : Source file for subtraction operation in the
 *                Arbitrary Precision Calculator (APC).
 *                Implements subtraction of two large integers
 *                stored as doubly linked lists.
 *
 *                Functions:
 *                - subtraction()
 *
 *  Notes       :
 *  - Digits are subtracted from least significant to most significant.
 *  - Borrowing is handled explicitly when the minuend digit < subtrahend digit.
 *  - Leading zeros in the result are removed.
 *  - Works even if numbers are of different lengths.
 ***********************************************************************/

#include "apc.h"
#include "list.h"

/* 
 * Function: subtraction
 * ---------------------
 * Performs subtraction of two numbers stored in doubly linked lists.
 *
 * Parameters:
 *   head1, tail1 → doubly linked list for Operand1 (minuend)
 *   head2, tail2 → doubly linked list for Operand2 (subtrahend)
 *   headR, tailR → doubly linked list to store the Result
 *
 * Returns:
 *   SUCCESS on successful subtraction
 *   FAILURE if input lists are NULL
 * 
 */
int subtraction(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR)
{
	/* Check if any operand list is NULL → invalid input */
	if (*head1 == NULL)
	{
		copy_list(headR, tailR, *head2);
		return SUCCESS;
	}
	if(*head2 == NULL)
	{
		copy_list(headR, tailR, *head1);
		return SUCCESS;
	}
	/* Start subtraction from the last digits (tails) */
	Dlist *temp1 = *tail1, *temp2 = *tail2;
	int borrow = 0;  // flag to manage borrowing during subtraction

	/* Loop until both lists are fully traversed */
	while (temp1 || temp2)
	{
		/* Extract digits (0 if one list is shorter) */
		int digit1 = temp1 ? temp1->data : 0;
		int digit2 = temp2 ? temp2->data : 0;

		/* Adjust digit1 if borrowing is needed */
		int subtract = digit1 - digit2 - borrow;

        if (subtract < 0) {
            subtract += 10;
            borrow = 1;
        } else {
            borrow = 0;
        }
		/* Insert the result digit at the beginning of result list */
		dl_insert_first(headR, tailR, subtract);

		/* Move to previous digits */
		if (temp1)
			temp1 = temp1->prev;
		if (temp2)
			temp2 = temp2->prev;
	}
	delete_leading_zero(headR, tailR);
	return SUCCESS;
}