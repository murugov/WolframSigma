#include "sfml_func.h"
#include "akinator.h"


void ShowComparisonScreen(sfRenderWindow* window, sfFont* font, const char* name_1, const char* name_2, 
                         const char* comparison, sfVector2u windowSize)
{
    sfTexture* backgroundTexture = sfTexture_createFromFile("SFML/src/StarWars/background.png", NULL);
    sfSprite* backgroundSprite = sfSprite_create();
    
    if (backgroundTexture)
    {
        sfSprite_setTexture(backgroundSprite, backgroundTexture, sfTrue);
        
        sfVector2u bgSize = sfTexture_getSize(backgroundTexture);
        float scaleX = (float)windowSize.x / (float)bgSize.x;
        float scaleY = (float)windowSize.y / (float)bgSize.y;
        float scale = (scaleX < scaleY) ? scaleX : scaleY;
        
        sfSprite_setScale(backgroundSprite, (sfVector2f){scale, scale});
        
        float offsetX = ((float)windowSize.x - bgSize.x * scale) / 2.0f;
        float offsetY = ((float)windowSize.y - bgSize.y * scale) / 2.0f;
        sfSprite_setPosition(backgroundSprite, (sfVector2f){offsetX, offsetY});
    }

    char formattedname_1[32] = {0}, formattedname_2[32] = {0};
    strncpy(formattedname_1, name_1, sizeof(formattedname_1) - 1);
    strncpy(formattedname_2, name_2, sizeof(formattedname_2) - 1);
    
    for (char* i = formattedname_1; *i; ++i) if (*i == ' ') *i = '_';
    for (char* i = formattedname_2; *i; ++i) if (*i == ' ') *i = '_';

    char imagePath1[256] = {0};
    snprintf(imagePath1, sizeof(imagePath1), "SFML/src/StarWars/%s/%s.png", formattedname_1, formattedname_1);
    sfTexture* texture1 = sfTexture_createFromFile(imagePath1, NULL);
    sfSprite* sprite1 = sfSprite_create();
    
    if (texture1)
    {
        sfSprite_setTexture(sprite1, texture1, sfTrue);
        sfVector2u texSize1 = sfTexture_getSize(texture1);
        float scale = (float)windowSize.y * 0.3f / texSize1.y;
        sfSprite_setScale(sprite1, (sfVector2f){scale, scale});
        float posX = (float)windowSize.x * 0.1f;
        float posY = (float)windowSize.y * 0.2f;
        sfSprite_setPosition(sprite1, (sfVector2f){posX, posY});
    }

    char imagePath2[256] = {0};
    snprintf(imagePath2, sizeof(imagePath2), "SFML/src/StarWars/%s/%s.png", formattedname_2, formattedname_2);
    sfTexture* texture2 = sfTexture_createFromFile(imagePath2, NULL);
    sfSprite* sprite2 = sfSprite_create();
    
    if (texture2)
    {
        sfSprite_setTexture(sprite2, texture2, sfTrue);
        sfVector2u texSize2 = sfTexture_getSize(texture2);
        float scale = (float)windowSize.y * 0.3f / texSize2.y;
        sfSprite_setScale(sprite2, (sfVector2f){scale, scale});
        float posX = (float)windowSize.x * 0.7f;
        float posY = (float)windowSize.y * 0.2f;
        sfSprite_setPosition(sprite2, (sfVector2f){posX, posY});
    }

    sfText* nameText1 = sfText_create();
    sfText_setFont(nameText1, font);
    sfText_setString(nameText1, name_1);
    sfText_setCharacterSize(nameText1, 30);
    sfText_setFillColor(nameText1, sfYellow);
    sfText_setStyle(nameText1, sfTextBold);
    
    sfFloatRect nameBounds1 = sfText_getLocalBounds(nameText1);
    float nameX1 = (float)windowSize.x * 0.1f + (((float)windowSize.x * 0.2f - nameBounds1.width) / 2.0f);
    float nameY1 = (float)windowSize.y * 0.55f;
    sfText_setPosition(nameText1, (sfVector2f){nameX1, nameY1});

    sfText* nameText2 = sfText_create();
    sfText_setFont(nameText2, font);
    sfText_setString(nameText2, name_2);
    sfText_setCharacterSize(nameText2, 30);
    sfText_setFillColor(nameText2, sfYellow);
    sfText_setStyle(nameText2, sfTextBold);
    
    sfFloatRect nameBounds2 = sfText_getLocalBounds(nameText2);
    float nameX2 = (float)windowSize.x * 0.7f + (((float)windowSize.x * 0.2f - nameBounds2.width) / 2.0f);
    float nameY2 = (float)windowSize.y * 0.55f;
    sfText_setPosition(nameText2, (sfVector2f){nameX2, nameY2});

    sfText* comparisonText = sfText_create();
    sfText_setFont(comparisonText, font);
    sfText_setString(comparisonText, comparison);
    sfText_setCharacterSize(comparisonText, 20);
    sfText_setFillColor(comparisonText, sfGreen);
    sfText_setStyle(comparisonText, sfTextBold);
    sfText_setLineSpacing(comparisonText, 1.2f);
    
    sfFloatRect compBounds = sfText_getLocalBounds(comparisonText);
    float compX = ((float)windowSize.x - compBounds.width) / 2.0f;
    float compY = (float)windowSize.y * 0.6f;
    sfText_setPosition(comparisonText, (sfVector2f){compX, compY});

    sfText* instructionText = sfText_create();
    sfText_setFont(instructionText, font);
    sfText_setString(instructionText, "Press ESC to exit");
    sfText_setCharacterSize(instructionText, 30);
    sfText_setFillColor(instructionText, sfWhite);
    
    float instructionX = ((float)windowSize.x - 200) / 2.0f;
    float instructionY = (float)windowSize.y * 0.9f;
    sfText_setPosition(instructionText, (sfVector2f){instructionX, instructionY});

    char musicPath[256] = {0};
    snprintf(musicPath, sizeof(musicPath), "SFML/src/StarWars/imperial-march.mp3");
    sfMusic* music = sfMusic_createFromFile(musicPath);
    if (music) sfMusic_play(music);

    while (sfRenderWindow_isOpen(window))
    {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed || 
                (event.type == sfEvtKeyPressed && event.key.code == sfKeyEscape))
                sfRenderWindow_close(window);
        }

        sfRenderWindow_clear(window, sfBlack);
    
        if (backgroundTexture)
            sfRenderWindow_drawSprite(window, backgroundSprite, NULL);
        
        if (texture1) sfRenderWindow_drawSprite(window, sprite1, NULL);
        if (texture2) sfRenderWindow_drawSprite(window, sprite2, NULL);
        
        sfRenderWindow_drawText(window, nameText1, NULL);
        sfRenderWindow_drawText(window, nameText2, NULL);
        sfRenderWindow_drawText(window, comparisonText, NULL);
        sfRenderWindow_drawText(window, instructionText, NULL);
        
        sfRenderWindow_display(window);
    }

    if (backgroundTexture)
    {
        sfTexture_destroy(backgroundTexture);
        sfSprite_destroy(backgroundSprite);
    }
    if (texture1)
    {
        sfTexture_destroy(texture1);
        sfSprite_destroy(sprite1);
    }
    if (texture2)
    {
        sfTexture_destroy(texture2);
        sfSprite_destroy(sprite2);
    }
    sfText_destroy(nameText1);
    sfText_destroy(nameText2);
    sfText_destroy(comparisonText);
    sfText_destroy(instructionText);
    if (music) sfMusic_destroy(music);
}


char* BuildComparisonString(const char* name_1, const char* name_2, tree_t* tree)
{
    if (!name_1 || !name_2 || !tree) return NULL;

    stk_t<node_t*> path_1 = {};
    STACK_CTOR(&path_1, tree->size / 2 + 1);
    stk_t<node_t*> path_2 = {};
    STACK_CTOR(&path_2, tree->size / 2 + 1);

    if (PathFinder(name_1, tree, &path_1) != AKIN_SUCCESS || 
        PathFinder(name_2, tree, &path_2) != AKIN_SUCCESS)
    {
        STACK_DTOR(&path_1);
        STACK_DTOR(&path_2);
        return NULL;
    }

    char* comparison = (char*)calloc(1024, sizeof(char)); // 1024
    if (!comparison)
    {
        STACK_DTOR(&path_1);
        STACK_DTOR(&path_2);
        return NULL;
    }
    comparison[0] = '\0';
    int length = 0;

    node_t *current_1 = NULL, *current_2 = NULL;
    StackPop(&path_1, &current_1);
    StackPop(&path_2, &current_2);

    length += snprintf(comparison + length, (size_t)(1024 - length), "Comparison of %s and %s:\n\n", name_1, name_2);

    bool hasCommon = false;
    if (path_1.size > 2 && path_2.size > 2)
    {
        while(current_1 == current_2 && path_1.size > 0 && path_2.size > 0)
        {
            length += snprintf(comparison + length, (size_t)(1024 - length), "-%s is %s, just like %s\n", name_1, current_1->item, name_2);
            StackPop(&path_1, &current_1);
            StackPop(&path_2, &current_2);
            hasCommon = true;
        }
    }

    if (hasCommon)
        length += snprintf(comparison + length, (size_t)(1024 - length), "\n");

    if (current_1 && current_1->parent)
    {
        if (current_1->parent->left == current_1)
            length += snprintf(comparison + length, (size_t)(1024 - length), "-%s is %s\n %s is not\n", 
                             name_1, current_1->parent->item, name_2);
        else
            length += snprintf(comparison + length, (size_t)(1024 - length), "-%s is not %s\n %s is\n", 
                             name_1, current_1->parent->item, name_2);
    }

    STACK_DTOR(&path_1);
    STACK_DTOR(&path_2);

    return comparison;
}

void CmpMode(tree_t* tree)
{
    const sfVideoMode mode = sfVideoMode_getDesktopMode();
    sfRenderWindow* window = sfRenderWindow_create(mode, "Compare Mode - Star Wars Akinator", sfFullscreen, NULL);
    if (!window) return;

    sfVector2u windowSize = sfRenderWindow_getSize(window);
    
    sfFont* font = sfFont_createFromFile("SFML/src/Agzo.ttf");
    if (!font)
    {
        sfRenderWindow_destroy(window);
        return;
    }

    sfTexture* backgroundTexture = sfTexture_createFromFile("SFML/src/StarWars/background.png", NULL);
    sfSprite* backgroundSprite = sfSprite_create();
    
    if (backgroundTexture)
    {
        sfSprite_setTexture(backgroundSprite, backgroundTexture, sfTrue);
        sfVector2u bgSize = sfTexture_getSize(backgroundTexture);
        float scaleX = (float)windowSize.x / (float)bgSize.x;
        float scaleY = (float)windowSize.y / (float)bgSize.y;
        float scale = (scaleX < scaleY) ? scaleX : scaleY;
        sfSprite_setScale(backgroundSprite, (sfVector2f){scale, scale});
        float offsetX = ((float)windowSize.x - bgSize.x * scale) / 2.0f;
        float offsetY = ((float)windowSize.y - bgSize.y * scale) / 2.0f;
        sfSprite_setPosition(backgroundSprite, (sfVector2f){offsetX, offsetY});
    }

    float fieldWidth = 400.0f;
    float fieldHeight = 50.0f;
    float fieldY = (float)windowSize.y * 0.3f;
    
    TextField* textField1 = CreateTextField((float)windowSize.x * 0.1f, fieldY, fieldWidth, fieldHeight, font);
    TextField* textField2 = CreateTextField((float)windowSize.x * 0.55f, fieldY, fieldWidth, fieldHeight, font);

    sfText* titleText = sfText_create();
    sfText_setFont(titleText, font);
    sfText_setString(titleText, "Compare two characters:");
    sfText_setCharacterSize(titleText, 50);
    sfText_setFillColor(titleText, sfGreen);
    sfText_setStyle(titleText, sfTextBold);
    
    sfFloatRect titleBounds = sfText_getLocalBounds(titleText);
    float titleX = ((float)windowSize.x - titleBounds.width) / 2.0f;
    float titleY = (float)windowSize.y * 0.1f;
    sfText_setPosition(titleText, (sfVector2f){titleX, titleY});

    sfText* label1 = sfText_create();
    sfText_setFont(label1, font);
    sfText_setString(label1, "First character:");
    sfText_setCharacterSize(label1, 30);
    sfText_setFillColor(label1, sfYellow);
    sfText_setPosition(label1, (sfVector2f){(float)windowSize.x * 0.1f, fieldY - 40});

    sfText* label2 = sfText_create();
    sfText_setFont(label2, font);
    sfText_setString(label2, "Second character:");
    sfText_setCharacterSize(label2, 30);
    sfText_setFillColor(label2, sfYellow);
    sfText_setPosition(label2, (sfVector2f){(float)windowSize.x * 0.55f, fieldY - 40});

    float buttonWidth = 250.0f;
    float buttonHeight = 50.0f;
    float buttonX = ((float)windowSize.x - buttonWidth) / 2.0f;
    float buttonY = (float)windowSize.y * 0.5f;
    
    sfRectangleShape* compareButton = sfRectangleShape_create();
    sfRectangleShape_setSize(compareButton, (sfVector2f){buttonWidth, buttonHeight});
    sfRectangleShape_setPosition(compareButton, (sfVector2f){buttonX, buttonY});
    sfRectangleShape_setFillColor(compareButton, sfColor_fromRGB(0, 150, 0));
    sfRectangleShape_setOutlineColor(compareButton, sfWhite);
    sfRectangleShape_setOutlineThickness(compareButton, 2);

    sfText* compareText = sfText_create();
    sfText_setFont(compareText, font);
    sfText_setString(compareText, "Compare");
    sfText_setCharacterSize(compareText, 30);
    sfText_setFillColor(compareText, sfWhite);
    
    sfFloatRect compareBounds = sfText_getLocalBounds(compareText);
    float compareTextX = buttonX + (buttonWidth - compareBounds.width) / 2.0f;
    float compareTextY = buttonY + (buttonHeight - compareBounds.height) / 2.0f - 5.0f;
    sfText_setPosition(compareText, (sfVector2f){compareTextX, compareTextY});

    sfText* errorText = sfText_create();
    sfText_setFont(errorText, font);
    sfText_setString(errorText, "");
    sfText_setCharacterSize(errorText, 30);
    sfText_setFillColor(errorText, sfRed);
    sfText_setStyle(errorText, sfTextBold);
    
    float errorX = ((float)windowSize.x - 1000) / 2.0f;
    float errorY = (float)windowSize.y * 0.7f;
    sfText_setPosition(errorText, (sfVector2f){errorX, errorY});

    sfText* instructionText = sfText_create();
    sfText_setFont(instructionText, font);
    sfText_setString(instructionText, "Press ESC to cancel");
    sfText_setCharacterSize(instructionText, 30);
    sfText_setFillColor(instructionText, sfWhite);
    
    float instructionX = ((float)windowSize.x - 200) / 2.0f;
    float instructionY = (float)windowSize.y * 0.9f;
    sfText_setPosition(instructionText, (sfVector2f){instructionX, instructionY});

    sfMusic* music = sfMusic_createFromFile("SFML/src/StarWars/background.mp3");
    if (music) sfMusic_play(music);

    bool comparing = true;
    TextField* activeField = textField1;

    while (sfRenderWindow_isOpen(window) && comparing)
    {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed || 
                (event.type == sfEvtKeyPressed && event.key.code == sfKeyEscape))
            {
                sfRenderWindow_close(window);
                comparing = false;
            }

            TextFieldInput(textField1, event);
            TextFieldInput(textField2, event);

            if (event.type == sfEvtMouseButtonPressed && event.mouseButton.button == sfMouseLeft)
            {
                sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
                
                TextFieldClick(textField1, mousePos.x, mousePos.y);
                TextFieldClick(textField2, mousePos.x, mousePos.y);
                
                if (textField1->isActive) activeField = textField1;
                else if (textField2->isActive) activeField = textField2;
                
                sfFloatRect buttonBounds = sfRectangleShape_getGlobalBounds(compareButton);
                if (mousePos.x >= buttonBounds.left && 
                    mousePos.x <= buttonBounds.left + buttonBounds.width &&
                    mousePos.y >= buttonBounds.top && 
                    mousePos.y <= buttonBounds.top + buttonBounds.height)
                {
                    const char* name_1 = GetTextFieldText(textField1);
                    const char* name_2 = GetTextFieldText(textField2);
                    
                    if (strlen(name_1) > 0 && strlen(name_2) > 0)
                    {
                        char* comparison = BuildComparisonString(name_1, name_2, tree);
                        if (comparison)
                        {
                            if (music) sfMusic_destroy(music);
                            ShowComparisonScreen(window, font, name_1, name_2, comparison, windowSize);
                            free(comparison);
                            comparing = false;
                        }
                        else
                            sfText_setString(errorText, "One or both characters not found!");
                    
                    }
                    else
                        sfText_setString(errorText, "Please enter both character names!");
                }
            }
        }

        sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
        sfFloatRect buttonBounds = sfRectangleShape_getGlobalBounds(compareButton);
        
        if (mousePos.x >= buttonBounds.left && 
            mousePos.x <= buttonBounds.left + buttonBounds.width &&
            mousePos.y >= buttonBounds.top && 
            mousePos.y <= buttonBounds.top + buttonBounds.height)
            sfRectangleShape_setFillColor(compareButton, sfColor_fromRGB(0, 200, 0));
        else
            sfRectangleShape_setFillColor(compareButton, sfColor_fromRGB(0, 150, 0));

        sfRenderWindow_clear(window, sfBlack);
    
        if (backgroundTexture)
            sfRenderWindow_drawSprite(window, backgroundSprite, NULL);
        
        DrawTextField(window, textField1);
        DrawTextField(window, textField2);
        sfRenderWindow_drawRectangleShape(window, compareButton, NULL);
        sfRenderWindow_drawText(window, titleText, NULL);
        sfRenderWindow_drawText(window, label1, NULL);
        sfRenderWindow_drawText(window, label2, NULL);
        sfRenderWindow_drawText(window, compareText, NULL);
        sfRenderWindow_drawText(window, errorText, NULL);
        sfRenderWindow_drawText(window, instructionText, NULL);
        
        sfRenderWindow_display(window);
    }

    if (backgroundTexture)
    {
        sfTexture_destroy(backgroundTexture);
        sfSprite_destroy(backgroundSprite);
    }
    DestroyTextField(textField1);
    DestroyTextField(textField2);
    sfText_destroy(titleText);
    sfText_destroy(label1);
    sfText_destroy(label2);
    sfText_destroy(compareText);
    sfText_destroy(errorText);
    sfText_destroy(instructionText);
    sfRectangleShape_destroy(compareButton);
    if (music) sfMusic_destroy(music);
    sfFont_destroy(font);
    sfRenderWindow_destroy(window);
}