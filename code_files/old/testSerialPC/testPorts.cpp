#include <windows.h>
#include <initguid.h>
#include <devguid.h>
#include <setupapi.h>
#include <stdio.h>


int main() {
	SP_DEVINFO_DATA devInfoData = {};
	devInfoData.cbSize = sizeof(devInfoData);
	
	// Get port info tree:
	HDEVINFO hDeviceInfo = SetupDiGetClassDevs(	&GUID_DEVCLASS_PORTS,
												0,
												NULL,
												DIGCF_PRESENT
												);
	if (hDeviceInfo == INVALID_HANDLE_VALUE)
		return 0;
	
	// Iterate over devices
	int nDevice = 0;
	while(SetupDiEnumDeviceInfo(	hDeviceInfo,
									nDevice++,
									&devInfoData)) {
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
		BYTE* hardwareID = new BYTE[(reqSize > 1) ? reqSize : 1];
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
			BYTE* friendlyName = new BYTE[(reqSize > 1) ? reqSize : 1];
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
			puts("PortID\tFriendly Name");
			printf("%s\t%s\n", hardwareID, friendlyName);
			//-----------------------------------------------------------------------
			//	End use friendly name
			//-----------------------------------------------------------------------
			delete[] friendlyName;
		}
		delete[] hardwareID;
	}
	return 0;
}