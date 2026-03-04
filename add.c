/***********************************************************************
 *  File Name   : add.c
 *  Description : Source file for addition operation in the 
 *                Arbitrary Precision Calculator (APC).
 *                Implements digit-wise addition on large numbers
 *                represented as doubly linked lists.
 *
 *                Functions:
 *                - addition()
 ***********************************************************************/

#include "apc.h"
#include "list.h"

/*
 * Function: addition
 * -------------------
 * Performs addition of two numbers represented as doubly linked lists.
 *
 * Each digit of the number is stored in a node of the doubly linked list.
 * The least significant digit is at the tail, and the most significant digit
 * is at the head.
 *
 * Parameters:
 *   head1, tail1 - pointers to the head and tail of the first number
 *   head2, tail2 - pointers to the head and tail of the second number
 *   headR, tailR - pointers to the head and tail of the result number
 *
 * Returns:
 *   SUCCESS if addition was performed correctly,
 *   FAILURE if input lists are NULL or insertion into result fails.
 */
int addition(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR)
{
    /* Check if both numbers are NULL */
    if (*head1 == NULL && *head2 == NULL)
        return SUCCESS;

    /* Start from the least significant digits (tail nodes) */
    Dlist *temp1 = *tail1;
    Dlist *temp2 = *tail2;

    int carry = 0;  // To handle carry during addition

    /* Loop until both lists are exhausted */
    while (temp1 || temp2)
    {
        /* Get current digits, if list is shorter use 0 */
        int digit1 = temp1 ? temp1->data : 0;
        int digit2 = temp2 ? temp2->data : 0;

        /* Perform digit-wise addition with carry */
        int sum = digit1 + digit2 + carry;

        /* Update carry for next iteration */
        carry = sum / 10;

        /* Insert the resulting digit at the beginning of result list */
        if (dl_insert_first(headR, tailR, sum % 10) == FAILURE)
            return FAILURE;

        /* Move to previous digits */
        if (temp1)
            temp1 = temp1->prev;
        if (temp2)
            temp2 = temp2->prev;
    }

    /* If there’s a leftover carry, insert it */
    if (carry)
    {
        if (dl_insert_first(headR, tailR, carry) == FAILURE)
            return FAILURE;
    }
    delete_leading_zero(headR, tailR);
    return SUCCESS;
}