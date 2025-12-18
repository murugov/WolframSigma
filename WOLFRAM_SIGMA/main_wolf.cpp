#include "wolfram.hpp"


int main(int argc, char *argv[])
{
    LogFileOpener(PATH_TO_LOGFILE);

    if (argc >= 3)
    {
        FILE *SourceFile = fopen(argv[1], "r");
        if (IS_BAD_PTR(SourceFile))
        {
            printf(ANSI_COLOR_RED "Bad pointer \"%s\"\n" ANSI_COLOR_RESET, argv[1]);
            LOG(ERROR, "Bad pointer \"%s\"", argv[1]);
            return EXIT_FAILURE;
        }

        FILE *LatexFile  = fopen(argv[2], "w");
        if (IS_BAD_PTR(LatexFile))
        {
            printf(ANSI_COLOR_RED "Bad pointer \"%s\"\n" ANSI_COLOR_RESET, argv[2]);
            LOG(ERROR, "Bad pointer \"%s\"", argv[2]);
            fclose(SourceFile);
            return EXIT_FAILURE;
        }

        if (VerifyOpInstrSetSort() != WOLF_SUCCESS)
        {
            printf(ANSI_COLOR_RED "Not sorted op_instr_set!\n" ANSI_COLOR_RESET);
            printf(ANSI_COLOR_RED "TODO: generation OpInstrSet.cpp with using \"make run-gen\"!\n" ANSI_COLOR_RESET);
            LOG(ERROR, "Not sorted op_instr_set in OpInstrSet.cpp!");
            return EXIT_FAILURE;
        }

        LatexFileOpener(PATH_TO_LATEX);
        
        parser_t *parser = (parser_t*)calloc(1, sizeof(parser_t));
        parserCtor(parser, argv[1]);

        node_t* ast = ParseWolf(parser);
        if (ast)
        {
            printf(ANSI_COLOR_GREEN "Successfully parsed\n" ANSI_COLOR_RESET);
            TaylorSeries(ast, 7);
            GenTrees(ast, __func__);
            FreeNodes(ast);
        }
        else
            printf(ANSI_COLOR_RED "Parsing failed!\n" ANSI_COLOR_RESET);    
    



        GenHTML("WolfDump");

        LatexFileCloser();

        fclose(SourceFile);
        fclose(LatexFile);
    }
    else
        printf(ANSI_COLOR_RED "Incorrect transfer of input files\n" ANSI_COLOR_RESET);


    LogFileCloser();
    return 0;
}