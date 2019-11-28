#ifndef __CLCH_H__
#define __CLCH_H__

/**
* @namespace	ColorLch
* @brief		Lch Color Space.
*
* @section LchDetail
* @Detailed
* Service of Lch color space operation.\n
* Using for conversion between RGB(sRGB) color space and Lab color space.\n
* Sometimes you need to specify the white primary.\n
* When you want to do the absolutely color conversion.
*	
******************************************************************************
*
* Important Info and parameter of color space.
* sRGB =
* 0.3127    0.3290 --> White point(D65)
* 0.6400    0.3300 --> Red
* 0.3000    0.6000 --> Green
* 0.1500    0.0600 --> Blue
*
* AdobeRGB =			 
* 0.3127    0.3290
* 0.6400    0.3300
* 0.2100    0.7100
* 0.1500    0.0600
*
* AppleRGB =			 
* 0.3127    0.3290
* 0.6250    0.3400
* 0.2800    0.5950
* 0.1550    0.0700
*
* ColorMatchRGB =
* 0.3457    0.3585
* 0.6300    0.3400
* 0.2950    0.6050
* 0.1500    0.0750
* 
******************************************************************************
* MatLab Code: John Xu
******************************************************************************
function Matrix = ColorSpace(WRGB)
% ColorSpace(RGBLab)
% ColorSpace - Find the color space of RGB Color space.
% RGBLab : 6*3 Matrix.
%
% Example: sRGB --> D65 and HDTV(ITR-R 709-2)
% D65 = [0.3127 0.3290;0.64 0.33;0.3 0.6;0.15 0.06]; 
% D50 = [0.3457 0.3585;0.64 0.33;0.3 0.6;0.15 0.06]; 
% WRGB = [0.3127 0.3290; //White point
%         0.64   0.33;   //Red
%         0.3    0.6;    //Green
%         0.15   0.06];  //Blue
% Matrix=ColorSpace(WRGB);
% RGB2XYZ = cVDinv(V);
% RGB2XYZ_D50

dimension = size(WRGB);
if dimension(1) ~= 4 
   error('The length of x is not 4')
   return
end
if dimension(2) ~= 2 
   error('The length of x is not 2')
   return
end

 % RGB2XYZ = c*V*D*inv(V);
White = [WRGB(1,1) WRGB(1,2) (1 - WRGB(1,1)-WRGB(1,2))];

V = [   WRGB(2,1) WRGB(2,2) 1-WRGB(2,1)-WRGB(2,2);
        WRGB(3,1) WRGB(3,2) 1-WRGB(3,1)-WRGB(3,2);
        WRGB(4,1) WRGB(4,2) 1-WRGB(4,1)-WRGB(4,2);    ];
V = V';
InvV = inv(V);
Beta = InvV * White';
D = [Beta(1)/0.3333 0 0; 0 Beta(2)/0.3333 0; 0 0 Beta(3)/0.3333];
RGB2XYZ = V * D;
c = sum(RGB2XYZ');
RGB2XYZ = RGB2XYZ/c(2);

% BFD = Bradford Transformation Matrix
% XYZ2D50 = InvBFD * D3 * BFD;
BFD = [  0.8951     0.2664     -0.1614;
        -0.7502     1.7135      0.0367;
         0.0389    -0.0685      1.0296];
D50 = [0.3457 0.3585 0.2958];
WhiteXYZ = White/White(2);
D50XYZ = D50/D50(2);
InvBFD = inv(BFD);
ConeWhite = BFD * WhiteXYZ';
ConeD50 = BFD * D50XYZ';
D2 = [ConeD50(1)/ConeWhite(1) ConeD50(2)/ConeWhite(2) ConeD50(3)/ConeWhite(3)];
D3 = [D2(1) 0 0; 0 D2(2) 0; 0 0 D2(3)];
BFD_Transformation = InvBFD * D3 * BFD;
Matrix = BFD_Transformation * RGB2XYZ;

*/
namespace ColorLch{
	//Color Space, Used for change output color space.
	enum class ColorSpace
	{	
		eCS_sRGB,
		eCS_AdobeRGB,
		eCS_AppleRGB,
		eCS_ColorMatchRGB,
		eCS_UnKnown = 0xFFFF
	};

	/**
	* @class CLCH
	* @brief		Lch Color Space.
	*
	* @Detailed
	* @ref LchDetail
	*/	
	class CLCH
	{
		//attribte
	public:
		void Adjust(double Adjust_L, double Adjust_c, double Adjust_h);
		void SetColorSpace(ColorSpace sig);
		double	L;  //Light L*
		double	c;  //Chroma
		double	h;  //Hue
		double	a;  //Data a*
		double	b;  //Data b*
		double	R;  //Red
		double	G;  //Green
		double	B;  //Blue
		double  X;	//CIE1931 X D50
		double  Y;	//CIE1931 Y D50
		double  Z;	//CIE1931 Z D50
		double  Xn;//WhiteX;
		double  Yn;//WhiteY;
		double  Zn;//WhiteZ;

		//operation
		//D50 Light Xn = 0.9643, Yn = 1.0, Zn = 0.8251
		CLCH(double WhiteX = 63195.0, double WhiteY = 65535.0, double WhiteZ = 54073.0);
		~CLCH();
		void	SetWhite(double WhiteX, double WhiteY, double WhiteZ);
		void	LCH2LAB();
		void	LAB2XYZ();
		void	XYZ2RGB();

		void	RGB2XYZ();
		void	XYZ2LAB();
		void	LAB2LCH();		
		
		void	PutData_Lch(double L, double c, double h);
		void	PutData_Lab(double L, double a, double b);		
		void	PutData_RGB(double R, double G, double B);
		void	PutData_XYZ(double X, double Y, double Z);
		CLCH&	operator=(const CLCH& temp);
	public:
		// The Light of RGB2XYZ and XYZ2RGB is all D50.
		// Default is sRGB color space.
		double m_RGB2XYZ[3][3];
		double m_XYZ2RGB[3][3];
	};
};//ColorLch
using namespace ColorLch;

double sRGB_Gamma_Decode(double data);
double sRGB_Gamma_Encode(double data);
unsigned char sRGB_Gamma_Decode255(unsigned char data);
unsigned char sRGB_Gamma_Encode255(unsigned char data);
double RGB2Density(double data);
double Density2RGB(double data);
double RGB2Density255(unsigned char data);
unsigned char Density2RGB255(double data);

//Other color tools
int XYZtoCorColorTemp(double *xyz, double *temp);
void RGB2HSB(const int R, const int G, const int B, int& h, int& s, int& b);

#define CS_sRGB					ColorSpace::eCS_sRGB
#define CS_AdobeRGB				ColorSpace::eCS_AdobeRGB
#define CS_AppleRGB				ColorSpace::eCS_AppleRGB
#define CS_ColorMatchRGB		ColorSpace::eCS_ColorMatchRGB
#define CS_UnKnown				ColorSpace::eCS_UnKnown

inline unsigned short Tiff_encode_L(double data)
{//range 0 ~ 100
	int intPart, rationPart;

	if (data < 0)
		return 0x0;
	else
	{
		intPart = (int)data;
		rationPart = (int)((data - intPart) * 256 + 0.5);
		return (0xFF00 & (int)(intPart * 255 / 100 + 0.5) << 8) | (0xFF & rationPart);
	}
}

inline unsigned short Tiff_encode_ab(double data)
{
	return (short)(data * 256);
}

inline double Tiff_decode_L(unsigned short data)
{
	return (double)data * 100 / 65535.0;
}

//bug???
inline double Tiff_decode_ab(unsigned short data)
{
	if (data < 0x8000)
		return (short)data / 256.0;
	else
		return (data - 0xFFFF) / 256.0;
}

inline double Tiff_decode_L_8(unsigned char data)
{
	return (double)data * 100 / 255.0;
}

inline double Tiff_decode_ab_8(unsigned char data)
{
	if (data < 128)
		return (double)data;
	else
		return data - 255;
}

#endif //__CLCH_H__