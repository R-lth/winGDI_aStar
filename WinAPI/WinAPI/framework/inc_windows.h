#pragma once

// 최적화와 메모리 누수 검사를 위한 헤더입니다.

// windows의 min, max를 사용하지 않아, c++ std의 min, max와 충돌하지 않습니다.
#ifndef NOMINMAX
#define NOMINMAX
#endif

// 거의 사용하지 않는 windows의 서브 기능은 헤더에서 제외시킵니다.
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include <Windows.h>
#include <iostream>

// 디버그 중 메모리 누수가 일어나면 어떤 파일의 몇 번째 줄에서 메모리 누수가 일어나는지 안내하는 매크로입니다.
#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif