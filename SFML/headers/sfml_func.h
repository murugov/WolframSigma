#ifndef SFML_FUNC_H
#define SFML_FUNC_H

#include <SFML/Graphics.h>
#include <SFML/Window.h>
#include <SFML/Audio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "akinator.h"


#define MAX_INPUT_LENGTH 32

struct TextField
{
    sfRectangleShape* background;
    sfText* text;
    char input[MAX_INPUT_LENGTH];
    sfBool isActive;
};

enum Verds
{
    YES = 0,
    NO  = 1
};


TextField* CreateTextField(float x, float y, float width, float height, sfFont* font);
void TextFieldClick(TextField* field, int mouseX, int mouseY);
void TextFieldInput(TextField* field, sfEvent event);
void DrawTextField(sfRenderWindow* window, TextField* field);
const char* GetTextFieldText(TextField* field);
void DestroyTextField(TextField* field);

int GameStart();
int SelectModeSFML();
int FindMode(const char* item);

void ShowResultScreen(sfRenderWindow* window, sfFont* font, const char* item, sfVector2u windowSize);
void MakeDifferenceBetweenNodes(sfRenderWindow* window, sfFont* font, const char* item, node_t* new_node_for_ans, node_t* current_node, tree_t* tree, sfVector2u windowSize);
void MakeNewNode(sfRenderWindow* window, sfFont* font, const char* item, sfVector2u windowSize, tree_t* tree, node_t* current_node);
int FinalFind(const char* item, tree_t* tree, node_t* current_node);

int DefineMode(tree_t* tree);

void CmpMode(tree_t* tree);

#endif