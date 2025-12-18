#include <math.h>
#include <stdarg.h>
#include "calculations.h"
#include "generators.h"
#include "readtree.h"
#include <ctype.h>

static FILE* gl_tex;
static bool  flag_tex_out;

//-------------------------------------------------МАССИВ-ОПЕРАЦИЙ-ДЛЯ-ВЫВОДА-------------------------------------------------------------

struct operation
{
    const int    number;
    const int    args;
    const char*  gnuplot;
    const char*  teh_f;
};

static struct operation operations[] =
{
    { ADD   , 2, "+"    , "+"       },        { SUB   , 2, "-"    , "-"        },
    { MUL   , 2, "*"    , "\\cdot"  },        { DIV   , 2, "/"    , "\\frac"   },
    { POW   , 2, "**"   , "^"       },        { SIN   , 1, "sin"  , "\\sin"    },
    { COS   , 1, "cos"  , "\\cos"   },        { TG    , 1, "tan"  , "\\tan"    },
    { CTG   , 1, "1/tan", "\\cot"   },        { LN    , 1, "log"  , "\\ln"     },
    { LOG   , 2, "log"  , "\\log_"  },        { SH    , 1, "sinh" , "\\sinh"   },
    { CH    , 1, "cosh" , "\\cosh"  },        { ARSIN , 1, "asin" , "\\arcsin" },
    { ARCOS , 1, "acos" , "\\arccos"},        { ARTG  , 1, "atan" , "\\arctan" },
    { ARCTG , 1, "pi/2 - atan", "\\arccot"},  { EXP   , 1, "exp"  , "exp"      },
    { TH    , 1, "tanh" , "\\tanh"  },        { CTH   , 1, "1/tanh", "\\coth"  }
};




TreeErr_t GenerateGraphic (node_t* root, double* coefs, double point, char main_var, var* vars)
{
    if (!root || !vars)
    {
        D_PRINT("data = NULL\n");
        return DF_ERR;
    }


    FILE* plot = file_opener(stderr, "plot.plt", "w");
    if (!plot) return DF_ERR;

    fprintf(plot, "set terminal pngcairo size 1200,800 enhanced font \"Arial,12\"\n"
                  "set output \"graphic/function_plot.png\"\n\n");

    fprintf(plot, "set grid\n"
                  "set xlabel \"%c\"\n"
                  "set ylabel \"y\"\n"
                  "set title \"graphic = ", main_var);


    fprintf(plot, "set key top right box\n\n");


    FILE* data = file_opener (stderr, "tree_data.txt", "r");
    if (!data) return DF_ERR;

    double* scale = GetScale (data);

    fprintf(plot, "set xrange [%lg:%lg]\n\n", scale[0], scale[1]);
    fprintf(plot, "set yrange [%lg:%lg]\n\n", scale[2], scale[3]);

    free(scale);

    fprintf(plot, "set style line 1 lc rgb \"blue\" lw 2\n"
                  "set style line 2 lc rgb \"red\" lw 2\n"
                  "set style line 3 lc rgb \"green\" lw 2\n\n");


    fprintf(plot, "f(%c) = ", main_var);
    GnuplotDump(root, plot, main_var);
    fprintf(plot, "\n");

    if (coefs)
    {
        int n = (int)coefs[0];
        
        fprintf(plot, "df(%c) = %.15lg + %.15lg * (%c - %.15lg)\n\n", 
                main_var, coefs[1], coefs[2], main_var, point);

        fprintf(plot, "t(%c) = %.15lg", main_var, coefs[1]);
        
        for (int i = 2; i <= n + 1; i++)
        {
            if (fabs(coefs[i]) > TOLOW) 
            {
                int power = i - 1;
                
                if (fabs(point) < TOLOW)
                {
                    fprintf(plot, " + %.15lg * %c**%d", coefs[i], main_var, power);
                }
                else
                {
                    fprintf(plot, " + %.15lg * (%c - %.15lg)**%d", 
                            coefs[i], main_var, point, power);
                }
            }
        }

        fprintf(plot, "\n\n");
    }

    fprintf(plot, "\n\n");

    fprintf(plot, "plot f(%c) with lines ls 1 title \"f(x)\", \\\n"
                  "df(%c) with lines ls 2 title \"f'(x)\", \\\n"
                  "t(%c) with lines ls 3 title \"Ряд Тейлора t(x)\"\n\n"
                  "set output\n", main_var, main_var, main_var );

    fclose(plot);

    if (system("gnuplot plot.plt")) printf("ERROR in graphic generation\n\nBad range!");

    else printf("\ngraphic has been succsesfully generated\n");

    return DF_OK;
}


// Генерация формулы
void GnuplotDump(node_t* root, FILE* plot, char var)
{
    if (root == NULL)
    {   
        fprintf(stderr, "GnuplotDump: zero pointer root\n");
        return;
    }

    switch (root->type)
    {
    case OP:
        {
            int op_index = CheckOperation(root->val.op);
            if (op_index == UNREC_COMMAND)
            {
                fprintf(plot, "unrec command\n");
                break;
            }
            
            const char* gnu_name = operations[op_index].gnuplot;
            int args_count = operations[op_index].args;
            int operation  = operations[op_index].number;
            
            if (args_count == 1 && operation != EXP)
            {
                fprintf(plot, "%s(", gnu_name);
                GnuplotDump(root->left, plot, var);
                fprintf(plot, ")");
            }
            else if (args_count == 2 || operation == EXP) 
            {
                if (operation == DIV)
                {
                    fprintf(plot, "(");
                    GnuplotDump(root->left, plot, var);
                    fprintf(plot, ")/(");
                    GnuplotDump(root->right, plot, var);
                    fprintf(plot, ")");
                }
                else if (operation == LOG)
                {
                    fprintf(plot, "(log(");
                    GnuplotDump(root->right, plot, var);
                    fprintf(plot, "))/(log(");
                    GnuplotDump(root->left, plot, var);
                    fprintf(plot, "))");
                }
                else if (operation == POW)
                {
                    fprintf(plot, "(");
                    GnuplotDump(root->left, plot, var);
                    fprintf(plot, ")**(");
                    GnuplotDump(root->right, plot, var);
                    fprintf(plot, ")");
                }
                else if (operation == EXP)
                {
                    fprintf(plot, "exp(");
                    GnuplotDump(root->left, plot, var);
                    fprintf(plot, ")");
                }
                else
                {
                    fprintf(plot, "(");
                    GnuplotDump(root->left, plot, var);
                    fprintf(plot, " %s ", gnu_name);
                    GnuplotDump(root->right, plot, var);
                    fprintf(plot, ")");
                }
            }
        }
        break;
    
    case VAR:
        fprintf(plot, "%c", var);
        break;

    case NUM:
        {
            double val = root->val.num;
            
            if (fabs(val - round(val)) < TOLOW && fabs(val) < TOLOW)
            {
                fprintf(plot, "%.0f", val);
            }
            else
            {
                fprintf(plot, "%lg", val);
            }
        }
        break;
    
    default:
        fprintf(stderr, "GnuplotDump: unknown node type %d\n", root->type);
        break;
    }
}


// Получение масштаба
double* GetScale(FILE* fp)
{
    char* text = NULL;
    file_read(fp, &text);

    char* begin = text;

    double* scale = (double*)calloc(4, sizeof(double));

    while (*text != '\n' && *text != '\0') text++;
    SYNTAX_SCALE(*text == '\0');
    text++;

    while (*text != '\0' && !isdigit(*text) && 
           *text != '-' && *text != '+' && *text != '.') text++;
    SYNTAX_SCALE(*text == '\0');

    int result1 = sscanf(text, "[%lf : %lf]", &scale[0], &scale[1]);
    if (result1 < 2) result1 = sscanf(text, "%lf : %lf", &scale[0], &scale[1]);
    SYNTAX_SCALE(result1 < 2);

    while (*text != '\n' && *text != '\0') text++;
    SYNTAX_SCALE(*text == '\0');
    text++;

    while (*text != '\0' && !isdigit(*text) && 
           *text != '-' && *text != '+' && *text != '.') text++;
    SYNTAX_SCALE(*text == '\0');

    int result2 = sscanf(text, "[%lf : %lf]", &scale[2], &scale[3]);
    if (result2 < 2) result2 = sscanf(text, "%lf : %lf", &scale[2], &scale[3]);
    SYNTAX_SCALE(result2 < 2);
    
    free(begin);
    return scale;
}