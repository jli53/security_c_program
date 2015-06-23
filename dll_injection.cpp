#include<windows.h>
#include<stdio.h>
#include<tlhelp32.h>

BOOL LoadDll(DWORD dwProcessId, LPTSTR lpszDllName){
	HANDLE hProcess = NULL;
	HANDLE hThread = NULL;
	LPVOID pszDllFile = NULL;

	hProcess = OpenProcess(PROCESS_ALL_ACCESS,FALSE,dwProcessId);
	if(hProcess==NULL)
		return FALSE;
	printf("open process %d successfully!\n\n",dwProcessId);

	int cch = 1+strlen(lpszDllName);
	pszDllFile = VirtualAllocEx(hProcess,NULL,cch,MEM_COMMIT,PAGE_READWRITE);
	if(pszDllFile == NULL)
		return FALSE;
	printf("allocate remote space successfully!\n\n");

	if((WriteProcessMemory(hProcess,(PVOID)pszDllFile,(PVOID)lpszDllName,cch,NULL))==FALSE)
	{
		return FALSE;
	}
	printf("write remote memory succesfully!\n\n");

	PTHREAD_START_ROUTINE pfnThreadRtn =(PTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("kernel32.dll"),"LoadLibraryA");
	if(pfnThreadRtn == NULL)
		return FALSE;
	printf("get LoadLibraryA function address successfully!\n\n");

	hThread = CreateRemoteThread(hProcess,NULL,0,pfnThreadRtn,pszDllFile,0,NULL);
	if(hThread==NULL)
		return FALSE;
	printf("create remote thread successfully!\n\n");

	system("pause");
	WaitForSingleObject(hThread,INFINITE);
	VirtualFreeEx(hProcess,(PVOID)pszDllFile,0,MEM_RELEASE);
	CloseHandle(hThread);
	CloseHandle(hProcess);
	return TRUE;
}

void main(){
	char lpDllName[MAX_PATH] = TEXT("test.dll");

	PROCESSENTRY32 ProcessEntry = {0};
	HANDLE hProcessSnap;
	ProcessEntry.dwSize = sizeof(PROCESSENTRY32);
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	BOOL bRet = Process32First(hProcessSnap,&ProcessEntry);

	while(bRet)
	{
		if(strcmp("calc.exe",ProcessEntry.szExeFile)==0){
			printf("%s",ProcessEntry.szExeFile);
			LoadDll(ProcessEntry.th32ProcessID,lpDllName);
			break;
		}
		bRet = Process32Next(hProcessSnap,&ProcessEntry);
	}
}