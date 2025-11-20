#include "tree.h"


int number_graph = 0;

TreeErr_t GenHTML()
{
    FILE *GraphsHTML = fopen("TREE/ReportFiles/GraphsHTML.html", "w");
    if (IS_BAD_PTR(GraphsHTML)) return TREE_ERROR;

    fprintf(GraphsHTML, "<!DOCTYPE html>\n"
                        "<html>\n"
                        "<head>\n"
                        "\t<title>TreeDump</title>\n"
                        "\t<style>\n"
                        "\t\tbody {\n"
                        "\t\t\tfont-family: Arial;\n"
                        "\t\t\tdisplay: flex;\n"
                        "\t\t\tflex-direction: column;\n"
                        "\t\t\talign-items: center;\n"
                        "\t\t\tpadding: 20px;\n"
                        "\t\t\tbackground-color: #f5f5f5;\n"
                        "\t\t}\n"
                        "\n"
                        "\t\t.graph-container {\n"
                        "\t\t\tmargin-top: 20px;\n"
                        "\t\t\tbackground: white;\n"
                        "\t\t\tpadding: 20px;\n"
                        "\t\t\tborder-radius: 8px;\n"
                        "\t\t\tbox-shadow: 0 2px 10px rgba(0,0,0,0.1);\n"
                        "\t\t\tmax-width: auto;\n"
                        "\t\t}\n"
                        "\n"
                        "\t\t.graph-container img {\n"
                        "\t\t\tmax-width: 100%%;\n"
                        "\t\t\theight: auto;\n"
                        "\t\t\tborder-radius: 4px;\n"
                        "\t\t}\n"
                        "\n"
                        "\t\t.graph-row {\n"
                        "\t\t\talign-items: center;\n"
                        "\t\t\tgap: 10px;\n"
                        "\t\t}\n"
                        "\t</style>\n"
                        "</head>\n"
                        "<body>\n"
                        "\t<h1 text-align: center;>TreeDump</h1>\n");

    for (int i = 0; i < number_graph; ++i)
    {
        fprintf(GraphsHTML, "\t<div class=\"graph-container\" id=\"graph%d\">\n", i);
        fprintf(GraphsHTML, "\t\t<div class=\"graph-row\">\n");
        fprintf(GraphsHTML, "\t\t\t<h4>graph%d/tree.dot.png:</h4>\n", i);
        fprintf(GraphsHTML, "\t\t\t<img src=\"graphs/graph%d/tree.dot.png\">\n", i);
        fprintf(GraphsHTML, "\t\t</div>\n");
        fprintf(GraphsHTML, "\t</div>\n");
    }

    fprintf(GraphsHTML, "</body>\n"
                        "</html>");

    fclose(GraphsHTML);
    return TREE_SUCCESS;
}