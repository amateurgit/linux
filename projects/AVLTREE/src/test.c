#include <stdio.h>
#include <stdlib.h>

#include "avltree.h"

int int_arr[] = { 3, 2, 1, 4, 5, 6, 7, 16, 15, 14, 13, 12, 11, 10, 8, 9 };

int main(int argc,  char *argv[])
{
    avltree_node *tree = NULL;

    /* 插入 */
    int sz = sizeof(int_arr) / sizeof(int_arr[0]);
    int i = 0;
    for (i = 0; i < sz; i++)
    {
        tree = avltree_insert(tree, int_arr[i]);
    }

    printf("高度(从 0 开始) : %d\n", avltree_height(tree));

    /* 遍历 */
    printf("前序遍历 :\n");
    avltree_preorder_traverse(tree);

    /* 查找最值 */
    printf("最大值 %d, 最小值 %d\n", avltree_maximum(tree)->key, avltree_minimum(tree)->key);

    /* 查询节点 */
    avltree_node *node = avltree_search(tree, 11);
    printf("key 为 11 的节点, 左节点 key = %d, 右节点 key = %d\n", 
            node->left->key, node->right->key);

    /* 删除 */
    printf("\n删除 key 为 13 的节点\n");
    tree = avltree_delete(tree, 13);
    avltree_preorder_traverse(tree);

    /* 销毁 */
    avltree_destroy(tree);

    exit(EXIT_SUCCESS);
}

