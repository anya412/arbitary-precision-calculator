/***********************************************************************
 *  File Name   : list.c
 *  Description : Source file for Doubly Linked List operations used in
 *                Arbitrary Precision Calculator (APC).
 *                Implements core helper functions for managing big
 *                integers represented as doubly linked lists.
 *
 *  Functions:
 *   - dl_insert_first()     : Insert a node at the beginning
 *   - dl_insert_last()      : Insert a node at the end
 *   - dl_delete_first()     : Delete the first node
 *   - dl_delete_list()      : Delete the entire list
 *   - compare_list()        : Compare two numbers
 *   - delete_leading_zero() : Remove leading zeroes
 *   - copy_list()           : Duplicate a list
 *   - print_list()          : Print list as number
 *
 *  Notes:
 *   - Each digit of a number is stored in a node (MSD at head, LSD at tail).
 *   - These functions are utility routines that arithmetic modules depend on.
 ***********************************************************************/

#include "list.h"

/* 
 * Inserts a new node at the beginning of the doubly linked list.
 */
int dl_insert_first(Dlist **head, Dlist **tail, int data)
{
    // Allocate memory for new node
    Dlist *newNode = malloc(sizeof(Dlist));
    if (newNode == NULL)
        return FAILURE;

    // Initialize new node
    newNode->data = data;
    newNode->next = *head;
    newNode->prev = NULL;

    // If list is empty → new node becomes both head and tail
    if (*head == NULL)
    {
        *head = *tail = newNode;
        return SUCCESS;
    }

    // Otherwise, insert new node before current head
    (*head)->prev = newNode;
    *head = newNode;   // update head pointer
    return SUCCESS;
}

/* 
 * Inserts a new node at the end of the doubly linked list.
 */
int dl_insert_last(Dlist **head, Dlist **tail, int data)
{
    // Allocate memory for new node
    Dlist *new = malloc(sizeof(Dlist));
    if (new == NULL)
        return FAILURE;

    // Initialize new node
    new->data = data;
    new->prev = *tail;
    new->next = NULL;

    // If list is empty → new node becomes both head and tail
    if (*head == NULL)
    {
        *head = *tail = new;
        return SUCCESS;
    }

    // Otherwise, attach node after tail
    (*tail)->next = new;
    *tail = new;  // update tail pointer
    return SUCCESS;
}

/* 
 * Deletes all nodes from the doubly linked list.
 */
int dl_delete_list(Dlist **head, Dlist **tail)
{
    if (*head == NULL)
        return FAILURE;

    Dlist *curr = *head;
    Dlist *next;

    // Traverse the list and free each node
    while (curr)
    {
        next = curr->next;
        free(curr);
        curr = next;
    }

    // Reset head and tail
    *head = *tail = NULL;
    return SUCCESS;
}

/* 
 * Deletes the first node of the list.
 */
int dl_delete_first(Dlist **head, Dlist **tail)
{
    if (*head == NULL)  // empty list
        return FAILURE;

    // If list has only one node
    if (*head == *tail)
    {
        free(*head);
        *head = *tail = NULL;
        return SUCCESS;
    }

    // More than one node → remove head
    Dlist *temp = *head;
    *head = (*head)->next;
    (*head)->prev = NULL;
    free(temp);
    return SUCCESS;
}

/* 
 * Function: compare_list
 * ----------------------
 * Compares two doubly linked lists representing numbers.
 * Comparison is done first by length, then digit by digit (MSB to LSB).
 * 
 * head1 : head of first list
 * head2 : head of second list
 *
 * returns: 
 *   1  if list1 > list2
 *  -1  if list1 < list2
 *   0  if both lists are equal
 */
int compare_list(Dlist *head1, Dlist *head2)
{
    if (head1 == NULL || head2 == NULL)
        return FAILURE;

    Dlist *temp1 = head1, *temp2 = head2;
    int list_len1 = 0, list_len2 = 0;

    // Calculate length of list1
    while (temp1)
    {
        temp1 = temp1->next;
        list_len1++;
    }

    // Calculate length of list2
    while (temp2)
    {
        temp2 = temp2->next;
        list_len2++;
    }

    // Compare by length first
    if (list_len1 > list_len2)
        return 1;
    else if (list_len1 < list_len2)
        return -1;
    else
    {
        // Lengths are equal → compare digit by digit
        temp1 = head1;
        temp2 = head2;
        for (int i = 0; i < list_len1; i++)
        {
            int digit1 = temp1->data, digit2 = temp2->data;

            if (digit1 > digit2)
                return 1;
            else if (digit1 < digit2)
                return -1;

            // move to next digit
            temp1 = temp1->next;
            temp2 = temp2->next;
        }
    }
    return 0; // lists are equal
}

/* 
 * Removes all leading zeroes from the list.
 */
int delete_leading_zero(Dlist **head, Dlist **tail)
{
    if (*head == NULL)
        return FAILURE;

    // Keep deleting nodes from front until a non-zero digit is found
    while (*head)
    {
        if ((*head)->data) // stop if digit is non-zero
            break;
        dl_delete_first(head, tail);
    }
    return SUCCESS;
}

/* 
 * Creates a copy of a doubly linked list.
 */
int copy_list(Dlist **headR, Dlist **tailR, Dlist *head)
{
    if (head == NULL)
        return FAILURE;

    // Traverse input list and insert nodes into new list
    while (head)
    {
        dl_insert_last(headR, tailR, head->data);
        head = head->next;
    }
    return SUCCESS;
}

/* 
 * Function: print_list
 * --------------------
 * Prints the number stored in the doubly linked list.
 * If list is empty → print "0".
 * Otherwise, prints digits from head to tail.
 */
void print_list(Dlist *head)
{
    if (head == NULL)
    {
        printf("0");
    }
    else
    {
        while (head)
        {
            printf("%d", head->data);
            head = head->next;
        }
    }
}