/***********************************************************************
 *  File Name   : power.c
 *  Description : Source file for exponentiation in the 
 *                Arbitrary Precision Calculator (APC).
 *                Computes power of a large integer base raised 
 *                to another large integer exponent.
 *
 *                Functions:
 *                - power()
 *
 *  Notes       :
 *  - Implements fast exponentiation using the 
 *    "exponentiation by squaring" algorithm.
 *  - Recursively divides the exponent by 2 and combines results.
 *  - Uses multiplication and division as helper operations.
 *  - Exponent must be non-negative.
 ***********************************************************************/

#include "apc.h"
#include "list.h"

/*
 * Function: power
 * ------------------------
 * Computes the exponentiation of a big integer: head1 ^ head2
 * Uses recursive exponentiation by squaring for efficiency.
 *
 * Parameters:
 *  head1, tail1 : base number (as doubly linked list)
 *  head2, tail2 : exponent (as doubly linked list, non-negative)
 *  headR, tailR : result list (output)
 *
 * Returns:
 *  SUCCESS if computation succeeds
 *  FAILURE on error (currently none for valid inputs)
 *
 * Notes:
 *  - Exponent must be non-negative.
 *  - Uses division by 2 to implement fast exponentiation.
 *  - Multiplication is used to combine results.
 */
int power(Dlist **head1, Dlist **tail1, Dlist **head2, Dlist **tail2, Dlist **headR, Dlist **tailR)
{
    // Case: exponent = 0 → result = 1
    if(*head2 == NULL)
    {
        dl_delete_list(headR, tailR);
        dl_insert_last(headR, tailR, 1);
        return SUCCESS;
    }

    // Create a list representing 1
    Dlist *oneH = NULL, *oneT = NULL;
    dl_insert_last(&oneH, &oneT, 1);
    
    // Case: exponent = 1 → result = base
    if(compare_list(*head2, oneH) == 0)
    {
        dl_delete_list(headR, tailR);
        copy_list(headR, tailR, *head1);    
        dl_delete_list(&oneH, &oneT);
        return SUCCESS;
    }
    dl_delete_list(&oneH, &oneT);  // free temporary '1' list
    
    // Create a list representing 2
    Dlist *twoH = NULL, *twoT = NULL;
    dl_insert_last(&twoH, &twoT, 2);
    
    // Divide exponent by 2 → halfH / halfT
    Dlist *halfH = NULL, *halfT = NULL;
    division(head2, tail2, &twoH, &twoH, &halfH, &halfT);
    dl_delete_list(&twoH, &twoT); // free temporary '2' list
    
    // Recursive call: temp = base ^ (exponent / 2)
    Dlist *tempH = NULL, *tempT = NULL;
    power(head1, tail1, &halfH, &halfT, &tempH, &tempT);
    dl_delete_list(&halfH, &halfT);

    // Square the result: temp * temp
    Dlist *mulH = NULL, *mulT = NULL;
    multiplication(&tempH, &tempT, &tempH, &tempT, &mulH, &mulT);
    dl_delete_list(&tempH, &tempT);

    // If exponent is odd → multiply once more by base
    int lsd = (*tail2)->data;  // least significant digit
    if (lsd % 2 != 0)
    {
        Dlist *tmpH = NULL, *tmpT = NULL;
        multiplication(&mulH, &mulT, head1, tail1, &tmpH, &tmpT);
        dl_delete_list(&mulH, &mulT);
        mulH = tmpH;
        mulT = tmpT;
    }

    // Clear any existing data in the output list before copying the result
    dl_delete_list(headR, tailR);

    // Copy the computed power result into the output list
    copy_list(headR, tailR, mulH);

    // Free temporary multiplication result
    dl_delete_list(&mulH, &mulT);

    return SUCCESS;
}