#include "wolfram.h"


int main(int argc, char *argv[])
{
    LogFileOpener(PATH_TO_LOGFILE);

    if (argc >= 3)
    {
        FILE *SourceFile = fopen(argv[1], "r");
        FILE *LatexFile  = fopen(argv[2], "w");

        if (IS_BAD_PTR(SourceFile))
        {
            printf(ANSI_COLOR_RED "Bad pointer \"%s\"\n" ANSI_COLOR_RESET, argv[1]);
            LOG(ERROR, "Bad pointer \"%s\"", argv[1]);
            return 1;
        }
        if (IS_BAD_PTR(LatexFile))
        {
            printf(ANSI_COLOR_RED "Bad pointer \"%s\"\n" ANSI_COLOR_RESET, argv[2]);
            LOG(ERROR, "Bad pointer \"%s\"", argv[2]);
            return 1;
        }

        // if (VerifyOpInstrSetSort() != SUCCESS)
        // {
        //     printf(ANSI_COLOR_RED "Not sorted op_instr_set!\n" ANSI_COLOR_RESET);
        //     printf(ANSI_COLOR_RED "TODO: generation OpInstrSet.cpp with using \"make run-gen\"!\n" ANSI_COLOR_RESET);
        //     LOG(ERROR, "Not sorted op_instr_set!");
        //     return 1;
        // }

        // LatexFileOpener(PATH_TO_LATEX);

        // tree_t *wolf_tree = NULL;
        // WolfCtor(&wolf_tree);

        // DataReader(PATH_TO_DATA, wolf_tree);

        // GenGraphs(wolf_tree->root, __func__);

        // TaylorSeries(wolf_tree, "x", 0.0, 7);

        // WolfDtor(wolf_tree);    
        // GenHTML();

        // LatexFileCloser();

        fclose(SourceFile);
        fclose(LatexFile);
    }
    else
        printf(ANSI_COLOR_RED "Incorrect transfer of input files\n" ANSI_COLOR_RESET);


    LogFileCloser();
    return 0;
}