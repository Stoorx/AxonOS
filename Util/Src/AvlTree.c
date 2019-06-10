//
// Created by Stoorx on 05.12.2018.
//

#include <Types.h>
#include <Null.h>
#include <AvlTree.h>

//
// Private functions ---- ---- ---- ---- ---- ---- ---- ----
//
UShort AvlTree$GetSubtreeHeight(AvlTreeNodePtr Node) {
    return Node ? Node->Height : (UShort)0;
}

Int AvlTree$getSubtreeBalance(AvlTreeNodePtr Node) {
    return AvlTree$GetSubtreeHeight(Node->Right) -
           AvlTree$GetSubtreeHeight(Node->Left);
}

Void AvlTree$ChangeHeight(AvlTreeNodePtr Node) {
    UShort heightLeft  = AvlTree$GetSubtreeHeight(Node->Left);
    UShort heightRight = AvlTree$GetSubtreeHeight(Node->Right);
    
    Node->Height = (heightLeft > heightRight ? heightLeft : heightRight) + (UShort)1;
}

AvlTreeNodePtr AvlTree$SwitchRight(AvlTreeNodePtr Node) {
    AvlTreeNodePtr tmpNode = Node->Left;
    Node->Left     = tmpNode->Right;
    tmpNode->Right = Node;
    
    AvlTree$ChangeHeight(Node);
    AvlTree$ChangeHeight(tmpNode);
    
    return tmpNode;
}

AvlTreeNodePtr AvlTree$switchLeft(AvlTreeNodePtr Node) {
    AvlTreeNodePtr tmpNode = Node->Right;
    Node->Right   = tmpNode->Left;
    tmpNode->Left = Node;
    
    AvlTree$ChangeHeight(Node);
    AvlTree$ChangeHeight(tmpNode);
    
    return tmpNode;
}

AvlTreeNodePtr AvlTree$subtreeBalance(AvlTreeNodePtr Node) {
    AvlTree$ChangeHeight(Node);
    
    Int diff = AvlTree$getSubtreeBalance(Node);
    
    if (diff == 2) {
        if (AvlTree$getSubtreeBalance(Node->Right) < 0) {
            Node->Right = AvlTree$SwitchRight(Node->Right);
        }
        return AvlTree$switchLeft(Node);
    }
    else if (diff == -2) {
        if (AvlTree$getSubtreeBalance(Node->Left) > 0) {
            Node->Left = AvlTree$switchLeft(Node->Left);
        }
        return AvlTree$SwitchRight(Node);
    }
    return Node;
}

AvlTreeNodePtr AvlTree$insertRecursive(AvlTreePtr Tree, AvlTreeNodePtr CurrentNode, AvlTreeNodePtr Node) {
    if (CurrentNode == NULL) {
        Node->Height = 1;
        Node->Right  = NULL;
        Node->Left   = NULL;
        return Node;
    }
    Int compareResult = Tree->Comparator(Node, CurrentNode);
    if (compareResult == Greater) {
        AvlTreeNodePtr tmp = AvlTree$insertRecursive(Tree, CurrentNode->Right, Node);
        if (tmp != NULL)
            CurrentNode->Right = tmp;
        else return NULL;
    }
    else if (compareResult == Less) {
        AvlTreeNodePtr tmp = AvlTree$insertRecursive(Tree, CurrentNode->Left, Node);
        if (tmp != NULL)
            CurrentNode->Left = tmp;
        else return NULL;
    }
    else {
        return NULL;
    }
    
    return AvlTree$subtreeBalance(CurrentNode);
}

AvlTreeNodePtr AvlTree$searchRecursive(AvlTreePtr Tree, AvlTreeNodePtr CurrentNode, AvlTreeNodePtr Node) {
    if (CurrentNode == NULL)
        return NULL;
    
    Int compareResult = Tree->Comparator(Node, CurrentNode);
    if (compareResult == Equals) {
        return CurrentNode;
    }
    
    if (compareResult == Greater) {
        if (CurrentNode->Right != NULL)
            return AvlTree$searchRecursive(Tree, CurrentNode->Right, Node);
        else return NULL;
    }
    else {
        if (CurrentNode->Left != NULL)
            return AvlTree$searchRecursive(Tree, CurrentNode->Left, Node);
        else return NULL;
    }
}

AvlTreeNodePtr AvlTree$getLowestSubtreeNode(AvlTreeNodePtr Subtree) {
    AvlTreeNodePtr currentNode = Subtree;
    while (currentNode->Left != NULL) {
        currentNode = currentNode->Left;
    }
    return currentNode;
}

AvlTreeNodePtr AvlTree$removeMinSubtreeRecursive(AvlTreeNodePtr Subtree) {
    if (Subtree->Left == NULL)
        return Subtree->Right;
    Subtree->Left = AvlTree$removeMinSubtreeRecursive(Subtree->Left);
    return AvlTree$subtreeBalance(Subtree);
}

AvlTreeNodePtr AvlTree$removeRecursive(
    AvlTreePtr Tree,
    AvlTreeNodePtr CurrentNode,
    AvlTreeNodePtr Node,
    AvlTreeNodePtr* DeletedNode
) {
    if (CurrentNode == NULL)
        return NULL;
    
    Int compareResult = Tree->Comparator(Node, CurrentNode);
    if (compareResult == Greater) {
        AvlTreeNodePtr tmpNode = AvlTree$removeRecursive(Tree, CurrentNode->Right, Node, DeletedNode);
        if (DeletedNode != NULL)
            CurrentNode->Right = tmpNode;
        else return NULL;
    }
    else if (compareResult == Less) {
        AvlTreeNodePtr tmpNode = AvlTree$removeRecursive(Tree, CurrentNode->Left, Node, DeletedNode);
        if (DeletedNode != NULL)
            CurrentNode->Left = tmpNode;
        else return NULL;
    }
    else {
        *DeletedNode = CurrentNode;
        AvlTreeNodePtr leftChild  = CurrentNode->Left;
        AvlTreeNodePtr rightChild = CurrentNode->Right;
        if (rightChild == NULL)
            return leftChild;
        AvlTreeNodePtr minNode = AvlTree$getLowestSubtreeNode(rightChild);
        minNode->Right = AvlTree$removeMinSubtreeRecursive(rightChild);
        minNode->Left  = leftChild;
        return AvlTree$subtreeBalance(minNode);
    }
    return CurrentNode;
}

//
// Functions ---- ---- ---- ---- ---- ---- ---- ----
//

Void AvlTree$AvlTree(AvlTreePtr Tree, AvlTreeNodesComparatorPtr Comparator) {
    Tree->Root          = NULL;
    Tree->Comparator    = Comparator;
    Tree->InsertCounter = 0;
    Tree->DeleteCounter = 0;
    Tree->SearchCounter = 0;
}

Int AvlTree$Insert(AvlTreePtr Tree, AvlTreeNodePtr Node) {
    AvlTreeNodePtr newRoot = AvlTree$insertRecursive(Tree, Tree->Root, Node);
    if (newRoot != NULL) {
        Tree->Root = newRoot;
        Tree->InsertCounter++;
        return 0;
    }
    else {
        return -1;
    }
}

AvlTreeNodePtr AvlTree$Search(AvlTreePtr Tree, AvlTreeNodePtr Node) {
    AvlTreeNodePtr foundNode = AvlTree$searchRecursive(Tree, Tree->Root, Node);
    Tree->SearchCounter++;
    return foundNode;
}

AvlTreeNodePtr AvlTree$GetLowestNode(AvlTreePtr Tree) {
    if (Tree->Root == NULL)
        return NULL;
    
    AvlTreeNodePtr currentNode = Tree->Root;
    while (currentNode->Left != NULL) {
        currentNode = currentNode->Left;
    }
    return currentNode;
}

AvlTreeNodePtr AvlTree$GetHighestNode(AvlTreePtr Tree) {
    if (Tree->Root == NULL)
        return NULL;
    
    AvlTreeNodePtr currentNode = Tree->Root;
    while (currentNode->Right != NULL) {
        currentNode = currentNode->Right;
    }
    return currentNode;
}

AvlTreeNodePtr AvlTree$Remove(AvlTreePtr Tree, AvlTreeNodePtr Node) {
    if (Tree->Root == NULL)
        return NULL;
    AvlTreeNodePtr deletedNode = NULL;
    AvlTreeNodePtr newRoot     = AvlTree$removeRecursive(Tree, Tree->Root, Node, &deletedNode);
    if (deletedNode != NULL) {
        Tree->Root = newRoot;
        Tree->DeleteCounter++;
    }
    return deletedNode;
}



