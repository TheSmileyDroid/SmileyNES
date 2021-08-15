#include <stdio.h>
#include "Bus.h"
#include "Smiley6502.h"

#define SFML_STATIC

#include "../include/SFML/Graphics.h"
#include <SFML/Window.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

void drawRAM(sfRenderWindow *window, int x, int y, uint16_t nAddr, int nRows, int nColumns, sfFont *font)
{
    sfText *text;
    text = sfText_create();
    sfText_setString(text, "Teste");
    sfText_setFont(text, font);
    sfText_setCharacterSize(text, 14);

    sfRenderWindow_drawText(window, text, NULL);

    sfText_destroy(text);
}
int main()
{
    sfRenderWindow *window;
    sfFont *font;
    sfEvent event;
    sfVideoMode mode = {640, 480, 32};
    window = sfRenderWindow_create(mode, "SmileyNES", sfResize | sfClose, NULL);

    font = sfFont_createFromFile("assets/roboto.ttf");

    while (sfRenderWindow_isOpen(window))
    {
        /* Process events */
        while (sfRenderWindow_pollEvent(window, &event))
        {
            /* Close window : exit */
            if (event.type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        /* Clear the screen */
        sfRenderWindow_clear(window, sfColor_fromRGB(100, 100, 240));

        drawRAM(window, 2, 2, 0x0000, 10, 10, font);

        /* Update the window */
        sfRenderWindow_display(window);
    }

    /* Cleanup resources */

    sfFont_destroy(font);
    sfRenderWindow_destroy(window);
    return 0;
}