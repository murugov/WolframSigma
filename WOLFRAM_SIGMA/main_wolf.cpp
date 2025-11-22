#include "wolfram.h"

#define PATH_TO_LOGFILE "WOLFRAM_SIGMA/ReportFiles/LogFile.log"
#define PATH_TO_DATA    "WOLFRAM_SIGMA/src/data.txt"


int main()
{
    LogFileOpener(PATH_TO_LOGFILE);

    tree_t *akin_tree = {0};
    WolfCtor(&akin_tree);

    DataReader(PATH_TO_DATA, akin_tree);
    // GenGraphs(akin_tree, __func__);

    // Akinator(akin_tree);

    // GenGraphs(akin_tree, __func__);

    // DataUpdate(PATH_TO_DATA, akin_tree);

    WolfDtor(akin_tree);    
    // GenHTML();

    LogFileCloser();

    return 0;
}