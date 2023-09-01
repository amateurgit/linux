#ifndef _AVL_TREE_H_
#define _AVL_TREE_H_

/* 参考文档 : https://www.cnblogs.com/skywang12345/p/3576969.html */

/* 平衡二叉树节点 */
typedef struct _avltree_node
{
    int key;
    int height;
    struct _avltree_node *left;
    struct _avltree_node *right;
} avltree_node;

/* 树高 */
int avltree_height(avltree_node *tree);

/* 插入节点 */
avltree_node *avltree_insert(avltree_node *tree, int key);

/* 查询节点 */
avltree_node *avltree_search(avltree_node *tree, int key);

/* 前序遍历 */
void avltree_preorder_traverse(avltree_node *tree);

/* 查询最大值和最小值 */
avltree_node *avltree_minimum(avltree_node *tree);
avltree_node *avltree_maximum(avltree_node *tree);


/* 删除节点 */
avltree_node *avltree_delete(avltree_node *tree, int key);

/* 销毁平衡二叉树 */
void avltree_destroy(avltree_node *tree);

#endif
