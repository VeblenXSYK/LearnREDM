#pragma once
#define WIN32_LEAN_AND_MEAN             // �� Windows ͷ���ų�����ʹ�õ�����
#include <windows.h>
#include <tchar.h> 

// DM
#include "DmMainOutput.h"

// lib
#ifdef _DEBUG
#pragma comment(lib,"DmMain_d.lib")
#else
#pragma comment(lib,"DmMain.lib")
#endif

using namespace DM;

#include "DUITreeCtrl.h"
#include "DUIStatic.h"
#include "DUIEdit.h"
#include "DUIListBoxEx.h"
#include "DUISliderCtrl.h"
#include "DUIButton.h"
#include "DUIScrollBase.h"
#include "DUIFlowLayout.h"

