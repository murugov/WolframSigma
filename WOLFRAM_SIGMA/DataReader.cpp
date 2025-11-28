#include "wolfram.h"
#include "SizeFile.h"
#include "LineCounter.h"


node_t* NodeReader(char* cur_pos, char** next_pos, node_t *parent)
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

        ArgTypes type = DetType(cur_pos);
        node_t *new_node = NewNode(type, cur_pos, NULL, NULL);
        if (IS_BAD_PTR(new_node)) return NULL;

        cur_pos += len_name + 1;
        SKIP_SPACES(cur_pos);

        new_node->parent = parent;
        
        new_node->left = NodeReader(cur_pos, &cur_pos, new_node);
        SKIP_SPACES(cur_pos);
        
        new_node->right = NodeReader(cur_pos, &cur_pos, new_node);
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


int NameNodeReader(char* cur_pos)
{
    ON_DEBUG( if (cur_pos == NULL) return 0; )

    int len_name = 0;
    sscanf(cur_pos, "\"%*[^\"]\"%n", &len_name);

    return len_name - 2;
}


WolfErr_t DataReader(const char *src, tree_t *tree)
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
    tree->root = NodeReader(buffer, &next_pos, NULL);
    ON_DEBUG( if (IS_BAD_PTR(tree->root)) return WOLF_ERROR;)

    return WOLF_SUCCESS;
}


// WolfErr_t InOrder(FILE *data, const node_t *node)
// {
//     ON_DEBUG( if (IS_BAD_PTR(data) || IS_BAD_PTR(node)) return WOLF_ERROR; )

//     fprintf(data, "( \"%s\" ", node->item);
    
//     if (!IS_BAD_PTR(node->left))  { if (InOrder(data, node->left))  return WOLF_ERROR; }
//     else fprintf(data, "nil ");
//     if (!IS_BAD_PTR(node->right)) { if (InOrder(data, node->right)) return WOLF_ERROR; }
//     else fprintf(data, "nil ");

//     fprintf(data, ") ");

//     return WOLF_SUCCESS;
// }