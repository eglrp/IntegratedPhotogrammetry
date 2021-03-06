#ifndef _LIDAR_CALIBRARION_JING_METHOD_ZJ_2016_05_03_H__
#define _LIDAR_CALIBRARION_JING_METHOD_ZJ_2016_05_03_H__


#include "CalibBasedef.h"
#include <vector>

#ifdef _LIDCALIB_
#define  _lidCalib_Dll_  __declspec(dllexport)
#else
#define  _lidCalib_Dll_  __declspec(dllimport)	
#endif


using namespace std;



class _lidCalib_Dll_ CLidCalib_Jing : public CLidarSelfCalibration
{
public:
	CLidCalib_Jing();
	virtual ~CLidCalib_Jing();

	int  LidMC_Calib_Coplanar(
		const char *pszResultFile,		/* 平差结果文件 */
		const int nCorrsp,	        /* number of Correspondent Objects */
		const int ngcp,		/* number of points (starting from the 1st) whose parameters should not be modified.	
							* All B_ij (see below) with i<ncon are assumed to be zero
							控制点放在观测值的起始部分
							在coplanar方法中暂时无用*/
		LidMC_Obj *pLidPatch,
	
//		TiePoint_Adj_mode TiePt_mode,
							//				 char *vmask,			/* visibility mask: vmask[i, j]=1 if point i visible in image j, 0 otherwise. nxm */
		double *pX,			/* initial parameter vector p0: (a1, ..., am, b1, ..., bn).
												* aj are the image j parameters, bi are the i-th point parameters,
												* size m*cnp + n*pnp
												未知数，偏心分量和安置角    
												pX=[Xt, Yt, Zt, r, p, h]^T 传入未知数初值）*/
		//				const int pnp,			/* number of parameters 未知数个数*/
		orsLidJingParam *pOriParam,            /*   条带参数   */
		int		nStrips                        /*   条带数     */
//		unsigned long	param_type				/* 检校参数类型 */
		);

	int  LidMC_Calib_ConjugatePoints (
		const char *pszResultFile,		/* 平差结果文件 */
		const int nConjugatePts,	    /* number of Correspondent Objects */
		const int ngcp,		/* number of points (starting from the 1st) whose parameters should not be modified.	
							* All B_ij (see below) with i<ncon are assumed to be zero
							控制点放在观测值的起始部分
							pLidObjs中的从0开始的ngcp个点为控制点*/
		LidMC_VP *pLidObjs,            /* conjugate points array */ 
		TiePoint_Adj_mode TiePt_mode,
			
							//				 char *vmask,			/* visibility mask: vmask[i, j]=1 if point i visible in image j, 0 otherwise. nxm */
		double *pX,			/* initial parameter vector p0: (a1, ..., am, b1, ..., bn).
												* aj are the image j parameters, bi are the i-th point parameters,
												* size m*cnp + n*pnp
												未知数，偏心分量和安置角    
												pX=[Xt, Yt, Zt, r, p, h]^T 传入未知数初值）*/
		//				const int pnp,			/* number of parameters 未知数个数*/
		orsLidJingParam *pOriParam,            /*   条带参数   */
		int		nStrips                        /*   条带数     */
//		unsigned long	param_type				/* 检校参数类型 */
		);

	//平差精度检查
	int  LidMC_QA(
		const int nVP,
		const int ngcp,
		LidMC_VP *pLidVP,
		double *pInitX,			/* 未知数初值*/
		double *pAdjX,			/* 未知数平差值*/
		const int pnp,			/* number of parameters 未知数个数*/
		Calib_Method  mType,
		orsLidJingParam oriParam,
		unsigned long	param_type		/* 参数类型*/
		);

public:
	double	*m_covar;	//未知数的协方差阵
	int		m_pn;		//未知数个数
	double  m_rms;		//单位权中误差（观测值）
	double  *m_corcoef;	//未知数之间的相关系数
};




#endif