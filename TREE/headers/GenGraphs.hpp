#include "wolfram.h"


template <typename treeElem_T>
TreeErr_t GenGraphs(tree_t<treeElem_T> *tree, const char *func)
{
    ON_DEBUG( if (IS_BAD_PTR(tree)) return TREE_ERROR; )

    char folder[64] = {0};
    snprintf(folder, sizeof(folder), "mkdir -p TREE/ReportFiles/graphs/graph%d", number_graph);
    system(folder);

    char file_path[64] = {0};
    snprintf(file_path, sizeof(file_path), "TREE/ReportFiles/graphs/graph%d/tree.dot", number_graph);

    FILE *graph = fopen(file_path, "w");
    if (IS_BAD_PTR(graph)) return TREE_ERROR;

    if (GenDot(graph, tree, func))  return TREE_ERROR;
    
    fclose(graph);
    
    char dot_cmd[128] = {0};
    snprintf(dot_cmd, sizeof(dot_cmd), "dot -Tpng %s -o %s.png", file_path, file_path);
    system(dot_cmd);

    number_graph++;
    return TREE_SUCCESS;
}

template <typename treeElem_T>
TreeErr_t CreateNode(FILE *src, node_t<treeElem_T> *node, int node_id)
{
    ON_DEBUG(
        if (IS_BAD_PTR(src))  return TREE_ERROR;
        if (IS_BAD_PTR(node)) return TREE_ERROR;
    )

    fprintf(src, "\tn%d [shape=record, label=\"{ptr = %p | data = %s | {<left> left = %p | <right> right = %p}}\", fillcolor=\"#87CEEB\", color=\"black\"]\n", node_id, (void*)node, node->item, (void*)node->left, (void*)node->right);
    
    return TREE_SUCCESS;
}


template <typename treeElem_T>
TreeErr_t GenDot(FILE *src, tree_t<treeElem_T> *tree, const char *func)
{
    ON_DEBUG(
        if (IS_BAD_PTR(src))  return TREE_ERROR;
        if (IS_BAD_PTR(tree)) return TREE_ERROR;
    )

    fprintf(src,"digraph G {\n"
                "\tlabel=<<B>Tree from %s()</B>>;\n"
                "\tfontcolor=\"%s\";\n"
                "\tfontname=\"Arial\";\n"
                "\tlabelloc=\"top\";\n"
                "\tlabeljust=\"center\";\n"
                "\tfontsize=20;\n"
                "\trankdir=TB;\n"
                "\tcenter=true;\n"
                "\tnodesep=0.5;\n"
                "\tranksep=0.5;\n"
                "\tnode[fontsize=9, shape=box, width=0.7, height=0.4, style=\"filled, rounded\", fontname=\"Arial\"];\n"
                "\n", func, (tree->error == 0) ? "#008000" : "#B22222");

    if (tree->root == NULL) { fprintf(src, "}"); return TREE_SUCCESS; }

    stk_t<node_t<treeElem_T>*> stk_ret = {};
    STACK_CTOR(&stk_ret, tree->size + 1);

    stk_t<size_t> stk_ids = {};
    STACK_CTOR(&stk_ids, tree->size + 1);

    size_t number_nodes = 0;

    StackPush(&stk_ret, tree->root);
    StackPush(&stk_ids, number_nodes);
    
    fprintf(src, "\tn%zu [shape=record, label=\"{ptr = %p | parent = %p | data = %s | {<left> left = %p | <right> right = %p}}\", fillcolor=\"#87CEEB\", color=\"black\"]\n", 
            number_nodes, tree->root, tree->root->parent, tree->root->item, tree->root->left, tree->root->right);
    number_nodes++;

    while (stk_ret.size > 0)
    {
        node_t<treeElem_T>* current_node = NULL;
        size_t current_id = 0;
        
        StackPop(&stk_ret, &current_node);
        StackPop(&stk_ids, &current_id);

        if (current_node == NULL) continue;

        if (current_node->left != NULL)
        {
            fprintf(src, "\tn%zu [shape=record, label=\"{ptr = %p | parent = %p | data = %s | {<left> left = %p | <right> right = %p}}\", fillcolor=\"#87CEEB\", color=\"black\"]\n", 
                    number_nodes, current_node->left, current_node->left->parent, current_node->left->item, current_node->left->left, current_node->left->right);
            
            fprintf(src, "\tn%zu:left -> n%zu\n", current_id, number_nodes);
            
            StackPush(&stk_ret, current_node->left);
            StackPush(&stk_ids, number_nodes);
            number_nodes++;
        }

        if (current_node->right != NULL)
        {
            fprintf(src, "\tn%zu [shape=record, label=\"{ptr = %p | parent = %p | data = %s | {<left> left = %p | <right> right = %p}}\", fillcolor=\"#87CEEB\", color=\"black\"]\n", 
                    number_nodes, current_node->right, current_node->right->parent, current_node->right->item, current_node->right->left, current_node->right->right);
            
            fprintf(src, "\tn%zu:right -> n%zu\n", current_id, number_nodes);
            
            StackPush(&stk_ret, current_node->right);
            StackPush(&stk_ids, number_nodes);
            number_nodes++;
        }
    }

    STACK_DTOR(&stk_ret);
    STACK_DTOR(&stk_ids);

    fprintf(src, "}");
    return TREE_SUCCESS;
}