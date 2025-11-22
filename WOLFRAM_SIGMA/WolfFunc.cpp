#include "wolfram.h"
#include "OpInstrSet.cpp"


WolfErr_t WolfCtor(tree_t **tree)
{
    *tree = (tree_t*)calloc(1, sizeof(tree_t));
    if (IS_BAD_PTR(tree)) return WOLF_ERROR;

    TREE_INIT((*tree));

    // (*tree)->root = NewNode(0, ARG_NUM);

    return (IS_BAD_PTR((*tree)->root)) ? WOLF_ERROR: WOLF_SUCCESS;
}


WolfErr_t WolfDtor(tree_t *tree)
{
    ON_DEBUG( if (IS_BAD_PTR(tree)) {LOG(ERROR, "AKIN_BAD_TREE_PTR"); return WOLF_ERROR; } )

    FreeNodes(tree->root);
    return WOLF_SUCCESS;
}


TreeErr_t HashSearch(hash_t hash, size_t *index)
{
    ON_DEBUG( if (IS_BAD_PTR(index)) return TREE_ERROR; )

    op_t *found = (op_t*)bsearch(&hash, op_instr_set, LEN_INSTR_SET, sizeof(op_instr_set[0]), CmpForBinSearch);
    if (found == NULL)
        return TREE_ERROR;
    
    *index = (size_t)(found - op_instr_set);
    return TREE_SUCCESS;
}


int CmpForBinSearch(const void *a, const void *b)
{
    const hash_t *hash_a = (const hash_t*)a;
    const op_t   *op_b   = (const op_t*)b;
    if (*hash_a - op_b->hash > 0)
        return 1;
    if (*hash_a - op_b->hash < 0)
        return -1;
    return 0;
}