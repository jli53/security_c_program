#include<stdio.h>
#include<string.h>
#include<stdlib.h>
int main(){
	FILE* fp;
	char fileName[30];
	char command[256];
	int i;
	char buffer[1024];
	int len;
	printf("please input file name:\n");
	gets(fileName);
	if((fp = fopen(fileName,"rb+"))==NULL){
		printf("open file %s error!\n", fileName);
		return -1;
	}
	fseek(fp,0,2);
	len = ftell(fp);
	for(i = 0; i<3; i++){
		memset(buffer,i,1024);
		rewind(fp);
		fwrite(buffer,1,len>1024?1024:len,fp);
	}
	fclose(fp);
	sprintf(command,"del %s",fileName);
	system(command);
	return 0;
}
