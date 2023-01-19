// MyView.cpp: файл реализации
//

#include "stdafx.h"
#include "SDIApp.h"
#include "MyView.h"
#include "SDIAppDoc.h"


// CMyView

IMPLEMENT_DYNCREATE(CMyView, CTreeView)

CMyView::CMyView()
{

}

CMyView::~CMyView()
{
}

BEGIN_MESSAGE_MAP(CMyView, CTreeView)
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// диагностика CMyView

#ifdef _DEBUG
void CMyView::AssertValid() const
{
	CTreeView::AssertValid();
}

#ifndef _WIN32_WCE
void CMyView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif
#endif //_DEBUG


// обработчики сообщений CMyView

void CMyView::UpdateTree()
{
	CTreeCtrl &tree = GetTreeCtrl();

	tree.DeleteAllItems();

	m_hRoot = tree.InsertItem(L"Color", -1, -1, NULL, TVI_FIRST);

	m_hItem1 = tree.InsertItem(L"Red", -1, -1, m_hRoot, TVI_LAST);
	m_hItem2 = tree.InsertItem(L"Blue", -1, -1, m_hRoot, TVI_LAST);

	tree.Expand(m_hRoot, TVE_EXPAND);

	tree.SetItemData(m_hItem1, 1);
	tree.SetItemData(m_hItem2, 0);

	m_hRoot1 = tree.InsertItem(L"Метод интерполяции", -1, -1, NULL, TVI_FIRST);

	m_hItem11 = tree.InsertItem(L"Кусочно-линейная интерполяция", -1, -1, m_hRoot1, TVI_LAST);
	m_hItem12 = tree.InsertItem(L"Многочлен Лагранжа", -1, -1, m_hRoot1, TVI_LAST);
	m_hItem13 = tree.InsertItem(L"Кубический сплайн", -1, -1, m_hRoot1, TVI_LAST);

	tree.Expand(m_hRoot1, TVE_EXPAND);

	tree.SetItemData(m_hItem11, 2);
	tree.SetItemData(m_hItem12, 3);
	tree.SetItemData(m_hItem13, 4);
}


int CMyView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	lpCreateStruct->style |= TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT | TVS_SHOWSELALWAYS;

	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Добавьте специализированный код создания


	return 0;
}


void CMyView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: добавьте свой код обработчика сообщений или вызов стандартного
	CTreeCtrl &tree = GetTreeCtrl();
	CTreeView::OnLButtonDown(nFlags, point);
	HTREEITEM sel = tree.GetSelectedItem();

	if (tree.GetItemData(sel) <= 1) {
		m_pDoc->m_bRed = tree.GetItemData(sel);
	}
	else if (tree.GetItemData(sel) == 2) {
		m_pDoc->m_methodToUse = 0;
	}
	else if (tree.GetItemData(sel) == 3)
	{
		m_pDoc->m_methodToUse = 1;
	}
	else if (tree.GetItemData(sel) == 4)
	{
		m_pDoc->m_methodToUse = 2;
	}
	m_pDoc->m_pView->Invalidate();


	//else if (tree.GetItemData(sel) == 4) {
	//	m_pDoc->SelectEmployee();
	//}
	//else if (tree.GetItemData(sel) == 5)
	//{
	//	m_pDoc->SelectHistory();
	//}

	m_pDoc->m_pView->Invalidate(FALSE);
	CTreeView::OnLButtonDown(nFlags, point);
}
