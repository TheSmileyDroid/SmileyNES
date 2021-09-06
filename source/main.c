<<<<<<< HEAD
#include <stdio.h>
#include "Bus.h"
#include "Smiley6502.h"

#define SFML_STATIC

#include <SFML/Graphics.h>
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
=======
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Bus.h"
#include "Smiley6502.h"

#define SFML_STATIC

#include "../include/SFML/Graphics.h"

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

Bus *nes;
const sfColor white = {255, 255, 255, 255};
const sfColor red = {255, 100, 100, 255};
const sfColor green = {100, 255, 100, 255};

void setText(sfText *text, int x, int y, char *str, sfColor color)
{
    sfText_setString(text, str);
    sfVector2f vec = {x, y};
    sfText_setPosition(text, vec);
    sfText_setColor(text, color);
}

void drawRAM(sfRenderWindow *window, int x, int y, uint16_t nAddr, int nRows, int nColumns, sfText *ramTexts[nRows])
{
    int nRamX = x, nRamY = y;
    for (int row = 0; row < nRows; row++)
    {
        sfVector2f postion = {nRamX, nRamY};
        sfText_setPosition(ramTexts[row], postion);
        char sOffset[nColumns * 6 + 10];
        char addr[6];
        sprintf(addr, "$%04x", nAddr);
        strcpy(sOffset, addr);
        strcat(sOffset, ":");

        for (int col = 0; col < nColumns; col++)
        {
            char addr[6];
            sprintf(addr, " %02x", busRead(nes, nAddr, true));
            strcat(sOffset, addr);
            nAddr += 1;
        }
        sfText_setString(ramTexts[row], sOffset);

        sfRenderWindow_drawText(window, ramTexts[row], NULL);

        nRamY += 14;
    }
}

void drawCpu(sfRenderWindow *window, int x, int y, sfText *cpuTexts[14])
{

    setText(cpuTexts[0], x, y, "STATUS:", white);
    setText(cpuTexts[1], x + 64, y, "N", nes->cpu->status & N ? red : green);
    setText(cpuTexts[2], x + 80, y, "V", nes->cpu->status & V ? red : green);
    setText(cpuTexts[3], x + 96, y, "-", nes->cpu->status & U ? red : green);
    setText(cpuTexts[4], x + 112, y, "B", nes->cpu->status & B ? red : green);
    setText(cpuTexts[5], x + 128, y, "D", nes->cpu->status & D ? red : green);
    setText(cpuTexts[6], x + 144, y, "I", nes->cpu->status & I ? red : green);
    setText(cpuTexts[7], x + 160, y, "Z", nes->cpu->status & Z ? red : green);
    setText(cpuTexts[8], x + 178, y, "C", nes->cpu->status & C ? red : green);
    /*
    setText(cpuTexts[9], x, y, "STATUS:", white);
    setText(cpuTexts[10], x, y, "STATUS:", white);
    setText(cpuTexts[11], x, y, "STATUS:", white);
    setText(cpuTexts[12], x, y, "STATUS:", white);
    setText(cpuTexts[13], x, y, "STATUS:", white);
    */
    for (int i = 0; i < 14; i++)
    {
        sfRenderWindow_drawText(window, cpuTexts[i], NULL);
    }
}

int main()
{
    sfRenderWindow *window;
    sfFont *font;
    sfEvent event;
    sfText *ramTexts[16];
    sfText *cpuTexts[14];
    sfVideoMode mode = {640, 480, 32};
    window = sfRenderWindow_create(mode, "SmileyNES", sfResize | sfClose, NULL);
    sfWindow_setFramerateLimit(window, 60);

    nes = createBus();

    font = sfFont_createFromFile("assets/roboto.ttf");

    // Inicializar textos
    for (int i = 0; i < 16; i++)
    {
        ramTexts[i] = sfText_create();
        sfText_setFont(ramTexts[i], font);
        sfText_setCharacterSize(ramTexts[i], 12);
    }
    for (int i = 0; i < 14; i++)
    {
        cpuTexts[i] = sfText_create();
        sfText_setFont(cpuTexts[i], font);
        sfText_setCharacterSize(cpuTexts[i], 12);
    }

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

        drawRAM(window, 2, 2, 0x0000, 16, 16, ramTexts);

        drawCpu(window, 448, 2, cpuTexts);

        /* Update the window */
        sfRenderWindow_display(window);
    }

    /* Cleanup resources */

    sfFont_destroy(font);
    for (int i = 0; i < 16; i++)
    {
        sfText_destroy(ramTexts[i]);
    }
    for (int i = 0; i < 14; i++)
    {
        sfText_destroy(cpuTexts[i]);
    }
    sfRenderWindow_destroy(window);
    return 0;
>>>>>>> 288559f1b14d788b6b0130864a5605111b3d1ab4
}