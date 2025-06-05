#include <iostream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "menu.h"
#include "graphics.h"

using namespace std;

const int WIDTH = 800;
const int HEIGHT = 600;
int totalParts;
int partWidth;
int partHeight;

std::vector<void*> buffers;
std::vector<int> order;

// Получить случайное имя файла из puzzle_images
string getRandomImagePath() {
    int imageCount = 3; // число доступных изображений 
    int index = rand() % imageCount + 1;
    char filename[50];
    sprintf(filename, "puzzle_images/%03d.bmp", index); 
    return string(filename);
}

bool isSolved() {
    for (int i = 0; i < totalParts; ++i)
        if (order[i] != i) return false;
    return true;
}

int getClickedTile(int x, int y) {
    if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT) return -1;
    int col = x / partWidth;
    int row = y / partHeight;
    return row * cols + col;
}

void drawPuzzle() {
    cleardevice();
    for (int i = 0; i < totalParts; ++i) {
        int drawCol = i % cols;
        int drawRow = i / cols;
        putimage(drawCol * partWidth, drawRow * partHeight, buffers[order[i]], COPY_PUT);
    }
}

void game() 
{
   totalParts = cols * rows;
   partWidth = WIDTH / cols;
   partHeight = HEIGHT / rows;
   buffers.resize(totalParts);
   order.resize(totalParts);
   
    srand(time(0));

    string imagePath = getRandomImagePath();
    readimagefile(imagePath.c_str(), 0, 0, WIDTH - 1, HEIGHT - 1);

    for (int row = 0; row < rows; ++row) {
        for (int col = 0; col < cols; ++col) {
            int index = row * cols + col;
            int left = col * partWidth;
            int top = row * partHeight;
            int right = left + partWidth - 1;
            int bottom = top + partHeight - 1;

            buffers[index] = malloc(imagesize(left, top, right, bottom));
            getimage(left, top, right, bottom, buffers[index]);
            order[index] = index;
        }
    }

    // Перемешать
    for (int i = 0; i < totalParts; ++i) {
        int j = rand() % totalParts;
        swap(order[i], order[j]);
    }

    drawPuzzle();

    int selected = -1;
    while (true) 
   {
        if (isSolved()) {
            setbkcolor(COLOR(0, 0, 0)); // Цвет фона
            setcolor(COLOR(255, 255, 255)); // Цвет текста
            settextstyle(DEFAULT_FONT, HORIZ_DIR, 3);
            outtextxy(300, HEIGHT / 2, (char*)"Ты победил!");
            break;
        }

        if (ismouseclick(WM_LBUTTONDOWN)) {
            int x, y;
            getmouseclick(WM_LBUTTONDOWN, x, y);
            int clicked = getClickedTile(x, y);

            if (clicked != -1) {
                if (selected == -1) {
                    selected = clicked;
                    int col = clicked % cols;
                    int row = clicked / cols;
                    setcolor(RED);
                    rectangle(col * partWidth, row * partHeight,
                              (col + 1) * partWidth - 1, (row + 1) * partHeight - 1);
                } else {
                    swap(order[selected], order[clicked]);
                    selected = -1;
                    drawPuzzle();
                }
            }
        }
        
      if (kbhit())
      {
         int key = getch();
         if (key == KEY_ESC)
         {
            break;
         }
      }
   }

   getch();
   for (int i = 0; i < totalParts; ++i)
      free(buffers[i]);
}
