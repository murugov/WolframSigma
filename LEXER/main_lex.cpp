#include "parser.hpp"


#define PATH_TO_DATA "src/data.txt"   // first version, after I will use argc/argv

int main()
{
    LogFileOpener(PATH_TO_LOGFILE);

    FILE *SourceFile = fopen(PATH_TO_DATA, "r");
    if (!SourceFile) { printf("Error: Cannot open file %s\n", PATH_TO_DATA); return EXIT_FAILURE; }
    
    char *buffer = NULL;
    int count_lines = 0;
    char **lines = DataReader(SourceFile, buffer, &count_lines);
    fclose(SourceFile);
    
    if (IS_BAD_PTR(lines)) { printf("Error: Failed to read file\n"); return EXIT_FAILURE; }
    
    RemoveComments(lines, &count_lines);

    lexer_t *lexer = LexerCtor(lines, count_lines, __FILE__);
    if (IS_BAD_PTR(lexer))
    {
        printf("Error: Failed to initialize lexer\n");
        return EXIT_FAILURE;
    }

    printf("\nlexer:    [%p]\n", lexer);
    printf("----------------\n");
    printf("tokens:     [%p]\n", lexer->tokens);
    printf("cur_token:  [%d]\n", lexer->cur_token);
    printf("lines:      [%p]\n", lexer->lines);
    printf("line_count: [%d]\n", lexer->line_count);
    printf("cur_line:   [%d]\n", lexer->cur_line);
    printf("cur_col:    [%d]\n", lexer->cur_col);
    printf("cur_pos:    [%p]\n", lexer->cur_pos);
    printf("file_name:  [%p]\n", lexer->file_name);
    printf("----------------\n");

    // node_t* ast = ParseGeneral(lexer);
    
    // if (ast)
    // {
    //     printf(ANSI_COLOR_GREEN "Successfully parsed!\n" ANSI_COLOR_RESET);
        
    //     GenGraphs(ast, __func__);
    //     FreeNodes(ast);
    // }
    // else
    //     printf(ANSI_COLOR_RED "Parsing failed\n" ANSI_COLOR_RESET);
    
    // parserDtor(parser);
    
    StackDump(lexer->tokens, __FILE__, __func__, __LINE__);
    
    LexerDtor(lexer);
    FreeLines(lines, count_lines);
    LogFileCloser();
    return 0;
}