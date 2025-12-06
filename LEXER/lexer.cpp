#include "lexer.h" 

static token_t* peeked_token = NULL;
static token_t *ReadNum(lexer_t *lexer);
static token_t *ReadName(lexer_t *lexer);


lexerErr_t LexerInit(lexer_t* lexer, char** lines, int line_count, const char* file_name)
{
    ON_DEBUG( if (IS_BAD_PTR(lines) || IS_BAD_PTR(file_name)) return LEX_ERROR; )

    lexer->lines = lines;
    lexer->line_count = line_count;
    lexer->cur_line = 0;
    lexer->cur_col = 1;
    lexer->cur_pos = lines[0]; 
    lexer->file_name = strdup(file_name);

    return LEX_SUCCESS;
}


token_t *NewToken(type_t type, const char* start, int length, int line, int col)
{
    token_t *new_token = (token_t*)calloc(1, sizeof(token_t));

    new_token->type   = type;
    new_token->start  = start;
    new_token->length = length;
    new_token->line   = line;
    new_token->col    = col;

    return new_token;
}


lexerErr_t SkipSpaces(lexer_t* lexer)
{
    ON_DEBUG( if (IS_BAD_PTR(lexer)) return LEX_ERROR; )

    while (!(IS_END(lexer)))
    {
        while (*lexer->cur_pos == ' ' || *lexer->cur_pos == '\t') { lexer->cur_pos++; lexer->cur_col++; }
        
        if (*lexer->cur_pos == '\0')
        {
            lexer->cur_line++;
            if (lexer->cur_line < lexer->line_count)
            {
                lexer->cur_pos = lexer->lines[lexer->cur_line];
                lexer->cur_col = 1;
            } 
            else
                break;
        }
        else
            break;
    }

    return LEX_SUCCESS;
}


token_t *NextToken(lexer_t* lexer)
{
    if (peeked_token)
    {
        token_t* token = peeked_token;
        peeked_token = NULL;
        return token;
    }

    SkipSpaces(lexer);
    if (IS_END(lexer)) return NewToken(TOKEN_EOF, "", 0, lexer->cur_line + 1, lexer->cur_col);
    
    char cur_sym = *lexer->cur_pos;
    const char* start = lexer->cur_pos;
    int line = lexer->cur_line + 1;
    int col  = lexer->cur_col;
    
    switch (cur_sym)
    {
        case '(':
            lexer->cur_pos++;
            lexer->cur_col++;
            return NewToken(TOKEN_LPAREN, start, 1, line, col);

        case ')':
            lexer->cur_pos++;
            lexer->cur_col++;
            return NewToken(TOKEN_RPAREN, start, 1, line, col);

        case ',':
            lexer->cur_pos++;
            lexer->cur_col++;
            return NewToken(TOKEN_COMMA, start, 1, line, col);

        case '+':
            lexer->cur_pos++;
            lexer->cur_col++;
            return NewToken(TOKEN_ADD, start, 1, line, col);

        case '-':
            lexer->cur_pos++;
            lexer->cur_col++;
            return NewToken(TOKEN_SUB, start, 1, line, col);

        case '*':
            lexer->cur_pos++;
            lexer->cur_col++;
            return NewToken(TOKEN_MUL, start, 1, line, col);

        case '/':
            lexer->cur_pos++;
            lexer->cur_col++;
            return NewToken(TOKEN_DIV, start, 1, line, col);

        case '^':
            lexer->cur_pos++;
            lexer->cur_col++;
            return NewToken(TOKEN_POW, start, 1, line, col);

        case '=':
            lexer->cur_pos++;
            lexer->cur_col++;
            return NewToken(TOKEN_EQ, start, 1, line, col);

        case '$':
            lexer->cur_pos++;
            lexer->cur_col++;
            return NewToken(TOKEN_DOLLAR, start, 1, line, col);

        default:
            break;
    }
    
    int num_digits = 0;
    sscanf(lexer->cur_pos, "%*g%n", &num_digits);
    if (num_digits)
        return ReadNum(lexer);
    
    if (isalpha(cur_sym))
        return ReadName(lexer);
    
    lexer->cur_pos++;
    lexer->cur_col++;
    return NewToken(TOKEN_UNDEF, start, 1, line, col);
}


token_t *PeekToken(lexer_t* lexer)
{
    if (!peeked_token)
    {
        const char* saved_pos = lexer->cur_pos;
        int saved_line        = lexer->cur_line;
        int saved_col         = lexer->cur_col;
        
        peeked_token = NextToken(lexer);
        
        lexer->cur_pos  = saved_pos;
        lexer->cur_line = saved_line;
        lexer->cur_col  = saved_col;
    }

    return peeked_token;
}


static token_t *ReadNum(lexer_t *lexer)
{
    double tmp_num = 0.0;
    int num_digits = 0;
    if (sscanf(lexer->cur_pos, "%lg%n", &tmp_num, &num_digits) != 1) return NULL;

    token_t* token = NewToken(TOKEN_NUM, lexer->cur_pos, num_digits, lexer->cur_line + 1, lexer->cur_col);
    
    lexer->cur_pos += num_digits;
    lexer->cur_col += num_digits;
    
    return token;
}


static token_t *ReadName(lexer_t *lexer)
{
    const char* start = lexer->cur_pos;
    int line = lexer->cur_line + 1;
    int col = lexer->cur_col;
    
    while (isalnum(*lexer->cur_pos) || *lexer->cur_pos == '_')
    {
        lexer->cur_pos++;
        lexer->cur_col++;
    }
    
    int length = (int)(lexer->cur_pos - start);
    
    const char* saved_pos = lexer->cur_pos;
    int saved_col = lexer->cur_col;
    int saved_line = lexer->cur_line;
    
    token_t* next_token = PeekToken(lexer);
    
    lexer->cur_pos = saved_pos;
    lexer->cur_col = saved_col;
    lexer->cur_line = saved_line;
    
    if (next_token && next_token->type == TOKEN_LPAREN)
        return NewToken(TOKEN_FUNC, start, length, line, col);
    else
        return NewToken(TOKEN_VAR, start, length, line, col);
}