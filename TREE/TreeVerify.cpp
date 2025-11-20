#include "list.h"


ListErr_t ErrDetect(list_t *list, list_context context, const char *file, const char *func, size_t line)
{
    ON_DEBUG(
        if (IS_BAD_PTR(list))       return LST_ERROR;
        if (IS_BAD_PTR(list->data)) return LST_ERROR;
        if (IS_BAD_PTR(list->next)) return LST_ERROR;
        if (IS_BAD_PTR(list->prev)) return LST_ERROR;
    )

    ListErr_t err_verd = ListVerify(list, context);

    if (err_verd == LST_ERROR)
    {
        ListDump(list, context.FuncName, file, func, line);
        return LST_ERROR;
    }

    return LST_SUCCESS;
}

ListErr_t ListVerify(list_t *list, list_context context)
{
    ON_DEBUG(
        if (IS_BAD_PTR(list))       return LST_ERROR;
        if (IS_BAD_PTR(list->data)) return LST_ERROR;
        if (IS_BAD_PTR(list->next)) return LST_ERROR;
        if (IS_BAD_PTR(list->prev)) return LST_ERROR;
    )
    
    list->error = 0;

    if (list->canary_1 != CANARY_1 || list->canary_2 != CANARY_2)
        list->error |= WRONG_CANARY;

    if (list->hash != GetDataHash(list))
        list->error |= WRONG_HASH;

    if (list->size < 0)
        list->error |= WRONG_SIZE;

    if (list->capacity <= 0)
        list->error |= WRONG_CAPACITY;

    else if (list->size > list->capacity)
        list->error |= DATA_OVERFLOW;
    
    else if (list->first_free >= list->capacity || list->first_free <= 0)
        list->error |= WRONG_FIRST_FREE;

    else if (list->prev[list->first_free] != -1)
        list->error |= WRONG_FIRST_FREE;

    else
    {
        size_t num_rep = 0;
        ssize_t next_free = list->first_free;
        while (list->next[next_free] != 0 && num_rep <= (size_t)list->capacity)
        {
            if (list->prev[next_free] != -1)
            {
                list->error |= WRONG_FREE_ITEM;
                break;
            }
            next_free = list->next[next_free];
        }

        if (ERR_CHECK(list, WRONG_FREE_ITEM) == LST_SUCCESS && (list->next[next_free] != 0))
            list->error |= WRONG_LAST_FREE;
    }

    if (list->size == 0 && context.IncomingFunc == LST_DELETE_AFTER)
        list->error |= ACCESS_EMPTY_DATA;
    
    if (context.index < 0 || list->prev[context.index] == -1 || context.index >= list->capacity)
        list->error |= INVALID_INDEX;

    if (list->prev[context.index] == -1 && (context.IncomingFunc == LST_INSRT_AFTER || context.IncomingFunc == LST_DELETE_AFTER))
        list->error |= INVALID_INDEX;

    if (list->next[context.index] == 0 && context.IncomingFunc == LST_DELETE_AFTER)
        list->error |= INVALID_INDEX;

    ssize_t next_item = list->next[0];
    for (size_t i = 0; i < (size_t)list->capacity && next_item != 0; ++i)
    {
        if (list->next[next_item] >= list->capacity || list->next[next_item] < 0)
        {
            list->error |= WRONG_NEXT_SUB;
            break;
        }
        else if (list->prev[list->next[next_item]] == -1)
        {
            list->error |= WRONG_NEXT_SUB;
            break;
        }
        next_item = list->next[next_item];
    }
    if (next_item != 0)
        list->error |= WRONG_NEXT_SUB;

    ssize_t prev_item = list->prev[0];
    for (size_t i = 0; i < (size_t)list->capacity && prev_item != 0; ++i)
    {
        if (list->prev[prev_item] >= list->capacity || list->prev[prev_item] < 0)
        {
            list->error |= WRONG_PREV_SUB;
            break;
        }
        prev_item = list->next[prev_item];
    }
    if (prev_item != 0)
        list->error |= WRONG_PREV_SUB;

    return (list->error != 0) ? LST_ERROR : LST_SUCCESS;
}