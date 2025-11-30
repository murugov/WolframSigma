#include "wolfram.h"
#include "OpInstrSet.cpp"


int GetOpPriority(hash_t op_hash)
{
    if (op_hash == HASH_ADD || op_hash == HASH_SUB) return 1;
    if (op_hash == HASH_MUL || op_hash == HASH_DIV) return 2;
    if (op_hash == HASH_POW) return 3;
    return 0;
}


bool NeedParentheses(node_t *parent, node_t *child, bool is_left)
{
    if (parent == NULL || child == NULL) return false;
    
    if (child->type != ARG_OP) return false;
    
    hash_t parent_op = HashStr(parent->item.op);
    hash_t child_op  = HashStr(child->item.op);
    
    int parent_priority = GetOpPriority(parent_op);
    int child_priority  = GetOpPriority(child_op);
    
    if (child_priority < parent_priority) return true;
    
    if (parent_op == HASH_DIV || parent_op == HASH_POW)
    {
        if (!is_left && parent_op == HASH_POW) return true;
        if (child_priority == parent_priority) return true;
    }
    
    if ((parent_op == HASH_SUB || parent_op == HASH_DIV) && !is_left)
    {
        if (child_priority == parent_priority) return true;
    }
    
    return false;
}


void NodeToLatex(node_t *node, FILE *file_latex, node_t *parent, bool is_left)
{
    if (node == NULL) return;

    switch (node->type) {
        case ARG_NUM:
            fprintf(file_latex, "{%g}", node->item.num);
            break;

        case ARG_VAR:
            fprintf(file_latex, "%s", node->item.var);
            break;

        case ARG_OP:
        {
            hash_t op_hash = HashStr(node->item.op);
            size_t index   = 0;
            
            if (HashSearch(op_hash, &index) == TREE_SUCCESS)
            {
                const char* op_symbols = op_instr_set[index].name;

                if (op_hash == HASH_LOG)
                {
                    fprintf(file_latex, "\\log_{");
                    NodeToLatex(node->left, file_latex, node, true);
                    fprintf(file_latex, "}\\left(");
                    NodeToLatex(node->right, file_latex, node, false);
                    fprintf(file_latex, "\\right)");
                }
                else if (op_hash == HASH_E)
                {
                    fprintf(file_latex, "e^{");
                    NodeToLatex(node->right, file_latex, node, false);
                    fprintf(file_latex, "}");
                }
                else if (op_instr_set[index].num_args == 2)
                {
                    bool left_paren  = NeedParentheses(node, node->left, true);
                    bool right_paren = NeedParentheses(node, node->right, false);
                    
                    if (left_paren) fprintf(file_latex, "(");
                    NodeToLatex(node->left, file_latex, node, true);
                    if (left_paren) fprintf(file_latex, ")");
                    
                    if (op_hash == HASH_MUL)
                        fprintf(file_latex, " \\cdot ");
                    else if (op_hash == HASH_DIV)
                        fprintf(file_latex, " \\over ");
                    else {
                        fprintf(file_latex, " %s ", op_symbols);
                    }
                    
                    if (right_paren) fprintf(file_latex, "(");
                    NodeToLatex(node->right, file_latex, node, false);
                    if (right_paren) fprintf(file_latex, ")");
                }
                else if (op_instr_set[index].num_args == 1)
                {
                    fprintf(file_latex, "%s\\left(", op_symbols);
                    NodeToLatex(node->right, file_latex, node, false);
                    fprintf(file_latex, "\\right)");
                }
            }
            break;
        }
        
        default:
            fprintf(file_latex, "?");
            break;
    }
}

void TreeToLatex(tree_t *tree, const char *filename)
{
    if (IS_BAD_PTR(tree) || IS_BAD_PTR(filename)) return;

    FILE *file_latex = fopen(filename, "w");
    if (IS_BAD_PTR(file_latex)) { 
        printf(ANSI_COLOR_RED "Error: Cannot open file %s\n" ANSI_COLOR_RESET, filename); 
        return; 
    }

    fprintf(file_latex, "\\documentclass{article}\n");
    fprintf(file_latex, "\\usepackage[utf8]{inputenc}\n");
    fprintf(file_latex, "\\usepackage{amsmath}\n");
    fprintf(file_latex, "\\usepackage{amssymb}\n");
    fprintf(file_latex, "\\title{Expression Tree}\n");
    fprintf(file_latex, "\\author{Wolfram}\n");
    fprintf(file_latex, "\\date{\\today}\n");
    fprintf(file_latex, "\\begin{document}\n");
    fprintf(file_latex, "\\maketitle\n\n");

    fprintf(file_latex, "\\section{Mathematical Expression}\n");
    fprintf(file_latex, "\\[\n");
    
    if (tree->root) NodeToLatex(tree->root, file_latex);
    else fprintf(file_latex, "\\text{Empty tree}");
    
    fprintf(file_latex, "\n\\]\n\n");

    fprintf(file_latex, "\\end{document}\n");
    
    fclose(file_latex);
}