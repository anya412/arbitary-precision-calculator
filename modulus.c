/***********************************************************************
 *  File Name   : modulus.c
 *  Description : Source file for modulus operation in the 
 *                Arbitrary Precision Calculator (APC).
 *                Implements remainder calculation using 
 *                long division logic on large numbers 
 *                represented as doubly linked lists.
 *
 *                Functions:
 *                - modulus()
 ***********************************************************************/

#include "apc.h"
#include "list.h"

/*
 * Function: modulus
 * -----------------
 * Finds the remainder when one big integer is divided by another.
 * Both numbers are represented as doubly linked lists,
 * where each node stores a single digit.
 *
 * Parameters:
 *   head1, tail1 : dividend (first number as doubly linked list)
 *   head2, tail2 : divisor  (second number as doubly linked list)
 *   headR, tailR : remainder (result list to store the remainder)
 *
 * Returns:
 *   SUCCESS if modulus operation succeeds,
 *   FAILURE if divisor is zero or if input is invalid.
 */
int modulus(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR)
{
    // Check for division by zero
    if (*head2 == NULL)
    {
        fprintf(stderr, "❌ ERROR: Modulo by zero !\n");
        return FAILURE;
    }

    // If dividend is NULL or smaller than divisor, remainder is dividend itself
    if (*head1 == NULL || compare_list(*head1, *head2) < 0)
    {
        copy_list(headR, tailR, *head1);
        return SUCCESS;
    }

    // Temporary pointer to traverse dividend
    Dlist *temp1 = *head1;
    // Create a temporary list to hold current working portion of dividend
    Dlist *list1_head = NULL, *list1_tail = NULL;

    // Count digits of divisor
    int len_divisor = 0;
    for (Dlist *t = *head2; t; t = t->next) 
        len_divisor++;

    // Take first 'len_divisor' digits of dividend
    for (int i = 0; i < len_divisor && temp1; i++)
    {
        dl_insert_last(&list1_head, &list1_tail, temp1->data);
        temp1 = temp1->next;
    }

    // If initial part of dividend < divisor, take one more digit
    if (compare_list(list1_head, *head2) < 0)
    {
        dl_insert_last(&list1_head, &list1_tail, temp1->data);
        temp1 = temp1->next;
    }

    // Perform long division process
    while (list1_head)
    {
        int div = 0;  // keeps count of how many times divisor fits

        // Remove leading zeros to keep numbers clean
        delete_leading_zero(&list1_head, &list1_tail);

        // Keep subtracting divisor from current part until smaller
        while (list1_head && compare_list(list1_head, *head2) >= 0)
        {
            Dlist *subtract_head = NULL, *subtract_tail = NULL;

            // Ensure no broken links in subtraction
            list1_head->prev = NULL;

            // Perform subtraction: list1_head = list1_head - divisor
            subtraction(&list1_head, &list1_tail, head2, tail2, &subtract_head, &subtract_tail);

            // Free old list and update with result
            dl_delete_list(&list1_head, &list1_tail);
            if (subtract_head == NULL)
            {
                dl_insert_last(&subtract_head, &subtract_tail, 0);
            }

            list1_head = subtract_head;
            list1_tail = subtract_tail;
            div++;
        }

        // Bring down next digit from dividend if available
        if (temp1)
        {
            dl_insert_last(&list1_head, &list1_tail, temp1->data);
            temp1 = temp1->next;
        }
        else
            break;
    }

    // Copy final remainder into result list
    copy_list(headR, tailR, list1_head);

    // Free temporary working list
    dl_delete_list(&list1_head, &list1_tail);

    return SUCCESS;
}