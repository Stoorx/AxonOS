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

typedef struct _AvlTreeNode AvlTreeNode, * AvlTreeNodePtr;

struct _AvlTreeNode {
    UShort         Height;
    AvlTreeNodePtr Left;
    AvlTreeNodePtr Right;
    Int            debugValue;
};


typedef Int (* AvlTreeNodesComparatorPtr)(AvlTreeNodePtr aFirst, AvlTreeNodePtr aSecond);

enum AvlTreeComparatorResult {
    Equals  = 0,
    Greater = 1,
    Less    = -1
};

struct _Avl_Tree {
    AvlTreeNodePtr            Root;
    AvlTreeNodesComparatorPtr Comparator;
    ULong                     SearchCounter;
    ULong                     InsertCounter;
    ULong                     DeleteCounter;
};

typedef struct _Avl_Tree AvlTree, * AvlTreePtr;


Void AvlTree$AvlTree(AvlTreePtr Tree, AvlTreeNodesComparatorPtr Comparator);
Int AvlTree$Insert(AvlTreePtr Tree, AvlTreeNodePtr Node);
AvlTreeNodePtr AvlTree$Search(AvlTreePtr Tree, AvlTreeNodePtr Node);
AvlTreeNodePtr AvlTree$GetLowestNode(AvlTreePtr Tree);
AvlTreeNodePtr AvlTree$GetHighestNode(AvlTreePtr Tree);
AvlTreeNodePtr AvlTree$Remove(AvlTreePtr Tree, AvlTreeNodePtr Node);

#ifdef __cplusplus
};
#endif
#endif //AXON_AVLTREE_H
