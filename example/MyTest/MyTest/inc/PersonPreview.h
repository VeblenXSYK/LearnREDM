//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: PersonPreview.h 
// File Des: Ԥ������
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-4-14	1.0		
//-------------------------------------------------------
#pragma once

#include <string>
#include "DMResFolderImpl.h"

class CShootSystem;
class PersonPreview : public DUIWindow
{
	DMDECLARE_CLASS_NAME(PersonPreview, L"PersonPreview", DMREG_Window);
public:
	PersonPreview();
	DM_BEGIN_MSG_MAP()
		DM_MSG_WM_PAINT(DM_OnPaint)
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_LBUTTONUP(OnLButtonUp)
		MSG_WM_MOUSEMOVE(OnMouseMove)
	DM_END_MSG_MAP()

	enum MouseMode 
	{ 
		NoneMode = -1, 
		MoveMode, 
		ZoomMode
	};

public:
	//---------------------------------------------------
	// Function Des: DUI����Ϣ�ַ�ϵ�к���
	//---------------------------------------------------
	void DM_OnPaint(IDMCanvas* pCanvas);
	void OnLButtonDown(UINT nFlags, CPoint pt);
	void OnLButtonUp(UINT nFlags, CPoint pt);
	void OnMouseMove(UINT nFlags, CPoint pt);

public:
	DMSmartPtrT<IDMSkin>          m_pSkin;
	std::wstring				  m_picPath;			// ����ÿؼ���ӦͼƬ��·��
	DUIWindow					  *p_Parent;			// ������

	// �϶�����
	bool						  m_bDown;				// ��갴��
	int							  m_iMode;				// ģʽ
	CPoint						  m_StartDragPt;
	CPoint						  m_TrackDragPt;
	CRect						  m_StartDragRc;

private:
	void OnDragLeft(CPoint pt);			// �϶����
	void OnDragLeftTop(CPoint pt);		// �϶����Ͻ�
	void OnDragTop(CPoint pt);			// �϶��ϱ�
	void OnDragRightTop(CPoint pt);		// �϶����Ͻ�
	void OnDragRight(CPoint pt);		// �϶��ұ�
	void OnDragRightBottom(CPoint pt);	// �϶����½�
	void OnDragBottom(CPoint pt);		// �϶��±�
	void OnDragLeftBottom(CPoint pt);	// �϶����½�
};