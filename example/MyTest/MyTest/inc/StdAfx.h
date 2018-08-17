#pragma once
#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料
#include <windows.h>
#include <tchar.h> 

// DM
#include "DmMainOutput.h"

// PSCore
#include "PSExport.h"
//#include "../PSCore/PSExport.h"


// lib
#ifdef _DEBUG
#pragma comment(lib,"DmMain_d.lib")
#pragma comment(lib,"PsDump_d.lib")
#pragma comment(lib,"PsDump_d.lib")
#pragma comment(lib,"Jpeg.lib")
#pragma comment(lib,"PSCore.lib")
#else
#pragma comment(lib,"DmMain.lib")
#endif

#pragma comment(lib, "legacy_stdio_definitions.lib")

/*
* 当libjpeg-turbo为vs2010编译时，vs2015下静态链接libjpeg-turbo会链接出错:找不到__iob_func,
* 增加__iob_func到__acrt_iob_func的转换函数解决此问题,
* 当libjpeg-turbo用vs2015编译时，不需要此补丁文件
*/
#if _MSC_VER >= 1900
#include "stdio.h" 
_ACRTIMP_ALT FILE* __cdecl __acrt_iob_func(unsigned);
#ifdef __cplusplus 
extern "C"
#endif 
FILE* __cdecl __iob_func(unsigned i) {
	return __acrt_iob_func(i);
}
#endif /* _MSC_VER>=1900 */

using namespace DM;

#include "PsdParser.h"
#include "DUITreeCtrl.h"
#include "DUIStatic.h"
#include "DUIEdit.h"
#include "DUIListBoxEx.h"
#include "DUISliderCtrl.h"
#include "DUIButton.h"
#include "DUIScrollBase.h"
#include "DUIFlowLayout.h"
#include "DUICheckBox.h"

