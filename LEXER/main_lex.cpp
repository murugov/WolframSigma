#include "parser.h"
#include "wolfram.h"

int main()
{
    LogFileOpener(PATH_TO_LOGFILE);

    FILE *SourceFile = fopen(PATH_TO_DATA, "r");
    if (!SourceFile) { printf("Error: Cannot open file %s\n", PATH_TO_DATA); return EXIT_FAILURE; }
    
    char *buffer = NULL;
    int count_lines = 0;
    char **arr_ptr = DataReader(SourceFile, buffer, &count_lines);
    fclose(SourceFile);
    
    if (IS_BAD_PTR(arr_ptr)) { printf("Error: Failed to read file\n"); return EXIT_FAILURE; }
    
    RemoveComments(arr_ptr, &count_lines);
    
    lexer_t *lexer = (lexer_t*)calloc(1, sizeof(lexer_t));
    if (IS_BAD_PTR(lexer)) return LEX_ERROR;

    if (LexerInit(lexer, arr_ptr, count_lines, PATH_TO_DATA) != LEX_SUCCESS)
    {
        printf("Error: Failed to initialize lexer\n");
        return EXIT_FAILURE;
    }

    parser_t* parser = parserCtor(lexer);
    if (IS_BAD_PTR(parser))
    {
        printf("Error: Failed to create parser\n");
        return EXIT_FAILURE;
    }

    printf("=== Parsing ===\n");
    node_t* ast = ParseGeneral(parser);
    
    if (ast)
    {
        printf(ANSI_COLOR_GREEN "Successfully parsed!\n" ANSI_COLOR_RESET);
        
        GenGraphs(ast, __func__);
        FreeNodes(ast);
    }
    else
        printf(ANSI_COLOR_RED "Parsing failed\n" ANSI_COLOR_RESET);
    
    parserDtor(parser);
    FreeLines(arr_ptr, count_lines);
    
    LogFileCloser();
    return EXIT_SUCCESS;
}