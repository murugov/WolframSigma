#include "sfml_func.h"

int FindMode(const char* item)
{
    int verd = 0;

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
    sfRectangleShape_setPosition(question_bar, (sfVector2f){((float)windowSize.x - barWidth) / 2.0f, (float)windowSize.y * 2.0f / 5.0f}); // Исправлено: barWidth вместо 800.0f
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
                    sfRenderWindow_close(window);
                }

                sfFloatRect bounds_to_no = sfRectangleShape_getGlobalBounds(no_button);
                if (mousePos.x >= bounds_to_no.left && mousePos.x <= bounds_to_no.left + bounds_to_no.width && 
                    mousePos.y >= bounds_to_no.top && mousePos.y <= bounds_to_no.top + bounds_to_no.height)
                {
                    verd = NO;
                    sfRenderWindow_close(window);
                }
            }
        }

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
    sfFont_destroy(font);
    sfRectangleShape_destroy(question_bar);
    sfRectangleShape_destroy(yes_button);
    sfRectangleShape_destroy(no_button);
    sfSprite_destroy(sprite);
    sfTexture_destroy(texture);
    sfRenderWindow_destroy(window);

    return verd;
}