// header.h: 표준 시스템 포함 파일
// 또는 프로젝트 특정 포함 파일이 들어 있는 포함 파일입니다.
//

#pragma once

#include "targetver.h"
#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용을 Windows 헤더에서 제외합니다.
// Windows 헤더 파일
#include <windows.h>
#include <windowsx.h>
// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <memory>
#include <vector>
#include <list>
#include <string>

#include <d3d11_4.h>
#include <d3d12.h>
#include <dxgi1_6.h>

#include <wrl.h>

#include <d3dcompiler.h>

#include <DirectXMath.h>
#include <DirectXCollision.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")

using namespace DirectX;
using Microsoft::WRL::ComPtr;

// size를 multiple의 배수로 변환한 값을 반환
// multiple이 2의 제곱수일 때 정상 작동
int Align(int size, int multiple);


// DX11용 열거자
enum ShaderType { ST_NONE, ST_VS, ST_HS, ST_DS, ST_GS, ST_PS, ST_CS };