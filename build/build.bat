@echo off

cl ..\raknet_c\*.cpp /O2 /I ..\fb-raknet\Source\ /D_USRDLL /D_WINDLL /LD ..\fb-raknet\Source\*.cpp /MT /link ws2_32.lib /EXPORT:RN_RakPeerGetInstance /EXPORT:RN_RakPeerDestroyInstance /EXPORT:RN_RakPeerStartup /EXPORT:RN_RakPeerConnect /EXPORT:RN_RakPeerShutdown /EXPORT:RN_RakPeerReceive /EXPORT:RN_RakPeerDeallocatePacket /EXPORT:RN_RakPeerSend /EXPORT:RN_SetRakNetProtocolVersion
del *.obj
