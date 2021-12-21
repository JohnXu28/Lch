#pragma once

//Modified from lcms2::cmscam02.c
class CAM02;
using Uint32 = unsigned int;
using XYZ = double[3];
using JCh = double[3];

enum SURROUND
{
	AVG_SURROUND		= 1,
	DIM_SURROUND		= 2,
	DARK_SURROUND		= 3,
	CUTSHEET_SURROUND	= 4
};

struct ViewingConditions {
	double	whitePoint[3];
	double Yb;
	double La;
	Uint32  surround;
	double D_value;
};

struct CAM02COLOR{
	double XYZ[3];
	double RGB[3];
	double RGBc[3];
	double RGBp[3];
	double RGBpa[3];
	double a, b, h, e, H, A, J, Q, s, t, C, M;
	double abC[2];
	double abs[2];
	double abM[2];
	void XYZtoCAT02();
	void ChromaticAdaptation(CAM02* pMod);
	void CAT02toHPE();
	void NonlinearCompression(CAM02* pMod);
	void ComputeCorrelates(CAM02* pMod);
	void InverseCorrelates(CAM02* pMod);
	void InverseNonlinearity(CAM02* pMod);
	void HPEtoCAT02();
	void InverseChromaticAdaptation(CAM02* pMod);
	void CAT02toXYZ();
} ;

class CAM02{
public:
	CAM02();
	void Init(const ViewingConditions* pVC);
	CAM02COLOR adoptedWhite;
	double LA, Yb;
	double F, c, Nc;
	unsigned int surround;
	double n, Nbb, Ncb, z, FL, D;

	//cmsContext ContextID;
	double compute_n();
	double compute_z();
	double computeNbb();
	double computeFL();
	double computeD();
	void Forward(const XYZ pIn, JCh& pOut);
	void Reverse(const JCh pIn, XYZ& pOut);
} ;

