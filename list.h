/***********************************************************************
 *  File Name   : list.h
 *  Description : Header file for doubly linked list utilities used in
 *                Arbitrary Precision Calculator (APC).
 *                Provides function declarations for basic operations
 *                on doubly linked lists that represent big integers.
 *
 *  Functions:
 *   - dl_insert_first()   : Insert a node at the beginning
 *   - dl_insert_last()    : Insert a node at the end
 *   - dl_delete_first()   : Delete a node from the beginning
 *   - dl_delete_list()    : Delete the entire list
 *   - compare_list()      : Compare two numbers stored as lists
 *   - delete_leading_zero(): Remove unnecessary leading zeroes
 *   - copy_list()         : Duplicate a list into another
 *
 *  Notes:
 *  - Each digit of a number is stored in one node.
 *  - Lists represent big integers, with head as MSD and tail as LSD.
 *  - These functions are the building blocks for arithmetic ops.
 ***********************************************************************/

#ifndef LIST_H
#define LIST_H

#include "apc.h"

/* Insert a node at the beginning */
int dl_insert_first(Dlist **head, Dlist **tail, int data);

/* Insert a node at the end */
int dl_insert_last(Dlist **head, Dlist **tail, int data);

/* delete a node at the beginning */
int dl_delete_first(Dlist **head, Dlist **tail);

/* Delete the entire list */
int dl_delete_list(Dlist **head, Dlist **tail);

/* Compare two lists (numbers) */
int compare_list(Dlist *head1, Dlist *head2);

/* Delete the leading zeroes */
int delete_leading_zero(Dlist **head, Dlist **tail);

/* Copy the list to another list */
int copy_list(Dlist **headR, Dlist **tailR, Dlist * head);

#endif /* LIST_H */