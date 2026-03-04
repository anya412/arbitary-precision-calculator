/***********************************************************************
 *  File Name   : divide.c
 *  Description : Source file for division operation in the 
 *                Arbitrary Precision Calculator (APC).
 *                Implements long division (manual method) 
 *                on large numbers represented as doubly linked lists.
 *
 *                Functions:
 *                - division()
 ***********************************************************************/

#include "apc.h"
#include "list.h"

/* 
 * Function: division
 * ------------------
 * Performs division of two big integers represented as doubly linked lists.
 * Implements the long division method (like manual pen-and-paper division).
 *
 * head1, tail1 : dividend (numerator)
 * head2, tail2 : divisor (denominator)
 * headR, tailR : quotient (result of division)
 *
 * returns:
 *   SUCCESS if operation succeeds
 *   FAILURE if divisor = 0
 */
int division(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR)
{
	// Check divide-by-zero
	if (*head2 == NULL)
	{
		fprintf(stderr, "❌ ERROR: Divide by zero !\n");
		return FAILURE;
	}

	// If dividend is empty or smaller than divisor → quotient = 0
	if (*head1 == NULL || compare_list(*head1, *head2) < 0)
		return SUCCESS;

	Dlist *temp1 = *head1;  // pointer to traverse dividend
	Dlist *list1_head = NULL, *list1_tail = NULL; // current portion of dividend

	// Step 1: Find length of divisor
	int len_divisor = 0;
	for (Dlist *t = *head2; t; t = t->next)
		len_divisor++;

	// Step 2: Copy first "len_divisor" digits from dividend into list1_head
	for (int i = 0; i < len_divisor && temp1; i++)
	{
		dl_insert_last(&list1_head, &list1_tail, temp1->data);
		temp1 = temp1->next;
	}

	// Step 3: If copied part < divisor, include one more digit
	if (compare_list(list1_head, *head2) < 0 && temp1)
	{
		dl_insert_last(&list1_head, &list1_tail, temp1->data);
		temp1 = temp1->next;
	}

	// Step 4: Perform long division digit by digit
	while (list1_head)
	{
		int div = 0; // quotient digit for this step

		// Remove leading zeros in the partial dividend
		delete_leading_zero(&list1_head, &list1_tail);

		// Repeated subtraction: while partial dividend >= divisor
		while (list1_head && compare_list(list1_head, *head2) >= 0)
		{
			Dlist *subtract_head = NULL, *subtract_tail = NULL;

			// Subtract divisor from current dividend part
			subtraction(&list1_head, &list1_tail, head2, tail2, &subtract_head, &subtract_tail);

			// Free old partial dividend and update with result
			dl_delete_list(&list1_head, &list1_tail);
			if (subtract_head == NULL) // if subtraction gave 0
			{
				dl_insert_last(&subtract_head, &subtract_tail, 0);
			}
			list1_head = subtract_head;
			list1_tail = subtract_tail;

			div++; // count how many times divisor fits
		}

		// Store this quotient digit in result
		dl_insert_last(headR, tailR, div);

		// Bring down the next digit from dividend (if any left)
		if (temp1)
		{
			dl_insert_last(&list1_head, &list1_tail, temp1->data);
			temp1 = temp1->next;
		}
		else
			break; // no more digits → division complete
	}

	// Free leftover partial dividend
	dl_delete_list(&list1_head, &list1_tail);
	return SUCCESS;
}