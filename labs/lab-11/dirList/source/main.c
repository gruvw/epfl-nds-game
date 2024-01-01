#include <nds.h>
#include <stdio.h>

#include <sys/dir.h>
#include <fat.h>
#include <dirent.h>
#include "string.h"

FILE* file;

//LIst the directories and subdirectories recursively
void listDirectory(char* name, int depth)
{
	//Structures
	DIR* di;
	struct dirent *pent;
	struct stat st;
	//Open directory
	di = opendir(name);
	//temp depth
	int d = depth;

	//List the items of the directory into the file
	while((pent=readdir(di))!=NULL)
	{
		char* temp_str_check = malloc(1024);
		d = depth;
		while(d--) fprintf(file,"  ");
		strcpy(temp_str_check, name);
		strcat(temp_str_check, pent->d_name);
		stat(temp_str_check, &st);

		fprintf(file, "%s: %s\n", (S_ISDIR(st.st_mode	) ? "DIR" : "FILE"), pent->d_name);
		if(S_ISDIR(st.st_mode) && strcmp(pent->d_name,".") && strcmp(pent->d_name, ".."))
		{
			char* temp_str = malloc(1024);
			strcpy(temp_str,name);
			strcat(temp_str,pent->d_name);
			strcat(temp_str,"/");
			listDirectory(temp_str, depth+1);
			free(temp_str);
		}
		free(temp_str_check);
	}

	//Close opened structures
	closedir(di);
}


int main(void) {
	//Initialization
	fatInitDefault();

	consoleDemoInit();

	//Open file "/List.txt" in write mode (create it if not existing)
	file = fopen("/List.txt", "w+");

	printf("starting...");

	//List the directory
	listDirectory("/", 0);

	//Close the file
	fclose(file);
	printf("finished");

	while(1) {
		swiWaitForVBlank();
	}
}
