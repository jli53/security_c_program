#include<stdio.h>
#include<windows.h>
#include<string.h>
int main(){
	char strName[MAX_PATH];
	char fileName[256];
	int index = 0;
	HMODULE hModule = GetModuleHandle(NULL);
	::GetModuleFileName(hModule,strName,sizeof(strName));
	for(int i = sizeof(strName)-1; i>=0; i--){
		if(strName[i] == '\\' || strName[i] == '/'){
			index = i+1;
			break;
		}
	}
	if(index==0){
		printf("find filename error!\n");
		return -1;
	}
	strcpy(fileName,strName+index);
	FILE *fp;
	fp = fopen("deleteself.bat","wb+");
	fprintf(fp,"@echo off\r\n");
	fprintf(fp,":start\r\n\t");
	fprintf(fp,"if not exist %s goto done\r\n\t", fileName);
	fprintf(fp,"del /f /q %s\r\n",fileName);
	fprintf(fp,"goto start\r\n");
	fprintf(fp,":done\r\n\t");
	fprintf(fp,"del /f /q %%0");
	fclose(fp);
	ShellExecute(NULL,"open","deleteself.bat",NULL,NULL,SW_HIDE);
	return 0;
}