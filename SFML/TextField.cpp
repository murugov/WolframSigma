#include "sfml_func.h"

TextField* CreateTextField(float x, float y, float width, float height, sfFont* font) 
{
    TextField* field = (TextField*)malloc(sizeof(TextField));
    
    field->background = sfRectangleShape_create();
    sfRectangleShape_setSize(field->background, (sfVector2f){width, height});
    sfRectangleShape_setPosition(field->background, (sfVector2f){x, y});
    sfRectangleShape_setFillColor(field->background, sfWhite);
    sfRectangleShape_setOutlineColor(field->background, sfBlack);
    sfRectangleShape_setOutlineThickness(field->background, 2);
    
    field->text = sfText_create();
    sfText_setFont(field->text, font);
    sfText_setCharacterSize(field->text, 24);
    sfText_setFillColor(field->text, sfBlack);
    sfText_setPosition(field->text, (sfVector2f){x + 10, y + 8});
    
    field->input[0] = '\0';
    field->isActive = sfFalse;
    
    return field;
}

void TextFieldClick(TextField* field, int mouseX, int mouseY)
{
    sfFloatRect bounds = sfRectangleShape_getGlobalBounds(field->background);
    field->isActive = (mouseX >= bounds.left && mouseX <= bounds.left + bounds.width &&
                      mouseY >= bounds.top && mouseY <= bounds.top + bounds.height);
    
    if (field->isActive) {
        sfRectangleShape_setOutlineColor(field->background, sfMagenta);
    } else {
        sfRectangleShape_setOutlineColor(field->background, sfBlack);
    }
}

void TextFieldInput(TextField* field, sfEvent event)
{
    if (!field->isActive) return;
    
    if (event.type == sfEvtTextEntered)
    {
        if (event.text.unicode >= 32 && event.text.unicode < 128) 
        {
            if (strlen(field->input) < MAX_INPUT_LENGTH - 1) 
            {
                char newChar = (char)event.text.unicode;
                strncat(field->input, &newChar, 1);
                sfText_setString(field->text, field->input);
            }
        }
    }
    
    if (event.type == sfEvtKeyPressed && event.key.code == sfKeyBackspace)
    {
        size_t len = strlen(field->input);
        if (len > 0)
        {
            field->input[len - 1] = '\0';
            sfText_setString(field->text, field->input);
        }
    }
}

void DrawTextField(sfRenderWindow* window, TextField* field)
{
    sfRenderWindow_drawRectangleShape(window, field->background, NULL);
    sfRenderWindow_drawText(window, field->text, NULL);
}

const char* GetTextFieldText(TextField* field) { return field->input; }

void DestroyTextField(TextField* field)
{
    sfRectangleShape_destroy(field->background);
    sfText_destroy(field->text);
    free(field);
}