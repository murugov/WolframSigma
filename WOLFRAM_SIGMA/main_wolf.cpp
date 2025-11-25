#include "wolfram.h"

#define PATH_TO_LOGFILE "WOLFRAM_SIGMA/ReportFiles/LogFile.log"
#define PATH_TO_DATA    "WOLFRAM_SIGMA/src/data.txt"


int main()
{
    LogFileOpener(PATH_TO_LOGFILE);

    tree_t *wolf_tree = {0};
    WolfCtor(&wolf_tree);

    DataReader(PATH_TO_DATA, wolf_tree);

    EnterVar();

    tree_t *diff_tree = {0};
    WolfCtor(&diff_tree);
    node_t *diff_node = DerivativeNode(wolf_tree->root, HashStr("x"));
    diff_tree->root = diff_node;

    SimplifyTree(diff_tree);

    GenGraphs(wolf_tree, __func__);
    GenGraphs(diff_tree, __func__);

    // double res = CalcExpression(diff_tree->root);
    // printf("res = %g\n", res);

    WolfDtor(wolf_tree);    
    WolfDtor(diff_tree);    
    GenHTML();

    LogFileCloser();

    return 0;
}