#include "sfml_func.h"

void ShowResultScreen(sfRenderWindow* window, sfFont* font, const char* item, sfVector2u windowSize)
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
    
    for (char* p = formattedItem; *p; p++) { if (*p == ' ') *p = '_'; }

    char imagePath[256] = {0};
    snprintf(imagePath, sizeof(imagePath), "SFML/src/StarWars/%s/%s.png", formattedItem, formattedItem);

    sfTexture* resultTexture = sfTexture_createFromFile(imagePath, NULL);
    sfSprite* resultSprite = sfSprite_create();
    
    if (resultTexture)
    {
        sfSprite_setTexture(resultSprite, resultTexture, sfTrue);
        
        sfVector2u textureSize = sfTexture_getSize(resultTexture);
        float scaleX = (float)windowSize.x * 0.5f / textureSize.x; 
        float scaleY = (float)windowSize.y * 0.5f / textureSize.y;
        float scale = (scaleX < scaleY) ? scaleX : scaleY;
        
        sfSprite_setScale(resultSprite, (sfVector2f){scale, scale});
        
        float resultX = ((float)windowSize.x - textureSize.x * scale) / 2.0f;
        float resultY = (float)windowSize.y * 0.4f - textureSize.y * scale / 3.0f;
        sfSprite_setPosition(resultSprite, (sfVector2f){resultX, resultY});
    } else {
        printf("Failed to load character image: %s\n", imagePath);
    }

    sfText* successText = sfText_create();
    sfText_setFont(successText, font);
    sfText_setString(successText, "I knew it! I am so smart!");
    sfText_setCharacterSize(successText, 50);
    sfText_setFillColor(successText, sfGreen);
    sfText_setStyle(successText, sfTextBold);
    
    sfFloatRect successBounds = sfText_getLocalBounds(successText);
    float successX = ((float)windowSize.x - successBounds.width) / 2.0f;
    float successY = (float)windowSize.y * 0.1f;
    sfText_setPosition(successText, (sfVector2f){successX, successY});

    sfText* nameText = sfText_create();
    sfText_setFont(nameText, font);
    sfText_setString(nameText, item);
    sfText_setCharacterSize(nameText, 60);
    sfText_setFillColor(nameText, sfYellow);
    sfText_setStyle(nameText, sfTextBold);
    
    sfFloatRect nameBounds = sfText_getLocalBounds(nameText);
    float nameX = ((float)windowSize.x - nameBounds.width) / 2.0f;
    float nameY = (float)windowSize.y * 0.75f;
    sfText_setPosition(nameText, (sfVector2f){nameX, nameY});

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
        
        sfRenderWindow_drawText(window, successText, NULL);
        sfRenderWindow_drawText(window, nameText, NULL);
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
    sfText_destroy(successText);
    sfText_destroy(nameText);
    sfText_destroy(instructionText);
}


void MakeDifferenceBetweenNodes(sfRenderWindow* window, sfFont* font, const char* item, node_t* new_node_for_ans, node_t* current_node, tree_t* tree, sfVector2u windowSize)
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

    float fieldWidth = 500.0f;
    float fieldHeight = 50.0f;
    float fieldX = ((float)windowSize.x - fieldWidth) / 2.0f;
    float fieldY = (float)windowSize.y * 0.3f;
    
    TextField* textField = CreateTextField(fieldX, fieldY, fieldWidth, fieldHeight, font);

    sfText* titleText = sfText_create();
    sfText_setFont(titleText, font);
    char question[256] = {0};
    snprintf(question, sizeof(question), "What is the difference between\n%s and %s?", new_node_for_ans->item, item);
    sfText_setString(titleText, question);
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
    
    sfRectangleShape* submitButton = sfRectangleShape_create();
    sfRectangleShape_setSize(submitButton, (sfVector2f){buttonWidth, buttonHeight});
    sfRectangleShape_setPosition(submitButton, (sfVector2f){buttonX, buttonY});
    sfRectangleShape_setFillColor(submitButton, sfColor_fromRGB(0, 150, 0));
    sfRectangleShape_setOutlineColor(submitButton, sfWhite);
    sfRectangleShape_setOutlineThickness(submitButton, 2);

    sfText* submitText = sfText_create();
    sfText_setFont(submitText, font);
    sfText_setString(submitText, "Submit");
    sfText_setCharacterSize(submitText, 30);
    sfText_setFillColor(submitText, sfWhite);
    
    sfFloatRect submitBounds = sfText_getLocalBounds(submitText);
    float submitTextX = buttonX + (buttonWidth - submitBounds.width) / 2.0f;
    float submitTextY = buttonY + (buttonHeight - submitBounds.height) / 2.0f - 5.0f;
    sfText_setPosition(submitText, (sfVector2f){submitTextX, submitTextY});

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

    bool submitted = false;

    while (sfRenderWindow_isOpen(window) && !submitted)
    {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed || 
                (event.type == sfEvtKeyPressed && event.key.code == sfKeyEscape))
                sfRenderWindow_close(window);

            TextFieldInput(textField, event);

            if (event.type == sfEvtKeyPressed && event.key.code == sfKeyEnter)
            {
                const char* inputText = GetTextFieldText(textField);
                if (strlen(inputText) > 0)
                {
                    node_t *new_node_for_qst = NewNode(strdup(inputText));
                    node_t *old_node = current_node;
                    
                    new_node_for_qst->left = new_node_for_ans;
                    new_node_for_qst->right = old_node;
                    new_node_for_ans->parent = new_node_for_qst;
                    new_node_for_qst->parent = old_node->parent;
                    
                    if (old_node->parent)
                    {
                        if (old_node->parent->left == old_node)
                            old_node->parent->left = new_node_for_qst;
                        else
                            old_node->parent->right = new_node_for_qst;
                    }
                    else
                        tree->root = new_node_for_qst;
                    
                    old_node->parent = new_node_for_qst;
                    
                    submitted = true;
                    printf("Successfully added new knowledge!\n");
                }
            }

            if (event.type == sfEvtMouseButtonPressed && event.mouseButton.button == sfMouseLeft)
            {
                sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
                
                TextFieldClick(textField, mousePos.x, mousePos.y);
                
                sfFloatRect buttonBounds = sfRectangleShape_getGlobalBounds(submitButton);
                if (mousePos.x >= buttonBounds.left && 
                    mousePos.x <= buttonBounds.left + buttonBounds.width &&
                    mousePos.y >= buttonBounds.top && 
                    mousePos.y <= buttonBounds.top + buttonBounds.height)
                {
                    const char* inputText = GetTextFieldText(textField);
                    if (strlen(inputText) > 0)
                    {
                        node_t *new_node_for_qst = NewNode(strdup(inputText));
                        node_t *old_node = current_node;
                        
                        new_node_for_qst->left = new_node_for_ans;
                        new_node_for_qst->right = old_node;
                        new_node_for_ans->parent = new_node_for_qst;
                        new_node_for_qst->parent = old_node->parent;
                        
                        if (old_node->parent)
                        {
                            if (old_node->parent->left == old_node)
                                old_node->parent->left = new_node_for_qst;
                            else
                                old_node->parent->right = new_node_for_qst;
                        }
                        else
                            tree->root = new_node_for_qst;
                        
                        old_node->parent = new_node_for_qst;
                        
                        submitted = true;
                        printf("Successfully added new knowledge!\n");
                    }
                }
            }
        }

        sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
        sfFloatRect buttonBounds = sfRectangleShape_getGlobalBounds(submitButton);
        
        if (mousePos.x >= buttonBounds.left && 
            mousePos.x <= buttonBounds.left + buttonBounds.width &&
            mousePos.y >= buttonBounds.top && 
            mousePos.y <= buttonBounds.top + buttonBounds.height)
            sfRectangleShape_setFillColor(submitButton, sfColor_fromRGB(0, 200, 0));
        else
            sfRectangleShape_setFillColor(submitButton, sfColor_fromRGB(0, 150, 0));

        sfRenderWindow_clear(window, sfBlack);
    
        if (backgroundTexture)
            sfRenderWindow_drawSprite(window, backgroundSprite, NULL);
        
        DrawTextField(window, textField);
        sfRenderWindow_drawRectangleShape(window, submitButton, NULL);
        sfRenderWindow_drawText(window, titleText, NULL);
        sfRenderWindow_drawText(window, submitText, NULL);
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
    sfText_destroy(submitText);
    sfText_destroy(instructionText);
    sfRectangleShape_destroy(submitButton);
    
    if (music) sfMusic_destroy(music);
}


void MakeNewNode(sfRenderWindow* window, sfFont* font, const char* item, sfVector2u windowSize, tree_t* tree, node_t* current_node)
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

    float fieldWidth = 500.0f;
    float fieldHeight = 50.0f;
    float fieldX = ((float)windowSize.x - fieldWidth) / 2.0f;
    float fieldY = (float)windowSize.y * 0.3f;
    
    TextField* textField = CreateTextField(fieldX, fieldY, fieldWidth, fieldHeight, font);

    sfText* titleText = sfText_create();
    sfText_setFont(titleText, font);
    sfText_setString(titleText, "What did you guess?");
    sfText_setCharacterSize(titleText, 50);
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
    
    sfRectangleShape* submitButton = sfRectangleShape_create();
    sfRectangleShape_setSize(submitButton, (sfVector2f){buttonWidth, buttonHeight});
    sfRectangleShape_setPosition(submitButton, (sfVector2f){buttonX, buttonY});
    sfRectangleShape_setFillColor(submitButton, sfColor_fromRGB(0, 150, 0));
    sfRectangleShape_setOutlineColor(submitButton, sfWhite);
    sfRectangleShape_setOutlineThickness(submitButton, 2);

    sfText* submitText = sfText_create();
    sfText_setFont(submitText, font);
    sfText_setString(submitText, "Submit");
    sfText_setCharacterSize(submitText, 30);
    sfText_setFillColor(submitText, sfWhite);
    
    sfFloatRect submitBounds = sfText_getLocalBounds(submitText);
    float submitTextX = buttonX + (buttonWidth - submitBounds.width) / 2.0f;
    float submitTextY = buttonY + (buttonHeight - submitBounds.height) / 2.0f - 5.0f;
    sfText_setPosition(submitText, (sfVector2f){submitTextX, submitTextY});

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

    bool submitted = false;
    node_t* new_node_for_ans = NULL;

    while (sfRenderWindow_isOpen(window) && !submitted)
    {
        sfEvent event;
        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed || 
                (event.type == sfEvtKeyPressed && event.key.code == sfKeyEscape))
                sfRenderWindow_close(window);

            TextFieldInput(textField, event);

            if (event.type == sfEvtKeyPressed && event.key.code == sfKeyEnter)
            {
                const char* inputText = GetTextFieldText(textField);
                if (strlen(inputText) > 0)
                {
                    new_node_for_ans = NewNode(strdup(inputText));
                    submitted = true;
                }
            }

            if (event.type == sfEvtMouseButtonPressed && event.mouseButton.button == sfMouseLeft)
            {
                sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
                
                TextFieldClick(textField, mousePos.x, mousePos.y);
                
                sfFloatRect buttonBounds = sfRectangleShape_getGlobalBounds(submitButton);
                if (mousePos.x >= buttonBounds.left && 
                    mousePos.x <= buttonBounds.left + buttonBounds.width &&
                    mousePos.y >= buttonBounds.top && 
                    mousePos.y <= buttonBounds.top + buttonBounds.height)
                {
                    const char* inputText = GetTextFieldText(textField);
                    if (strlen(inputText) > 0)
                    {
                        new_node_for_ans = NewNode(strdup(inputText));
                        submitted = true;
                    }
                }
            }
        }

        sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);
        sfFloatRect buttonBounds = sfRectangleShape_getGlobalBounds(submitButton);
        
        if (mousePos.x >= buttonBounds.left && 
            mousePos.x <= buttonBounds.left + buttonBounds.width &&
            mousePos.y >= buttonBounds.top && 
            mousePos.y <= buttonBounds.top + buttonBounds.height)
            sfRectangleShape_setFillColor(submitButton, sfColor_fromRGB(0, 200, 0));
        else
            sfRectangleShape_setFillColor(submitButton, sfColor_fromRGB(0, 150, 0));

        sfRenderWindow_clear(window, sfBlack);
    
        if (backgroundTexture)
            sfRenderWindow_drawSprite(window, backgroundSprite, NULL);
        
        DrawTextField(window, textField);
        sfRenderWindow_drawRectangleShape(window, submitButton, NULL);
        sfRenderWindow_drawText(window, titleText, NULL);
        sfRenderWindow_drawText(window, submitText, NULL);
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
    sfText_destroy(submitText);
    sfText_destroy(instructionText);
    sfRectangleShape_destroy(submitButton);
    
    if (music) sfMusic_destroy(music);

    // После ввода нового персонажа переходим к вводу различия
    if (new_node_for_ans != NULL)
    {
        MakeDifferenceBetweenNodes(window, font, item, new_node_for_ans, current_node, tree, windowSize);
    }
}


int FinalFind(const char* item, tree_t* tree, node_t* current_node)
{
    int verd = -1;

    const sfVideoMode mode   = sfVideoMode_getDesktopMode();
    sfRenderWindow*   window = sfRenderWindow_create(mode, "Stars Akinator", sfFullscreen, NULL);
    if (!window)
        return -1;

    sfTexture* texture = sfTexture_createFromFile("SFML/src/StarWars/background.png", NULL);
    if (!texture)
    {
        sfRenderWindow_destroy(window);
        return -1;
    }

    sfSprite* sprite = sfSprite_create();
    sfSprite_setTexture(sprite, texture, sfTrue);
    
    sfVector2u textureSize = sfTexture_getSize(texture);
    sfVector2u windowSize  = sfRenderWindow_getSize(window);
    
    float scaleX = (float)windowSize.x / (float)textureSize.x;
    float scaleY = (float)windowSize.y / (float)textureSize.y;
    float scale  = (scaleX < scaleY) ? scaleX : scaleY;
    
    sfSprite_setScale(sprite, (sfVector2f){scale, scale});
    
    float offsetX = ((float)windowSize.x - (float)textureSize.x * scale) / 2.0f;
    float offsetY = ((float)windowSize.y - (float)textureSize.y * scale) / 2.0f;
    sfSprite_setPosition(sprite, (sfVector2f){offsetX, offsetY});

    float barWidth = 1000.0f;
    float barHeight = 100.0f;
    
    sfRectangleShape* question_bar = sfRectangleShape_create();
    sfRectangleShape_setSize(question_bar, (sfVector2f){barWidth, barHeight});
    sfRectangleShape_setPosition(question_bar, (sfVector2f){((float)windowSize.x - barWidth) / 2.0f, (float)windowSize.y * 2.0f / 5.0f});
    sfRectangleShape_setFillColor(question_bar, sfMagenta);
    sfRectangleShape_setOutlineColor(question_bar, sfWhite);
    sfRectangleShape_setOutlineThickness(question_bar, 5);

    sfFont* font = sfFont_createFromFile("SFML/src/Agzo.ttf");
    if (!font)
    {
        sfSprite_destroy(sprite);
        sfTexture_destroy(texture);
        sfRenderWindow_destroy(window);
        return -1;
    }

    sfText* question_text = sfText_create();
    sfText_setFont(question_text, font);
    char question[128] = {0};
    snprintf(question, sizeof(question), "It is %s?", item);
    sfText_setString(question_text, question);
    sfText_setCharacterSize(question_text, 40);
    sfText_setFillColor(question_text, sfWhite);
    
    sfFloatRect textBounds = sfText_getLocalBounds(question_text);
    float barX = ((float)windowSize.x - barWidth) / 2.0f;
    float textX = barX + (barWidth - textBounds.width) / 2.0f;
    float textY = (float)windowSize.y * 2.0f / 5.0f + (barHeight - textBounds.height) / 2.0f - 10.0f;
    sfText_setPosition(question_text, (sfVector2f){textX, textY});

    float button_size_x = 200.0f;
    float button_size_y = 40.0f;
    float button_gap = 40.0f;
    float total_buttons_width = button_size_x * 2.0f + button_gap;
    float buttons_start_x = ((float)windowSize.x - total_buttons_width) / 2.0f;
    float buttons_y = (float)windowSize.y * 3.0f / 5.0f + 20.0f;

    sfRectangleShape* yes_button = sfRectangleShape_create();
    sfRectangleShape_setSize(yes_button, (sfVector2f){button_size_x, button_size_y});
    sfRectangleShape_setPosition(yes_button, (sfVector2f){buttons_start_x, buttons_y});
    sfRectangleShape_setFillColor(yes_button, sfMagenta);
    sfRectangleShape_setOutlineColor(yes_button, sfWhite);
    sfRectangleShape_setOutlineThickness(yes_button, 5);

    sfRectangleShape* no_button = sfRectangleShape_create();
    sfRectangleShape_setSize(no_button, (sfVector2f){button_size_x, button_size_y});
    sfRectangleShape_setPosition(no_button, (sfVector2f){buttons_start_x + button_size_x + button_gap, buttons_y});
    sfRectangleShape_setFillColor(no_button, sfMagenta);
    sfRectangleShape_setOutlineColor(no_button, sfWhite);
    sfRectangleShape_setOutlineThickness(no_button, 5);

    sfText* yes_text = sfText_create();
    sfText_setFont(yes_text, font);
    sfText_setString(yes_text, "YES");
    sfText_setCharacterSize(yes_text, 30);
    sfFloatRect yes_text_bounds = sfText_getLocalBounds(yes_text);
    float yes_text_x = buttons_start_x + (button_size_x - yes_text_bounds.width) / 2.0f;
    float yes_text_y = buttons_y + (button_size_y - yes_text_bounds.height) / 2.0f - 5.0f;
    sfText_setPosition(yes_text, (sfVector2f){yes_text_x, yes_text_y});

    sfText* no_text = sfText_create();
    sfText_setFont(no_text, font);
    sfText_setString(no_text, "NO");
    sfText_setCharacterSize(no_text, 30);
    sfFloatRect no_text_bounds = sfText_getLocalBounds(no_text);
    float no_text_x = buttons_start_x + button_size_x + button_gap + (button_size_x - no_text_bounds.width) / 2.0f;
    float no_text_y = buttons_y + (button_size_y - no_text_bounds.height) / 2.0f - 5.0f;
    sfText_setPosition(no_text, (sfVector2f){no_text_x, no_text_y});

    sfMusic* music = sfMusic_createFromFile("SFML/src/StarWars/background.mp3");
    if (music) sfMusic_play(music);

    while (sfRenderWindow_isOpen(window))
    {
        sfEvent event;
        sfVector2i mousePos = sfMouse_getPositionRenderWindow(window);

        while (sfRenderWindow_pollEvent(window, &event))
        {
            if (event.type == sfEvtClosed || (event.type == sfEvtKeyPressed && event.key.code == sfKeyEscape))
                sfRenderWindow_close(window);

            if (event.type == sfEvtMouseButtonPressed && event.mouseButton.button == sfMouseLeft)
            {
                sfFloatRect bounds_to_yes = sfRectangleShape_getGlobalBounds(yes_button);
                if (mousePos.x >= bounds_to_yes.left && mousePos.x <= bounds_to_yes.left + bounds_to_yes.width && 
                    mousePos.y >= bounds_to_yes.top && mousePos.y <= bounds_to_yes.top + bounds_to_yes.height)
                {
                    verd = YES;
                    break;
                }

                sfFloatRect bounds_to_no = sfRectangleShape_getGlobalBounds(no_button);
                if (mousePos.x >= bounds_to_no.left && mousePos.x <= bounds_to_no.left + bounds_to_no.width && 
                    mousePos.y >= bounds_to_no.top && mousePos.y <= bounds_to_no.top + bounds_to_no.height)
                {
                    verd = NO;
                    break;
                }
            }
        }

        if (verd == YES || verd == NO) break;

        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawSprite(window, sprite, NULL);
        sfRenderWindow_drawRectangleShape(window, question_bar, NULL);
        sfRenderWindow_drawText(window, question_text, NULL);
        sfRenderWindow_drawRectangleShape(window, yes_button, NULL);
        sfRenderWindow_drawRectangleShape(window, no_button, NULL);
        sfRenderWindow_drawText(window, yes_text, NULL);
        sfRenderWindow_drawText(window, no_text, NULL);
        sfRenderWindow_display(window);
    }

    if (music) sfMusic_destroy(music);
    sfText_destroy(question_text);
    sfText_destroy(yes_text);
    sfText_destroy(no_text);
    sfRectangleShape_destroy(question_bar);
    sfRectangleShape_destroy(yes_button);
    sfRectangleShape_destroy(no_button);
    sfSprite_destroy(sprite);
    sfTexture_destroy(texture);
    
    if (verd == YES)
    {
        ShowResultScreen(window, font, item, windowSize);
        sfFont_destroy(font);
        sfRenderWindow_destroy(window);
    }
    else if (verd == NO)
    {
        MakeNewNode(window, font, item, windowSize, tree, current_node);
        sfFont_destroy(font);
        sfRenderWindow_destroy(window);
    }
    else
    {
        sfFont_destroy(font);
        sfRenderWindow_destroy(window);
    }

    return verd;
}