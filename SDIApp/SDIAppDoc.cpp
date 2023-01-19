
// SDIAppDoc.cpp : реализация класса CSDIAppDoc
//

#include "stdafx.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "SDIApp.h"
#endif

#include "SDIAppDoc.h"

#include <propkey.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <gsl/gsl_errno.h>
#include <gsl/gsl_spline.h>


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <fstream>

// CSDIAppDoc

IMPLEMENT_DYNCREATE(CSDIAppDoc, CDocument)

BEGIN_MESSAGE_MAP(CSDIAppDoc, CDocument)
END_MESSAGE_MAP()


// создание/уничтожение CSDIAppDoc

CSDIAppDoc::CSDIAppDoc()
{
	// TODO: добавьте код для одноразового вызова конструктора
	m_bRed = true;

}

CSDIAppDoc::~CSDIAppDoc()
{
}

BOOL CSDIAppDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: добавьте код повторной инициализации
	// (Документы SDI будут повторно использовать этот документ)
	/*for (int i=0; i < 1000; i++)
	{
		pt point;
		point.x = i;
		point.y = i*sin(i) / log(i);
		m_Points.push_back(point);
	}*/

	m_pTree->UpdateTree();
	// m_pTree->m_pDoc = this;

	return TRUE;
}




// сериализация CSDIAppDoc

void CSDIAppDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: добавьте код сохранения
		size_t count = xx.size();
		ar << count;

		for (size_t i = 0; i < count; i++)
		{
			ar << xx[i];
			ar << yy[i];
		}
	}
	else
	{
		// TODO: добавьте код загрузки
		size_t count;
		ar >> count;

		//xx.clear();
		//yy.clear();
		//for (size_t i = 0; i < count; i++)
		//{
		//	pt point;
		//	ar >> point.x;
		//	ar >> point.y;
		//	m_Points.push_back(point);
		//}
		xx.clear();
		yy.clear();

		double x, y;
		std::ifstream file(ar.m_strFileName);
		while (!file.eof())
		{
			file >> x;
			file >> y;
			xx.push_back(x);
			yy.push_back(y);
		}
		file.close();
	}
}

#ifdef SHARED_HANDLERS

// Поддержка для эскизов
void CSDIAppDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// Измените этот код для отображения данных документа
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// Поддержка обработчиков поиска
void CSDIAppDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// Задайте содержимое поиска из данных документа. 
	// Части содержимого должны разделяться точкой с запятой ";"

	// Например:  strSearchContent = _T("точка;прямоугольник;круг;объект ole;");
	SetSearchContent(strSearchContent);
}

void CSDIAppDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// диагностика CSDIAppDoc

#ifdef _DEBUG
void CSDIAppDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSDIAppDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}

//void CSDIAppDoc::Method1()
//{
//	m_PointsInterpol.clear();
//	pt point;
//	int i;
//	double xi, yi, x[10], y[10];
//	printf("#m=0,S=17\n");
//	for (i = 0; i < 10; i++)
//	{
//		//x[i] = i + 0.5 * sin(i);
//		//y[i] = i + cos(i * i);
//		x[i] = i + 0.5 * sin(i);
//		y[i] = i + cos(i * i);
//		printf("%g %g\n", x[i], y[i]);
//	}
//	printf("#m=1,S=0\n");
//	{
//		gsl_interp_accel* acc = gsl_interp_accel_alloc();
//		gsl_spline* spline = gsl_spline_alloc(gsl_interp_cspline, 10);
//		gsl_spline_init(spline, x, y, 10);
//		for (xi = x[0]; xi < x[9]; xi += 0.01)
//		{
//			yi = gsl_spline_eval(spline, xi, acc);
//			printf("%g %g\n", xi, yi);
//			point.x = xi;
//			point.y = yi;
//			m_PointsInterpol.push_back(point);
//		}
//		gsl_spline_free(spline);
//		gsl_interp_accel_free(acc);
//	}
//
//}
//
//void CSDIAppDoc::Method2()
//{
//
//}
#endif //_DEBUG


// команды CSDIAppDoc
