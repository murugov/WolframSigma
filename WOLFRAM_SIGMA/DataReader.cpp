#include "wolfram.h"
#include "SizeFile.h"
#include "LineCounter.h"
#include "lexer.hpp"


WolfErr_t DataReader(FILE *data, tree_t *tree)
{
    ON_DEBUG( if (IS_BAD_PTR(data)) return WOLF_ERROR; )

    ssize_t file_size = SizeFile(data);
    if (file_size < 0) return WOLF_ERROR;

    char* buffer = (char*)calloc((size_t)file_size + 1, sizeof(char));
    if (IS_BAD_PTR(buffer)) return WOLF_ERROR;

    size_t len_buffer = fread((void*)buffer, sizeof(char), (size_t)file_size, data);
    buffer[len_buffer] = '\0';
    
    const char* const_buffer = buffer;
    tree->root = GetGeneral(&const_buffer);
    ON_DEBUG( if (IS_BAD_PTR(tree->root)) return WOLF_ERROR;)

    return WOLF_SUCCESS;
}