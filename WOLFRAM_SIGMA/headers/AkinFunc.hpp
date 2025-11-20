#include "wolfram.h"


template <typename treeElem_T>
WolfErr_t AkinCtor(tree_t<treeElem_T> **tree)
{
    *tree = (tree_t<treeElem_T>*)calloc(1, sizeof(tree_t<treeElem_T>));
    if (IS_BAD_PTR(tree)) return WOLF_ERROR;

    TREE_INIT((*tree));

    treeElem_T root_val = TREE_POISON;
    (*tree)->root = NewNode(root_val);

    return (IS_BAD_PTR((*tree)->root)) ? WOLF_ERROR: WOLF_SUCCESS;
}


template <typename treeElem_T>
WolfErr_t AkinDtor(tree_t<treeElem_T> *tree)
{
    ON_DEBUG( if (IS_BAD_PTR(tree)) {LOG(ERROR, "AKIN_BAD_TREE_PTR"); return WOLF_ERROR; } )

    FreeNodes(tree->root);
    return WOLF_SUCCESS;
}


template <typename treeElem_T>
WolfErr_t Akinator(tree_t<treeElem_T> *tree)
{
    ON_DEBUG( if (IS_BAD_PTR(tree)) {LOG(ERROR, "AKIN_BAD_TREE_PTR"); return WOLF_ERROR; } )
    
    if (tree->root == NULL) {
        printf(ANSI_COLOR_MAGENTA "Akinator went on vacation!\n" ANSI_COLOR_RESET);
        return WOLF_ERROR;
    }

    Speak("\aHello, human! My name is Stars Akinator.", ANSI_COLOR_MAGENTA);
    Speak("\aI can:\n"
          "1) guess the element you've guessed\n"
          "2) give a description of the element\n"
          "3) compare two elements", ANSI_COLOR_YELLOW);
    Speak("\aSelect one of the operating modes:", ANSI_COLOR_MAGENTA);
    
    int mode = 0;
    scanf("%d", &mode);
    while (mode != FIND_MODE && mode != DEFINE_MODE && mode != CMP_MODE)
    {
        Speak("\aInvalid input!", ANSI_COLOR_RED);
        scanf("%d", &mode);
        while (getchar() != '\n');
    }
    while (getchar() != '\n');

    SelectMode(mode, tree);

    return WOLF_SUCCESS;
}


template <typename treeElem_T>
WolfErr_t SelectMode(int mode, tree_t<treeElem_T> *tree)
{
    ON_DEBUG(if (IS_BAD_PTR(tree)) return WOLF_ERROR;)

    switch (mode)
    {
    case FIND_MODE:
        FindMode(tree);
        break;
    case DEFINE_MODE:
        DefineMode(tree);
        break;
    case CMP_MODE:
        CmpMode(tree);
        break;
    default:
        break;
    }

    return WOLF_SUCCESS;
}


template <typename treeElem_T>
node_t<treeElem_T>* NodeFind(const char* name, node_t<treeElem_T> *current)
{
    ON_DEBUG( if (IS_BAD_PTR(name) || IS_BAD_PTR(current)) return NULL; )

    if (strcmp(name, current->item) == 0) return current;

    node_t<treeElem_T>* found = NULL;

    if (current->left != NULL)
    {
        found = NodeFind(name, current->left);
        if (found != NULL) return found;
    }

    if (current->right != NULL)
    {
        found = NodeFind(name, current->right);
        if (found != NULL) return found;
    }

    return NULL;
}

template <typename treeElem_T>
WolfErr_t FindMode(tree_t<treeElem_T> *tree)
{
    ON_DEBUG( if (IS_BAD_PTR(tree)) return WOLF_ERROR; )
    
    node_t<treeElem_T> *current = tree->root;

    char *answer = NULL;
    size_t capacity = 0;
    ssize_t len = 0;
    
    while (current->left != NULL || current != NULL)
    {
        printf("Is it %s?\n", current->item);

        YesOrNo(&answer, &len, &capacity);

        if (strncmp(answer, "yes", 3) == 0)
        {
            if (current->left == NULL) break;
            current = current->left;
        }
        else
        {
            if (current->right == NULL) break;
            current = current->right;
        }
    }

    if (strncmp(answer, "yes", 3) == 0)
    {
        printf(ANSI_COLOR_GREEN "I knew it! I'm so smart!\n" ANSI_COLOR_RESET);
    }
    else
    {
        printf("What did you guess?\n");
        EnterAnswer(&answer, &len, &capacity);
        
        node_t<treeElem_T> *new_node_for_ans = NewNode(strdup(answer));
        
        printf("What is the difference between %s and %s?\n", answer, current->item);
        EnterAnswer(&answer, &len, &capacity);

        node_t<treeElem_T> *new_node_for_qst = NewNode(strdup(answer));
        node_t<treeElem_T> *old_node = current;
        
        new_node_for_qst->left = new_node_for_ans;
        new_node_for_qst->right = old_node;
        new_node_for_ans->parent = new_node_for_qst;
        new_node_for_qst->parent = old_node->parent;
        
        if (old_node->parent)
        {
            if (old_node->parent->left == old_node)
                old_node->parent->left = new_node_for_qst;
            else
                old_node->parent->right = new_node_for_qst;
        }
        else
            tree->root = new_node_for_qst;
        
        printf(ANSI_COLOR_GREEN "Successfully added new knowledge!\n" ANSI_COLOR_RESET);        
    }

    free(answer);
    return WOLF_SUCCESS;
}


template <typename treeElem_T>
WolfErr_t PathFinder(const char* name, tree_t<treeElem_T> *tree, stk_t<node_t<treeElem_T>*> *path)
{
    ON_DEBUG( if (IS_BAD_PTR(name) || IS_BAD_PTR(tree) || IS_BAD_PTR(path)) return WOLF_ERROR; )

    node_t<treeElem_T> *found = NodeFind(name, tree->root);
    if (found == NULL) return WOLF_ERROR;


    node_t<treeElem_T> *parent = found->parent;
    node_t<treeElem_T> *child  = found;

    StackPush(path, child);
    while (parent != NULL)
    {
        StackPush(path, parent);
        parent = parent->parent;
        child  = child->parent;
    }
    
    return WOLF_SUCCESS;
}


template <typename treeElem_T>
WolfErr_t DefineMode(tree_t<treeElem_T> *tree)
{
    ON_DEBUG( if (IS_BAD_PTR(tree)) return WOLF_ERROR; )
    
    Speak("\vPlease enter the name of the hidden item:", ANSI_COLOR_GREEN);
    char *name = NULL;
    size_t  capacity = 0;
    ssize_t len      = 0;

    if (EnterAnswer(&name, &len, &capacity) == WOLF_ERROR) return WOLF_ERROR;

    node_t<treeElem_T> *found = NodeFind(name, tree->root);
    if (found == NULL)
    {
        printf(ANSI_COLOR_RED "The node named \"%s\" is missing!\n" ANSI_COLOR_RESET, name);
        free(name);
        return WOLF_ERROR;
    }

    node_t<treeElem_T> *parent = found->parent;
    node_t<treeElem_T> *child  = found;
    while (parent != NULL)
    {
        printf(ANSI_COLOR_YELLOW "-%s is%s %s\n" ANSI_COLOR_RESET, found->item, (child == parent->right) ? " not" : "", parent->item);
        parent = parent->parent;
        child  = child->parent;
    }

    free(name);
    return WOLF_SUCCESS;
}


template <typename treeElem_T>
WolfErr_t CmpMode(tree_t<treeElem_T> *tree)
{
    ON_DEBUG( if (IS_BAD_PTR(tree)) return WOLF_ERROR; )

    char* name_1 = NULL;
    char* name_2 = NULL;

    size_t  capacity = 0;
    ssize_t len      = 0;

    printf("Enter the name of the first element:\n");
    if (EnterAnswer(&name_1, &len, &capacity)) return WOLF_ERROR;
    printf("Enter the name of the second element:\n");
    if (EnterAnswer(&name_2, &len, &capacity)) return WOLF_ERROR;

    stk_t<node_t<treeElem_T>*> path_1 = {};
    STACK_CTOR(&path_1, tree->size / 2 + 1);

    PathFinder(name_1, tree, &path_1);

    stk_t<node_t<treeElem_T>*> path_2 = {};
    STACK_CTOR(&path_2, tree->size / 2 + 1);

    PathFinder(name_2, tree, &path_2);

    node_t<treeElem_T> *current_1 = NULL;
    node_t<treeElem_T> *current_2 = NULL;

    StackPop(&path_1, &current_1);
    StackPop(&path_2, &current_2);

    if (path_1.size > 2 && path_2.size > 2)
    {
        while(current_1 == current_2 && path_1.size > 0 && path_2.size > 0)
        {
            printf(ANSI_COLOR_CYAN "-%s is %s, just like %s\n" ANSI_COLOR_RESET, name_1, current_1->item, name_2);
            StackPop(&path_1, &current_1);
            StackPop(&path_2, &current_2);
        }
    }
    else
    {
        StackPop(&path_1, &current_1);
        StackPop(&path_2, &current_2);
    }

    if (current_1->parent->left == current_1)
        printf(ANSI_COLOR_CYAN "-%s is %s, but %s is not\n" ANSI_COLOR_RESET, name_1, current_1->parent->item, name_2);
    else
        printf(ANSI_COLOR_CYAN "-%s is not %s, but %s is\n" ANSI_COLOR_RESET, name_1, current_1->parent->item, name_2);

    STACK_DTOR(&path_1);
    STACK_DTOR(&path_2);

    free(name_1);
    free(name_2);
    return WOLF_SUCCESS;
}