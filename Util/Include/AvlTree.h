//
// Created by Stoorx on 05.12.2018.
//

#ifndef AXON_AVLTREE_H
#define AXON_AVLTREE_H

#include <Types.h>
#include <Null.h>

#ifdef __cplusplus
extern "C" {
#endif

#define PARENT_STRUCTURE(_MemberPointer, _ParentType, _MemberName) \
    ((_ParentType *)((ULong)(VoidPtr)(_MemberPointer) - \
                    ((ULong)(VoidPtr)(&(((_ParentType *)0)->_MemberName)))))
                    
typedef struct _AVL_TREE_NODE AVL_TREE_NODE, *PAVL_TREE_NODE;

struct _AVL_TREE_NODE {
    UShort Height;
    PAVL_TREE_NODE Left;
    PAVL_TREE_NODE Right;
    Int debugValue;
};


typedef Int (*PAVL_TREE_NODES_COMPARATOR)(PAVL_TREE_NODE aFirst, PAVL_TREE_NODE aSecond);

enum RtlAvlTreeComparatorResult {
    Equals = 0,
    Greater = 1,
    Less = -1,
    ComparisionError = -2
};
struct _AVL_TREE {
    PAVL_TREE_NODE Root;
    PAVL_TREE_NODES_COMPARATOR Comparator;
    ULong SearchCounter;
    ULong InsertCounter;
    ULong DeleteCounter;
};
typedef struct _AVL_TREE AVL_TREE, *PAVL_TREE;


Void RtlAvlTreeInitialize(PAVL_TREE Tree, PAVL_TREE_NODES_COMPARATOR Comparator);
Int RtlAvlTreeInsert(PAVL_TREE Tree, PAVL_TREE_NODE Node);
PAVL_TREE_NODE RtlAvlTreeSearch(PAVL_TREE Tree, PAVL_TREE_NODE Node);
PAVL_TREE_NODE RtlAvlTreeGetLowestNode(PAVL_TREE Tree);
PAVL_TREE_NODE RtlAvlTreeGetHighestNode(PAVL_TREE Tree);
PAVL_TREE_NODE RtlAvlTreeRemove(PAVL_TREE Tree, PAVL_TREE_NODE Node);

#ifdef __cplusplus
};
#endif
#endif //AXON_AVLTREE_H
