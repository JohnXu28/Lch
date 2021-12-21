#include "stdafx.h"
#include "cam02.h"
#include<math.h>

const int D_CALCULATE = -1;

void CAM02COLOR::XYZtoCAT02()
{
	RGB[0] = (XYZ[0] * 0.7328) + (XYZ[1] * 0.4296) + (XYZ[2] * -0.1624);
	RGB[1] = (XYZ[0] * -0.7036) + (XYZ[1] * 1.6975) + (XYZ[2] * 0.0061);
	RGB[2] = (XYZ[0] * 0.0030) + (XYZ[1] * 0.0136) + (XYZ[2] * 0.9834);
}

void CAM02COLOR::ChromaticAdaptation(CAM02* pMod)
{
	Uint32 i;

	for (i = 0; i < 3; i++) 
	{
		RGBc[i] = ((pMod->adoptedWhite.XYZ[1] *
			(pMod->D / pMod->adoptedWhite.RGB[i])) +
			(1.0 - pMod->D)) * RGB[i];
	}
}

void CAM02COLOR::CAT02toHPE()
{
	double M[9];

	M[0] = ((0.38971 * 1.096124) + (0.68898 * 0.454369) + (-0.07868 * -0.009628));
	M[1] = ((0.38971 * -0.278869) + (0.68898 * 0.473533) + (-0.07868 * -0.005698));
	M[2] = ((0.38971 * 0.182745) + (0.68898 * 0.072098) + (-0.07868 * 1.015326));
	M[3] = ((-0.22981 * 1.096124) + (1.18340 * 0.454369) + (0.04641 * -0.009628));
	M[4] = ((-0.22981 * -0.278869) + (1.18340 * 0.473533) + (0.04641 * -0.005698));
	M[5] = ((-0.22981 * 0.182745) + (1.18340 * 0.072098) + (0.04641 * 1.015326));
	M[6] = (-0.009628);
	M[7] = (-0.005698);
	M[8] = (1.015326);

	RGBp[0] = (RGBc[0] * M[0]) + (RGBc[1] * M[1]) + (RGBc[2] * M[2]);
	RGBp[1] = (RGBc[0] * M[3]) + (RGBc[1] * M[4]) + (RGBc[2] * M[5]);
	RGBp[2] = (RGBc[0] * M[6]) + (RGBc[1] * M[7]) + (RGBc[2] * M[8]);
}

void CAM02COLOR::NonlinearCompression(CAM02* pMod)
{
	Uint32 i;
	double temp;

	for (i = 0; i < 3; i++)
	{
		if (RGBp[i] < 0)
		{
			temp = pow((-1.0 * pMod->FL * RGBp[i] / 100.0), 0.42);
			RGBpa[i] = (-1.0 * 400.0 * temp) / (temp + 27.13) + 0.1;
		}
		else
		{
			temp = pow((pMod->FL * RGBp[i] / 100.0), 0.42);
			RGBpa[i] = (400.0 * temp) / (temp + 27.13) + 0.1;
		}
	}

	A = (((2.0 * RGBpa[0]) + RGBpa[1] +
		(RGBpa[2] / 20.0)) - 0.305) * pMod->Nbb;
}

void CAM02COLOR::ComputeCorrelates(CAM02* pMod)
{
	double a, b, temp, e, t, r2d, d2r;

	a = RGBpa[0] - (12.0 * RGBpa[1] / 11.0) + (RGBpa[2] / 11.0);
	b = (RGBpa[0] + RGBpa[1] - (2.0 * RGBpa[2])) / 9.0;

	r2d = (180.0 / 3.141592654);
	if (a == 0)
	{
		if (b == 0)
			h = 0;
		else if (b > 0)
			h = 90;
		else
			h = 270;
	}
	else if (a > 0) 
	{
		temp = b / a;
		if (b > 0)       
			h = (r2d * atan(temp));
		else if (b == 0) 
			h = 0;
		else 
			h = (r2d * atan(temp)) + 360;
	}
	else 
	{
		temp = b / a;
		h = (r2d * atan(temp)) + 180;
	}

	d2r = (3.141592654 / 180.0);
	e = ((12500.0 / 13.0) * pMod->Nc * pMod->Ncb) *
		(cos((h * d2r + 2.0)) + 3.8);

	if (h < 20.14) 
	{
		temp = ((h + 122.47) / 1.2) + ((20.14 - h) / 0.8);
		H = 300 + (100 * ((h + 122.47) / 1.2)) / temp;
	}
	else if (h < 90.0) 
	{
		temp = ((h - 20.14) / 0.8) + ((90.00 - h) / 0.7);
		H = (100 * ((h - 20.14) / 0.8)) / temp;
	}
	else if (h < 164.25) 
	{
		temp = ((h - 90.00) / 0.7) + ((164.25 - h) / 1.0);
		H = 100 + ((100 * ((h - 90.00) / 0.7)) / temp);
	}
	else if (h < 237.53) 
	{
		temp = ((h - 164.25) / 1.0) + ((237.53 - h) / 1.2);
		H = 200 + ((100 * ((h - 164.25) / 1.0)) / temp);
	}
	else 
	{
		temp = ((h - 237.53) / 1.2) + ((360 - h + 20.14) / 0.8);
		H = 300 + ((100 * ((h - 237.53) / 1.2)) / temp);
	}

	J = 100.0 * pow((A / pMod->adoptedWhite.A),
		(pMod->c * pMod->z));

	Q = (4.0 / pMod->c) * pow((J / 100.0), 0.5) *
		(pMod->adoptedWhite.A + 4.0) * pow(pMod->FL, 0.25);

	t = (e * pow(((a * a) + (b * b)), 0.5)) /
		(RGBpa[0] + RGBpa[1] +
			((21.0 / 20.0) * RGBpa[2]));

	C = pow(t, 0.9) * pow((J / 100.0), 0.5) *
		pow((1.64 - pow(0.29, pMod->n)), 0.73);

	M = C * pow(pMod->FL, 0.25);
	s = 100.0 * pow((M / Q), 0.5);
}

void CAM02COLOR::InverseCorrelates(CAM02* pMod)
{

	double t, e, p1, p2, p3, p4, p5, hr, d2r;
	d2r = 3.141592654 / 180.0;

	t = pow((C / (pow((J / 100.0), 0.5) *
		(pow((1.64 - pow(0.29, pMod->n)), 0.73)))),
		(1.0 / 0.9));
	e = ((12500.0 / 13.0) * pMod->Nc * pMod->Ncb) *
		(cos((h * d2r + 2.0)) + 3.8);

	A = pMod->adoptedWhite.A * pow(
		(J / 100.0),
		(1.0 / (pMod->c * pMod->z)));

	p1 = e / t;
	p2 = (A / pMod->Nbb) + 0.305;
	p3 = 21.0 / 20.0;

	hr = h * d2r;

	if (fabs(sin(hr)) >= fabs(cos(hr))) 
	{
		p4 = p1 / sin(hr);
		b = (p2 * (2.0 + p3) * (460.0 / 1403.0)) /
			(p4 + (2.0 + p3) * (220.0 / 1403.0) *
				(cos(hr) / sin(hr)) - (27.0 / 1403.0) +
				p3 * (6300.0 / 1403.0));
		a = b * (cos(hr) / sin(hr));
	}
	else 
	{
		p5 = p1 / cos(hr);
		a = (p2 * (2.0 + p3) * (460.0 / 1403.0)) /
			(p5 + (2.0 + p3) * (220.0 / 1403.0) -
				((27.0 / 1403.0) - p3 * (6300.0 / 1403.0)) *
				(sin(hr) / cos(hr)));
		b = a * (sin(hr) / cos(hr));
	}

	RGBpa[0] = ((460.0 / 1403.0) * p2) +
		((451.0 / 1403.0) * a) +
		((288.0 / 1403.0) * b);
	RGBpa[1] = ((460.0 / 1403.0) * p2) -
		((891.0 / 1403.0) * a) -
		((261.0 / 1403.0) * b);
	RGBpa[2] = ((460.0 / 1403.0) * p2) -
		((220.0 / 1403.0) * a) -
		((6300.0 / 1403.0) * b);
}

void CAM02COLOR::InverseNonlinearity(CAM02* pMod)
{
	double c1;

	for (int i = 0; i < 3; i++)
	{
		if ((RGBpa[i] - 0.1) < 0)
			c1 = -1;
		else
			c1 = 1;

		RGBp[i] = c1 * (100.0 / pMod->FL) *
			pow(((27.13 * fabs(RGBpa[i] - 0.1)) /
				(400.0 - fabs(RGBpa[i] - 0.1))),
				(1.0 / 0.42));
	}
}

void CAM02COLOR::HPEtoCAT02()
{
	double M[9];

	M[0] = ((0.7328 * 1.910197) + (0.4296 * 0.370950));
	M[1] = ((0.7328 * -1.112124) + (0.4296 * 0.629054));
	M[2] = ((0.7328 * 0.201908) + (0.4296 * 0.000008) - 0.1624);
	M[3] = ((-0.7036 * 1.910197) + (1.6975 * 0.370950));
	M[4] = ((-0.7036 * -1.112124) + (1.6975 * 0.629054));
	M[5] = ((-0.7036 * 0.201908) + (1.6975 * 0.000008) + 0.0061);
	M[6] = ((0.0030 * 1.910197) + (0.0136 * 0.370950));
	M[7] = ((0.0030 * -1.112124) + (0.0136 * 0.629054));
	M[8] = ((0.0030 * 0.201908) + (0.0136 * 0.000008) + 0.9834);;

	RGBc[0] = (RGBp[0] * M[0]) + (RGBp[1] * M[1]) + (RGBp[2] * M[2]);
	RGBc[1] = (RGBp[0] * M[3]) + (RGBp[1] * M[4]) + (RGBp[2] * M[5]);
	RGBc[2] = (RGBp[0] * M[6]) + (RGBp[1] * M[7]) + (RGBp[2] * M[8]);
}

void CAM02COLOR::InverseChromaticAdaptation(CAM02* pMod)
{
	for (int i = 0; i < 3; i++) 
		RGB[i] = RGBc[i] /
			((pMod->adoptedWhite.XYZ[1] * pMod->D / pMod->adoptedWhite.RGB[i]) + 1.0 - pMod->D);	
}

void CAM02COLOR::CAT02toXYZ()
{
	XYZ[0] = (RGB[0] * 1.096124) + (RGB[1] * -0.278869) + (RGB[2] * 0.182745);
	XYZ[1] = (RGB[0] * 0.454369) + (RGB[1] * 0.473533) + (RGB[2] * 0.072098);
	XYZ[2] = (RGB[0] * -0.009628) + (RGB[1] * -0.005698) + (RGB[2] * 1.015326);
}


/***********************************
class CAM02
************************************/
double CAM02::compute_n()
{
	return (Yb / adoptedWhite.XYZ[1]);
}

double CAM02::compute_z()
{
	return (1.48 + pow(n, 0.5));
}

double CAM02::computeNbb()
{
	return (0.725 * pow((1.0 / n), 0.2));
}

double CAM02::computeFL()
{
	double k, FL;

	k = 1.0 / ((5.0 * LA) + 1.0);
	FL = 0.2 * pow(k, 4.0) * (5.0 * LA) + 0.1 *
		(pow((1.0 - pow(k, 4.0)), 2.0)) *
		(pow((5.0 * LA), (1.0 / 3.0)));

	return FL;
}

double CAM02::computeD()
{
	return F - (1.0 / 3.6) * (exp(((-LA - 42) / 92.0)));	
}

CAM02::CAM02()
{
	ViewingConditions VC;
	////d65
	VC.whitePoint[0] = 95.05;
	VC.whitePoint[1] = 100.00;
	VC.whitePoint[2] = 108.89;

	//d50, sRGB -> D50 Xn, Yn, Zn
	//VC.whitePoint[0] = 96.44;
	//VC.whitePoint[1] = 100.0;
	//VC.whitePoint[2] = 82.50;

	const double PI = 3.14159265358979323846;
	VC.La = (64 / PI) / 5.0;
	VC.Yb = 20.0;//20% gray
	VC.D_value = D_CALCULATE;
	VC.surround = AVG_SURROUND;
	Init(&VC);
}

void CAM02::Init(const ViewingConditions* pVC)
{
	adoptedWhite.XYZ[0] = pVC->whitePoint[0];//X
	adoptedWhite.XYZ[1] = pVC->whitePoint[1];//Y
	adoptedWhite.XYZ[2] = pVC->whitePoint[2];//Z

	LA = pVC->La;
	Yb = pVC->Yb;
	D = pVC->D_value;
	surround = pVC->surround;

	switch (surround) 
	{
	case CUTSHEET_SURROUND:
		F = 0.8;
		c = 0.41;
		Nc = 0.8;
		break;

	case DARK_SURROUND:
		F = 0.8;
		c = 0.525;
		Nc = 0.8;
		break;

	case DIM_SURROUND:
		F = 0.9;
		c = 0.59;
		Nc = 0.95;
		break;

	default:
		// Average surround
		F = 1.0;
		c = 0.69;
		Nc = 1.0;
	}

	n = compute_n();
	z = compute_z();
	Nbb = computeNbb();
	FL = computeFL();

	if (D == D_CALCULATE) 
		D = computeD();	

	Ncb = Nbb;
	
	adoptedWhite.XYZtoCAT02();
	adoptedWhite.ChromaticAdaptation(this);
	adoptedWhite.CAT02toHPE();
	adoptedWhite.NonlinearCompression(this);
}

void CAM02::Forward(const XYZ pIn, JCh& pOut)
{
	CAM02COLOR clr;

	clr.XYZ[0] = pIn[0];//->X;
	clr.XYZ[1] = pIn[1];//->Y;
	clr.XYZ[2] = pIn[2];//->Z;

	clr.XYZtoCAT02();
	clr.ChromaticAdaptation(this);
	clr.CAT02toHPE();
	clr.NonlinearCompression(this);
	clr.ComputeCorrelates(this);

	pOut[0] = clr.J;
	pOut[1] = clr.C;
	pOut[2] = clr.h;
}

void CAM02::Reverse(const JCh pIn, XYZ& pOut)
{
	CAM02COLOR clr;

	clr.J = pIn[0];
	clr.C = pIn[1];
	clr.h = pIn[2];

	clr.InverseCorrelates(this);
	clr.InverseNonlinearity(this);
	clr.HPEtoCAT02();
	clr.InverseChromaticAdaptation(this);
	clr.CAT02toXYZ();

	pOut[0] = clr.XYZ[0];
	pOut[1] = clr.XYZ[1];
	pOut[2] = clr.XYZ[2];
}