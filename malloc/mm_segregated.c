/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "ateam",
    /* First member's full name */
    "Harry Bovik",
    /* First member's email address */
    "bovik@cs.cmu.edu",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)


#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

#define LIST_MAX 16

static char *heap_listp;
static void *free_lists[LIST_MAX];

static void insert_node(void *bp, size_t size)
{
    size_t insert_size = size;
    size_t pos = 0;
    void *pred_bp = NULL;
    void *succ_bp = NULL;

    while (pos < LIST_MAX - 1 && insert_size > 1) {
        insert_size >>= 1;
        pos++;
    }

    succ_bp = free_lists[pos];
    if (succ_bp == NULL) {
        SET_PTR(PRED_PTR(bp), NULL);
        SET_PTR(SUCC_PTR(bp), NULL);
        free_lists[pos] = bp;
        return;
    }

    while (succ_bp != NULL && GET_SIZE(HDRP(succ_bp)) < size) {
        pred_bp = succ_bp;
        succ_bp = SUCC_NODE(succ_bp);
    }

    if (pred_bp == NULL) {
        SET_PTR(PRED_PTR(bp), NULL);
        SET_PTR(SUCC_PTR(bp), succ_bp);
        SET_PTR(PRED_PTR(succ_bp), bp);
        free_lists[pos] = bp;
    } else if (succ_bp == NULL) {
        SET_PTR(PRED_PTR(bp), pred_bp);
        SET_PTR(SUCC_PTR(bp), NULL);
        SET_PTR(SUCC_PTR(pred_bp), bp);
    } else {
        SET_PTR(PRED_PTR(bp), pred_bp);
        SET_PTR(SUCC_PTR(bp), succ_bp);
        SET_PTR(SUCC_PTR(pred_bp), bp);
        SET_PTR(PRED_PTR(succ_bp), bp);
    }
}

static void delete_node(void *bp)
{
    size_t size = GET_SIZE(HDRP(bp));
    size_t pos = 0;

    while (pos < LIST_MAX - 1 && size > 1) {
        size >>= 1;
        pos ++;
    }

    if (PRED_NODE(bp) == NULL) {
        free_lists[pos] = SUCC_NODE(bp);
        if (SUCC_NODE(bp) != NULL)
            SET_PTR(PRED_PTR(SUCC_NODE(bp)), NULL);
    } else if (SUCC_NODE(bp) == NULL){
        SET_PTR(SUCC_PTR(PRED_NODE(bp)), NULL);
    } else {
        SET_PTR(SUCC_PTR(PRED_NODE(bp)), SUCC_NODE(bp));
        SET_PTR(PRED_PTR(SUCC_NODE(bp)), PRED_NODE(bp));
    }
}

static void *coalesce(void *bp)
{
    size_t prev_alloc = GET_ALLOC(HDRP(PREV_BLKP(bp)));
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp)));
    size_t size = GET_SIZE(HDRP(bp));

    if (prev_alloc && next_alloc) {
        insert_node(bp, size);
        return bp;
    } else if (prev_alloc && !next_alloc) {
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        delete_node(NEXT_BLKP(bp));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
    } else if (!prev_alloc && next_alloc) {
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        delete_node(PREV_BLKP(bp));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
        bp = PREV_BLKP(bp);
    } else {
        size = size + GET_SIZE(HDRP(PREV_BLKP(bp))) + GET_SIZE(HDRP(NEXT_BLKP(bp)));
        delete_node(PREV_BLKP(bp));
        delete_node(NEXT_BLKP(bp));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
    }

    insert_node(bp, size);

    return bp;
}

static void *extend_heap(size_t words)
{
    char *bp;
    size_t size;

    size = (words % 2) ? (words + 1) * WSIZE : words * WSIZE;
    if ((bp = mem_sbrk(size)) == (void *)-1)
        return NULL;

    PUT(HDRP(bp), PACK(size, 0));
    PUT(FTRP(bp), PACK(size, 0));
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1));

    return coalesce(bp);
}

static void *find_fit(size_t new_size)
{
    size_t search_size = new_size;
    size_t pos = 0;
    void *target_bp = NULL;

    while (pos < LIST_MAX) {
        if (search_size <= 1 && free_lists[pos] != NULL) {
            target_bp = free_lists[pos];
            while (target_bp != NULL && GET_SIZE(HDRP(target_bp)) < new_size) {
                target_bp = SUCC_NODE(target_bp);
            }

            if (target_bp != NULL) break;
        }
        search_size >>= 1;
        pos ++;
    }

    return target_bp;
}

static void *place(void *bp, size_t new_size)
{
    size_t old_size = GET_SIZE(HDRP(bp));
    size_t diff_size = old_size - new_size;

    delete_node(bp);

    if (diff_size < 2 * DSIZE) {
        PUT(HDRP(bp), PACK(old_size, 1));
        PUT(FTRP(bp), PACK(old_size, 1));
    } else if (new_size >= 96) {
        PUT(HDRP(bp), PACK(diff_size, 0));
        PUT(FTRP(bp), PACK(diff_size, 0));
        PUT(HDRP(NEXT_BLKP(bp)), PACK(new_size, 1));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(new_size, 1));
        insert_node(bp, diff_size);
        return NEXT_BLKP(bp);
    } else {
        PUT(HDRP(bp), PACK(new_size, 1));
        PUT(FTRP(bp), PACK(new_size, 1));
        PUT(HDRP(NEXT_BLKP(bp)), PACK(diff_size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(diff_size, 0));
        insert_node(NEXT_BLKP(bp), diff_size);
    }

    return bp;
}

/* 
 * mm_init - initialize the malloc package.
 */
int mm_init(void)
{
    if ((heap_listp = mem_sbrk(4 * WSIZE)) == (void *) -1)
        return -1;
    PUT(heap_listp, 0);                                      //Alignment Padding
    PUT(heap_listp + (1 * WSIZE), PACK(DSIZE, 1));  //Prologue Header
    PUT(heap_listp + (2 * WSIZE), PACK(DSIZE, 1));  //Prologue Footer
    PUT(heap_listp + (3 * WSIZE), PACK(0, 1)); //Epilogue Header

    heap_listp += (2 * WSIZE);

    for (size_t i = 0; i < LIST_MAX; i++) {
        free_lists[i] = NULL;
    }

    if (extend_heap(CHUNKSIZE / WSIZE) == (void *) 0)
        return -1;

    return 0;
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size)
{
    size_t new_size;
    size_t extend_size;
    char *bp;

    if (size == 0)
        return NULL;

    if (size <= DSIZE)
        new_size = 2 * DSIZE;
    else
        new_size = ALIGN(size + DSIZE);

    if ((bp = find_fit(new_size)) != NULL) {
        bp = place(bp, new_size);
        return bp;
    }

    extend_size = MAX(new_size, CHUNKSIZE);
    if ((bp = extend_heap(extend_size / WSIZE)) == NULL)
        return NULL;
    bp = place(bp, new_size);
    return bp;
}

/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    size_t size = GET_SIZE(HDRP(ptr));
    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));

    coalesce(ptr);
}

//static void realloc_place(void *bp, size_t new_size)
//{
//    size_t original_size = GET_SIZE(HDRP(bp));
//    size_t diff_size = original_size - new_size;
//
//    if (diff_size < 2 * DSIZE) {
//        PUT(HDRP(bp), PACK(original_size, 1));
//        PUT(FTRP(bp), PACK(original_size, 1));
//    } else {
//        PUT(HDRP(bp), PACK(new_size, 1));
//        PUT(FTRP(bp), PACK(new_size, 1));
//        PUT(HDRP(NEXT_BLKP(bp)), PACK(diff_size, 0));
//        PUT(FTRP(NEXT_BLKP(bp)), PACK(diff_size, 0));
//        insert_node(NEXT_BLKP(bp), diff_size);
//    }
//}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{

    if (ptr == NULL)
        return mm_malloc(size);

    if (size == 0) {
        mm_free(ptr);
        return NULL;
    }

    void *new_ptr = ptr;
    size_t old_size = GET_SIZE(HDRP(ptr));
    size_t new_size = 0;
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(ptr)));
    size_t next_size = GET_SIZE(HDRP(NEXT_BLKP(ptr)));
    size_t available_size = old_size + next_size;

    if (size <= DSIZE)
        new_size = 2 * DSIZE;
    else
        new_size = ALIGN(size + DSIZE);

    if (old_size >= new_size) {
        return ptr;
    } else if (!next_alloc || !next_size) {

        if (available_size < new_size) {
            if (extend_heap(MAX((new_size - available_size), CHUNKSIZE) / WSIZE) == NULL)
                return NULL;

            available_size += MAX((new_size - available_size), CHUNKSIZE);
        }

        delete_node(NEXT_BLKP(ptr));
        PUT(HDRP(ptr), PACK(available_size, 1));
        PUT(FTRP(ptr), PACK(available_size, 1));
//        realloc_place(ptr, new_size);
    } else {
        new_ptr = mm_malloc(size);
        memcpy(new_ptr, ptr, old_size);
        mm_free(ptr);
    }

    return new_ptr;
}