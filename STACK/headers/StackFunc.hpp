#include "stack.hpp"

#ifndef STACK_FUNC_HPP
#define STACK_FUNC_HPP


template <typename stackElem_T>
StackErr_t StackInit(stk_t<stackElem_T> *stk, const char *name, const char *file, const char *func, int line)
{
    ON_DEBUG( if (IS_BAD_PTR(stk)) { LOG(ERROR, "STK_BAD_STK_PTR"); return STK_ERROR; } )
    
    (*stk).id.name = name;
    (*stk).id.file = file;
    (*stk).id.func = func;
    (*stk).id.line = line;

    ON_DEBUG( LOG(DEBUG, "Created id for stack \"%s\"", stk->id.name); )
    return STK_SUCCESS;
}


template <typename stackElem_T>
StackErr_t StackCtor(stk_t<stackElem_T> *stk, ssize_t capacity)
{   
    ON_DEBUG( if (IS_BAD_PTR(stk)) { LOG(ERROR, "STK_BAD_STK_PTR"); return STK_ERROR; } )

    if (capacity < 0) { stk->error = STK_WRONG_CAPACITY; LOG(ERROR, "STK_WRONG_CAPACITY"); return STK_ERROR; }

    stk->canary_1 = STK_CANARY_1;

    stk->size     = 0;
    stk->capacity = capacity;

    stk->data = (stackElem_T*)calloc((size_t)stk->capacity, sizeof(stackElem_T));
    if (IS_BAD_PTR(stk->data)) { stk->error |= STK_BAD_DATA_PTR; LOG(ERROR, "STK_BAD_DATA_PTR"); return STK_ERROR; }

    for (size_t i = 0; i < (size_t)stk->capacity; ++i)
        stk->data[i] = STK_POISON;

    stk->hash = HashFunc(stk);
    ON_DEBUG( if(stk->hash == 0) { stk->error |= STK_WRONG_HASH; LOG(ERROR, "STK_WRONG_HASH"); return STK_ERROR; } )
    ON_DEBUG( LOG(INFO, "Hash updated"); )

    stk->canary_2 = STK_CANARY_2;

    ON_DEBUG( LOG(INFO, "Stack \"%s\" created", stk->id.name); )
    return STK_SUCCESS;
}


template <typename stackElem_T>
StackErr_t StackPush(stk_t<stackElem_T> *stk, const stackElem_T value)
{
    ON_DEBUG( if (IS_BAD_PTR(stk)) { LOG(ERROR, "STK_BAD_STK_PTR"); return STK_ERROR; } )
    ON_ERR_FIND( if (ERR_DETECT(stk, STK_PUSH)) return STK_ERROR; )

    if ((size_t)stk->size + 1 > (size_t)stk->capacity)
    {
        if (StackRealloc(stk) == STK_ERROR)
        {
            stk->error |= STK_WRONG_REALLOC;
            LOG(ERROR, "STK_WRONG_REALLOC");
            return STK_ERROR; 
        }
    }

    stk->data[(size_t)stk->size] = value;
    stk->size++;

    ON_DEBUG( 
        char str_value[32] = {0};
        snprintf(str_value, sizeof(str_value), FORMAT_SPECIFIER(value), value);
        LOG(DEBUG, "PUSH %s in \"%s\"", str_value, stk->id.name);
    )

    stk->hash = HashFunc(stk);
    ON_DEBUG( if(stk->hash == 0) { stk->error |= STK_WRONG_HASH; LOG(ERROR, "STK_WRONG_HASH"); return STK_ERROR; } )
    ON_DEBUG( LOG(INFO, "Hash updated"); )

    return STK_SUCCESS;
}


template <typename stackElem_T>
StackErr_t StackPop(stk_t<stackElem_T> *stk, stackElem_T *last_value)
{
    ON_DEBUG( if (IS_BAD_PTR(stk)) { LOG(ERROR, "STK_BAD_STK_PTR"); return STK_ERROR; } )
    ON_ERR_FIND( if (ERR_DETECT(stk, STK_POP)) return STK_ERROR; )

    if ((size_t)stk->size - 1 < (size_t)stk->capacity / 2 && (size_t)stk->capacity / 2 > MIN_STK_LEN)
    {
        if (StackRevRealloc(stk) == STK_ERROR)
        {
            stk->error |= STK_WRONG_REV_REALLOC;
            LOG(ERROR, "STK_WRONG_REV_REALLOC");
            return STK_ERROR; 
        }
    }

    stk->size--;
    *last_value = stk->data[(size_t)stk->size];
    stk->data[(size_t)stk->size] = STK_POISON;

    stk->hash = HashFunc(stk);
    ON_DEBUG( if(stk->hash == 0) { stk->error |= STK_WRONG_HASH; LOG(ERROR, "STK_WRONG_HASH"); return STK_ERROR; } )
    ON_DEBUG( LOG(INFO, "Hash updated"); )

    ON_DEBUG(
        char str_value[32] = {0};
        snprintf(str_value, sizeof(str_value), FORMAT_SPECIFIER(*last_value), *last_value);
        LOG(DEBUG, "POP %s from \"%s\"", str_value, stk->id.name);
    )

    return STK_SUCCESS;
}


template <typename stackElem_T>
StackErr_t StackRealloc(stk_t<stackElem_T> *stk)
{
    ON_DEBUG( if (IS_BAD_PTR(stk)) { LOG(ERROR, "STK_BAD_STK_PTR"); return STK_ERROR; } )
    ON_ERR_FIND( if (ERR_DETECT(stk, STK_REALLOC)) { LOG(ERROR, "STK_WRONG_REALLOC"); return STK_ERROR; } )

    ssize_t new_capacity = stk->capacity << 1;
    stackElem_T *new_data = (stackElem_T*)realloc(stk->data, (size_t)new_capacity * sizeof(stackElem_T));
    if(IS_BAD_PTR(new_data)) { stk->error |= STK_WRONG_REALLOC; LOG(ERROR, "STK_WRONG_REALLOC"); return STK_ERROR; }

    for (size_t i = (size_t)stk->capacity; i < (size_t)new_capacity; ++i)
        new_data[i] = STK_POISON;
    
    stk->capacity = new_capacity;
    stk->data     = new_data;

    ON_DEBUG( LOG(INFO, "Stack \"%s\" reallocated", stk->id.name); )

    stk->hash = HashFunc(stk);
    ON_DEBUG( if(stk->hash == 0) { stk->error |= STK_WRONG_HASH; LOG(ERROR, "STK_WRONG_HASH"); return STK_ERROR; } )
    ON_DEBUG( LOG(INFO, "Hash updated"); )

    return STK_SUCCESS;
}


template <typename stackElem_T>
StackErr_t StackRevRealloc(stk_t<stackElem_T> *stk)
{
    ON_DEBUG( if (IS_BAD_PTR(stk)) { LOG(ERROR, "STK_BAD_STK_PTR"); return STK_ERROR; } )
    ON_ERR_FIND( if (ERR_DETECT(stk, STK_REV_REALLOC)) { LOG(ERROR, "STK_WRONG_REV_REALLOC"); return STK_ERROR; } )

    ssize_t new_capacity = stk->capacity >> 1;
    stackElem_T *new_data = (stackElem_T*)realloc(stk->data, (size_t)new_capacity * sizeof(stackElem_T));
    if(IS_BAD_PTR(new_data)) { stk->error |= STK_WRONG_REV_REALLOC; LOG(ERROR, "STK_WRONG_REV_REALLOC"); return STK_ERROR; }

    for (size_t i = (size_t)stk->size; i < (size_t)new_capacity; ++i)
        new_data[i] = STK_POISON;
    
    stk->capacity = new_capacity;
    stk->data     = new_data;

    ON_DEBUG( LOG(INFO, "Stack \"%s\" reverse reallocated", stk->id.name); )

    stk->hash = HashFunc(stk);
    ON_DEBUG( if(stk->hash == 0) { stk->error |= STK_WRONG_HASH; LOG(ERROR, "STK_WRONG_HASH"); return STK_ERROR; } )
    ON_DEBUG( LOG(INFO, "Hash updated"); )

    return STK_SUCCESS;
}


template <typename stackElem_T>
StackErr_t StackDtor(stk_t<stackElem_T> *stk)
{
    ON_DEBUG( 
        if (IS_BAD_PTR(stk)) { LOG(ERROR, "STK_BAD_STK_PTR"); return STK_ERROR; }
        if (IS_BAD_PTR(stk->data)) { LOG(ERROR, "STK_BAD_DATA_PTR"); return STK_ERROR; }
    )

    free(stk->data);
    stk->size = -1;
    stk->capacity = -1;
    stk->error = STK_DESTROYED;
    stk->hash = 0;
    
    ON_DEBUG( LOG(INFO, "Stack \"%s\" destroed", stk->id.name); )
    
    stk->id.name = NULL;
    stk->id.file = NULL;
    stk->id.func = NULL;
    stk->id.line = 0;

    return STK_SUCCESS;
}


template <typename stackElem_T>
stk_hash_t HashFunc(stk_t<stackElem_T> *stk)
{
    ON_DEBUG( if (IS_BAD_PTR(stk) || IS_BAD_PTR(stk->data)) return 0; )
    
    stk_hash_t new_hash = 0;

    for (size_t i = 0; i < (size_t)stk->capacity; ++i)
        new_hash = (new_hash << 5) - new_hash + (stk_hash_t)(stk->data[i]);

    new_hash = (new_hash << 5) - new_hash + (stk_hash_t)(stk->size);
    new_hash = (new_hash << 5) - new_hash + (stk_hash_t)(stk->capacity);

    return new_hash;
}

#endif