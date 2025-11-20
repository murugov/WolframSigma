#include "stack.h"

#ifndef STACK_VERIF_HPP
#define STACK_VERIF_HPP


template <typename stackElem_T>
StackErr_t ErrDetect(stk_t<stackElem_T> *stk, StackFunc IncomingFunc, const char */*file*/, const char */*func*/, int /*line*/)
{
    ON_DEBUG( if (IS_BAD_PTR(stk)) { LOG(ERROR, "STK_BAD_STK_PTR"); return STK_ERROR; } )

    StackErr_t err_verd = StackVerify(stk, IncomingFunc);
    if (err_verd == STK_ERROR) { /*StackDump(stk, file, func, line);*/ return STK_ERROR; }
    
    return STK_SUCCESS;
}


template <typename stackElem_T>
StackErr_t StackVerify(stk_t<stackElem_T> *stk, StackFunc IncomingFunc)
{
    ON_DEBUG( if (IS_BAD_PTR(stk)) { LOG(ERROR, "STK_BAD_STK_PTR"); return STK_ERROR; } )
    
    stk->error = STK_NO_ERRORS;

    if (stk->canary_1 != STK_CANARY_1 || stk->canary_2 != STK_CANARY_2)
        stk->error |= STK_WRONG_CANARY;

    if (HashFunc(stk) != stk->hash)
        stk->error |= STK_WRONG_HASH;

    if (IS_BAD_PTR(stk->data))
        stk->error |= STK_BAD_DATA_PTR;

    if (stk->size < 0 && IncomingFunc != STK_POP)
        stk->error |= STK_WRONG_SIZE;

    if (stk->size > stk->capacity)
        stk->error |= STK_DATA_OVERFLOW;

    if (stk->size - 1 < 0 && IncomingFunc == STK_POP)
        stk->error |= STK_ACCESS_EMPTY_DATA;

    if (stk->capacity < 0)
        stk->error |= STK_WRONG_CAPACITY;

    if (ERR_CHECK(STK_BAD_DATA_PTR) == STK_NO_ERRORS)
    {
        if (ERR_CHECK((STK_WRONG_SIZE | STK_WRONG_CAPACITY)) == STK_NO_ERRORS)
        {
            for(size_t i = (size_t)stk->size; i < (size_t)stk->capacity; ++i)
            {
                if (stk->data[i] != STK_POISON) { stk->error |= STK_WRONG_POISON_VAL; break; }
            }
        }
    }

    return (stk->error != STK_NO_ERRORS) ? STK_ERROR : STK_SUCCESS;
}

#endif