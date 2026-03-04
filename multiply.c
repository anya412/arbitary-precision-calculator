/***********************************************************************
 *  File Name   : multiply.c
 *  Description : Source file for multiplication operation in the 
 *                Arbitrary Precision Calculator (APC).
 *                Implements digit-by-digit multiplication 
 *                with support for large numbers stored as 
 *                doubly linked lists.
 *
 *                Functions:
 *                - multiplication()
 ***********************************************************************/

#include "apc.h"
#include "list.h"

/*
 * Function: multiplication
 * ------------------------
 * Multiplies two big integers represented as doubly linked lists.
 * Each node of the list stores a single digit.
 *
 * Parameters:
 *  head1, tail1 : first number (as a doubly linked list)
 *  head2, tail2 : second number (as a doubly linked list)
 *  headR, tailR : result list (will store product)
 *
 * Returns:
 *  SUCCESS if multiplication succeeds,
 *  FAILURE if either list is empty.
 */
int multiplication(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR)
{

	/* If either number is empty, Return SUCCESS with empty headR*/
	if (*head1 == NULL || *head2 == NULL)
		return SUCCESS;

	/*
	 * temp1 -> used to iterate through first number
	 * temp2 -> used to iterate through second number
	 */
	Dlist *temp1 = *tail1, *temp2 = *tail2;

	/* Temporary result lists for partial products and intermediate sums */
	Dlist *headR1, *tailR1, *headR2, *tailR2;
	headR1 = tailR1 = headR2 = tailR2 = NULL;

	int count = 0; // keeps track of how many zeros to pad (place value)

	/* Outer loop: iterate through digits of second number (multiplier) */
	while (temp2)
	{
		int carry = 0;

		/* Add zero padding at beginning of partial product (for place value) */
		for (int i = 0; i < count; i++)
		{
			dl_insert_first(&headR1, &tailR1, 0);
		}

		/* Inner loop: multiply current digit of second number with all digits of first number */
		while (temp1)
		{
			int mul = (temp1->data) * (temp2->data) + carry;
			carry = mul / 10;

			/* If last digit of multiplier, add directly into final result */
			if (temp2->next == NULL)
				dl_insert_first(headR, tailR, mul % 10);
			else
				dl_insert_first(&headR1, &tailR1, mul % 10);

			temp1 = temp1->prev;
		}

		/* If carry remains after finishing inner loop */
		if (carry)
		{
			if (temp2->next == NULL)
				dl_insert_first(headR, tailR, carry);
			else
				dl_insert_first(&headR1, &tailR1, carry);
		}

		/* If we already have a result, add partial product to it */
		if (count > 0)
		{
			addition(headR, tailR, &headR1, &tailR1, &headR2, &tailR2);

			/* Free old lists and update result */
			dl_delete_list(headR, tailR);
			dl_delete_list(&headR1, &tailR1);

			*headR = headR2;
			*tailR = tailR2;

			headR1 = tailR1 = NULL;
			headR2 = tailR2 = NULL;
		}

		/* Move to previous digit of multiplier */
		temp2 = temp2->prev;
		temp1 = *tail1; // Reset temp1 for next iteration
		count++;
	}

	delete_leading_zero(headR, tailR);
	return SUCCESS;
}