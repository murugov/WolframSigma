#ifndef WOLFRAM_H
#define WOLFRAM_H

#include "tree.h"
#include "colors.h"
#include "Speak.h"
// #include "sfml_func.h"


enum WolfErr_t
{
    WOLF_SUCCESS = 0,
    WOLF_ERROR   = 1
};

enum Modes
{
    FIND_MODE   = 1,
    DEFINE_MODE = 2,
    CMP_MODE    = 3
};


// struct wolfram
// {
//     type_t type;

// };


template <typename treeElem_T>
WolfErr_t DataReader(const char *src, tree_t<treeElem_T> *tree);

template <typename treeElem_T>
WolfErr_t DataUpdate(const char *src, tree_t<treeElem_T> *tree);

template <typename treeElem_T>
WolfErr_t PreOrder(FILE *data, const node_t<treeElem_T> *node);

template <typename treeElem_T>
node_t<treeElem_T>* NodeReader(char* cur_pos, char** next_pos, node_t<treeElem_T> *parent);

WolfErr_t EnterAnswer(char** answer, ssize_t *len, size_t *capacity);
WolfErr_t YesOrNo(char** answer, ssize_t *len, size_t *capacity);
int NameNodeReader(char* cur_pos);

template <typename treeElem_T>
WolfErr_t AkinCtor(tree_t<treeElem_T> **tree);

template <typename treeElem_T>
WolfErr_t AkinDtor(tree_t<treeElem_T> *tree);

template <typename treeElem_T>
WolfErr_t Akinator(tree_t<treeElem_T> *tree);

template <typename treeElem_T>
WolfErr_t SelectMode(int mode, tree_t<treeElem_T> *tree);

template <typename treeElem_T>
WolfErr_t FindMode(tree_t<treeElem_T> *tree);

template <typename treeElem_T>
WolfErr_t DefineMode(tree_t<treeElem_T> *tree);

template <typename treeElem_T>
WolfErr_t CmpMode(tree_t<treeElem_T> *tree);

template <typename treeElem_T>
node_t<treeElem_T>* NodeFind(const char* name, node_t<treeElem_T> *current);

template <typename treeElem_T>
WolfErr_t PathFinder(const char* name, tree_t<treeElem_T> *tree, stk_t<node_t<treeElem_T>*> *path);

#include "AkinFunc.hpp"
#include "DataReader.hpp"

#endif