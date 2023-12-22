/********************************************************************/
/*                           anim_proc_plus                         */
/*                           ==============                         */
/*    Функции для анимации  произвольного набора плоских объектов   */
/*        ( с примером - полёт квадрата с машущими крыльями         */
/*     с поворотом и увеличением  масштаба с середине траектории )  */
/********************************************************************/
#include "tdef.h"

#include "resource.h"

// #define ALL_STEP_TIME  400        // число  временных  интервалов

#define ALL_PT         4         // число  точек 
#define X_SPACE_ANIM   100.0     // Размер области вывода по X
#define Y_SPACE_ANIM    70.0     // Размер области вывода по Y

#define  K_SCALE  0.1        // коэффициент пропорциональности
#define  K_GAB    0.1           // Габаритный  коэффициент отступа
                                //  выводимого изображение от края границы вывода

#define  PI       3.14159265

int pr_start=0,znak=1;
double ang_wing=0.0;
/*-------------------------------------*/
/*  Координаты  квадрата               */
/*-------------------------------------*/
CVect  kvadro[ALL_PT] =
{	{  0.5,   0.5  },
	{ -0.5,   0.5 },
	{ -0.5,  -0.5 },
	{  0.5,  -0.5 }
};
/*-------------------------------------*/
/*  Координаты крыла                   */
/*-------------------------------------*/
CVect  wing[ALL_PT] =
{	{  0.0,   0.0  },
	{  0.5,   0.15 },
	{  1.0,   0.0 },
	{  0.5,   0.3 }
};

CVect thread[2] = { {0,0},{0,-5} },

tail[7] = { 
	{sin(2 * PI / 7), cos(2 * PI / 7)},
	{sin(2 * 2*PI / 7), cos(2 * 2 * PI / 7)},
	{sin(3 * 2 * PI / 7), cos(3 * 2 * PI / 7)},
	{sin(4 * 2 * PI / 7), cos(4 * 2 * PI / 7)},
	{sin(5 * 2 * PI / 7), cos(5 * 2 * PI / 7)},
	{sin(6 * 2 * PI / 7), cos(6 * 2 * PI / 7)},
	{sin(2 * PI), cos(2 * PI)}
};

void trans_coor (CVect* pvOld, CVect* pvNew , int all_pt , CMatr mpr);
CMatr  make_matr_pr (RECT &cp );
void draw_polygon (HDC hDc, CVect* pVect, int nPoints);

/********************************************************************/
/*                        anim_main                                 */
/*                        ==========                                */
/*      Управляющая  процедура  отрисовки  полёта  квадрата         */
/********************************************************************/
double ang;
BOOL  anim_main (HWND hwnd)
{
    int    i;

	extern int timer, derection;

	double dk,d_ang;
	CVect  vt,kvadro_win[ALL_PT],kvadro_real[ALL_PT],vt_wing, thread_real[2], thread_win[2], tail_win[7];

    CMatr  ms,mr,mt,mpr,ms_wing,mr_wing,mt_wing,mreal;

	PAINTSTRUCT ps;
	HDC hdc = BeginPaint (hwnd, &ps);

	if(pr_start==0)
    {
  	  pr_start=1;
   	  EndPaint (hwnd, &ps);
	  return TRUE;
    }

	HPEN hpen_prv = (HPEN)SelectObject(hdc, GetStockObject(NULL_PEN));

	HBRUSH
		hbr_red = CreateSolidBrush(RGB(255, 0, 0)),
		hbr_green = CreateSolidBrush(RGB(0, 255, 0)),
		hbr_prv = (HBRUSH)SelectObject(hdc, hbr_red);

    // определить размеры клиентской области окна
	RECT rc;
	GetClientRect (hwnd, &rc);

	mpr=make_matr_pr ( rc );

	double k_sc = X_SPACE_ANIM * K_SCALE, k_sc_wing = X_SPACE_ANIM * K_SCALE ;
	
    /*---------------------------------------*/
	/*      Перемещение по траектории        */
	/*---------------------------------------*/


	
	ang = double( timer) / 75;

	if (ang > PI/4)
	{
		derection = -1;
		
	}
	if (ang < -PI / 4)
	{
		derection = 1;
	}

	vt.x = X_SPACE_ANIM/2;
	vt.y = Y_SPACE_ANIM;


	SelectObject(hdc, hpen_prv);

	// квадрат

	MatrTransl(mt, vt);
	MatrRot(mr, ang);
	MatrScale(ms, k_sc, k_sc);
	mreal = ms * mr * mt;
	trans_coor(thread, thread_real, 2, mreal);
	trans_coor(thread, thread_win, 2, mreal * mpr);
	MoveToEx(hdc, thread_win[0].x, thread_win[0].y, 0);
	LineTo(hdc, thread_win[1].x, thread_win[1].y);

	double kf = 10*ang;
	if (kf < 0) kf = -kf;

	// крылья

	MatrTransl(mt, thread_real[1]);
	MatrRot(mr, ang );
	MatrScale(ms, k_sc - kf, k_sc - kf);
	trans_coor(tail, tail_win, 7, ms * mr * mt * mpr);
	
	SelectObject(hdc, hbr_red);

	draw_polygon(hdc, tail_win, 7);


	
   /*---------------------------------------*/

	SelectObject (hdc, hpen_prv);
	SelectObject (hdc, hbr_prv);
	DeleteObject (hbr_red);
	DeleteObject (hbr_green);

	EndPaint (hwnd, &ps);

	return TRUE;
}

/********************************************************************/
/*                        draw_polygon                              */
/*                        ============                              */
/*      Рисование многоугольника  на экране по  вещественным коорд  */
/********************************************************************/
void draw_polygon (HDC hDc, CVect* pVect, int nPoints)
{
	POINT* pPnt = new POINT [nPoints];
	for (int i=0; i<nPoints; i++) {
		pPnt[i].x = LONG(pVect[i].x);
		pPnt[i].y = LONG(pVect[i].y);
	}
	Polygon (hDc, pPnt, nPoints);
	delete [] pPnt;
}

/********************************************************************/
/*                        make_matr_pr                              */
/*                        ============                              */
/*              Формирование  матрицы  проекции                     */
/********************************************************************/
CMatr  make_matr_pr (RECT &cp )
{
	int a, b, gab_a, gab_b, xc_w, yc_w;
	double x_max, y_max, x_min, y_min, k_scale, k1, k2,
		   dx, dy, xc, yc,l_snar,l_pushka;

	CMatr  mt1,ms,mr,mt2;

	CVect  vt;
  
  /*--------------------------------*/
  /*  Параметры  окна  вывода       */
  /*--------------------------------*/ 
  a=abs(cp.right-cp.left);
  b=abs(cp.top-cp.bottom);

  gab_a=(int)((double)a*K_GAB);  // отступ по ширине
  gab_b=(int)((double)b*K_GAB);  // отступ по высоте

  a-=gab_a;
  b-=gab_b;

  xc_w=(cp.right+cp.left  )/2;
  yc_w=(cp.top  +cp.bottom)/2;

  /*-----------------------------------------*/
  /*  Определение  масштабного коэффициента  */
  /*-----------------------------------------*/ 
   x_min=0;
   y_min=0;

   x_max=X_SPACE_ANIM;
   y_max=Y_SPACE_ANIM;

   dx=fabs(x_max-x_min);
   dy=fabs(y_max-y_min);

   xc=(x_max+x_min)/2;
   yc=(y_max+y_min)/2;

   k1=a/dx;
   k2=b/dy;

   k_scale= ( k1 < k2 ) ? k1 : k2;

   vt.x=-xc;
   vt.y=-yc;
   MatrTransl (mt1,	vt);
   MatrScale ( ms ,  k_scale , k_scale);
   // Зеркальное отображение
   MatrScale ( mr ,  1 , -1);

   vt.x=xc_w;
   vt.y=yc_w;
   MatrTransl (mt2,	vt);

   return ( mt1 * ms * mr * mt2);
}

/********************************************************************/
/*                           trans_coor                             */
/*                           ==========                             */
/*  Изменение массива координат  по  матрице  преобразований        */
/********************************************************************/

void trans_coor (CVect* pvOld, CVect* pvNew , int all_pt , CMatr mpr)
{
    for (int i=0 ; i< all_pt ; i++)
       pvNew[i]= pvOld[i] * mpr;
}


