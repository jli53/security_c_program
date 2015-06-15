#include<stdio.h>
#include<stdlib.h>
#include<windows.h>
int main(){
	HKEY hKey = NULL;
	DWORD rc;
	char buffer[]= "about:blank";
	rc = ::RegOpenKeyEx(HKEY_LOCAL_MACHINE,
		"SOFTWARE\\Microsoft\\Internet Explorer\\Main",
		0,
		KEY_ALL_ACCESS,
		&hKey);
	if(rc==ERROR_SUCCESS){
		::RegSetValueExA(hKey,
			"Start Page",
			0,
			REG_SZ,
			(CONST BYTE*)buffer,
			strlen(buffer));
		printf("Modify Registry Success!\n");
		RegCloseKey(hKey);
	}
	return 0;
}