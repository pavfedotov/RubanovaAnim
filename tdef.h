#include <math.h>
#include <stdio.h>
#include <windows.h>

#define S_DIMS 3

// --- таймер ----------------------
 // интервал времени (мсек)
#define TIMER_INTERVAL 10
// число  временных  интервалов
#define ALL_STEP_TIME  400        

 // идентификатор
#define TIMER_ID 1000

 // координаты
struct CVect {
	double x, y;
};


 // матрица преобразования 
struct CMatr {
	double m[S_DIMS][S_DIMS];
};

 // обнуление матрицы
void MatrZero (CMatr& m_);

 // установка единичной матрицы
void MatrIdentity (CMatr& m_);

// установка матрицы масштабирования
void MatrScale (CMatr& m_ , double kx , double ky);

 // установка матрицы перемещения
void MatrTransl (CMatr&       m_,
				 const CVect& v_);

 // установка матрицы поворота
void MatrRot (CMatr&        m_,
			  const double& dAng_);

 // умножение матриц
CMatr operator * (const CMatr& m1_,
				  const CMatr& m2_);

 // умножение вектора на матрицу
CVect operator * (const CVect& v_,
				  const CMatr& m_);

 // печать в строку
void MatrPrint (const CMatr& m1_, LPSTR pcBuff);
