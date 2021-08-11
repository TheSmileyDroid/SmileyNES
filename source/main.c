#include <stdio.h>
#include "Bus.h"
#include "Smiley6502.h"

#define SFML_STATIC

#include <SFML/Graphics.h>
#include <SFML/Window.h>

int main()
{
    sfVideoMode mode = {640, 480};
    sfRenderWindow *window = sfRenderWindow_create(mode, "SmileyNES", sfResize | sfClose, NULL);
    sfRenderWindow_setVerticalSyncEnabled(window, sfTrue);

    while (sfRenderWindow_isOpen(window))
    {
        sfEvent *event;
        while (sfRenderWindow_pollEvent(window, event))
        {
            if (event->type == sfEvtClosed)
                sfRenderWindow_close(window);
        }

        sfRenderWindow_clear(window, sfColor_fromRGB(100, 100, 255));
        sfRenderWindow_display(window);
    }
    return 0;
}