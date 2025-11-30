#include "wolfram.h"

#define PATH_TO_LOGFILE "WOLFRAM_SIGMA/ReportFiles/LogFile.log"
#define PATH_TO_DATA    "WOLFRAM_SIGMA/src/data.txt"
#define PATH_TO_LATEX   "WOLFRAM_SIGMA/ReportFiles/LatexDump.tex"

int main()
{
    LogFileOpener(PATH_TO_LOGFILE);

    tree_t *wolf_tree = {0};
    WolfCtor(&wolf_tree);

    DataReader(PATH_TO_DATA, wolf_tree);

    // EnterVar();
    
    // tree_t *diff_tree = {0};
    // WolfCtor(&diff_tree);
    // node_t *diff_node = NDerivativeNode(wolf_tree->root, HashStr("x"), 2);
    // diff_tree->root = diff_node;
    // SimplifyTree(diff_tree);
    // TreeToLatex(diff_tree, PATH_TO_LATEX);

    GenGraphs(wolf_tree, __func__);
    // GenGraphs(diff_tree, __func__);

    // double res = CalcExpression(diff_tree->root);
    // printf("res = %g\n", res);

    TaylorSeries(wolf_tree, "x", 0, 3);

    WolfDtor(wolf_tree);    
    // WolfDtor(diff_tree);    
    GenHTML();

    LogFileCloser();

    return 0;
}