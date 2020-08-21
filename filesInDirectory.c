#include <stdio.h>
#include <dirent.h>

int main(void) {
  DIR *d;
  int noOfFiles;
  struct dirent **fileList;
  noOfFiles = scandir(".", &fileList, NULL, alphasort);
  for(int i = 0; i < noOfFiles; i++) printf("%s\n", fileList[i]->d_name);
  return(0);
}
