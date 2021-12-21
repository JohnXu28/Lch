// RGB.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include<iomanip>
#include <math.h>
#include <stdlib.h>
#include <fstream>
#include <Lch\clch.h>
#include <Lch\cam02.h>
using namespace std;

void Display(const char *str, double data1, double data2, double data3)
{
	printf("%10s : %9.4f  %9.4f  %9.4f\n", str, (float)data1, (float)data2, (float)data3);
}

#define format setw(10) << setprecision(4) << resetiosflags(ios_base::scientific)
CAM02 cam02;
double D50_D65[3][3] = {
{ 9.554963020477077e-01, -2.305757326815163e-02,   6.326855946257419e-02},
{ -2.834515154187274e-02,   1.010027086433618e+00,   2.107798453333271e-02},
{ 1.226037740008138e-02, -2.056595116267407e-02,   1.330314926427188e+00} };

void Display(CLCH &lch)
{
	const int precision = 8;	
	double D50[3] = { lch.X, lch.Y, lch.Z };
	double D65[3] = {0};

	D65[0] = D50_D65[0][0] * D50[0] + D50_D65[0][1] * D50[1] + D50_D65[0][2] * D50[2];
	D65[1] = D50_D65[1][0] * D50[0] + D50_D65[1][1] * D50[1] + D50_D65[1][2] * D50[2];
	D65[2] = D50_D65[2][0] * D50[0] + D50_D65[2][1] * D50[1] + D50_D65[2][2] * D50[2];

	double JCH[3] = { 0 };
	cam02.Forward(D65, JCH);

	Display("sRGB(G220)", 255 * sRGB_Gamma_Encode(lch.R/255.0), 255 * sRGB_Gamma_Encode(lch.G / 255.0), 255 * sRGB_Gamma_Encode(lch.B/255.0));
	Display("sRGB(G100)", lch.R, lch.G, lch.B);
	Display("Lab", lch.L, lch.a, lch.b);
	Display("Lch", lch.L, lch.c, lch.h);
	double Sum = lch.X + lch.Y + lch.Z;
	Display("Yxy", lch.Y * 100, lch.X/Sum, lch.Y/Sum);
	Display("Density", RGB2Density(lch.R / 255), RGB2Density(lch.G / 255), RGB2Density(lch.B / 255));
	Display("Reflect", (lch.R / 2.55), (lch.G / 2.55), (lch.B / 2.55));
	Display("CAM02", JCH[0], JCH[1], JCH[2]);

	cout << endl;
}

int InputDatasRGB()
{
	double R, G, B;
	CLCH lch;
	cout << "sRGB Range: 0 ~ 255." << endl;
	cout << "Input sRGB (Gamma 2.2) : ";
	while (1)
	{
		cin >> R >> G >> B; cout << endl;
		if ((R == 0) && (G == 0) && (B == 0))
			break;

		R = sRGB_Gamma_Decode255((unsigned char)R);
		G = sRGB_Gamma_Decode255((unsigned char)G);
		B = sRGB_Gamma_Decode255((unsigned char)B);

		lch.Set_RGB(R, G, B);		
		Display(lch);
	}
	return 1;
}

int InputDataRGB()
{
	double R, G, B;
	CLCH lch;
	cout << "RGB Range: 0 ~ 255." << endl;
	cout << "Input RGB : ";
	while (1)
	{
		cin >> R >> G >> B; cout << endl;
		if ((R == 0) && (G == 0) && (B == 0))
			break;

		lch.Set_RGB(R, G, B);		
		Display(lch);
	}
	return 1;
}

int InputDataLab()
{
	double L, a, b;
	CLCH lch;
	cout << "Lab Range: L:0 ~ 100, a:-128 ~ 127, b:-128 ~ 127." << endl;
	cout << "Input Lab : ";
	while (1)
	{
		cin >> L >> a >> b; cout << endl;
		if ((L == 0) && (a == 0) && (b == 0))
			break;
		lch.Set_Lab(L, a, b);
		//CalibrateLch(lch); we did got data out of range.
		Display(lch);
	}
	return 1;
}

int InputDataLch()
{
	double L, c, h;
	CLCH lch;
	cout << "Lch Range: L:0 ~ 100, c:0 ~ 128, h:0 ~ 360." << endl;

	while (1)
	{
		cin >> L >> c >> h; cout << endl;
		if ((L == 0) && (c == 0) && (h == 0))
			break;
		lch.Set_Lch(L, c, h);		
		Display(lch);
	}
	return 1;
}

int InputDataDensity()
{
	double Density;
	double R, G, B;
	CLCH lch;
	cout << "Density Range:0 ~ 3.0 ." << endl;
	cout << "Input Density : ";
	while (1)
	{
		cin >> Density; cout << endl;
		if (Density == 0)
			break;

		R = G = B = 255 * pow((double)10, (double)-Density);
		lch.Set_RGB(R, G, B);		
		Display(lch);
	}
	return 1;
}

int InputDataReflection()
{
	double Reflection;
	double R, G, B;
	CLCH lch;
	cout << "Reflection Range:0 ~ 100." << endl;
	cout << "Input Relfection : ";
	while (1)
	{
		cin >> Reflection; cout << endl;
		if (Reflection == 0)
			break;

		R = G = B = 2.55 * Reflection;
		lch.Set_RGB(R, G, B);		
		Display(lch);
	}
	return 1;
}

int InSet_XYZ()
{
	double xyz[3];

	cout << "xyz Range: 0.0 ~ 1.0" << endl;
	int XYZtoColorTemp(double *xyz, double *temp);
	while (1)
	{
		cin >> xyz[0] >> xyz[1];
		xyz[2] = 1 - xyz[0] - xyz[1];
		if ((xyz[0] == 0) && (xyz[1] == 0))
			break;
		double temp;
		XYZtoCorColorTemp((double*)xyz, &temp);
		cout << temp << endl << endl;;
	}
	return 1;
}

enum Option {
	Quit = 0,
	sRGB,
	RGB,
	Lab,
	Lch,
	Density,
	Reflection,
	ColorTempreture,
	OptionMax,
};

char *option[OptionMax] = { "Quit", "sRGB220", "sRGB100", "Lab", "Lch", "Density", "Reflection", "Color Tempreture" };

void DisplayManu()
{
	cout << "Select Input Data Format. \n";
	for (int i = 0; i < OptionMax; i++)
		cout << i << "):" << option[i] << endl;
}

void Conversion()
{

	int Select;

	while (1)
	{
		DisplayManu();
		cin >> Select;

		if ((Select < 0) || (Select >= OptionMax))
		{
			cout << "Unknown command or Quit. " << endl;
			break;
		}
		else
			cout << "Select Input : " << option[Select] << endl;

		Option option = (Option)Select;
		switch (option)
		{
		case sRGB:
			cout << "Input sRGB(Gamma 2.2). Quit: 0 0 0 or CTrl+C\n" << endl;
			InputDatasRGB();
			break;
		case RGB:
			cout << "Input sRGB(Gamma 1.0). Quit: 0 0 0 or CTrl+C\n" << endl;
			InputDataRGB();
			break;
		case Lab:
			cout << "Input L a b. Quit: 0 0 0 or CTrl+C\n" << endl;
			InputDataLab();
			break;
		case Lch:
			cout << "Input L c h. Quit: 0 0 0 or CTrl+C\n" << endl;
			InputDataLch();
			break;
		case Density:
			cout << "Input Density. Quit: 0 or CTrl+C\n" << endl;
			InputDataDensity();
			break;

		case Reflection:
			cout << "Input Reflection. Quit: 0 or CTrl+C\n" << endl;
			InputDataReflection();
			break;

		case ColorTempreture:
			cout << "Input xyz. Quit: 0 or CTrl+C\n" << endl;
			InSet_XYZ();
			break;
		default:
			cout << "Unknown Command." << endl;
			exit(1);
			break;
		}
	}
}


int main(int argc, char* argv[])
{
	Conversion();
	return 0;
}