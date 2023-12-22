
#include "tdef.h"

 // обнуление матрицы
void MatrZero (CMatr& m_)
{
	ZeroMemory (&m_, sizeof(m_));
}  //func MatrZero

 // установка единичной матрицы
void MatrIdentity (CMatr& m_)
{
	MatrZero (m_);
	m_.m[0][0] = m_.m[1][1] = m_.m[2][2] = 1;
}  //func MatrIdentity

// установка матрицы масштабирования
void MatrScale (CMatr& m_ , double kx , double ky)
{
	MatrZero (m_);
	m_.m[0][0] = kx;
	m_.m[1][1] = ky;
	m_.m[2][2] = 1;
}  //func MatrScale

 // установка матрицы перемещения
void MatrTransl (CMatr&       m_,
				 const CVect& v_)
{
	MatrIdentity (m_);
	m_.m[2][0] = v_.x;
	m_.m[2][1] = v_.y;
}  //func MatrTransl

 // установка матрицы поворота
void MatrRot (CMatr&        m_,
			  const double& dAng_)
{
	MatrIdentity (m_);
	m_.m[0][0] = m_.m[1][1] = cos(dAng_);
	m_.m[1][0] = -sin(dAng_);
	m_.m[0][1] =  sin(dAng_);
}  //func MatrRot

 // умножение матриц
CMatr operator * (const CMatr& m1_,
				  const CMatr& m2_)
{
	int i, j, k;
	CMatr mRes;
	MatrZero (mRes);
	for (i=0; i<S_DIMS; i++)
		for (j=0; j<S_DIMS; j++)
			for (k=0; k<S_DIMS; k++)
				mRes.m[i][j] += m1_.m[i][k] * m2_.m[k][j];
	return mRes;
}  //operator * (CMatr, CMatr)


 // умножение вектора на матрицу
CVect operator * (const CVect& v_,
				  const CMatr& m_)
{
	int i, j, k;
	CVect vRes;
	vRes.x = v_.x * m_.m[0][0] + v_.y * m_.m[1][0] + m_.m[2][0];
	vRes.y = v_.x * m_.m[0][1] + v_.y * m_.m[1][1] + m_.m[2][1];
	return vRes;
}  //operator * (CVect, CMatr)

 // печать в строку
void MatrPrint (const CMatr& m1_, LPSTR pcBuff)
{
	int i, j, pos = 0;
	for (i=0; i<S_DIMS; i++) {
		for (j=0; j<S_DIMS; j++)
			pos += wsprintf(pcBuff+pos, "%f ", m1_.m[i][j]);
		pos += wsprintf(pcBuff+pos, "\n");
	}
}  //matr MatrPrint


struct Ct {

	Ct ()
	{
		char buff[1000];
	    int i;
		CMatr m;
		//MatrIdentity (m);
		MatrRot (m, 3.14/3);

		MatrPrint (m, buff);
		MessageBox (0, buff, "m MatrRot (3.14/3)", 0);


		CMatr m1, m2;

		for (i=0; i < S_DIMS*S_DIMS; i++)
			m1.m[0][i] = i;

		MatrPrint (m1, buff);
		MessageBox (0, buff, "m1", 0);
	
		for (i=0; i < S_DIMS*S_DIMS; i++)
			m2.m[0][i] = i*2;

		MatrPrint (m2, buff);
		MessageBox (0, buff, "m2", 0);

		m = m1 * m2 * m2 * m1;
		MatrPrint (m /*1 * m2 * m2 * m1*/, buff);
		MessageBox (0, buff, "m1 * m2", 0);
	}


};

//Ct t;