#include <string.h>
#include <ncurses.h>
 
int main()
{
    initscr();
    raw();
    noecho();
    curs_set(0);
 
    char* c = "Hello, World!";
 
    mvprintw(LINES/2,(COLS-strlen(c))/2,c);
    refresh();
 
    getch();
    endwin();
 
    return 0;
}
