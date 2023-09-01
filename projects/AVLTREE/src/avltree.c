#include <stdio.h>
#include <stdlib.h>
#include "avltree.h"

#define MAX(a, b) ( (a) > (b) ? (a) : (b) )

static avltree_node *left_left_rotation(avltree_node *tree)
{
    avltree_node *left;

    left = tree->left;

    /*
       原根节点 tree 的左子树 变成 左子树的右子树, 左子树的右子树变成原根节点
       左子树变成新的根节点
     */
    tree->left = left->right;
    left->right = tree;

    tree->height = MAX(avltree_height(tree->left), avltree_height(tree->right)) + 1;
    left->height = MAX(avltree_height(left->left), tree->height) + 1;

    return left;
}

static avltree_node *right_right_rotation(avltree_node *tree)
{
    avltree_node *right;

    right = tree->right;

    /*
       原根节点 tree 的右子树 变成 右子树的左子树, 右子树的左子树变成原根节点tree
       右子树变成新的根节点
     */
    tree->right = right->left;
    right->left = tree;

    tree->height = MAX(avltree_height(tree->left), avltree_height(tree->right)) + 1;
    right->height = MAX(tree->height, avltree_height(right->right)) + 1;

    return right;
}

static avltree_node *left_right_rotation(avltree_node *tree)
{
    /* RR 选择 */
    tree->left = right_right_rotation(tree->left);

    /* LL 选择 */
    return left_left_rotation(tree);
}

static avltree_node *right_left_rotation(avltree_node *tree)
{
    /* LL 选择 */
    tree->right = left_left_rotation(tree->right);

    /* RR 选择 */
    return right_right_rotation(tree);
}

/* 树高从 0 开始 */
int avltree_height(avltree_node *tree)
{
    if (tree)
    {
        return tree->height;
    }
    else
    {
        return -1;
    }
}

static avltree_node *avltree_create_node(int key)
{
    avltree_node *pnode;

    pnode = (avltree_node *)malloc(sizeof(avltree_node));
    if (!pnode)
    {
        return NULL;
    }

    pnode->key = key;
    pnode->height = 0;
    pnode->left = NULL;
    pnode->right = NULL;

    return pnode;
}

/*
 *  删除节点 node, 返回根节点
 *  参数
 *      tree - 根节点
 *      node - 待删节点
 *  返回值
 *      根节点
 */
static avltree_node *avltree_delete_node(avltree_node *tree, avltree_node *node)
{
    if (!tree || !node)
    {
        return NULL;
    }

    /* 待删节点在 tree 的左子树 */
    if (node->key < tree->key)
    {
        tree->left = avltree_delete_node(tree->left, node);
        if (2 == avltree_height(tree->right) - avltree_height(tree->left))
        {
            avltree_node *r = tree->right;
            if (avltree_height(r->left) > avltree_height(r->right))
            {
                tree = right_left_rotation(tree);
            }
            else
            {
                tree = right_right_rotation(tree);
            }
        }
    }
    /* 待删节点在 tree 的右子树 */
    else if (node->key > tree->key)
    {
        tree->right = avltree_delete_node(tree->right, node);
        if (2 == avltree_height(tree->left) - avltree_height(tree->right))
        {
            avltree_node *l = tree->left;
            if (avltree_height(l->right) > avltree_height(l->left))
            {
                tree = left_right_rotation(tree);
            }
            else
            {
                tree = left_left_rotation(tree);
            }
        }
    }
    /* tree 就是要删除的节点 */
    else
    {
        /* tree 的左右子树都非空 */
        if (tree->left && tree->right)
        {
            /* 左子树比右子树高 */
            if (avltree_height(tree->left) > avltree_height(tree->right))
            {
                /*  
                 * 1. 查找 tree 左子树最大值节点
                 * 2. 该节点 key 赋给 tree
                 * 3. 删除该节点
                 */
                avltree_node *max = avltree_maximum(tree->left);
                tree->key = max->key;
                tree->left = avltree_delete_node(tree->left, max);
            }
            /* 左右子树高度相同 或 右子树比左子树高 */
            else
            {
                /*  
                 * 1. 查找 tree 右子树的最小值节点
                 * 2. 该节点 key 赋给 tree
                 * 3. 删除该节点
                 */
                avltree_node *min = avltree_minimum(tree->right);
                tree->key = min->key;
                tree->right = avltree_delete_node(tree->right, min);
            }
        }
        /* tree 的左右子树至少有一个为空 */
        else 
        {
            avltree_node *tmp = tree;
            tree = tree->left ? tree->left : tree->right;
            free(tmp);
        }
    }

    return tree;
}

avltree_node *avltree_insert(avltree_node *tree, int key)
{
    if (!tree)
    {
        tree = avltree_create_node(key);
        if (!tree)
        {
            printf("avltree_create_node error\n");
            return NULL;
        }
    }
    else if (key < tree->key)
    {
        /* 插入左子树 */
        tree->left = avltree_insert(tree->left, key);

        /* 调整平衡 */
        if (avltree_height(tree->left) - avltree_height(tree->right) == 2)
        {
            if (key < tree->left->key)
            {
                tree = left_left_rotation(tree);
            }
            else
            {
                tree = left_right_rotation(tree);
            }
        }
    }
    else if (key > tree->key)
    {
        /* 插入右子树 */
        tree->right = avltree_insert(tree->right, key);

        /* 调整平衡 */
        if (avltree_height(tree->right) - avltree_height(tree->left) == 2)
        {
            if (key > tree->right->key)
            {
                tree = right_right_rotation(tree);
            }
            else
            {
                tree =  right_left_rotation(tree);
            }
        }
    }
    else
    {
        printf("平衡树中已存在 %d\n", key);
    }

    tree->height = MAX(avltree_height(tree->left), avltree_height(tree->right)) + 1;

    return tree;
}


void avltree_preorder_traverse(avltree_node *tree)
{
    if (tree)
    {
        printf("key = %d, height = %d\n", tree->key, tree->height);
        avltree_preorder_traverse(tree->left);
        avltree_preorder_traverse(tree->right);
    }

    return;
}

avltree_node *avltree_minimum(avltree_node *tree)
{
    if (!tree)
    {
        return NULL;
    }

    while(tree->left)
    {
        tree = tree->left;
    }
    return tree;
}

avltree_node *avltree_maximum(avltree_node *tree)
{
    if (!tree)
    {
        return NULL;
    }

    while(tree->right)
    {
        tree = tree->right;
    }
    return tree;
}


/*
 * 删除节点(key是节点值)，返回根节点
 *
 * 参数说明：
 *     tree AVL树的根结点
 *     key 待删除的结点的键值
 * 返回值：
 *     根节点
 */
avltree_node *avltree_delete(avltree_node *tree, int key)
{
    avltree_node *node = avltree_search(tree, key);
    if (node)
    {
        tree = avltree_delete_node(tree, node);
    }

    return tree;
}

avltree_node *avltree_search(avltree_node *tree, int key)
{
    if (!tree || key == tree->key)
    {
        return tree;
    }

    if (key < tree->key)
    {
        return avltree_search(tree->left, key);
    }
    else
    {
        return avltree_search(tree->right, key);
    }
}

void avltree_destroy(avltree_node *tree)
{
    if (!tree)
    {
        return;
    }

    if (tree->left)
    {
        avltree_destroy(tree->left);
    }

    if (tree->right)
    {
        avltree_destroy(tree->right);
    }

    free(tree);

    return;
}











