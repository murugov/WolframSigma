#include "wolfram.h"

FILE *file_latex = NULL;

int main()
{
    LogFileOpener(PATH_TO_LOGFILE);

    // LatexFileOpener(PATH_TO_LATEX);

    tree_t *wolf_tree = {0};
    WolfCtor(&wolf_tree);

    DataReader(PATH_TO_DATA, wolf_tree);

    GenGraphs(wolf_tree, __func__);

    // TreeToLatex(wolf_tree);
    
    // EnterVar();
     
    // tree_t *diff_tree = {0};
    // WolfCtor(&diff_tree);
    // node_t *diff_node = NDerivativeNode(CopyNode(wolf_tree->root), HashStr("x"), 3);
    // diff_tree->root = diff_node;
    // SimplifyTree(diff_tree);
    // // TreeToLatex(diff_tree->root);
    // GenGraphs(diff_tree, __func__);
    // WolfDtor(diff_tree);

    // GenGraphs(diff_tree, __func__);

    // double res = CalcExpression(diff_tree->root);
    // printf("res = %g\n", res);

    // TaylorSeries(wolf_tree, "x", 0.0, 3);

    WolfDtor(wolf_tree);    
    GenHTML();

    // LatexFileCloser();
    LogFileCloser();

    return 0;
}