#include <stdio.h>
#include <dirent.h>
#include <ncurses.h>

void print_menu(int highlight, int noOfFiles);

int main(void) {
  DIR *d;
  int noOfFiles;

  int highlight = 1;
	int choice = 0;
	int c;

  struct dirent **fileList;

  noOfFiles = scandir(".", &fileList, NULL, alphasort);

  initscr();
	clear();
	noecho();
	cbreak();
  keypad(stdscr, TRUE);

  for(int i = 0; i < noOfFiles; ++i) {
    if(highlight == i + 1) {
      attron(A_REVERSE);
      mvprintw(5+i, 5, "%s", fileList[i]->d_name);
      attroff(A_REVERSE);
    } else {
      mvprintw(5+i, 5, "%s", fileList[i]->d_name);
    }
  }
  mvprintw(5+noOfFiles+1, 5, "Estoy en %d: Lei %s",highlight,fileList[highlight-1]->d_name);
  refresh();

  do {
    c = getch();
    switch(c)
		{	case KEY_UP:
				if(highlight == 1)
					highlight = noOfFiles;
				else
					--highlight;
				break;
			case KEY_DOWN:
				if(highlight == noOfFiles)
					highlight = 1;
				else
					++highlight;
				break;
			// case 10:
			// 	choice = highlight;
			// 	break;
		}
    if(choice != 0)
			break;
    for(int i = 0; i < noOfFiles; ++i) {
      if(highlight == i + 1) {
        attron(A_REVERSE);
    		mvprintw(5+i, 5, "%s", fileList[i]->d_name);
    		attroff(A_REVERSE);
      } else {
        mvprintw(5+i, 5, "%s", fileList[i]->d_name);
      }
  	}
    move(5+noOfFiles+1, 5);
    clrtoeol();
    printw("Estoy en %d: Lei %s",highlight,fileList[highlight-1]->d_name);
    refresh();
  } while (c != 'q');
  // mvprintw(23, 0, "Seleccionaste %s en la posicion %d\n", fileList[choice-1]->d_name, choice);
  clrtoeol();
  refresh();
  endwin();
  return(0);
}
