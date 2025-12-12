#ifndef STACK_HPP
#define STACK_HPP

#include "CONFIG.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "IsBadPtr.hpp"
#include "FormatSpecifier.hpp"
#include "logger.hpp"

#define MIN_STK_LEN 16
#define STK_POISON  0


enum StackErr_t
{
    STK_SUCCESS = 0,
    STK_ERROR   = 1
};

enum StackErrCodes_t
{
    STK_NO_ERRORS         = 0,
    STK_WRONG_CANARY      = 1 << 0,
    STK_WRONG_HASH        = 1 << 1,
    STK_BAD_STK_PTR       = 1 << 2,
    STK_BAD_DATA_PTR      = 1 << 3,
    STK_WRONG_SIZE        = 1 << 4,
    STK_WRONG_CAPACITY    = 1 << 5,
    STK_WRONG_REALLOC     = 1 << 6,
    STK_WRONG_REV_REALLOC = 1 << 7,
    STK_WRONG_POISON_VAL  = 1 << 8,
    STK_DATA_OVERFLOW     = 1 << 9,
    STK_ACCESS_EMPTY_DATA = 1 << 10,
    STK_DESTROYED         = 1 << 11
};

enum StackFunc
{
    STK_DEFUALT     = 0x00,
    STK_CTOR        = 0x01,
    STK_DTOR        = 0x02,
    STK_POP         = 0x03,
    STK_PUSH        = 0x04,
    STK_REALLOC     = 0x05,
    STK_REV_REALLOC = 0x06
};

enum StackCanary
{
    STK_CANARY_1 = 111,
    STK_CANARY_2 = 222,
    STK_CANARY_3 = 333,
    STK_CANARY_4 = 444
};


typedef size_t stk_err_t;
typedef size_t stk_canary_t;
typedef size_t stk_hash_t;


struct stack_id
{
    const char* name;
    const char* file;
    const char* func;
    int         line;
};

template <typename stackElem_T>
struct stk_t
{
    stk_canary_t canary_1;
    stackElem_T* data;
    ssize_t      size;
    ssize_t      capacity;
    stk_err_t    error;
    stack_id     id;
    stk_hash_t   hash;
    stk_canary_t canary_2;
};


template <typename stackElem_T>
StackErr_t StackInit(stk_t<stackElem_T> *stk, const char *name, const char *file, const char *func, int line);

template <typename stackElem_T>
StackErr_t ErrDetect(stk_t<stackElem_T> *stk, StackFunc IncomingFunc, const char *file, const char *func, int line);

template <typename stackElem_T>
StackErr_t StackCtor(stk_t<stackElem_T> *stk, ssize_t capacity);

template <typename stackElem_T>
StackErr_t StackDtor(stk_t<stackElem_T> *stk);

template <typename stackElem_T>
StackErr_t StackPush(stk_t<stackElem_T> *stk, const stackElem_T value);

template <typename stackElem_T>
StackErr_t StackPop(stk_t<stackElem_T> *stk, stackElem_T *last_value);

template <typename stackElem_T>
StackErr_t StackRealloc(stk_t<stackElem_T> *stk);

template <typename stackElem_T>
StackErr_t StackRevRealloc(stk_t<stackElem_T> *stk);

template <typename stackElem_T>
StackErr_t StackVerify(stk_t<stackElem_T> *stk, StackFunc IncomingFunc = STK_DEFUALT);

template <typename stackElem_T>
StackErr_t StackDump(stk_t<stackElem_T> *stk, const char *file, const char *func, int line);

template <typename stackElem_T>
    stk_hash_t HashFunc(stk_t<stackElem_T> *stk);


#define ERR_DETECT(stk, IncomingFunc) ErrDetect(stk, IncomingFunc, __FILE__, __func__, __LINE__)
#define ERR_CHECK(err_code) ((stk->error) & err_code) == err_code

#define STACK_CTOR(stk, capacity) StackInit(stk, #stk, __FILE__, __func__, __LINE__); StackCtor(stk, capacity)
#define STACK_DTOR(stk) StackDtor(stk)

#include "StackFunc.hpp"
#include "StackDump.hpp"
#include "StackVerify.hpp"

#endif