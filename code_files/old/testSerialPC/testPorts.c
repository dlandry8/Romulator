#include <windows.h>
#include <initguid.h>
#include <devguid.h>
#include <setupapi.h>
#include <stdio.h>
#include <string.h>

int getPortNumber(char*);

int main() {
	char portNumber[7];
	int portStatus = getPortNumber(portNumber);
	//printf("%s\n",portSta);
	if (portStatus > 0)
		printf("The Arduino is at port %s.", portNumber);
	else
		puts("The Arduino is not connected.");
	return 0;
}

int getPortNumber(char* inputString) {
	int foundIt = -1; 	
	SP_DEVINFO_DATA devInfoData = {};
	devInfoData.cbSize = sizeof(devInfoData);
	
	// Get port info tree:
	HDEVINFO hDeviceInfo = SetupDiGetClassDevs(	NULL,
												NULL,
												NULL,
												DIGCF_ALLCLASSES | DIGCF_PRESENT
												);
	if (hDeviceInfo == INVALID_HANDLE_VALUE)
		return -1;
	
	// Iterate over devices
	int nDevice = 0;
	while(SetupDiEnumDeviceInfo(	hDeviceInfo,
									nDevice++,
									&devInfoData)
			&& foundIt < 0) {
		DWORD regDataType;
		DWORD reqSize = 0;
		
		// Find size required to hold device info.
		SetupDiGetDeviceRegistryProperty(	hDeviceInfo,
											&devInfoData,
											SPDRP_HARDWAREID,
											NULL,
											NULL,
											0,
											&reqSize);
		BYTE* hardwareID = (BYTE*)malloc((reqSize > 1) ? sizeof(BYTE) * reqSize : 1);
		// Store in buffer
		if (SetupDiGetDeviceRegistryProperty(	hDeviceInfo,
												&devInfoData,
												SPDRP_HARDWAREID,
												&regDataType,
												hardwareID,
												sizeof(hardwareID) * reqSize,
												NULL)) {
			// Find size needed to hold friendly name.
			reqSize = 0;
			SetupDiGetDeviceRegistryProperty(	hDeviceInfo,
												&devInfoData,
												SPDRP_FRIENDLYNAME,
												NULL,
												NULL,
												0,
												&reqSize);
			BYTE* friendlyName = (BYTE*)malloc((reqSize > 1) ? sizeof(BYTE) * reqSize : 1);
			// Store in buffer:
			if (!SetupDiGetDeviceRegistryProperty(	hDeviceInfo,
													&devInfoData,
													SPDRP_FRIENDLYNAME,
													NULL,
													friendlyName,
													sizeof(friendlyName) * reqSize,
													NULL)) {
				// Device does not have this property set:
				memset(friendlyName, 0, (reqSize > 1) ? reqSize : 1);
			}
			//-----------------------------------------------------------------------
			//	Use Friendly Name
			//-----------------------------------------------------------------------
			if (strstr(friendlyName, "Arduino Mega 2560") != NULL) {
				puts("Friendly Name:");
				printf("%s\n", friendlyName);
				//char comPort2[4];
				inputString = strncpy(inputString, friendlyName + 19, strlen(friendlyName) - 20);
				inputString[strlen(friendlyName) - 2 - 19 + 1] = '\0';
				foundIt = 1;
			}
			//-----------------------------------------------------------------------
			//	End use friendly name
			//-----------------------------------------------------------------------
			free(friendlyName);
		}
		free(hardwareID);
	}
	return foundIt;
}