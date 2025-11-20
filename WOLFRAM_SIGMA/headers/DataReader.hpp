#include "wolfram.h"
#include "SizeFile.h"
#include "LineCounter.h"


#define SKIP_SPACES(ptr) while (isspace((int)*ptr)) ptr++

template <typename treeElem_T>
node_t<treeElem_T>* NodeReader(char* cur_pos, char** next_pos, node_t<treeElem_T> *parent)
{
    if (cur_pos == NULL) return NULL;
    
    SKIP_SPACES(cur_pos);

    if (*cur_pos == '(')
    {
        cur_pos++;
        SKIP_SPACES(cur_pos);

        int len_name = NameNodeReader(cur_pos);
        cur_pos++;
        *(cur_pos + len_name) = '\0';

        node_t<treeElem_T> *new_node = NewNode(cur_pos);
        if (IS_BAD_PTR(new_node)) return NULL;

        cur_pos += len_name + 1;
        SKIP_SPACES(cur_pos);

        new_node->parent = parent;
        
        new_node->left = NodeReader<treeElem_T>(cur_pos, &cur_pos, new_node);
        SKIP_SPACES(cur_pos);
        
        new_node->right = NodeReader<treeElem_T>(cur_pos, &cur_pos, new_node);
        SKIP_SPACES(cur_pos);
        
        if (*cur_pos == ')') cur_pos++;
        if (next_pos) *next_pos = cur_pos;

        return new_node;
    }
    else if (strncmp(cur_pos, "nil", 3) == 0)
    {
        cur_pos += 3;
        if (next_pos) *next_pos = cur_pos;
        return NULL;
    }
    
    if (next_pos) *next_pos = cur_pos;
    return NULL;
}

#undef SKIP_SPACES


template <typename treeElem_T>
WolfErr_t DataReader(const char *src, tree_t<treeElem_T> *tree)
{
    FILE *data = fopen(src, "r");
    if (IS_BAD_PTR(data)) return WOLF_ERROR;

    ssize_t file_size = SizeFile(data);
    if (file_size < 0) return WOLF_ERROR;

    char* buffer = (char*)calloc((size_t)file_size + 1, sizeof(char));
    if (IS_BAD_PTR(buffer)) return WOLF_ERROR;

    size_t len_buffer = fread(buffer, sizeof(char), (size_t)file_size, data);
    buffer[len_buffer] = '\0';

    fclose(data);

    char* next_pos = buffer;
    tree->root = NodeReader<treeElem_T>(buffer, &next_pos, NULL);
    ON_DEBUG( if (IS_BAD_PTR(tree->root)) return WOLF_ERROR;)

    return WOLF_SUCCESS;
}


template <typename treeElem_T>
WolfErr_t PreOrder(FILE *data, const node_t<treeElem_T> *node)
{
    ON_DEBUG( if (IS_BAD_PTR(data) || IS_BAD_PTR(node)) return WOLF_ERROR; )

    fprintf(data, "( \"%s\" ", node->item);
    
    if (!IS_BAD_PTR(node->left))  { if (PreOrder(data, node->left))  return WOLF_ERROR; }
    else fprintf(data, "nil ");
    if (!IS_BAD_PTR(node->right)) { if (PreOrder(data, node->right)) return WOLF_ERROR; }
    else fprintf(data, "nil ");

    fprintf(data, ") ");

    return WOLF_SUCCESS;
}


template <typename treeElem_T>
WolfErr_t DataUpdate(const char *src, tree_t<treeElem_T> *tree)
{
    ON_DEBUG( if (IS_BAD_PTR(src) || IS_BAD_PTR(tree)) return WOLF_ERROR; )

    FILE *data = fopen(src, "w");
    if (IS_BAD_PTR(data)) return WOLF_ERROR;

    PreOrder(data, tree->root);

    fclose(data);
    return WOLF_SUCCESS;
}