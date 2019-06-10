//
// Created by Stoorx on 05.06.2019.
//

#include <Catch.hpp>
#include <AvlTree.h>
#include <set> // STL !

class TestValueNode {
public:
    explicit TestValueNode(Int value) {
        Value = value;
        Node.debugValue = value;
        Node.Right      = nullptr;
        Node.Left       = nullptr;
        Node.Height     = 0;
    }
    
    AvlTreeNode Node;
    Int         Value;
};

TEST_CASE("AVL Tree") {
    srand(1);
    AvlTree tree;
    AvlTree$AvlTree(
        &tree,
        [](AvlTreeNodePtr first, AvlTreeNodePtr second) -> Int {
            auto firstValueNode  = PARENT_STRUCTURE(first, TestValueNode, Node);
            auto secondValueNode = PARENT_STRUCTURE(second, TestValueNode, Node);
            return (firstValueNode->Value == secondValueNode->Value)
                   ? Equals
                   : (
                       firstValueNode->Value < secondValueNode->Value
                       ? Less
                       : Greater
                   );
        }
    );
    
    std::set<Int> stdTree;
    
    for (Int i = 0; i < 1000000; i++) {
        auto command = rand() % 3;
        switch (command) {
            case 0: {
                // Search
                Int  value          = (rand() % 100) - 50;
                auto testNode       = TestValueNode(value);
                auto resultNode     = AvlTree$Search(&tree, &testNode.Node);
                auto stdResultValue = stdTree.find(value);
                if (resultNode != nullptr) {
                    REQUIRE(stdResultValue != stdTree.end());
                    REQUIRE(*stdResultValue == PARENT_STRUCTURE(resultNode, TestValueNode, Node)->Value);
                }
                else {
                    INFO("i=" << i);
                    REQUIRE(stdResultValue == stdTree.end());
                }
                break;
            }
            case 1: {
                // Insert
                Int  value     = (rand() % 100) - 50;
                auto stdResult = stdTree.insert(value);
                auto node      = new TestValueNode(value);
                auto result    = AvlTree$Insert(&tree, &node->Node);
                if (result != -1) {
                    REQUIRE(stdResult.second);
                }
                else {
                    REQUIRE(!stdResult.second);
                }
                break;
            }
            case 2: {
                // Delete
                Int  value     = (rand() % 100) - 50;
                auto stdResult = stdTree.erase(value);
                auto tmpNode   = TestValueNode(value);
                auto nodePtr   = AvlTree$Remove(&tree, &tmpNode.Node);
                if (nodePtr != nullptr) {
                    REQUIRE(stdResult == 1);
                    delete nodePtr;
                }
                else {
                    INFO("i=" << i);
                    REQUIRE(stdResult == 0);
                }
                break;
            }
            default:
                CATCH_ERROR("Incorrect test!");
        }
    }
}

