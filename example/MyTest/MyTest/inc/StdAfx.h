#pragma once
#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
#include <windows.h>
#include <tchar.h> 

// DM
#include "DmMainOutput.h"

// lib
#ifdef _DEBUG
#pragma comment(lib,"DmMain_d.lib")
#pragma comment(lib,"PsDump_d.lib")
#pragma comment(lib,"PsDump_d.lib")
#pragma comment(lib,"Jpeg.lib")
#else
#pragma comment(lib,"DmMain.lib")
#endif

/*
* ��libjpeg-turboΪvs2010����ʱ��vs2015�¾�̬����libjpeg-turbo�����ӳ���:�Ҳ���__iob_func,
* ����__iob_func��__acrt_iob_func��ת���������������,
* ��libjpeg-turbo��vs2015����ʱ������Ҫ�˲����ļ�
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

