#include "sfml_func.h"


void ShowDefinitionScreen(sfRenderWindow* window, sfFont* font, const char* item, const char* definition, sfVector2u windowSize)
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

    char formattedItem[32] = {0};
    strncpy(formattedItem, item, sizeof(formattedItem) - 1);
    
    for (char* p = formattedItem; *p; p++) { 
        if (*p == ' ') *p = '_'; 
    }

    char imagePath[256] = {0};
    snprintf(imagePath, sizeof(imagePath), "SFML/src/StarWars/%s/%s.png", formattedItem, formattedItem);

    sfTexture* resultTexture = sfTexture_createFromFile(imagePath, NULL);
    sfSprite* resultSprite = sfSprite_create();
    
    if (resultTexture)
    {
        sfSprite_setTexture(resultSprite, resultTexture, sfTrue);
        
        sfVector2u textureSize = sfTexture_getSize(resultTexture);
        float scaleX = (float)windowSize.x * 0.4f / textureSize.x; 
        float scaleY = (float)windowSize.y * 0.4f / textureSize.y;
        float scale = (scaleX < scaleY) ? scaleX : scaleY;
        
        sfSprite_setScale(resultSprite, (sfVector2f){scale, scale});
        
        float resultX = ((float)windowSize.x - textureSize.x * scale) / 2.0f;
        float resultY = (float)windowSize.y * 0.2f;
        sfSprite_setPosition(resultSprite, (sfVector2f){resultX, resultY});
    } else {
        printf("Failed to load character image: %s\n", imagePath);
    }

    sfText* nameText = sfText_create();
    sfText_setFont(nameText, font);
    sfText_setString(nameText, item);
    sfText_setCharacterSize(nameText, 60);
    sfText_setFillColor(nameText, sfYellow);
    sfText_setStyle(nameText, sfTextBold);
    
    sfFloatRect nameBounds = sfText_getLocalBounds(nameText);
    float nameX = ((float)windowSize.x - nameBounds.width) / 2.0f;
    float nameY = (float)windowSize.y * 0.05f;
    sfText_setPosition(nameText, (sfVector2f){nameX, nameY});

    sfText* definitionText = sfText_create();
    sfText_setFont(definitionText, font);
    sfText_setString(definitionText, definition);
    sfText_setCharacterSize(definitionText, 20);
    sfText_setFillColor(definitionText, sfGreen);
    sfText_setStyle(definitionText, sfTextBold);
    
    sfFloatRect defBounds = sfText_getLocalBounds(definitionText);
    float defX = ((float)windowSize.x - defBounds.width) / 2.0f;
    float defY = (float)windowSize.y * 0.64f;
    sfText_setPosition(definitionText, (sfVector2f){defX, defY});

    sfText* instructionText = sfText_create();
    sfText_setFont(instructionText, font);
    sfText_setString(instructionText, "Press ESC to exit");
    sfText_setCharacterSize(instructionText, 30);
    sfText_setFillColor(instructionText, sfWhite);
    
    float instructionX = ((float)windowSize.x - 200) / 2.0f;
    float instructionY = (float)windowSize.y * 0.9f;
    sfText_setPosition(instructionText, (sfVector2f){instructionX, instructionY});

    char musicPath[256] = {0};
    snprintf(musicPath, sizeof(musicPath), "SFML/src/StarWars/%s/%s.mp3", formattedItem, formattedItem);
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
        
        if (resultTexture)
            sfRenderWindow_drawSprite(window, resultSprite, NULL);
        
        sfRenderWindow_drawText(window, nameText, NULL);
        sfRenderWindow_drawText(window, definitionText, NULL);
        sfRenderWindow_drawText(window, instructionText, NULL);
        
        sfRenderWindow_display(window);
    }

    if (backgroundTexture)
    {
        sfTexture_destroy(backgroundTexture);
        sfSprite_destroy(backgroundSprite);
    }
    if (resultTexture)
    {
        sfTexture_destroy(resultTexture);
        sfSprite_destroy(resultSprite);
    }
    sfText_destroy(nameText);
    sfText_destroy(definitionText);
    sfText_destroy(instructionText);
    if (music) sfMusic_destroy(music);
}

char* BuildDefinitionString(node_t* found, tree_t* tree)
{
    if (!found || !tree) return NULL;
    
    char* definition = (char*)calloc(1024, sizeof(char));
    if (!definition) return NULL;
    definition[0] = '\0';
    
    node_t* parent = found->parent;
    node_t* child = found;
    int length = 0;
    
    length += snprintf(definition + length, (size_t)(1024 - length), "Definition of %s:\n", found->item);
    
    while (parent != NULL)
    {
        if (child == parent->left)
            length += snprintf(definition + length, (size_t)(1024 - length), "- %s is %s\n", found->item, parent->item);
        else
            length += snprintf(definition + length, (size_t)(1024 - length), "- %s is not %s\n", found->item, parent->item);
        
        parent = parent->parent;
        child = child->parent;
    }
    
    return definition;
}

int DefineMode(tree_t* tree)
{
    const sfVideoMode mode = sfVideoMode_getDesktopMode();
    sfRenderWindow* window = sfRenderWindow_create(mode, "Define Mode - Star Wars Akinator", sfFullscreen, NULL);
    if (!window)
        return -1;

    sfVector2u windowSize = sfRenderWindow_getSize(window);
    
    sfFont* font = sfFont_createFromFile("SFML/src/Agzo.ttf");
    if (!font)
    {
        sfRenderWindow_destroy(window);
        return -1;
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

    float fieldWidth = 500.0f;
    float fieldHeight = 50.0f;
    float fieldX = ((float)windowSize.x - fieldWidth) / 2.0f;
    float fieldY = (float)windowSize.y * 0.3f;
    
    TextField* textField = CreateTextField(fieldX, fieldY, fieldWidth, fieldHeight, font);

    sfText* titleText = sfText_create();
    sfText_setFont(titleText, font);
    sfText_setString(titleText, "Enter character name to define:");
    sfText_setCharacterSize(titleText, 40);
    sfText_setFillColor(titleText, sfGreen);
    sfText_setStyle(titleText, sfTextBold);
    
    sfFloatRect titleBounds = sfText_getLocalBounds(titleText);
    float titleX = ((float)windowSize.x - titleBounds.width) / 2.0f;
    float titleY = (float)windowSize.y * 0.1f;
    sfText_setPosition(titleText, (sfVector2f){titleX, titleY});

    float buttonWidth = 200.0f;
    float buttonHeight = 50.0f;
    float buttonX = ((float)windowSize.x - buttonWidth) / 2.0f;
    float buttonY = (float)windowSize.y * 0.5f;
    
    sfRectangleShape* searchButton = sfRectangleShape_create();
    sfRectangleShape_setSize(searchButton, (sfVector2f){buttonWidth, buttonHeight});
    sfRectangleShape_setPosition(searchButton, (sfVector2f){buttonX, buttonY});
    sfRectangleShape_setFillColor(searchButton, sfColor_fromRGB(0, 150, 0));
    sfRectangleShape_setOutlineColor(searchButton, sfWhite);
    sfRectangleShape_setOutlineThickness(searchButton, 2);

    sfText* searchText = sfText_create();
    sfText_setFont(searchText, font);
    sfText_setString(searchText, "Search");
    sfText_setCharacterSize(searchText, 30);
    sfText_setFillColor(searchText, sfWhite);
    
    sfFloatRect searchBounds = sfText_getLocalBounds(searchText);
    float searchTextX = buttonX + (buttonWidth - searchBounds.width) / 2.0f;
    float searchTextY = buttonY + (buttonHeight - searchBounds.height) / 2.0f - 5.0f;
    sfText_setPosition(searchText, (sfVector2f){searchTextX, searchTextY});

    sfText* errorText = sfText_create();
    sfText_setFont(errorText, font);
    sfText_setString(errorText, "");
    sfText_setCharacterSize(errorText, 30);
    sfText_setFillColor(errorText, sfRed);
    sfText_setStyle(errorText, sfTextBold);
    
    float errorX = ((float)windowSize.x - 400) / 2.0f;
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

    bool searching = true;

    while (sfRenderWindow_isOpen(window) && searching)
    {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed || 
                (event.type == sfEvtKeyPressed && event.key.code == sfKeyEscape))
            {
                sfRenderWindow_close(window);
                searching = false;
            }

            TextFieldInput(textField, event);

            if (event.type == sfEvtMouseButtonPressed && event.mouseButton.button == sfMouseLeft)
            {
                sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
                
                TextFieldClick(textField, mousePos.x, mousePos.y);
                
                sfFloatRect buttonBounds = sfRectangleShape_getGlobalBounds(searchButton);
                if (mousePos.x >= buttonBounds.left && 
                    mousePos.x <= buttonBounds.left + buttonBounds.width &&
                    mousePos.y >= buttonBounds.top && 
                    mousePos.y <= buttonBounds.top + buttonBounds.height)
                {
                    const char* inputText = GetTextFieldText(textField);
                    if (strlen(inputText) > 0)
                    {
                        node_t* found = NodeFind(inputText, tree->root);
                        if (found != NULL)
                        {
                            char* definition = BuildDefinitionString(found, tree);
                            if (definition)
                            {
                                ShowDefinitionScreen(window, font, found->item, definition, windowSize);
                                free(definition);
                            }
                            searching = false;
                        }
                        else
                            sfText_setString(errorText, "Character not found!");
                    }
                }
            }
        }

        sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
        sfFloatRect buttonBounds = sfRectangleShape_getGlobalBounds(searchButton);
        
        if (mousePos.x >= buttonBounds.left && 
            mousePos.x <= buttonBounds.left + buttonBounds.width &&
            mousePos.y >= buttonBounds.top && 
            mousePos.y <= buttonBounds.top + buttonBounds.height)
            sfRectangleShape_setFillColor(searchButton, sfColor_fromRGB(0, 200, 0));
        else
            sfRectangleShape_setFillColor(searchButton, sfColor_fromRGB(0, 150, 0));

        sfRenderWindow_clear(window, sfBlack);
    
        if (backgroundTexture)
            sfRenderWindow_drawSprite(window, backgroundSprite, NULL);
        
        DrawTextField(window, textField);
        sfRenderWindow_drawRectangleShape(window, searchButton, NULL);
        sfRenderWindow_drawText(window, titleText, NULL);
        sfRenderWindow_drawText(window, searchText, NULL);
        sfRenderWindow_drawText(window, errorText, NULL);
        sfRenderWindow_drawText(window, instructionText, NULL);
        
        sfRenderWindow_display(window);
    }

    if (backgroundTexture)
    {
        sfTexture_destroy(backgroundTexture);
        sfSprite_destroy(backgroundSprite);
    }
    DestroyTextField(textField);
    sfText_destroy(titleText);
    sfText_destroy(searchText);
    sfText_destroy(errorText);
    sfText_destroy(instructionText);
    sfRectangleShape_destroy(searchButton);
    if (music) sfMusic_destroy(music);
    sfFont_destroy(font);
    sfRenderWindow_destroy(window);

    return 0;
}