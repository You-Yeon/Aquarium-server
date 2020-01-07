// Common.cpp : 정적 라이브러리를 위한 함수를 정의합니다.
//

#include "pch.h"
#include "framework.h"
#include "Common.h"

//#SHOW15 Server information
Guid g_protocolVersion = Guid::From({ 0xe71e4cec, 0x204e, 0x4374, { 0xb1, 0x63, 0x18, 0x6d, 0xc5, 0xc8, 0x4f, 0x5 } });
int g_serverPort = 35475;
