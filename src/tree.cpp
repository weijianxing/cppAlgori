#ifndef BSTREE_H
#define BSTREE_H

#include <iostream>
using namespace std;

template <typename T>
class BSTreeNode {
public:
    T key;
    BSTreeNode *left;
    BSTreeNode *right;
    BSTreeNode *parent;
    BSTreeNode(T value, BSTreeNode *p, BSTreeNode *l, BSTreeNode *r)
        : key(value), parent(p), left(l), right(r) {}

    T operator*() const { return key; }
};

template <typename T>
class BSTree
{
public:
    typedef BSTreeNode<T> Node;
    typedef BSTreeNode<T>* Node_pointer;

public:
    BSTree();
    ~BSTree();

    //前序遍历
    void preOrder() { preOrder(rootTree); }
    //中序遍历
    void inOrder() { inOrder(rootTree); }
    //后序遍历
    void postOrder() { postOrder(rootTree); }
    //最大值
    T max();
    //最小值
    T min();
    // 找结点(x)的后继结点。即，查找"二叉树中数据值大于该结点"的"最小结点"。
    Node* successor(Node *x);
    // 找结点(x)的前驱结点。即，查找"二叉树中数据值小于该结点"的"最大结点"。
    Node* predecessor(Node *x);
    // (递归实现)查找"二叉树"中键值为key的节点
    Node* search(T key);
    //插入
    void insert(T key);
    //删除
    void remove(T key);

    //清除
    void clear();
private:
    void insert(Node_pointer &tree, Node *z);
    Node* remove(Node_pointer &tree, Node *z);

    void preOrder(Node *tree);
    void inOrder(Node *tree);
    void postOrder(Node *tree);
    Node* max(Node *tree);
    Node* min(Node *tree);
    Node* search(Node* tree, T key);
    void destroy(Node_pointer &tree);
private:
    Node *rootTree;
};

template <typename T>
BSTree<T>::BSTree() : rootTree(nullptr)
{
}

template <typename T>
BSTree<T>::~BSTree()
{
    clear();
}

template <typename T>
void BSTree<T>::insert(T key)
{
    Node* z = new Node(key, nullptr, nullptr, nullptr);
    insert(rootTree, z);
}

template <typename T>
void BSTree<T>::insert(Node_pointer &tree, Node *z)
{
    Node* y = nullptr;
    Node* x = tree;

    while (x != nullptr) {
        y = x;
        if (z->key < x->key) {
            x = x->left;
        }
        else {
            x = x->right;
        }
    }

    z->parent = y;
    if (y == nullptr) {
        tree = z;
    }
    else if (z->key < y->key) {
        y->left = z;
    }
    else {
        y->right = z;
    }
}


template <typename T>
void BSTree<T>::preOrder(Node *tree)
{
    if (tree == nullptr) return;
    std::cout << tree->key << " ";
    preOrder(tree->left);
    preOrder(tree->right);
}

template <typename T>
void BSTree<T>::inOrder(Node *tree)
{
    if (!tree) return;
    inOrder(tree->left);
    std::cout << tree->key << " ";
    inOrder(tree->right);
}

template <typename T>
void BSTree<T>::postOrder(Node *tree)
{
    if (!tree) return;
    postOrder(tree->left);
    postOrder(tree->right);
    std::cout << tree->key << " ";
}

template <typename T>
void BSTree<T>::clear()
{
    std::cout << "===destroy: ";
    destroy(rootTree);
    std::cout << std::endl;
}

template <typename T>
void BSTree<T>::destroy(Node_pointer &tree)
{
    if (tree == nullptr) return;

    if (tree->left) {
        destroy(tree->left);
    }
    if (tree->right) {
        destroy(tree->right);
    }

    std::cout << tree->key << " ";
    delete tree;
    tree = nullptr;
}

template <typename T>
typename BSTree<T>::Node*  BSTree<T>::max(Node *tree)
{
    if (!tree) return nullptr;

    Node* x = tree;
    while (x->right) {
        x = x->right;
    }
    return x;
}

template <typename T>
T BSTree<T>::max()
{
    Node* x = max(rootTree);

    if (x) {
        return x->key;
    } else {
        return NULL;
    }

}

template <typename T>
typename BSTree<T>::Node* BSTree<T>::min(Node *tree)
{
    if (!tree) return nullptr;

    Node* x = tree;
    while (x->left) {
        x = x->left;
    }
    return x;
}

template <typename T>
T BSTree<T>::min()
{
    Node* x = min(rootTree);

    if (x) {
        return x->key;
    } else {
        return (T)nullptr;
    }
}

// 找结点(x)的后继结点。即，查找"二叉树中数据值大于该结点"的"最小结点"。
template <typename T>
typename BSTree<T>::Node* BSTree<T>::successor(Node *x)
{
    if (!x) return nullptr;

    if (x->right) {
        return min(x->right);
    }

    //1. x为一个左节点,父节点为后驱
    //2. x为一个右节点，寻找父节点的父节点。。。，直到找到一个自己作为左孩子的父节点。
    Node* y = x->parent;
    while ((y!=nullptr) && (x==y->right)) {
        x = y;
        y = x->parent;
    }

    return y;
}

// 找结点(x)的前驱结点。即，查找"二叉树中数据值小于该结点"的"最大结点"。
template <typename T>
typename BSTree<T>::Node* BSTree<T>::predecessor(Node *x)
{
    if (!x) return nullptr;

    if (x->left) {
        return max(x->left);
    }

    //1. x为一个右节点,父节点为后驱
    //2. x为一个左节点，寻找父节点的父节点。。。，直到找到一个自己作为右孩子的父节点。
    Node* y = x->parent;
    while ((y != nullptr) && (x == y->left)) {
        x = y;
        y = x->parent;
    }

    return y;

}

template <typename T>
typename BSTree<T>::Node* BSTree<T>::remove(Node_pointer &tree, Node *z)
{
    Node* x = nullptr;
    Node* y = nullptr; //删除的节点

    //1. 假如z只含有一个子节点或者无子节点，删除z节点
    //2. 假如z含有两个子节点，则删除后驱节点，然后z节点值替换
    //y最多一个孩子节点
    if ((z->left == nullptr) || (z->right == nullptr)) {
        y = z;
    } else {
        y = successor(z);
    }

    //查找子节点
    if (y->left != nullptr) {
        x = y->left;
    } else {
        x = y->right;
    }

    if (x != nullptr) {
        x->parent = y->parent;
    }

    if (y->parent == nullptr) {
        tree = x;
    }
    else if (y == y->parent->left) {
        y->parent->left = x;
    }
    else {
        y->parent->right = x;
    }

    return y;
}

//删除
template <typename T>
void BSTree<T>::remove(T key)
{
    Node* x = search(key);
    if (!x) return;

    Node* y = remove(rootTree, x);
    if (y) {
        delete y;
    }
}

template <typename T>
typename BSTree<T>::Node* BSTree<T>::search(Node* tree, T key)
{

    if (tree==nullptr || tree->key == key) {
        return tree;
    }

    if (key < tree->key) {
        return search(tree->left, key);
    } else {
        return search(tree->right, key);
    }
}

template <typename T>
typename BSTree<T>::Node* BSTree<T>::search(T key)
{
    return search(rootTree, key);
}

#endif

int main()
{
    BSTree<int> * pBinTree = new BSTree<int>();
    if ( pBinTree == NULL )
        return 0;

    pBinTree->insert( 15 );
    pBinTree->insert( 3 );
    pBinTree->insert( 20 );
    pBinTree->insert( 8 );
    pBinTree->insert( 10 );
    pBinTree->insert( 18);
    pBinTree->insert( 6 );
    pBinTree->insert( 1);
    pBinTree->insert( 26);
    
    pBinTree->preOrder();
    
    // cout << endl;
    // Node* y = pBinTree.;
    // cout << " 递归前序遍历    ：" ;
    // pBinTree->predecessor();
    // cout << endl;


    delete pBinTree;
    pBinTree = NULL;
    system( "pause" );
    return 1;
}