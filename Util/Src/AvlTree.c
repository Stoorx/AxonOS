//
// Created by Stoorx on 05.12.2018.
//

#include <Types.h>
#include <Null.h>
#include <AvlTree.h>

//
// Private functions ---- ---- ---- ---- ---- ---- ---- ----
//
UShort RtlpAvlTreeGetSubtreeHeight(PAVL_TREE_NODE Node) {
    return Node ? Node->Height : (UShort)0;
}

Int RtlpAvlTreeGetSubtreeBalance(PAVL_TREE_NODE Node) {
    return RtlpAvlTreeGetSubtreeHeight(Node->Right) -
           RtlpAvlTreeGetSubtreeHeight(Node->Left);
}

Void RtlpAvlTreeChangeHeight(PAVL_TREE_NODE Node) {
    UShort heightLeft  = RtlpAvlTreeGetSubtreeHeight(Node->Left);
    UShort heightRight = RtlpAvlTreeGetSubtreeHeight(Node->Right);
    
    Node->Height = (heightLeft > heightRight ? heightLeft : heightRight) + (UShort)1;
}

PAVL_TREE_NODE RtlpAvlTreeSwitchRight(PAVL_TREE_NODE Node) {
    PAVL_TREE_NODE tmpNode = Node->Left;
    Node->Left     = tmpNode->Right;
    tmpNode->Right = Node;
    
    RtlpAvlTreeChangeHeight(Node);
    RtlpAvlTreeChangeHeight(tmpNode);
    
    return tmpNode;
}

PAVL_TREE_NODE RtlpAvlTreeSwitchLeft(PAVL_TREE_NODE Node) {
    PAVL_TREE_NODE tmpNode = Node->Right;
    Node->Right   = tmpNode->Left;
    tmpNode->Left = Node;
    
    RtlpAvlTreeChangeHeight(Node);
    RtlpAvlTreeChangeHeight(tmpNode);
    
    return tmpNode;
}

PAVL_TREE_NODE RtlpAvlTreeSubtreeBalance(PAVL_TREE_NODE Node) {
    RtlpAvlTreeChangeHeight(Node);
    
    Int diff = RtlpAvlTreeGetSubtreeBalance(Node);
    
    if (diff == 2) {
        if (RtlpAvlTreeGetSubtreeBalance(Node->Right) < 0) {
            Node->Right = RtlpAvlTreeSwitchRight(Node->Right);
        }
        return RtlpAvlTreeSwitchLeft(Node);
    }
    else if (diff == -2) {
        if (RtlpAvlTreeGetSubtreeBalance(Node->Left) > 0) {
            Node->Left = RtlpAvlTreeSwitchLeft(Node->Left);
        }
        return RtlpAvlTreeSwitchRight(Node);
    }
    return Node;
}

PAVL_TREE_NODE RtlpAvlTreeInsertRecursive(PAVL_TREE Tree, PAVL_TREE_NODE CurrentNode, PAVL_TREE_NODE Node) {
    if (CurrentNode == NULL) {
        Node->Height = 1;
        Node->Right  = NULL;
        Node->Left   = NULL;
        return Node;
    }
    Int compareResult = Tree->Comparator(Node, CurrentNode);
    if (compareResult == Greater) {
        PAVL_TREE_NODE tmp = RtlpAvlTreeInsertRecursive(Tree, CurrentNode->Right, Node);
        if (tmp != NULL)
            CurrentNode->Right = tmp;
        else return NULL;
    }
    else if (compareResult == Less) {
        PAVL_TREE_NODE tmp = RtlpAvlTreeInsertRecursive(Tree, CurrentNode->Left, Node);
        if (tmp != NULL)
            CurrentNode->Left = tmp;
        else return NULL;
    }
    else {
        return NULL; // Value found or comparision error
    }
    
    return RtlpAvlTreeSubtreeBalance(CurrentNode);
}

PAVL_TREE_NODE RtlpAvlTreeSearchRecursive(PAVL_TREE Tree, PAVL_TREE_NODE CurrentNode, PAVL_TREE_NODE Node) {
    if (CurrentNode == NULL)
        return NULL;
    
    Int compareResult = Tree->Comparator(Node, CurrentNode);
    if (compareResult == Equals) {
        return CurrentNode;
    }
    
    if (compareResult == Greater) {
        if (CurrentNode->Right != NULL)
            return RtlpAvlTreeSearchRecursive(Tree, CurrentNode->Right, Node);
        else return NULL;
    }
    
    if (compareResult == Less) {
        if (CurrentNode->Left != NULL)
            return RtlpAvlTreeSearchRecursive(Tree, CurrentNode->Left, Node);
        else return NULL;
    }
    
    return NULL;
}

PAVL_TREE_NODE RtlpAvlTreeGetLowestSubtreeNode(PAVL_TREE_NODE Subtree) {
    PAVL_TREE_NODE currentNode = Subtree;
    while (currentNode->Left != NULL) {
        currentNode = currentNode->Left;
    }
    return currentNode;
}

PAVL_TREE_NODE RtlpAvlTreeRemoveMinSubtreeRecursive(PAVL_TREE_NODE Subtree) {
    if (Subtree->Left == NULL)
        return Subtree->Right;
    Subtree->Left = RtlpAvlTreeRemoveMinSubtreeRecursive(Subtree->Left);
    return RtlpAvlTreeSubtreeBalance(Subtree);
}

PAVL_TREE_NODE RtlpAvlTreeRemoveRecursive(
    PAVL_TREE Tree,
    PAVL_TREE_NODE CurrentNode,
    PAVL_TREE_NODE Node,
    PAVL_TREE_NODE* DeletedNode
) {
    if (CurrentNode == NULL)
        return NULL;
    
    Int compareResult = Tree->Comparator(Node, CurrentNode);
    if (compareResult == Greater) {
        PAVL_TREE_NODE tmpNode = RtlpAvlTreeRemoveRecursive(Tree, CurrentNode->Right, Node, DeletedNode);
        if (DeletedNode != NULL)
            CurrentNode->Right = tmpNode;
        else return NULL;
    }
    else if (compareResult == Less) {
        PAVL_TREE_NODE tmpNode = RtlpAvlTreeRemoveRecursive(Tree, CurrentNode->Left, Node, DeletedNode);
        if (DeletedNode != NULL)
            CurrentNode->Left = tmpNode;
        else return NULL;
    }
    else if(compareResult == Equals){
        *DeletedNode = CurrentNode;
        PAVL_TREE_NODE leftChild  = CurrentNode->Left;
        PAVL_TREE_NODE rightChild = CurrentNode->Right;
        if (rightChild == NULL)
            return leftChild;
        PAVL_TREE_NODE minNode = RtlpAvlTreeGetLowestSubtreeNode(rightChild);
        minNode->Right = RtlpAvlTreeRemoveMinSubtreeRecursive(rightChild);
        minNode->Left  = leftChild;
        return RtlpAvlTreeSubtreeBalance(minNode);
    }
    return CurrentNode;
}

//
// Functions ---- ---- ---- ---- ---- ---- ---- ----
//

Void RtlAvlTreeInitialize(PAVL_TREE Tree, PAVL_TREE_NODES_COMPARATOR Comparator) {
    Tree->Root          = NULL;
    Tree->Comparator    = Comparator;
    Tree->InsertCounter = 0;
    Tree->DeleteCounter = 0;
    Tree->SearchCounter = 0;
}

Int RtlAvlTreeInsert(PAVL_TREE Tree, PAVL_TREE_NODE Node) {
    PAVL_TREE_NODE newRoot = RtlpAvlTreeInsertRecursive(Tree, Tree->Root, Node);
    if (newRoot != NULL) {
        Tree->Root = newRoot;
        Tree->InsertCounter++;
        return 0;
    }
    else {
        return -1;
    }
}

PAVL_TREE_NODE RtlAvlTreeSearch(PAVL_TREE Tree, PAVL_TREE_NODE Node) {
    PAVL_TREE_NODE foundNode = RtlpAvlTreeSearchRecursive(Tree, Tree->Root, Node);
    Tree->SearchCounter++;
    return foundNode;
}

PAVL_TREE_NODE RtlAvlTreeGetLowestNode(PAVL_TREE Tree) {
    if (Tree->Root == NULL)
        return NULL;
    
    PAVL_TREE_NODE currentNode = Tree->Root;
    while (currentNode->Left != NULL) {
        currentNode = currentNode->Left;
    }
    return currentNode;
}

PAVL_TREE_NODE RtlAvlTreeGetHighestNode(PAVL_TREE Tree) {
    if (Tree->Root == NULL)
        return NULL;
    
    PAVL_TREE_NODE currentNode = Tree->Root;
    while (currentNode->Right != NULL) {
        currentNode = currentNode->Right;
    }
    return currentNode;
}

PAVL_TREE_NODE RtlAvlTreeRemove(PAVL_TREE Tree, PAVL_TREE_NODE Node) {
    if (Tree->Root == NULL)
        return NULL;
    PAVL_TREE_NODE deletedNode = NULL;
    PAVL_TREE_NODE newRoot     = RtlpAvlTreeRemoveRecursive(Tree, Tree->Root, Node, &deletedNode);
    if (deletedNode != NULL) {
        Tree->Root = newRoot;
        Tree->DeleteCounter++;
    }
    return deletedNode;
}



