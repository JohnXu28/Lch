#include "stdafx.h"
#include "clch.h"
#include <memory>
/*	 
* sRGB color space conversion for example.
*				(Bradford Conversion)	       (D65 RGB2XYZ)
* |X|		  | 1.0479	0.0229	-0.0502|   |0.4124	0.3576	0.1805|	  |R|
* |Y|		= | 0.0296  0.9904	-0.0171| * |0.2126	0.7152	0.0722| * |G|
* |Z|D50	  |-0.0092	0.0151	 0.7519|   |0.0193	0.1192	0.9505|	  |B|D65
*
* -------------->
*
* |X|		  |0.4361	0.3851	0.1431|   |R|
* |Y|		= |0.2225	0.7169	0.0606| * |G|
* |Z|D50	  |0.0139	0.0971	0.7141|   |B|D65
*
* Using Matlab function "ColorSpace" to find out the RGB2XYZ Matrix, For example,
* >> sRGB=[0.3127 0.3290;0.64 0.33;0.3 0.6;0.15 0.06];
* >> RGB2XYZ = ColorSpace(sRGB);
* >> RGB2XYZ = 
* 0.4361    0.3852    0.1431
* 0.2225    0.7169    0.0606
* 0.0139    0.0971    0.7141
*******************************************************************************************************
Other Color space
*******************************************************************************************************
RGB Working Space  	Reference White  	  	
Adobe RGB (1998) 	D50 			
	RGB to XYZ [M]
	 0.6097559  0.2052401  0.1492240 
	 0.3111242  0.6256560  0.0632197
	 0.0194811  0.0608902  0.7448387

	XYZ to RGB [M]-1
	 1.9624274 -0.6105343 -0.3413404
	-0.9787684  1.9161415  0.0334540
	 0.0286869 -0.1406752  1.3487655

AppleRGB 	D50
	 0.4755678  0.3396722  0.1489800
	 0.2551812  0.6725693  0.0722496
	 0.0184697  0.1133771  0.6933632

	 2.8510695 -1.3605261 -0.4708281
	-1.0927680  2.0348871  0.0227598
	 0.1027403 -0.2964984  1.4510659

Bruce RGB 	D50
	 0.4941816  0.3204834  0.1495550
	 0.2521531  0.6844869  0.0633600
	 0.0157886  0.0629304  0.7464909

	 2.6502856 -1.2014485 -0.4289936
	-0.9787684  1.9161415  0.0334540
	 0.0264570 -0.1361227  1.3458542

CIE RGB 	D50
	 0.4868870  0.3062984  0.1710347
	 0.1746583  0.8247541  0.0005877
	-0.0012563  0.0169832  0.8094831

	 2.3638081 -0.8676030 -0.4988161
	-0.5005940  1.3962369  0.1047562
	 0.0141712 -0.0306400  1.2323842

NTSC RGB 	D50
	 0.6343706  0.1852204  0.1446290
	 0.3109496  0.5915984  0.0974520
	-0.0011817  0.0555518  0.7708399

	 1.8464881 -0.5521299 -0.2766458
	-0.9826630  2.0044755 -0.0690396
	 0.0736477 -0.1453020  1.3018376

PAL/SECAM RGB 	D50                                             
	 0.4552773  0.3675500  0.1413926
	 0.2323025  0.7077956  0.0599019
	 0.0145457  0.1049154  0.7057489

	 2.9603944 -1.4678519 -0.4685105
	-0.9787684  1.9161415  0.0334540
	 0.0844874 -0.2545973  1.4216174

SMPTE-C RGB 	D50
	 0.4163290  0.3931464  0.1547446
	 0.2216999  0.7032549  0.0750452
	 0.0136576  0.0913604  0.7201920

	 3.3921940 -1.8264027 -0.5385522
	-1.0770996  2.0213975  0.0207989
	 0.0723073 -0.2217902  1.3960932

sRGB 	D50
	 0.4360747  0.3850649  0.1430804
	 0.2225045  0.7168786  0.0606169
	 0.0139322  0.0971045  0.7141733

	 3.1338561 -1.6168667 -0.4906146
	-0.9787684  1.9161415  0.0334540
	 0.0719453 -0.2289914  1.4052427
*******************************************************************************************************
*/
//RGB2XYZ(sRGB)
double sRGB_D50[3][3] = {
	{0.4361, 0.3851, 0.1431},
	{0.2225, 0.7169, 0.0606},
	{0.0139, 0.0971, 0.7141}};

	//XYZ2RGB(sRGB)
	double inv_sRGB_D50[3][3]={
		{3.1341,   -1.6174,   -0.4907},
		{-0.9788,    1.9163,    0.0334},
		{0.0720,   -0.2290,    1.4054}};

		//RGB2XYZ(AdobeRGB)
		double AdobeRGB_D50[3][3] = {
			{0.6098,    0.2053,    0.1492},
			{0.3111,    0.6257,    0.0632},
			{0.0195,    0.0609,    0.7448}};

			//XYZ2RGB(AdobeRGB)
			double inv_AdobeRGB_D50[3][3]={
				{1.9625,   -0.6107,   -0.3414},
				{-0.9788,    1.9163,    0.0334},
				{0.0287,   -0.1407,    1.3489}};

				//RGB2XYZ(AppleRGB)
				double AppleRGB_D50[3][3] = {
					{0.4756,    0.3397,    0.1490},
					{0.2552,    0.6726,    0.0723},
					{0.0185,    0.1133,    0.6933}};

					//XYZ2RGB(AppleRGB)
					double inv_AppleRGB_D50[3][3]={
						{ 2.8513,   -1.3610,   -0.4709},
						{-1.0928,    2.0350,    0.0227},
						{0.1027,   -0.2965,    1.4512}};

						//RGB2XYZ(ColorMatchRGB)
						double ColorMatchRGB_D50[3][3] = {
							{0.5095,    0.3209,    0.1339},
							{0.2750,    0.6581,    0.0670},
							{0.0243,    0.1088,    0.6921}};

							//XYZ2RGB(ColorMatchRGB)
							double inv_ColorMatchRGB_D50[3][3]={
								{2.6416,   -1.2231,   -0.3929},
								{-1.1121,    2.0592,    0.0160},
								{0.0822,   -0.2808,    1.4562}};			
/*
* CLCH color conversion class
*/									   
CLCH::CLCH(double WhiteX, double WhiteY, double WhiteZ)
{
	//SetColorSpace(CS_AdobeRGB);
	SetColorSpace(CS_sRGB);
	L = 0.0;	c = 0.0;	h = 0.0;
	a = 0.0;	b = 0.0;
	R = 0;	G = 0;	B = 0;
	X = 0;	Y = 0;	Z = 0;
	Xn = WhiteX/65535.0;
	Yn = WhiteY/65535.0;
	Zn = WhiteZ/65535.0;	
}

CLCH::~CLCH()
{

}

void CLCH::SetColorSpace(ColorSpace sig)
{
	switch(sig)
	{
	case CS_AdobeRGB:
		memcpy(m_RGB2XYZ, AdobeRGB_D50, sizeof(double) * 9);
		memcpy(m_XYZ2RGB, inv_AdobeRGB_D50, sizeof(double) * 9);
		break;

	case CS_AppleRGB:
		memcpy(m_RGB2XYZ, AppleRGB_D50, sizeof(double) * 9);
		memcpy(m_XYZ2RGB, inv_AppleRGB_D50, sizeof(double) * 9);
		break;

	case CS_ColorMatchRGB:
		memcpy(m_RGB2XYZ, ColorMatchRGB_D50, sizeof(double) * 9);
		memcpy(m_XYZ2RGB, inv_ColorMatchRGB_D50, sizeof(double) * 9);
		break;

	case CS_sRGB:
	case CS_UnKnown:
		memcpy(m_RGB2XYZ, sRGB_D50, sizeof(double) * 9);
		memcpy(m_XYZ2RGB, inv_sRGB_D50, sizeof(double) * 9);

		break;
	}
}

void CLCH::SetWhite(double WhiteX, double WhiteY, double WhiteZ)
{
	Xn = WhiteX/65535.0;
	Yn = WhiteY/65535.0;
	Zn = WhiteZ/65535.0;
}

/***********************************************
	Lch --> Lab --> XYZ --> RGB
***********************************************/
void CLCH::LCH2LAB()
{
	double angle;
	angle = h * 0.0174533;// 0174533 = 2 * Pei / 360;
	a = c * cos(angle);
	b = c * sin(angle);
}

void CLCH::LAB2XYZ()
{
	double fX, fY, fZ;
	if(L > 7.9996)
	{
		fY = (L + 16) / 116;
		Y = pow(fY, 3);
	}
	else
	{
		Y = L / 903.3;
		fY = 7.787 * Y + 0.137931;
	}

	fX = (a / 500) + fY;
	if (fX > 0.2069)
		X = pow(fX, 3);
	else
		X = (fX - 0.137931) / 7.787;

	fZ = fY - (b / 200);
	if (fZ > 0.2069)
		Z = pow(fZ, 3);
	else
		Z = (fZ - 0.137931) / 7.787;

	X = X * Xn;
	Y = Y * Yn;
	Z = Z * Zn;
}

void CLCH::XYZ2RGB()
{
	R =	(X *  m_XYZ2RGB[0][0] + Y * m_XYZ2RGB[0][1] + Z * m_XYZ2RGB[0][2]) * 255;
	G = (X *  m_XYZ2RGB[1][0] + Y * m_XYZ2RGB[1][1] + Z * m_XYZ2RGB[1][2]) * 255;
	B =	(X *  m_XYZ2RGB[2][0] + Y * m_XYZ2RGB[2][1] + Z * m_XYZ2RGB[2][2]) * 255;
	
	if(R > 255)		R = 255;
	if(G > 255)		G = 255;
	if(B > 255)		B = 255;

	if(R < 0)		R = 0;
	if(G < 0)		G = 0;
	if(B < 0)		B = 0;
}

/***********************************************
	Lch <-- Lab <-- XYZ <-- RGB
***********************************************/
void CLCH::RGB2XYZ()
{
	X = (float)(R * m_RGB2XYZ[0][0] + G * m_RGB2XYZ[0][1] + B * m_RGB2XYZ[0][2]) / 255.0;
	Y = (float)(R * m_RGB2XYZ[1][0] + G * m_RGB2XYZ[1][1] + B * m_RGB2XYZ[1][2]) / 255.0;
	Z = (float)(R * m_RGB2XYZ[2][0] + G * m_RGB2XYZ[2][1] + B * m_RGB2XYZ[2][2]) / 255.0;
}

void CLCH::XYZ2LAB()
{
	double tempX, tempY, tempZ, fX, fY, fZ;
	tempX = X / Xn; 
	tempY = Y / Yn; 
	tempZ = Z / Zn;

	if (tempY > 0.008856)
		L = 116 * pow(tempY, 0.3333333) - 16;
	else
		L = 903.3 * tempY;

	if(tempX > 0.008856)
		fX = pow(tempX, 0.3333333);
	else 
		fX = 7.787 * tempX + 0.137931; //0.137931 = 16 / 116;

	if(Y> 0.008856)
		fY = pow(tempY, 0.3333333);
	else 
		fY = 7.787 * tempY + 0.137931; //0.137931 = 16 / 116;

	if(Z > 0.008856)
		fZ = pow(tempZ, 0.3333333);
	else 
		fZ = 7.787 * tempZ + 0.137931; //0.137931 = 16 / 116;

	a = 500 * (fX - fY);
	b = 200 * (fY - fZ);
}

void CLCH::LAB2LCH()
{
	c = sqrt((a*a) + (b*b));
	h = atan2(b, a) * 57.2958; //57.2958 = 360/2pei;

	if (h < 0)
		h += 360;
}

void CLCH::PutData_Lch(double light, double chroma, double hue)
{
	L = light;
	c = chroma;
	h = hue;
	if (light >= 100)		light = 100;
	if ( chroma <= 0)		chroma = 0;

	LCH2LAB();
	LAB2XYZ();
	XYZ2LAB();
}

void CLCH::PutData_Lab(double light, double Data_a, double Data_b)
{
	L = light;
	a = Data_a;
	b = Data_b;
	if (light >= 100)		light = 100;;
	LAB2LCH();
	LAB2XYZ();
	XYZ2RGB();
}

void CLCH::PutData_RGB(double Red, double Green, double Blue)
{
	R = Red;	G = Green;	B = Blue;
	if(R < 0)		R = 0;
	if(R > 255)		R = 255;
	if(G < 0)		G = 0;
	if(G > 255)		G = 255; 
	if(B < 0)		B = 0;
	if(B > 255)		B = 255;
	RGB2XYZ();
	XYZ2LAB();
	LAB2LCH();
}

void CLCH::PutData_XYZ(double InX, double InY, double InZ)
{
	X = InX; Y = InY; Z = InZ;
	XYZ2LAB();
	LAB2LCH();
	XYZ2RGB();
}

CLCH& CLCH::operator=(const CLCH& temp)
{
	PutData_Lch(temp.L, temp.c, temp.h);
	return *this;
}

void CLCH::Adjust(double Adjust_L, double Adjust_c, double Adjust_h)
{
	L = L + Adjust_L;
	if(L > 100) L = 100;
	if(L < 0) L = 0;

	c = c + Adjust_c;
	if(c > 100) c = 100;
	if(c < 0) c = 0;

	h = h + Adjust_h;
	if(h > 360) h -= 360;
	if(h < 0) h += 360;

	LCH2LAB();
	LAB2XYZ();
	XYZ2RGB();
}

//Other Useful function.
//Range 0 ~ 1
double sRGB_Gamma_Decode(double data)
{	
	if(data <= 0.03928)
		return (data / 12.92);
	else
		return pow((data+0.055)/1.055, 2.4);
}

//Range 0 ~ 1
double sRGB_Gamma_Encode(double data)
{
	if(data < 0.00304)
		return (data * 12.92);
	else
		return (1.055 * pow(data, 0.4167) - 0.055);
}

unsigned char sRGB_Gamma_Decode255(unsigned char data)
{
	double In = data / 255.0;
	return (unsigned char)(255 * sRGB_Gamma_Decode(In) + 0.5);
}

unsigned char sRGB_Gamma_Encode255(unsigned char data)
{
	double In = data / 255.0;
	return (unsigned char)(255 * sRGB_Gamma_Encode(In) + 0.5);
}

double RGB2Density(double data)
{	return -log10(data);}

double Density2RGB(double data)
{	return pow(10, -data);}

double RGB2Density255(unsigned char data)
{	return -log10(data/255.0);}

unsigned char Density2RGB255(double data)
{	return (unsigned char )(255 * pow(10, -data) + 0.5);}

/*
*      Name:   XYZtoCorColorTemp.c
*
*      Author: Bruce Justin Lindbloom
*
*      Copyright (c) 2003 Bruce Justin Lindbloom. All rights reserved.
*
*      Input:  xyz = pointer to the input array of X, Y and Z color components (in that order).
*              temp = pointer to where the computed correlated color temperature should be placed.
*
*      Output: *temp = correlated color temperature, if successful.
*                    = unchanged if unsuccessful.
*
*      Return: 0 if successful, else -1.
*
*      Description:
*              This is an implementation of Robertson's method of computing the correlated color
*              temperature of an XYZ color. It can compute correlated color temperatures in the
*              range [1666.7K, infinity].
*
*      Reference:
*              "Color Science: Concepts and Methods, Quantitative Data and Formulae", Second Edition,
*              Gunter Wyszecki and W. S. Stiles, John Wiley & Sons, 1982, pp. 227, 228.
*/
#include "stdafx.h"
#include <math.h>

/* LERP(a,b,c) = linear interpolation macro, is 'a' when c == 0.0 and 'b' when c == 1.0 */
#define LERP(a,b,c)     (((b) - (a)) * (c) + (a))

typedef struct UVT {
	double  u;
	double  v;
	double  t;
} UVT;

double rt[31] = {       /* reciprocal temperature (K) */
	DBL_MIN, 10.0e-6, 20.0e-6, 30.0e-6, 40.0e-6, 50.0e-6,
	60.0e-6, 70.0e-6, 80.0e-6, 90.0e-6, 100.0e-6, 125.0e-6,
	150.0e-6, 175.0e-6, 200.0e-6, 225.0e-6, 250.0e-6, 275.0e-6,
	300.0e-6, 325.0e-6, 350.0e-6, 375.0e-6, 400.0e-6, 425.0e-6,
	450.0e-6, 475.0e-6, 500.0e-6, 525.0e-6, 550.0e-6, 575.0e-6,
	600.0e-6
};

UVT uvt[31] = {
		{ 0.18006, 0.26352, -0.24341 },
		{ 0.18066, 0.26589, -0.25479 },
		{ 0.18133, 0.26846, -0.26876 },
		{ 0.18208, 0.27119, -0.28539 },
		{ 0.18293, 0.27407, -0.30470 },
		{ 0.18388, 0.27709, -0.32675 },
		{ 0.18494, 0.28021, -0.35156 },
		{ 0.18611, 0.28342, -0.37915 },
		{ 0.18740, 0.28668, -0.40955 },
		{ 0.18880, 0.28997, -0.44278 },
		{ 0.19032, 0.29326, -0.47888 },
		{ 0.19462, 0.30141, -0.58204 },
		{ 0.19962, 0.30921, -0.70471 },
		{ 0.20525, 0.31647, -0.84901 },
		{ 0.21142, 0.32312, -1.0182 },
		{ 0.21807, 0.32909, -1.2168 },
		{ 0.22511, 0.33439, -1.4512 },
		{ 0.23247, 0.33904, -1.7298 },
		{ 0.24010, 0.34308, -2.0637 },
		{ 0.24792, 0.34655, -2.4681 },	/* Note: 0.24792 is a corrected value for the error found in W&S as 0.24702 */
		{ 0.25591, 0.34951, -2.9641 },
		{ 0.26400, 0.35200, -3.5814 },
		{ 0.27218, 0.35407, -4.3633 },
		{ 0.28039, 0.35577, -5.3762 },
		{ 0.28863, 0.35714, -6.7262 },
		{ 0.29685, 0.35823, -8.5955 },
		{ 0.30505, 0.35907, -11.324 },
		{ 0.31320, 0.35968, -15.628 },
		{ 0.32129, 0.36011, -23.325 },
		{ 0.32931, 0.36038, -40.770 },
		{ 0.33724, 0.36051, -116.45 }
};


int XYZtoCorColorTemp(double *xyz, double *temp)
{
	double us, vs, p, di, dm;
	int i;


	if ((xyz[0] < 1.0e-20) && (xyz[1] < 1.0e-20) && (xyz[2] < 1.0e-20))
		return(-1);     /* protect against possible divide-by-zero failure */
	us = (4.0 * xyz[0]) / (xyz[0] + 15.0 * xyz[1] + 3.0 * xyz[2]);
	vs = (6.0 * xyz[1]) / (xyz[0] + 15.0 * xyz[1] + 3.0 * xyz[2]);
	dm = 0.0;
	for (i = 0; i < 31; i++) {
		di = (vs - uvt[i].v) - uvt[i].t * (us - uvt[i].u);
		if ((i > 0) && (((di < 0.0) && (dm >= 0.0)) || ((di >= 0.0) && (dm < 0.0))))
			break;  /* found lines bounding (us, vs) : i-1 and i */
		dm = di;
	}
	if (i == 31)
		return(-1);     /* bad XYZ input, color temp would be less than minimum of 1666.7 degrees, or too far towards blue */
	di = di / sqrt(1.0 + uvt[i].t * uvt[i].t);
	dm = dm / sqrt(1.0 + uvt[i - 1].t * uvt[i - 1].t);
	p = dm / (dm - di);     /* p = interpolation parameter, 0.0 : i-1, 1.0 : i */
	p = 1.0 / (LERP(rt[i - 1], rt[i], p));
	*temp = p;
	return(0);      /* success */
}

void RGB2HSB(const int R, const int G, const int B, int& h, int& s, int& b)
{
	if (R > G)
	{
		if (G >= B) // r>g>b
		{
			h = ((G - B) * 60 / (R - B));
			s = (R - B) * 100 / R;
			b = R * 100 / 255;
		}
		else
		{
			if (R >= B) //r>b>g
			{
				h = 360 - ((B - G) * 60 / (R - G));
				s = (R - G) * 100 / R;
				b = R * 100 / 255;
			}
			else
			{	//b>r>g
				h = 240 + ((R - G) * 60 / (B - G));
				s = (B - G) * 100 / B;
				b = B * 100 / 255;
			}
		}
	}
	else
	{
		if (R > B)//g>r>b
		{
			h = 120 - ((R - B) * 60 / (G - B));
			s = (G - B) * 100 / G;
			b = G * 100 / 255;
		}
		else
		{
			if (G > B)//g>b>r
			{
				h = 120 + ((B - R) * 60 / (G - R));
				s = (G - R) * 100 / G;
				b = G * 100 / 255;
			}
			else
			{//b>g>r
				if (B == R)
				{
					h = 0;
					s = 0;
					b = B * 100 / 255;
				}
				else
				{
					if (B == 0)
					{
						h = 0;
						s = 0;
						b = 0;
					}
					else
					{
						h = 240 - ((G - R) * 60 / (B - R));
						s = (B - R) * 100 / B;
						b = B * 100 / 255;
					}
				}
			}
		}
	}
}
