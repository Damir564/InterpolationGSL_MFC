
// SDIAppView.cpp : реализация класса CSDIAppView
//

#include "stdafx.h"
// SHARED_HANDLERS можно определить в обработчиках фильтров просмотра реализации проекта ATL, эскизов
// и поиска; позволяет совместно использовать код документа в данным проекте.
#ifndef SHARED_HANDLERS
#include "SDIApp.h"
#endif

#include "SDIAppDoc.h"
#include "SDIAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <gsl/gsl_interp.h>
#include <gsl/gsl_spline.h>

#define PI 3.14

using namespace std;

// CSDIAppView

IMPLEMENT_DYNCREATE(CSDIAppView, CView)

BEGIN_MESSAGE_MAP(CSDIAppView, CView)
	// Стандартные команды печати
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
END_MESSAGE_MAP()

// создание/уничтожение CSDIAppView

CSDIAppView::CSDIAppView()
{
	// TODO: добавьте код создания

}

CSDIAppView::~CSDIAppView()
{
}

BOOL CSDIAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: изменить класс Window или стили посредством изменения
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// рисование CSDIAppView

void CSDIAppView::OnDraw(CDC* pDC)
{
	CSDIAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	CRect rect;
	GetClientRect(&rect);
	//pDC->MoveTo(0, rect.Height() / 2);

	int arrayLength = pDoc->xx.size();
	// double xx[10], yy[10];
	
	//vector<double> xx = pDoc->xx;
	//vector<double> yy = pDoc->yy;
	if (arrayLength == 0)
		return;

	double* xx = &pDoc->xx[0];
	double* yy = &pDoc->yy[0];
	double splitBy = yy[arrayLength - 1];
	double xi, yi;
	//double xx[10], yy[10];
	// TODO: добавьте здесь код отрисовки для собственных данных
	//float x, y;
	//for (int i = 0; i < rect.Width(); i++)
	//{
	//	x = (float)i * mx;
	//	// y = sin(x)*my;
	//	y = sin(i) * rect.Height();
	//	// pDC->LineTo(i, rect.Height() / 2 - y);
	//	pDC->LineTo(i, y);
	//}
	//pDC->LineTo(0, rect.Height() / 2);
	//for (x = 0; x <= 2 * PI; x += PI / 1024)
	//{
	//	// y = sin(x)*my;
	//	y = sin(x);
	//	// pDC->LineTo(i, rect.Height() / 2 - y);
	//	pDC->LineTo(x * rect.Width() / (PI * 2), (y + 1) * rect.Height() / 2);
	//}
	//pDC->LineTo(0, rect.Height() / 2);
	// Задаём точки функции

	CPen pen0;
	pen0.CreatePen(PS_SOLID, 3, RGB(0, 0, 0));
	pDC->SelectObject(&pen0);
	pDC->MoveTo(xx[0] * rect.Width() / splitBy, yy[0] * rect.Height() / splitBy);
	//for (int i = 0; i < arrayLength; i++)
	//{
	//	xx[i] = i + 0.5 * sin(i);
	//	yy[i] = i + cos(i * i);
	//}
	// выводим точки функции
	for (int i = 0; i < arrayLength; i++)
	{
		pDC->LineTo(xx[i] * rect.Width() / splitBy, yy[i] * rect.Height() / splitBy);
	}
	pDC->MoveTo(xx[0] * rect.Width() / splitBy, yy[0] * rect.Height() / splitBy);

	CPen pen;
	if (pDoc->m_bRed)
		pen.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	else
		pen.CreatePen(PS_SOLID, 1, RGB(0, 0, 255));

	pDC->SelectObject(&pen);
	

	if (pDoc->m_methodToUse == 0)
	{
		// кусочно линейная интерполяция
		double xi, yi, aa[2];
		// aa[0] = yy[0] - (yy[1] - yy[0]) / (xx[1] - xx[0]) * xx[0];
		//aa[1] = (y[i] - y[i - 1]) / (x[i] - x[i - 1]);
		//aa[0] = y[i - 1] - aa[1] * x[i - 1];
		for (int i = 1; i < 10; i++)
		{
			// aa[1] = (yy[i] - yy[i - 1]) / (xx[i] - xx[i - 1]);
			for (xi = xx[i - 1]; xi < xx[i]; xi += 0.01)
			{
				aa[1] = (yy[i] - yy[i - 1]) / (xx[i] - xx[i - 1]);
				aa[0] = yy[i - 1] - aa[1] * xx[i - 1];
				yi = aa[0] + aa[1] * xi;
				pDC->LineTo(xi * rect.Width() / splitBy, yi * rect.Height() / splitBy);
				// deltaY = yi - tempY;
			}
			// aa[0] = aa[1];
		}
		pDC->MoveTo(xx[0] * rect.Width() / splitBy, yy[0] * rect.Height() / splitBy);
	}
	else if (pDoc->m_methodToUse == 1)
	{
		// Лаграндж
		for (xi = xx[0]; xi < xx[arrayLength - 1]; xi += 0.01)
		{
			double sum = 0;
			for (int i = 0; i < arrayLength; ++i)
			{
				double l = 1;
				for (int j = 0; j < arrayLength; ++j)
				{
					if (j != i)
					{
						l *= (xi - xx[j]) / (xx[i] - xx[j]);
					}
				}
				sum += yy[i] * l;
			}
			pDC->LineTo(xi * rect.Width() / splitBy, sum * rect.Height() / splitBy);
		}
		pDC->MoveTo(xx[0] * rect.Width() / splitBy, yy[0] * rect.Height() / splitBy);
	}
	else if (pDoc->m_methodToUse == 2)
	{
	// кубическиий сплайн
		gsl_interp_accel* acc
			= gsl_interp_accel_alloc();
		gsl_spline* spline
			= gsl_spline_alloc(gsl_interp_cspline, 10);
		gsl_spline_init(spline, xx, yy, 10);
		for (xi = xx[0]; xi < xx[9]; xi += 0.01)
		{
			yi = gsl_spline_eval(spline, xi, acc);
			pDC->LineTo(xi * rect.Width() / splitBy, yi * rect.Height() / splitBy);
			//printf("%g %g\n", xi, yi);
		}
		gsl_spline_free(spline);
		gsl_interp_accel_free(acc);
		pDC->MoveTo(xx[0] * rect.Width() / splitBy, yy[0] * rect.Height() / splitBy);
	}
	//for (int i = 0; i <= 10; i++)
	//{
	//	x = i * PI / 10;
	//	y = sin(x);
	//	pDC->LineTo(x * rect.Width(), rect.Height() / 2  - y);
	//}

	//if (pDoc->m_PointsInterpol.size() > 0)
	//{
	//	for (int i = 0; i != pDoc->m_PointsInterpol.size(); ++i)
	//	{
	//		// x = (float)pDoc->m_PointsInterpol.at(i).x * mx;
	//		x = (float)pDoc->m_PointsInterpol.at(i).x * 2;
	//		// y = pDoc->m_PointsInterpol.at(i).y / my;
	//		y = pDoc->m_PointsInterpol.at(i).y * 2;
	//		pDC->LineTo(x, rect.Height() / 2 - y);
	//	}
	//}
}


// печать CSDIAppView

BOOL CSDIAppView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// подготовка по умолчанию
	return DoPreparePrinting(pInfo);
}

void CSDIAppView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте дополнительную инициализацию перед печатью
}

void CSDIAppView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: добавьте очистку после печати
}


// диагностика CSDIAppView

#ifdef _DEBUG
void CSDIAppView::AssertValid() const
{
	CView::AssertValid();
}

void CSDIAppView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CSDIAppDoc* CSDIAppView::GetDocument() const // встроена неотлаженная версия
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSDIAppDoc)));
	return (CSDIAppDoc*)m_pDocument;
}
#endif //_DEBUG


// обработчики сообщений CSDIAppView
