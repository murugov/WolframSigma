#include "sfml_func.h"

int GameStart()
{
    const sfVideoMode mode   = sfVideoMode_getDesktopMode();
    sfRenderWindow*   window = sfRenderWindow_create(mode, "Stars Akinator", sfFullscreen, NULL);
    if (!window)
        return EXIT_FAILURE;

    sfTexture* texture = sfTexture_createFromFile("SFML/src/preview.png", NULL);
    if (!texture)
    {
        sfRenderWindow_destroy(window);
        return EXIT_FAILURE;
    }

    sfSprite* sprite = sfSprite_create();
    sfSprite_setTexture(sprite, texture, sfTrue);
    
    sfVector2u textureSize = sfTexture_getSize(texture);
    sfVector2u windowSize  = sfRenderWindow_getSize(window);
    
    float scaleX = (float)windowSize.x / textureSize.x;
    float scaleY = (float)windowSize.y / textureSize.y;
    float scale  = (scaleX < scaleY) ? scaleX : scaleY;
    
    sfSprite_setScale(sprite, (sfVector2f){scale, scale});
    
    float offsetX = (windowSize.x - textureSize.x * scale) / 2;
    float offsetY = (windowSize.y - textureSize.y * scale) / 2;
    sfSprite_setPosition(sprite, (sfVector2f){offsetX, offsetY});

    sfRectangleShape* start_button = sfRectangleShape_create();
    sfRectangleShape_setSize(start_button, (sfVector2f){300, 60});
    sfRectangleShape_setPosition(start_button, (sfVector2f){750, 550});
    sfRectangleShape_setFillColor(start_button, sfMagenta);
    sfRectangleShape_setOutlineColor(start_button, sfWhite);
    sfRectangleShape_setOutlineThickness(start_button, 5);

    sfFont* font = sfFont_createFromFile("SFML/src/Agzo.ttf");
    if (!font)
    {
        sfSprite_destroy(sprite);
        sfTexture_destroy(texture);
        sfRenderWindow_destroy(window);
        return EXIT_FAILURE;
    }
    sfText* text = sfText_create();
    sfText_setFont(text, font);
    sfText_setString(text, "START");
    sfText_setCharacterSize(text, 50);
    sfText_setPosition(text, (sfVector2f){815, 550});

    sfMusic* music = sfMusic_createFromFile("SFML/src/preview.mp3");
    if (!music)
    {
        sfText_destroy(text);
        sfFont_destroy(font);
        sfSprite_destroy(sprite);
        sfTexture_destroy(texture);
        sfRenderWindow_destroy(window);
        return EXIT_FAILURE;
    }

    sfMusic_play(music);

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
                sfFloatRect bounds = sfRectangleShape_getGlobalBounds(start_button);
                if (mousePos.x >= bounds.left && mousePos.x <= bounds.left + bounds.width && mousePos.y >= bounds.top && mousePos.y <= bounds.top + bounds.height)
                    sfRenderWindow_close(window);
            }
        }

        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawSprite(window, sprite, NULL);
        sfRenderWindow_drawRectangleShape(window, start_button, NULL);
        sfRenderWindow_drawText(window, text, NULL);
        sfRenderWindow_display(window);
    }

    sfMusic_destroy(music);
    sfText_destroy(text);
    sfFont_destroy(font);
    sfRectangleShape_destroy(start_button);
    sfSprite_destroy(sprite);
    sfTexture_destroy(texture);
    sfRenderWindow_destroy(window);

    return EXIT_SUCCESS;
}


int SelectModeSFML()
{
    int version = 0;

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
    
    float scaleX = (float)windowSize.x / textureSize.x;
    float scaleY = (float)windowSize.y / textureSize.y;
    float scale  = (scaleX < scaleY) ? scaleX : scaleY;
    
    sfSprite_setScale(sprite, (sfVector2f){scale, scale});
    
    float offsetX = (windowSize.x - textureSize.x * scale) / 2;
    float offsetY = (windowSize.y - textureSize.y * scale) / 2;
    sfSprite_setPosition(sprite, (sfVector2f){offsetX, offsetY});

    sfRectangleShape* select_mode = sfRectangleShape_create();
    sfRectangleShape_setSize(select_mode, (sfVector2f){650, 60});
    sfRectangleShape_setPosition(select_mode, (sfVector2f){(windowSize.x - 600) / 2, windowSize.y * 2 / 5});
    sfRectangleShape_setFillColor(select_mode, sfMagenta);
    sfRectangleShape_setOutlineColor(select_mode, sfWhite);
    sfRectangleShape_setOutlineThickness(select_mode, 5);

    float button_size_x = 240;
    float button_size_y = 40;

    sfRectangleShape* find_button = sfRectangleShape_create();
    sfRectangleShape_setSize(find_button, (sfVector2f){button_size_x, button_size_y});
    sfRectangleShape_setPosition(find_button, (sfVector2f){(windowSize.x - button_size_x) / 6, windowSize.y * 3 / 5});
    sfRectangleShape_setFillColor(find_button, sfMagenta);
    sfRectangleShape_setOutlineColor(find_button, sfWhite);
    sfRectangleShape_setOutlineThickness(find_button, 5);

    sfRectangleShape* define_button = sfRectangleShape_create();
    sfRectangleShape_setSize(define_button, (sfVector2f){button_size_x, button_size_y});
    sfRectangleShape_setPosition(define_button, (sfVector2f){(windowSize.x - button_size_x) / 2, windowSize.y * 3 / 5});
    sfRectangleShape_setFillColor(define_button, sfMagenta);
    sfRectangleShape_setOutlineColor(define_button, sfWhite);
    sfRectangleShape_setOutlineThickness(define_button, 5);

    sfRectangleShape* cmp_button = sfRectangleShape_create();
    sfRectangleShape_setSize(cmp_button, (sfVector2f){button_size_x, button_size_y});
    sfRectangleShape_setPosition(cmp_button, (sfVector2f){(windowSize.x - button_size_x) * 5 / 6, windowSize.y * 3 / 5});
    sfRectangleShape_setFillColor(cmp_button, sfMagenta);
    sfRectangleShape_setOutlineColor(cmp_button, sfWhite);
    sfRectangleShape_setOutlineThickness(cmp_button, 5);

    // exit_button

    sfFont* font = sfFont_createFromFile("SFML/src/Agzo.ttf");
    if (!font)
    {
        sfSprite_destroy(sprite);
        sfTexture_destroy(texture);
        sfRenderWindow_destroy(window);
        return -1;
    }

    sfText* select_text = sfText_create();
    sfText_setFont(select_text, font);
    sfText_setString(select_text, "SELECT MODE");
    sfText_setCharacterSize(select_text, 50);
    sfText_setPosition(select_text, (sfVector2f){(windowSize.x - button_size_x) / 2 - 30, windowSize.y * 2 / 5});

    sfText* find_text = sfText_create();
    sfText_setFont(find_text, font);
    sfText_setString(find_text, "FIND");
    sfText_setCharacterSize(find_text, 30);
    sfText_setPosition(find_text, (sfVector2f){(windowSize.x - button_size_x) / 6 + button_size_x / 25 * 8, windowSize.y * 3 / 5 + button_size_y / 40});

    sfText* define_text = sfText_create();
    sfText_setFont(define_text, font);
    sfText_setString(define_text, "DEFINE");
    sfText_setCharacterSize(define_text, 30);
    sfText_setPosition(define_text, (sfVector2f){(windowSize.x - button_size_x) / 2 + button_size_x / 25 * 6, windowSize.y * 3 / 5 + button_size_y / 40});

    sfText* cmp_text = sfText_create();
    sfText_setFont(cmp_text, font);
    sfText_setString(cmp_text, "COMPARE");
    sfText_setCharacterSize(cmp_text, 30);
    sfText_setPosition(cmp_text, (sfVector2f){(windowSize.x - button_size_x) * 5 / 6 + button_size_x / 25 * 4, windowSize.y * 3 / 5 + button_size_y / 40});


    sfMusic* music = sfMusic_createFromFile("SFML/src/StarWars/background.mp3");
    if (!music)
    {
        sfText_destroy(find_text);
        sfFont_destroy(font);
        sfSprite_destroy(sprite);
        sfTexture_destroy(texture);
        sfRenderWindow_destroy(window);
        return -1;
    }

    sfMusic_play(music);

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
                sfFloatRect bounds_to_find = sfRectangleShape_getGlobalBounds(find_button);
                if (mousePos.x >= bounds_to_find.left && mousePos.x <= bounds_to_find.left + bounds_to_find.width && mousePos.y >= bounds_to_find.top && mousePos.y <= bounds_to_find.top + bounds_to_find.height)
                {
                    version = FIND_MODE;
                    sfRenderWindow_close(window);
                }

                sfFloatRect bounds_to_define = sfRectangleShape_getGlobalBounds(define_button);
                if (mousePos.x >= bounds_to_define.left && mousePos.x <= bounds_to_define.left + bounds_to_define.width && mousePos.y >= bounds_to_define.top && mousePos.y <= bounds_to_define.top + bounds_to_define.height)
                {
                    version = DEFINE_MODE;
                    sfRenderWindow_close(window);
                }

                sfFloatRect bounds_to_cmp = sfRectangleShape_getGlobalBounds(cmp_button);
                if (mousePos.x >= bounds_to_cmp.left && mousePos.x <= bounds_to_cmp.left + bounds_to_cmp.width && mousePos.y >= bounds_to_cmp.top && mousePos.y <= bounds_to_cmp.top + bounds_to_cmp.height)
                {
                    version = CMP_MODE;
                    sfRenderWindow_close(window);
                }
            }
        }

        sfRenderWindow_clear(window, sfBlack);
        sfRenderWindow_drawSprite(window, sprite, NULL);
        sfRenderWindow_drawRectangleShape(window, select_mode, NULL);
        sfRenderWindow_drawRectangleShape(window, find_button, NULL);
        sfRenderWindow_drawRectangleShape(window, define_button, NULL);
        sfRenderWindow_drawRectangleShape(window, cmp_button, NULL);
        sfRenderWindow_drawText(window, select_text, NULL);
        sfRenderWindow_drawText(window, find_text, NULL);
        sfRenderWindow_drawText(window, define_text, NULL);
        sfRenderWindow_drawText(window, cmp_text, NULL);
        sfRenderWindow_display(window);
    }

    sfMusic_destroy(music);
    sfText_destroy(select_text);
    sfText_destroy(find_text);
    sfText_destroy(define_text);
    sfText_destroy(cmp_text);
    sfFont_destroy(font);
    sfRectangleShape_destroy(select_mode);
    sfRectangleShape_destroy(find_button);
    sfRectangleShape_destroy(define_button);
    sfRectangleShape_destroy(cmp_button);
    sfSprite_destroy(sprite);
    sfTexture_destroy(texture);
    sfRenderWindow_destroy(window);

    return version;
}