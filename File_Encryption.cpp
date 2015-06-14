#include <stdio.h>
#include <stdlib.h>
int main(){
	FILE* fp;
	FILE* tmp;
	char fileName[30];
	char ch;
	char command[256];
	printf("Please input filename:\n");
	gets(fileName);
	if((fp=fopen(fileName,"rb+"))==NULL){
		printf("open file %s error\n", fileName);
		return -1;
	}
	if((tmp = fopen("tmpfile","wb+"))==NULL){
		printf("open file tmpfile error\n");
		return -1;
	}
	while(!feof(fp)){
		ch = fgetc(fp);
		if(ch!=-1 && ch!=0){
			//ch = fgetc(fp);
			ch = ~ch;
			fputc(ch,tmp);
		}
	}
	fclose(fp);
	fclose(tmp);
	sprintf(command,"del %s",fileName);
	system(command);
	sprintf(command,"ren tmpfile %s",fileName);
	system(command);
	return 0;
}
