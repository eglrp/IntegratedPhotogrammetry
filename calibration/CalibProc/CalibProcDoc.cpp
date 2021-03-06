// CalibProcDoc.cpp : implementation of the CCalibProcDoc class
//

#include "stdafx.h"
#include "CalibProc.h"

#include "CalibProcDoc.h"
#include "math.h"

#include "AlignPrj.h"

#include "\openrs\desktop\include\orsImage\orsIImageSourceReader.h"
#include "\openrs\desktop\include\orsImage\orsIImageService.h"
#include "\openrs\desktop\include\orsImageGeometry\orsIImageGeometryService.h"
#include "\openrs\desktop\include\orsImage\orsIImageWriter.h"
#include "\openrs\desktop\include\orsGuiBase\orsIProcessMsgBar.h"

#include "lidBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCalibProcDoc

IMPLEMENT_DYNCREATE(CCalibProcDoc, CDocument)

BEGIN_MESSAGE_MAP(CCalibProcDoc, CDocument)
	//{{AFX_MSG_MAP(CCalibProcDoc)
	ON_COMMAND(ID_ReadTrj, OnReadTrj)
	ON_COMMAND(ID_READ_ATNFILE, OnReadAtnfile)
	ON_COMMAND(ID_Read_TS_Bin, OnReadTSBin)
	ON_COMMAND(ID_GPS_Interpolation, OnGPSInterpolation)
	ON_COMMAND(ID_Cal_LaserPosition, OnCalLaserPosition)
	ON_COMMAND(ID_StatPointError, OnStatPointError)
	ON_COMMAND(ID_check_Projection, OncheckProjection)
	ON_COMMAND(ID_DetectCircleTarget, OnDetectCircleTarget)
	ON_COMMAND(ID_SegmentCircle, OnSegmentCircle)
	ON_COMMAND(ID_ImageTypeTrans, OnImageTypeTrans)
	ON_COMMAND(ID_GetOverlap, OnGetOverlap)
	ON_COMMAND(ID_GetBoundary, OnGetBoundary)
	ON_COMMAND(ID_GetOverlapData, OnGetOverlapData)
	ON_COMMAND(ID_QC_Overlap, OnQCOverlap)
	ON_COMMAND(ID_Point_Interpolation, OnPointInterpolation)
	ON_COMMAND(ID_Lidar_Point_Clip, OnLidarPointClip)
	ON_COMMAND(ID_Geodetic_To_Geocentric, OnGeodeticToGeocentric)
	ON_COMMAND(ID_Trajectory_Clipping, OnTrajectoryClipping)
	ON_COMMAND(ID_TransTPlane, OnTransTPlane)
	ON_COMMAND(ID_CalPos_TPlane, OnCalPosTPlane)
	ON_COMMAND(ID_ATNToLAS11, OnATNToLAS11)
	ON_COMMAND(ID_ATN2TPlane, OnATN2TPlane)
	ON_COMMAND(ID_TiePoint, OnTiePoint)
	ON_COMMAND(ID_CreateATNIdx, OnCreateATNIdx)
	ON_COMMAND(ID_TEST_levmar, OnTESTlevmar)
	ON_COMMAND(ID_CoplanarAdjustment, OnCoplanarAdjustment)
	ON_COMMAND(ID_SimulateRectangle, OnSimulateRectangle)
	ON_COMMAND(ID_TP2UTM, OnTp2utm)
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_GCP_WGS84_To_TPlane, &CCalibProcDoc::OnGcpWgs84ToTplane)
	ON_COMMAND(ID_LIDARCALIBRATION_TRUEPOINT, &CCalibProcDoc::OnLidarcalibrationTruepoint)
	ON_COMMAND(ID_CoplanarAdjustment3Param, &CCalibProcDoc::OnCoplanaradjustment3param)
	ON_COMMAND(ID_CoplanarAdj6P_ZConstraint, &CCalibProcDoc::OnCoplanaradj6pZconstraint)
	ON_COMMAND(ID_MountingError, &CCalibProcDoc::OnMountingerror)
	ON_COMMAND(ID_AdjustmentEvaluation, &CCalibProcDoc::OnAdjustmentevaluation)
	ON_COMMAND(ID_ImgInorRectify, &CCalibProcDoc::OnImginorrectify)
	ON_COMMAND(ID_Geodetic2UTM, &CCalibProcDoc::OnGeodetic2utm)
	ON_COMMAND(ID_Test, &CCalibProcDoc::OnTest)
//	ON_COMMAND(ID_32824, &CCalibProcDoc::On32824)
	ON_COMMAND(ID_ImgCalib, &CCalibProcDoc::OnImgcalib)
	ON_COMMAND(ID_PosInterpolateFormLIDAR, &CCalibProcDoc::OnPosinterpolateformlidar)
	ON_COMMAND(ID_Rotate, &CCalibProcDoc::OnRotate)
	ON_COMMAND(ID_ATN_Crop, &CCalibProcDoc::OnAtnCrop)
	ON_COMMAND(ID_QC_Plane, &CCalibProcDoc::OnQcPlane)
	ON_COMMAND(ID_QC_VP, &CCalibProcDoc::OnQcVp)
	ON_COMMAND(ID_HONV_Stat, &CCalibProcDoc::OnHonvStat)
	ON_COMMAND(ID_SelectTiePtByPlane, &CCalibProcDoc::OnSelecttieptbyplane)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCalibProcDoc construction/destruction

ORS_GET_IMAGE_SERVICE_IMPL();

CCalibProcDoc::CCalibProcDoc()
{
	// TODO: add one-time construction code here

}

CCalibProcDoc::~CCalibProcDoc()
{
}

#include "..\LidCalib\TrjReader.h"
#include "orsATNdriver\ATNReader.h"
#include "..\LidCalib\ts_scanbin_reader.h"

#include "Geometry.h"
#include "..\LidCalib\LidarGeometry.h"
#include "..\lastool\laswriter.h"

#include "..\lastool\lasreader.h"
#include <time.h>

#include "..\LidCalib\CircleDetection.h"
#include "..\LidCalib\pointinterpolation.h"

#include "..\LidQC\StripOverlap.h"

#include "\openrs\desktop\include\orsSRS\orsIEllipsoid.h"
#include "\openrs\desktop\include\orsSRS\orsISpatialReferenceService.h"

#include "..\lastool\laswriter_fast.h"
#include "..\lastool\lasreader_fast.h"
#include "SetMisalignDlg.h"
#include "..\LidCalib\ATN_TPReader.h"

#include "..\lidBasLib\PolygonFilling.h"

#include "orsMath\orsIMatrixService.h"



BOOL CCalibProcDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CCalibProcDoc serialization

void CCalibProcDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CCalibProcDoc diagnostics

#ifdef _DEBUG
void CCalibProcDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CCalibProcDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CCalibProcDoc commands


void CCalibProcDoc::OnReadTrj() 
{
	CFileDialog  dlg(TRUE,"trajectory",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"航迹文件(*.trj)|*.trj|(*.*)|*.*||",NULL);	
	CTrjReader trjreader;
	
	if(dlg.DoModal()!=IDOK)
		return;

	CString strPath=dlg.GetPathName();

//	trjreader.open(strPath.GetBuffer(0));
	trjreader.readTrjPos(strPath.GetBuffer(0));
	
}


void CCalibProcDoc::OnReadAtnfile() 
{
	CFileDialog  dlg(TRUE,"Attune",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"atn文件(*.las)|*.las|(*.*)|*.*||",NULL);	
	CATNReader atnreader;
	ATNPoint	point;
	
	if(dlg.DoModal()!=IDOK)
		return;
	
	CString strPath=dlg.GetPathName();
	
	atnreader.open(strPath.GetBuffer(0));

	while(atnreader.read_point(&point))
	{
		if(point.gpstime>450001.907620)
		{
			TRACE("\n");
		}
	}
}


void CCalibProcDoc::OnReadTSBin() 
{
	CFileDialog  dlg(TRUE,"TerraScan",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"bin文件(*.bin)|*.bin|(*.*)|*.*||",NULL);	
	ScanHdr Hdr;
	ScanPnt *Tbl=NULL;
	UINT *Sec=NULL;
	RgbClr *Clr=NULL;
//	int Cnt;
	CScanBinReader reader;
	double x, y, z;

	if(dlg.DoModal()!=IDOK)
		return;
	
	CString strPath=dlg.GetPathName();

	reader.open(strPath.GetBuffer(0), &Hdr);

	while(reader.read_point(x, y, z))
	{
		;
	}
}

void CCalibProcDoc::OnGPSInterpolation() 
{
	CFileDialog  trjdlg(TRUE,"trajectory",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"航迹文件(*.trj)|*.trj|(*.*)|*.*||",NULL);	
	CTrjReader trjreader;
	
	if(trjdlg.DoModal()!=IDOK)
		return;
	
	CString strtrjPath=trjdlg.GetPathName();
	trjreader.readTrjPos(strtrjPath.GetBuffer(0));
	trjreader.qsort_time_stamp_ascending();


	CFileDialog  atndlg(TRUE,"Attune",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"atn文件(*.las)|*.las|(*.*)|*.*||",NULL);	
	CATNReader atnreader;
	ATNPoint	point;
	
	if(atndlg.DoModal()!=IDOK)
		return;
	
	CString stratnPath=atndlg.GetPathName();
	
	atnreader.open(stratnPath.GetBuffer(128));
	
	double x, y, z;
	while(atnreader.read_point(&point))
	{
		trjreader.interpolation_linear(point.gpstime, x, y, z);
	}
}

void CCalibProcDoc::OnCalLaserPosition() 
{
// 	CFileDialog  trjdlg(TRUE,"trajectory",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"航迹文件(*.trj)|*.trj|(*.*)|*.*||",NULL);	
// 	CTrjReader trjreader;
// 	
// 	if(trjdlg.DoModal()!=IDOK)
// 		return;
// 	
// 	CString strtrjPath=trjdlg.GetPathName();
// 	trjreader.readTrjPos(strtrjPath.GetBuffer(128));
// 	trjreader.qsort_time_stamp_ascending();
// 	
	
	CFileDialog  atndlg(TRUE,"Attune",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"atn文件(*.las)|*.las|(*.*)|*.*||",NULL);	
	CATNReader atnreader;
	ATNPoint	point;
//	LASpoint	*ptBuf=NULL;
	POINT3D     *delta=NULL;
	LASheader  header;
	LASwriter  lasWriter;
//	LasPoint   lasPt;
	
	if(atndlg.DoModal()!=IDOK)
		return;
	
	CString stratnPath=atndlg.GetPathName();
	
	atnreader.open(stratnPath.GetBuffer(128));

//	header=atnreader.m_header;

	CFileDialog  lasdlg(FALSE,"导出解算后的LAS文件",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"LAS文件(*.LAS)|*.LAS|(*.*)|*.*||",NULL);	
	if(lasdlg.DoModal()!=IDOK)
		return;

	CString  strLasPath=lasdlg.GetPathName();
	FILE *fp=0;
	fp=fopen(strLasPath.GetBuffer(128), "wb");
	if(fp==0)
		return;

	lasWriter.open(fp, NULL);

	double x, y, z;
	double pos[3];
	double t[3];
	double angle[3];
	double leverArm[3];
	double misalign[3];
	orsLidarGeometry	lidGeo;
	double lat, lon, h;

	lidGeo.SetORSPlatform(getPlatform());
	lidGeo.InitCoordTransform();
	int ptNum=atnreader.m_npoints;

//	ptBuf=new LASpoint[ptNum];
	delta=new POINT3D[ptNum];
	double *gpstime=NULL;
	gpstime=new double[ptNum];
	POINT3D *imu=NULL;
	imu=new POINT3D[ptNum];
	double *scanA=NULL;
	scanA=new double[ptNum];
	double *range=NULL;
	range=new double[ptNum];

	ATNPoint atn[6];
	POINT3D  calpt[6];

	int iter=0;
	for(int i=0; i<ptNum; i++)
	{
		atnreader.read_point(&point);
		gpstime[i]=point.gpstime;
		delta[i].X=point.longitude;
		delta[i].Y=point.latitude;
		delta[i].Z=point.altitude;

		imu[i].X=point.roll;
		imu[i].Y=point.pitch;
		imu[i].Z=point.heading;

		scanA[i]=point.scanAngle;
		range[i]=point.range;

		
	
// 		ptBuf[i].edge_of_flight_line=point.edge_of_flight_line;
// 		ptBuf[i].intensity=point.intensity;
// 		ptBuf[i].number_of_returns_of_given_pulse=point.number_of_returns_of_given_pulse;
// 		ptBuf[i].point_source_ID=point.point_source_ID;
// 		ptBuf[i].return_number=point.return_number;
// 		ptBuf[i].scan_angle_rank=point.scan_angle_rank;
// 		ptBuf[i].scan_direction_flag=point.scan_direction_flag;
// 		ptBuf[i].user_data=point.user_data;

//		trjreader.interpolation_linear(point.gpstime, x, y, z);


		t[0]=x;
		t[1]=y;
		t[2]=z;

		angle[0]=point.roll;
		angle[1]=point.pitch;
		angle[2]=point.heading;

		leverArm[0]=leverArm[1]=leverArm[2]=0;
		misalign[0]=misalign[1]=misalign[2]=0;
	
//		lidGeo.calLaserPosition_TangentPlane(angle, leverArm, misalign, point.range, point.scanAngle, pos);
//		lidGeo.TangentPlane2Geographic(point.latitude/180*PI, point.longitude/180*PI, point.altitude, 
//										pos[1], pos[0], -pos[2],
//										&lat, &lon, &h);
					
//		lat=lat*180/PI;
//		lon=lon*180/PI;
		
		t[0]=point.latitude;	t[1]=point.longitude;	t[2]=point.altitude;
		lidGeo.calLaserPosition_WGS84_Geocentric(t, angle, leverArm, misalign, point.range, point.scanAngle, pos);
		lidGeo.WGS84_Geocentric_To_Geodetic(pos[0], pos[1], pos[2], &lat, &lon, &h);

		lat=lat;
		lon=lon;

//		if(point.number_of_returns_of_given_pulse>=point.return_number)
//		{
			atn[point.return_number]=point;
			calpt[point.return_number].X=lon;
			calpt[point.return_number].Y=lat;
			calpt[point.return_number].Z=h;
//			iter++;
//		}
// 		else
// 		{
// 			memset(atn, 0, sizeof(ATNPoint)*5);
// 			memset(calpt, 0, sizeof(POINT3D)*5);
// 			iter=0;
// 		}

		if(i==4706443)
		{
			TRACE("");
		}

		if(point.return_number==point.number_of_returns_of_given_pulse && 
			point.number_of_returns_of_given_pulse>1)
		{
			if(fabs(atn[point.return_number].range-atn[point.return_number-1].range)<1e-6)
			{
				point.user_data=255;
			}
		}

		if(fabs(lat-point.y)>8e-9 || fabs(lon-point.x)>8e-9 || fabs(h-point.z)>0.5)
		{
			TRACE("\t%d %e %e %lf\n", i, lat-point.y, lon-point.x, h-point.z);
		}
// 		if(i==846954)
// 		{
// 			TRACE("\t%d %e %e %lf\n", i, lat-point.y, lon-point.x, h-point.z);
// 		}

		delta[i].Y=lat-point.y;
		delta[i].X=lon-point.x;
		delta[i].Z=h-point.z;

// 		if(fabs(lat-point.y)>3e-9 || fabs(lon-point.x)>3e-9 || fabs(h-point.z)>0.3)
// 		{
// 			TRACE("\t%d\n", atnreader.m_pcount);
// 		}

		lidGeo.ProjectTransform(point.y*180/PI, point.x*180/PI, point.z, &x, &y, &z);
		lidGeo.ProjectTransform(lat*180/PI, lon*180/PI, h, &x, &y, &z);

//		x=lon; y=lat; z=h;

		lasWriter.write_point(x, y, z, point.intensity, point.return_number, point.number_of_returns_of_given_pulse,
			point.scan_direction_flag, point.edge_of_flight_line, point.classification, point.scan_angle_rank,
			point.user_data, point.point_source_ID, point.gpstime);
	}

	lasWriter.close();
	
	if(fp)
	{
		fclose(fp);
		fp=0;
	}

// 	if(ptBuf)
// 	{
// 		delete ptBuf;
// 		ptBuf=NULL;
// 	}
	if(delta)
	{
		delete delta;
		delta=NULL;
	}

	if(gpstime)
	{
		delete gpstime;
		gpstime=NULL;
	}
	if(imu)
	{
		delete imu;
		imu=NULL;
	}
	if(scanA)
	{
		delete scanA;
		scanA=NULL;
	}
	if(range)
	{
		delete range;
		range=NULL;
	}
}


void CCalibProcDoc::OnStatPointError() 
{
	CFileDialog  newdlg(TRUE,"解算后的LAS文件",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"解算后的LAS文件(*.LAS)|*.LAS|(*.*)|*.*||",NULL);	
	CFileDialog  orgdlg(TRUE,"原始的LAS文件",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"原始LAS文件(*.LAS)|*.LAS|(*.*)|*.*||",NULL);
	
	if(newdlg.DoModal()!=IDOK)
		return;

	CString strNewLasPath=newdlg.GetPathName();

	if(orgdlg.DoModal()!=IDOK)
		return;

	CString strOrgLasPath=orgdlg.GetPathName();

	FILE *fpnew=NULL;
	FILE *fporg=NULL;

	fpnew=fopen(strNewLasPath.GetBuffer(128), "rb");
	if(fpnew==0)	return;

	fporg=fopen(strOrgLasPath.GetBuffer(128), "rb");
	if(fporg==0)	return;

	LASreader newReader, orgReader;

	newReader.open(fpnew);
	orgReader.open(fporg);

	if(newReader.npoints!=orgReader.npoints)
	{
		AfxMessageBox("激光点数不一致!");
		return;
	}

	POINT3D  *pt=NULL, *newPoint=NULL, *orgPoint=NULL;
	unsigned short newInten, orgInten;
	double  newpt[3], orgpt[3];
	double  newtime, orgtime;
	int ptNum=newReader.npoints;
	double max_x, max_y, max_z, min_x, min_y, min_z, mean_x, mean_y, mean_z;
	max_x=-1.7e+308;	max_y=-1.7e+308;	max_z=-1.7e+308;
	min_x=1.7e+308;	min_y=1.7e+308;	min_z=1.7e+308;
	mean_x=mean_y=mean_z=0;

	pt=new POINT3D[ptNum];
	newPoint=new POINT3D[ptNum];
	orgPoint=new POINT3D[ptNum];

	int i;
	for(i=0; i<ptNum; i++)
	{//计算点坐标之差
		newReader.read_point(newpt);
		newReader.get_gpstime(&newtime);
		orgReader.read_point(orgpt);
		orgReader.get_gpstime(&orgtime);

		newReader.get_intensity(&newInten);
		orgReader.get_intensity(&orgInten);

		newPoint[i].X=newpt[0];
		newPoint[i].Y=newpt[1];
		newPoint[i].Z=newpt[2];
		orgPoint[i].X=orgpt[0];
		orgPoint[i].Y=orgpt[1];
		orgPoint[i].Z=orgpt[2];

		

		if(fabs(newtime-orgtime)>1e-6)
		{//不是对应点
			pt[i].X=10000;
			pt[i].Y=10000;
			pt[i].Z=10000;
		}
		else if(newReader.point.user_data==255)
		{//不统计第四次回波点
			pt[i].X=0;
			pt[i].Y=0;
			pt[i].Z=0;
		}
		else
		{
			pt[i].X=newpt[0]-orgpt[0];
			pt[i].Y=newpt[1]-orgpt[1];
			pt[i].Z=newpt[2]-orgpt[2];
		}

		if(fabs(pt[i].X)>0.2 && fabs(pt[i].Y)>0.2 && fabs(pt[i].Z)>0.2)
		{
			//TRACE("\t%d\n", i);
		}

		if(max_x < pt[i].X)
			max_x = pt[i].X;
		if(min_x > pt[i].X)
			min_x = pt[i].X;

		if(max_y < pt[i].Y)
			max_y = pt[i].Y;
		if(min_y > pt[i].Y)
			min_y = pt[i].Y;

		if(max_z < pt[i].Z)
			max_z = pt[i].Z;
		if(min_z > pt[i].Z)
			min_z = pt[i].Z;
		
		mean_x+=pt[i].X;
		mean_y+=pt[i].Y;
		mean_z+=pt[i].Z;
	}
	//统计均值
	mean_x/=ptNum;
	mean_y/=ptNum;
	mean_z/=ptNum;

	double x_sec, y_sec, z_sec;
	int secNum=256;		//按256分段
	int *x_histogram=NULL, *y_histogram=NULL, *z_histogram=NULL;
	double x_variance, y_variance, z_variance;

	x_sec=(max_x-min_x)/secNum;
	y_sec=(max_y-min_y)/secNum;
	z_sec=(max_z-min_z)/secNum;

	x_histogram=new int[secNum];
	y_histogram=new int[secNum];
	z_histogram=new int[secNum];
	memset(x_histogram, 0, sizeof(int)*secNum);
	memset(y_histogram, 0, sizeof(int)*secNum);
	memset(z_histogram, 0, sizeof(int)*secNum);

	int secID;
	x_variance=y_variance=z_variance=0;
	for(i=0; i<ptNum; i++)
	{
		x_variance+=(pt[i].X-mean_x)*(pt[i].X-mean_x);
		y_variance+=(pt[i].Y-mean_y)*(pt[i].Y-mean_y);
		z_variance+=(pt[i].Z-mean_z)*(pt[i].Z-mean_z);

		//统计直方图
		secID=int((pt[i].X-min_x)/x_sec);
		if(secID==secNum)
			secID--;
		x_histogram[secID]++;

		secID=int((pt[i].Y-min_y)/y_sec);
		if(secID==secNum)
			secID--;
		y_histogram[secID]++;
		
		secID=int((pt[i].Z-min_z)/z_sec);
		if(secID==secNum)
			secID--;
		z_histogram[secID]++;

	}

	//统计方差
	x_variance/=ptNum;
	y_variance/=ptNum;
	z_variance/=ptNum;

	//写统计文件
	FILE *fout=NULL;
	CFileDialog  dlg(FALSE,"保存统计信息",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"保存统计信息(*.txt)|*.txt|(*.*)|*.*||",NULL);
	bool  *bsel=NULL;
	bsel=new bool[ptNum];
	memset(bsel, 0, sizeof(bool)*ptNum);

	if(dlg.DoModal()==IDOK)
	{
		fout=fopen(dlg.GetPathName().GetBuffer(128), "wt");

		fprintf(fout,  "##  LiDAR position error statistics\n");
		fprintf(fout, "## total point number: %d\n", ptNum);
		fprintf(fout, "## mean error: mx=%lf  my=%lf  mz=%lf\n", mean_x, mean_y, mean_z);
		fprintf(fout, "## variance: vx=%lf  vy=%lf  vz=%lf\n", x_variance, y_variance, z_variance);
		fprintf(fout, "## Histogram of position error\n");
		fprintf(fout, "## number of steps: %d\n", secNum);
		fprintf(fout, "## XMin=%lf  XMax=%lf  YMin=%lf  YMax=%lf  ZMin=%lf  ZMax=%lf\n", min_x, max_x, min_y, max_y, min_z, max_z);
		
		double xsecPos, ysecPos, zsecPos;
		for(i=0; i<secNum; i++)
		{
			xsecPos=min_x+i*x_sec;
			ysecPos=min_y+i*y_sec;
			zsecPos=min_z+i*z_sec;

			fprintf(fout, "%10.5lf %d %10.5lf %d %10.5lf %d\n",
				xsecPos, x_histogram[i], ysecPos, y_histogram[i], zsecPos, z_histogram[i]);
		}
		
		//随机写500个点的误差
		int nRandom=500;
		
		fprintf(fout, "## random selection %d point error\n", nRandom);
		fprintf(fout, "## x_err y_err z_err\n");
		srand((unsigned)time(NULL));
		for(i=0; i<nRandom; i++)
		{
			bool found;
			int idx;

			do 
			{
				found = false;
				idx = rand() /double(RAND_MAX) * ptNum;
				
				if(bsel[idx]==0)
					found=true;

			} while (!found);
			
			fprintf(fout, "%10.5lf %10.5lf %10.5lf\n", pt[idx].X, pt[idx].Y, pt[idx].Z);
			bsel[idx]=true;
		}

	}

	
	//函数结束，释放内存
	if(fpnew)
	{
		fclose(fpnew);
		fpnew=NULL;
	}
	if(fporg)
	{
		fclose(fporg);
		fporg=NULL;
	}
	if(fout)
	{
		fclose(fout);
		fout=NULL;
	}
	
	if(pt)
	{
		delete[] pt;
		pt=NULL;
	}
	if(newPoint)
	{
		delete[] newPoint;
		newPoint=NULL;
	}
	if(orgPoint)
	{
		delete[] orgPoint;
		orgPoint=NULL;
	}
	if(x_histogram)
	{
		delete[] x_histogram;
		x_histogram=NULL;
	}
	if(y_histogram)
	{
		delete[] y_histogram;
		y_histogram=NULL;
	}
	if(z_histogram)
	{
		delete[] z_histogram;
		z_histogram=NULL;
	}
	if(bsel)
	{
		delete[] bsel;
		bsel=NULL;
	}
}

void CCalibProcDoc::OncheckProjection() 
{
	CFileDialog  newdlg(TRUE,"经纬度坐标LAS文件",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"经纬度坐标LAS文件(*.LAS)|*.LAS|(*.*)|*.*||",NULL);	
	CFileDialog  orgdlg(TRUE,"平面坐标LAS文件",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"平面坐标LAS文件(*.LAS)|*.LAS|(*.*)|*.*||",NULL);

	CFileDialog  atndlg(TRUE,"Attune",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"atn文件(*.las)|*.las|(*.*)|*.*||",NULL);	
	CATNReader atnreader;
	ATNPoint	point;
	LASheader  header;
	LASwriter  lasWriter;
	//	LasPoint   lasPt;
	
	if(atndlg.DoModal()!=IDOK)
		return;
	
	CString stratnPath=atndlg.GetPathName();
	
	atnreader.open(stratnPath.GetBuffer(128));

	if(newdlg.DoModal()!=IDOK)
		return;
	
	CString strNewLasPath=newdlg.GetPathName();
	
	if(orgdlg.DoModal()!=IDOK)
		return;
	
	CString strOrgLasPath=orgdlg.GetPathName();
	
	FILE *fpnew=NULL;
	FILE *fporg=NULL;
	
	fpnew=fopen(strNewLasPath.GetBuffer(128), "rb");
	if(fpnew==0)	return;
	
	fporg=fopen(strOrgLasPath.GetBuffer(128), "rb");
	if(fporg==0)	return;
	
	LASreader newReader, orgReader;
	
	newReader.open(fpnew);
	orgReader.open(fporg);

	int ptNum=newReader.npoints;
	int i;
	unsigned short newInten, orgInten;
	double  newpt[3], orgpt[3];
	double  newtime, orgtime;
	
	ATNPoint atn[6];
	POINT3D  calpt[6];
	LASpoint  lspt[6];
	for(i=0; i<ptNum; i++)
	{//计算点坐标之差
		newReader.read_point(newpt);
		newReader.get_gpstime(&newtime);
		orgReader.read_point(orgpt);
		orgReader.get_gpstime(&orgtime);
		
		newReader.get_intensity(&newInten);
		orgReader.get_intensity(&orgInten);

		atnreader.read_point(&point);

		atn[point.return_number]=point;
		calpt[point.return_number].X=orgpt[0];
		calpt[point.return_number].Y=orgpt[1];
		calpt[point.return_number].Z=orgpt[2];

		lspt[point.return_number]=orgReader.point;

		if(i==846954)
		{
			TRACE("");
		}
	}


	if(fpnew)
	{
		fclose(fpnew);
		fpnew=NULL;
	}
	if(fporg)
	{
		fclose(fporg);
		fporg=NULL;
	}
}


void CCalibProcDoc::OnDetectCircleTarget() 
{
	POINT2D *pData1=NULL, *pData2=NULL;
	int ptNum1=50, ptNum2;
	double r1=2, r2=1;
	double sq_r1, sq_r2;
	POINT2D center;
	int i;

	CFileDialog  dlg(TRUE,"LAS文件",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"LAS文件(*.LAS)|*.LAS|(*.*)|*.*||",NULL);	
	double xc, yc;

	pData1=new POINT2D[10000];
	pData2=new POINT2D[10000];

	if(dlg.DoModal()!=IDOK)
		return;

	CString strName=dlg.GetPathName();
	LASreader reader;

	FILE *fp=NULL;
	fp=fopen(strName.GetBuffer(128),"rb");
	if(fp==NULL)
		return;

	reader.open(fp);
	xc=(reader.header.max_x+reader.header.min_x)/2;
	yc=(reader.header.max_y+reader.header.min_y)/2;

	ptNum1=0;	ptNum2=0;
	double pt[3];
	sq_r1=r1*r1;
	sq_r2=r2*r2;
	while(reader.read_point(pt))
	{
		double dis2=(pt[0]-xc)*(pt[0]-xc)+(pt[1]-yc)*(pt[1]-yc);
		if(dis2-sq_r1<=0 && dis2-sq_r2>0)
		{
			pData1[ptNum1].x=pt[0]-xc;
			pData1[ptNum1].y=pt[1]-yc;
			
			ptNum1++;
		}
		else if(dis2-sq_r2<=0)
		{
			pData2[ptNum2].x=pt[0]-xc;
			pData2[ptNum2].y=pt[1]-yc;
			
			ptNum2++;
		}
	}	
	if(fp)
		fclose(fp);

	TRACE("ptData 1 num=%d\n", ptNum1);
	for(i=0; i<ptNum1; i++)
	{
		TRACE("%.3f %.3f\n", pData1[i].x, pData1[i].y);
	}

	TRACE("ptData 2 num=%d\n", ptNum2);
	for(i=0; i<ptNum2; i++)
	{
		TRACE("%.3f %.3f\n", pData2[i].x, pData2[i].y);
	}

//	simulate_onecircle(ptNum, r, pData);
//	DetectCircleCenter_2d(pData, ptNum, r, center);
	
	DetectConcentric_2d(pData1, ptNum1, r1, pData2, ptNum2, r2, center);


	if(pData1)
	{
		delete pData1;
		pData1=NULL;
	}
	if(pData2)
	{
		delete pData2;
		pData2=NULL;
	}
}


void CCalibProcDoc::OnSegmentCircle() 
{
	POINT3D *pData1=NULL, *pData2=NULL;
	int ptNum1=50, ptNum2;
	double len=3, r1=2, r2=1;
	double sq_r1, sq_r2;
	POINT2D center;
	int i;
	
	CFileDialog  dlg(TRUE,"LAS文件",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"LAS文件(*.LAS)|*.LAS|(*.*)|*.*||",NULL);	
	double xc, yc;
	
	pData1=new POINT3D[10000];
//	pData2=new POINT2D[10000];
	
	if(dlg.DoModal()!=IDOK)
		return;
	
	CString strName=dlg.GetPathName();
	LASreader reader;
	
	FILE *fp=NULL;
	fp=fopen(strName.GetBuffer(128),"rb");
	if(fp==NULL)
		return;
	
	reader.open(fp);
	xc=(reader.header.max_x+reader.header.min_x)/2;
	yc=(reader.header.max_y+reader.header.min_y)/2;
	
	ptNum1=0;	ptNum2=0;
	double pt[3];
	sq_r1=len*len;
	sq_r2=r2*r2;
	double dis2;
	while(reader.read_point(pt))
	{
		dis2=(pt[0]-xc)*(pt[0]-xc)+(pt[1]-yc)*(pt[1]-yc);
		if(dis2-sq_r1<=0/* && dis2-sq_r2>0*/)
		{
			pData1[ptNum1].X=pt[0]-xc;
			pData1[ptNum1].Y=pt[1]-yc;
			pData1[ptNum1].Z=pt[2];

			ptNum1++;
		}
	}	
	if(fp)
		fclose(fp);
	
	
	CFileDialog  outdlg(FALSE,"待检测影像",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"待检测影像(*.tif)|*.tif|(*.*)|*.*||",NULL);

	
	if(outdlg.DoModal()!=IDOK)
		return;
	CString strOutput=outdlg.GetPathName();
	LASpointXYZI *pBuf=NULL;

	pBuf=new LASpointXYZI[ptNum1];

	sq_r1=r1*r1;
	sq_r2=r2*r2;
	for(i=0; i<ptNum1; i++)
	{
		dis2=(pData1[i].X/*-xc*/)*(pData1[i].X/*-xc*/)+(pData1[i].Y/*-yc*/)*(pData1[i].Y/*-yc*/);
		if(dis2-sq_r1<=0 && dis2-sq_r2>0)
		{
			pBuf[i].x=pData1[i].X;
			pBuf[i].y=pData1[i].Y;
			pBuf[i].z=pData1[i].Z;
			pBuf[i].intensity=60;
		}
		else if(dis2-sq_r2<=0)
		{
			pBuf[i].x=pData1[i].X;
			pBuf[i].y=pData1[i].Y;
			pBuf[i].z=pData1[i].Z;
			pBuf[i].intensity=180;
		}
		else
		{
			pBuf[i].x=pData1[i].X;
			pBuf[i].y=pData1[i].Y;
			pBuf[i].z=pData1[i].Z;
			pBuf[i].intensity=100;
		}
		
	}

	TRACE("ptData 1 num=%d\n", ptNum1);
	for(i=0; i<ptNum1; i++)
	{
		TRACE("%.3f %.3f %.3f %d\n", pBuf[i].x, pBuf[i].y, pBuf[i].z, pBuf[i].intensity);
	}
	
	SetORSPlatform(getPlatform());
	
	ConvertToImage(strOutput.GetBuffer(128), pBuf, ptNum1, 2, 0.005);

	if(pData1)
	{
		delete pData1;
		pData1=NULL;
	}
// 	if(pData2)
// 	{
// 		delete pData2;
// 		pData2=NULL;
// 	}
	if(pBuf)
	{
		delete pBuf;
		pBuf=NULL;
	}
}


void CCalibProcDoc::OnImageTypeTrans() 
{
	CFileDialog  inputdlg(TRUE,"影像文件",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"影像文件(*.tif)|*.tif|(*.*)|*.*||",NULL);	

	if(inputdlg.DoModal()!=IDOK)
		return;
	
	CString strName=inputdlg.GetPathName();

	CFileDialog outputdlg(FALSE,"转换影像",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"转换影像(*.bmp)|*.bmp|(*.*)|*.*||",NULL);

	if(outputdlg.DoModal()!=IDOK)
		return;

	CString strOutput=outputdlg.GetPathName();

	ref_ptr<orsIImageService>  imageService = 
		ORS_PTR_CAST(orsIImageService,getPlatform()->getService(ORS_SERVICE_IMAGE) );
	ref_ptr<orsIImageSourceReader> ImgReader;
	ImgReader=imageService->openImageFile(strName.GetBuffer(128));
	if (!ImgReader.get())
	{//could not open the image
		return ;
	}

	orsRect_i cacheRect = ImgReader->getBoundingRect();
	orsBandSet bandSet;
	bandSet.push_back( 0 );

	ref_ptr<orsIImageData> imageData = ImgReader->getImageData( cacheRect, 1, bandSet );
	
	float *buf=(float*)(imageData->getBandBuf(0));
	
	int imgWid=ImgReader->getWidth();
	int imgHei=ImgReader->getHeight();

	BYTE  *rbuf=new BYTE[imgWid*imgHei];
	memset(rbuf, 0, sizeof(BYTE)*imgWid*imgHei);

	for(int i=0; i<imgHei; i++)
	{
		for(int j=0; j<imgHei; j++)
		{
			if(buf[i*imgWid+j]>0)
			{
				rbuf[i*imgWid+j]=BYTE(buf[i*imgWid+j]+0.5);
			}
		}
	}

	ref_ptr<orsIImageGeometry> pWriterGeom = ORS_CREATE_OBJECT( orsIImageGeometry, ORS_IMAGEGEOMETRY_AFFINE );
	
	ref_ptr<orsIImageWriter> pImgWriter = NULL;
	pImgWriter = ORS_CREATE_OBJECT( orsIImageWriter, ORS_IMAGE_WRITER_DEFAULT );
	{
		geoImgINFO	imgInfo;
		
		imgInfo.tile_wid = 256;
		imgInfo.tile_hei = 256;
		
		imgInfo.transType = geoImgTranOFFSETSCALE;
		imgInfo.i0 = 0;	imgInfo.x0 = 0;
		imgInfo.j0 = 0;	imgInfo.y0 = 0;
		imgInfo.k0 = 0;	imgInfo.z0 = 0;
		
		imgInfo.xScale = 0.002;
		imgInfo.yScale = 0.002;
		imgInfo.zScale = 1.0;
		
		
		orsSIZE size(imgWid, imgHei);
		
		//		ref_ptr<orsIOGRString> hcsWkt;
		
		if( !pImgWriter->Create( strOutput.GetBuffer(128), size, 1, ORS_DT_BYTE, &imgInfo, NULL ) )
		{
			return;
		}
		
		if( NULL != pWriterGeom.get() ) {
			
			ref_ptr<orsIProperty> affinePropert = getPlatform()->createProperty();
			
			double adfGeoTransform[6];
			// 平移，缩放转换为仿射变换
			adfGeoTransform[0] = imgInfo.x0 - imgInfo.i0 * imgInfo.xScale;
			adfGeoTransform[1] = imgInfo.xScale;
			adfGeoTransform[2] = 0.0;
			adfGeoTransform[3] = imgInfo.y0 - imgInfo.j0 * imgInfo.yScale;;
			adfGeoTransform[4] = 0;
			adfGeoTransform[5] = imgInfo.yScale;
			
			for( int i=0; i<6; i++ )
				affinePropert->addAttr( ORS_PROPERTY_AFFINE, adfGeoTransform[i] );
			
			pWriterGeom->initFromProperty( affinePropert.get() );
		} 
		
		pImgWriter->WriteBandRect( 0, 0, 0, imgWid, imgHei, rbuf );
		}

	if(rbuf)
	{
		delete rbuf;
		rbuf=NULL;
	}
}


void CCalibProcDoc::OnGetOverlap() 
{
	CFileDialog  lasdlg(TRUE,"LAS文件",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"LAS文件(*.LAS)|*.LAS|(*.*)|*.*||",NULL);	
	CString strLasName;
	LASreader reader1, reader2;
	FILE *fp1=NULL, *fp2=NULL;
	Overlap_Block pOBlock;


	if(lasdlg.DoModal()!=IDOK)
	{
		return;
	}

	strLasName=lasdlg.GetPathName();
	fp1=fopen(strLasName.GetBuffer(128),"rb");
	if(fp1==NULL)
		return;
	reader1.open(fp1);

	if(lasdlg.DoModal()!=IDOK)
	{
		return;
	}

	strLasName=lasdlg.GetPathName();
	fp2=fopen(strLasName.GetBuffer(128),"rb");
	if(fp2==NULL)
		return;
	reader2.open(fp2);



	CalOverlapBlock(&reader1, &reader2, 20, &pOBlock);

	CFileDialog  overlapDlg(FALSE,"overlap block",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"txt文件(*.txt)|*.txt|(*.*)|*.*||",NULL);	
	FILE *fout=NULL;
	
	if(overlapDlg.DoModal()!=IDOK)
		return;

	strLasName=overlapDlg.GetPathName();
	fout=fopen(strLasName.GetBuffer(128), "wt");

	int idx;
	for(int i=0; i<pOBlock.row; i++)
	{
		for(int j=0; j<pOBlock.col; j++)
		{
			idx=i*pOBlock.col+j;
			fprintf(fout, "%d ", pOBlock.pOverlap[idx]);
		}
		fprintf(fout, "\n");
	}


	if(fp1)	fclose(fp1);	fp1=NULL;
	if(fp2)	fclose(fp2);	fp2=NULL;
	if(fout)	fclose(fout);	fout=NULL;
}

void CCalibProcDoc::OnGetBoundary() 
{
	CFileDialog  lasdlg(TRUE,"LAS文件",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"LAS文件(*.LAS)|*.LAS|(*.*)|*.*||",NULL);	
	CString strLasName;
	LASreader reader1;
	FILE *fp1=NULL;
	
	
	if(lasdlg.DoModal()!=IDOK)
	{
		return;
	}
	
	strLasName=lasdlg.GetPathName();
	fp1=fopen(strLasName.GetBuffer(128),"rb");
	if(fp1==NULL)
		return;
	reader1.open(fp1);
	
		
//	CArray<LASpointXYZI, LASpointXYZI>  pOverlapData;
	CArray<POINT3D, POINT3D> pBoundary;
	CalLasBoundary(&reader1, 5.0, &pBoundary);
	
	CFileDialog  overlapDlg(FALSE,"Boundary",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"txt文件(*.txt)|*.txt|(*.*)|*.*||",NULL);	
	FILE *fout=NULL;
	
	if(overlapDlg.DoModal()!=IDOK)
		return;
	
	strLasName=overlapDlg.GetPathName();
	fout=fopen(strLasName.GetBuffer(128), "wt");
	
	for(int i=0; i<pBoundary.GetSize(); i++)
	{
		fprintf(fout, "%.3f %.3f %.3f\n", pBoundary[i].X, pBoundary[i].Y, pBoundary[i].Z);
	}
	
	
	if(fp1)	fclose(fp1);	fp1=NULL;
	if(fout)	fclose(fout);	fout=NULL;	
}

void CCalibProcDoc::OnGetOverlapData() 
{
	CArray<LASpointXYZI, LASpointXYZI>  pOverlapData;
	CFileDialog  lasdlg(TRUE,"LAS文件",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"LAS文件(*.LAS)|*.LAS|(*.*)|*.*||",NULL);	
	CString strLasName;
	LASreader reader1, reader2;
	FILE *fp1=NULL, *fp2=NULL;
	Overlap_Block pOBlock;
	
	
	if(lasdlg.DoModal()!=IDOK)
	{
		return;
	}
	
	strLasName=lasdlg.GetPathName();
	fp1=fopen(strLasName.GetBuffer(128),"rb");
	if(fp1==NULL)
		return;
	reader1.open(fp1);
	
	if(lasdlg.DoModal()!=IDOK)
	{
		return;
	}
	
	strLasName=lasdlg.GetPathName();
	fp2=fopen(strLasName.GetBuffer(128),"rb");
	if(fp2==NULL)
		return;
	reader2.open(fp2);

	GetOverlapData(&reader1, &reader2, &pOverlapData);


	CFileDialog  overlapDlg(FALSE,"overlap point",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"txt文件(*.txt)|*.txt|(*.*)|*.*||",NULL);	
	FILE *fout=NULL;
	
	if(overlapDlg.DoModal()!=IDOK)
		return;
	
	strLasName=overlapDlg.GetPathName();
	fout=fopen(strLasName.GetBuffer(128), "wt");
	
//	int idx;
	for(int i=0; i<pOverlapData.GetSize(); i++)
	{
		fprintf(fout, "%.3f %.3f %.3f %d\n", pOverlapData[i].x, pOverlapData[i].y, pOverlapData[i].z, pOverlapData[i].intensity);
	}
	
	
	if(fp1)	fclose(fp1);	fp1=NULL;
	if(fp2)	fclose(fp2);	fp2=NULL;
	if(fout)	fclose(fout);	fout=NULL;	
}

void CCalibProcDoc::OnQCOverlap() 
{
	CMemoryPool<POINT3D, POINT3D&> pOverlapErr;
	CFileDialog  lasdlg(TRUE,"LAS文件",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"LAS文件(*.LAS)|*.LAS|(*.*)|*.*||",NULL);	
	CString strLasName1, strLasName2, strOutput;
//	LASreader reader1, reader2;
//	FILE *fp1=NULL, *fp2=NULL;
//	Overlap_Block pOBlock;
	
	lasdlg.m_ofn.lpstrTitle="打开参考LAS";
	if(lasdlg.DoModal()!=IDOK)
	{
		return;
	}
	
	strLasName1=lasdlg.GetPathName();
// 	fp1=fopen(strLasName.GetBuffer(128),"rb");
// 	if(fp1==NULL)
// 		return;
// 	reader1.open(fp1);
	
	lasdlg.m_ofn.lpstrTitle="打开待配准LAS";
	if(lasdlg.DoModal()!=IDOK)
	{
		return;
	}
	
	strLasName2=lasdlg.GetPathName();

	CFileDialog  outDlg(FALSE,"las",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"LAS文件(*.LAS)|*.LAS|(*.*)|*.*||",NULL);	
	outDlg.m_ofn.lpstrTitle="保存差值LAS";
	if(outDlg.DoModal()!=IDOK)
		return;

	strOutput=outDlg.GetPathName();
	
	QC_CalOverlapError(strLasName1.GetBuffer(128), strLasName2.GetBuffer(128), strOutput.GetBuffer(128), 0);

// 	QC_CalOverlapError(strLasName1.GetBuffer(128), strLasName2.GetBuffer(128), &pOverlapErr, 0);
// 	
// 	
// 	CFileDialog  overlapDlg(FALSE,"overlap error",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"txt文件(*.txt)|*.txt|(*.*)|*.*||",NULL);	
// 	FILE *fout=NULL;
// 	
// 	if(overlapDlg.DoModal()!=IDOK)
// 		return;
// 	
// 	CString strOutput;
// 	strOutput=overlapDlg.GetPathName();
// 	fout=fopen(strOutput.GetBuffer(128), "wt");
// 	
// 	//	int idx;
// 	POINT3D *pt;
// 	for(int i=0; i<pOverlapErr.GetNumberOfItems(); i++)
// 	{
// 		pt=pOverlapErr.GetItem(i);
// 		fprintf(fout, "%.3f %.3f %.3f\n", pt->X, pt->Y, pt->Z);
// 	}
	
	
// 	if(fp1)	fclose(fp1);	fp1=NULL;
// 	if(fp2)	fclose(fp2);	fp2=NULL;
//	if(fout)	fclose(fout);	fout=NULL;
}

void CCalibProcDoc::OnPointInterpolation() 
{
	CFileDialog  dlg(TRUE,"离散点",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"离散点(*.txt)|*.txt|(*.*)|*.*||",NULL);	
	CString strFileName;
	
	if(dlg.DoModal()!=IDOK)
		return;

	strFileName=dlg.GetPathName();
	CMemoryPool<POINT3D, POINT3D&> pData;
	POINT3D	*pt;
	FILE *fp=NULL;

	fp=fopen(strFileName.GetBuffer(128), "r");
	while(!feof(fp))
	{
		pt=pData.PoolAlloc();
		fscanf(fp, "%lf %lf %lf\n", &(pt->X), &(pt->Y), &(pt->Z));
	}

	fclose(fp);

	CFileDialog  outDlg(FALSE,"image",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"image文件(*.img)|*.img|(*.*)|*.*||",NULL);	
	if(outDlg.DoModal()!=IDOK)
		return;
	
	strFileName=outDlg.GetPathName();
	SetORSPlatform(getPlatform());
	ConvertToImage(strFileName.GetBuffer(128), &pData, 1.0);

}

void CCalibProcDoc::OnLidarPointClip() 
{
	CFileDialog  lasdlg(TRUE,"LAS文件",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"LAS文件(*.LAS)|*.LAS|(*.*)|*.*||",NULL);
	CFileDialog  trjdlg(TRUE,"轨迹文件",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"轨迹文件(*.trj)|*.trj|(*.*)|*.*||",NULL);
	CString strLasFile1, strLasFile2, strTrjFile1, strTrjFile2;

	if(lasdlg.DoModal()!=IDOK)
		return;
	strLasFile1=lasdlg.GetPathName();

	if(lasdlg.DoModal()!=IDOK)
		return;
	strLasFile2=lasdlg.GetPathName();

	if(trjdlg.DoModal()!=IDOK)
		return;
	strTrjFile1=trjdlg.GetPathName();

	if(trjdlg.DoModal()!=IDOK)
		return;
	strTrjFile2=trjdlg.GetPathName();
	
	OverlapClip_Centreline(strLasFile1.GetBuffer(128), strLasFile2.GetBuffer(128), strTrjFile1.GetBuffer(128), strTrjFile2.GetBuffer(128));

}


void CCalibProcDoc::OnGeodeticToGeocentric() 
{
	
	return;


	ref_ptr<orsISpatialReferenceService> pSRService;
	ref_ptr<orsIEllipsoid> pEllipsoid;

	pSRService = ORS_PTR_CAST(orsISpatialReferenceService, getPlatform()->getService( ORS_SERVICE_SRS ) );
	pEllipsoid=pSRService->CreateEllipsoid();

	double X, Y, Z;
	double B, L, H;
	
	double lat, lon;

	B=lat/180*PI;
	L=lon/180*PI;

	double _x, _y, _z;

	pEllipsoid->Geodetic_To_Geocentric(B, L, H, &_x, &_y, &_z);
	
	
	pEllipsoid->Geocentric_To_Geodetic(_x, _y, _z, &X, &Y, &Z);

}

//按时间裁剪轨迹数据
void CCalibProcDoc::OnTrajectoryClipping() 
{
	CFileDialog  trjdlg(TRUE,"trajectory",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"航迹文件(*.trj)|*.trj|(*.*)|*.*||",NULL);
	CFileDialog  lasdlg(TRUE,"LAS",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"las(*.LAS)|*.LAS|(*.*)|*.*||",NULL);
		
	if(trjdlg.DoModal()!=IDOK)
		return;
	
	CString strTrjPath=trjdlg.GetPathName();

	if(lasdlg.DoModal()!=IDOK)
		return;

	CString strLasPath=lasdlg.GetPathName();

	TrajPos *pTrjBuf=NULL;
	int num;

	bool bReturn=CutTrajectory(strLasPath.GetBuffer(128), strTrjPath.GetBuffer(128), &pTrjBuf, num);
	if(!bReturn)
		return;

	CString strClipTrj;
	int	pos;
	
	pos=strTrjPath.ReverseFind('.');
	strClipTrj=strTrjPath.Left(pos);
	strClipTrj+="_clip.trj";

	CTrjReader  trjReader;
	CTrjWriter  trjWriter;
	TrajHdr pHeader;

	
	trjReader.read_header(strTrjPath.GetBuffer(128), &pHeader);
	pHeader.PosCnt=num;
	pHeader.BegTime=pTrjBuf[0].Time;
	pHeader.EndTime=pTrjBuf[num-1].Time;

	trjWriter.open(strClipTrj.GetBuffer(128));
	trjWriter.write_header(&pHeader);
	trjWriter.writeTrjPos(pTrjBuf, num);

	trjWriter.close();


	if(pTrjBuf)
	{
		delete pTrjBuf;
		pTrjBuf=NULL;
	}

}

//将原始POS数据转换到指定的切平面坐标系下
//并计算转换后的切平面点误差
//飞机场：B=36.132288889, L=114.342013889, H=0;   //36°7'56.24''    114°20'31.25''
const double B=36.133888889;		//36°8'2''
const double L=114.341111111;		//114°20'28''
const double H=0;
void CCalibProcDoc::OnTransTPlane() 
{
	CFileDialog  lasdlg(TRUE,"ATN",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"ATN(*.LAS)|*.LAS|(*.*)|*.*||",NULL);

	if(lasdlg.DoModal()!=IDOK)
		return;

	CString  strInput, strOutput;
	CATNReader atnReader;
	CATNWriter atnWriter;
	ATNPoint	point, dstPt;;
	LASheader  header;
	orsLidarGeometry lidGeo;
	double lat0, lon0, h0;
	double xmax, xmin, ymax, ymin, zmax, zmin;
	double pos[3], gc_pos[3];
	POINT3D *pDelta=NULL;
	int	n, i, ptNum;
	double mean_x, mean_y, mean_z;
	double Rn[9];
			
	lidGeo.SetORSPlatform(getPlatform());
	lat0=B/180.0*PI;
	lon0=L/180.0*PI;
	h0=H;
	lidGeo.SetTPlaneCoordinate(lat0, lon0, h0);
	

	strInput=lasdlg.GetPathName();
	n=strInput.ReverseFind('.');
	strOutput=strInput.Left(n);
	strOutput+="_TPlane.LAS";

	atnReader.open(strInput.GetBuffer(128));
	atnWriter.create(strOutput.GetBuffer(128));
	atnReader.read_header(&header);

	pDelta=new POINT3D[atnReader.m_header.number_of_point_records];
	ptNum=atnReader.m_header.number_of_point_records;

	header.x_offset=0;	header.y_offset=0;	header.z_offset=0;
	header.x_scale_factor=0.0001;	header.y_scale_factor=0.0001;	header.z_scale_factor=0.0001;
	atnWriter.write_header(&header);
	xmax=-1.7e+308;	ymax=-1.7e+308;	zmax=-1.7e+308;
	xmin=1.7e+308;	ymin=1.7e+308;	zmin=1.7e+308;
	i=0;
	while(atnReader.read_point(&point))
	{	
		dstPt=point;
		
		//点坐标
		lidGeo.Geographic2TangentPlane(point.y, point.x, point.z, &(dstPt.x), &(dstPt.y), &(dstPt.z));
		
		if(xmax<dstPt.x)
			xmax=dstPt.x;
		else if(xmin>dstPt.x)
			xmin=dstPt.x;

		if(ymax<dstPt.y)
			ymax=dstPt.y;
		else if(ymin>dstPt.y)
			ymin=dstPt.y;

		if(zmax<dstPt.z)
			zmax=dstPt.z;
		else if(zmin>dstPt.z)
			zmin=dstPt.z;
		
		//POS
		lidGeo.POS_Geographic2TangentPlane(point.latitude, point.longitude, point.altitude, point.roll, point.pitch, point.heading, 
			&(dstPt.latitude), &(dstPt.longitude), &(dstPt.altitude), &(dstPt.roll), &(dstPt.pitch), &(dstPt.heading), Rn);
		
		double xt, yt, zt;
		//检查线元素转切平面坐标是否正确
//		lidGeo.

		lidGeo.calLaserPosition_TPlane(dstPt.latitude, dstPt.longitude, dstPt.altitude, 
			dstPt.roll, dstPt.pitch, dstPt.heading, Rn,
			dstPt.range, dstPt.scanAngle, pos, pos+1, pos+2);

		double angle[3], leverArm[3], misalign[3], t[3];
		angle[0]=point.roll;
		angle[1]=point.pitch;
		angle[2]=point.heading;
		
		leverArm[0]=leverArm[1]=leverArm[2]=0;
		misalign[0]=misalign[1]=misalign[2]=0;
		
		t[0]=point.latitude;	t[1]=point.longitude;	t[2]=point.altitude;
		lidGeo.calLaserPosition_WGS84_Geocentric(t, angle, leverArm, misalign, point.range, point.scanAngle, gc_pos);
		
		lidGeo.Geocentric_To_TangentPlane(gc_pos[0], gc_pos[1], gc_pos[2], &xt, &yt, &zt);

		/////////测试地心转切平面
		double xt1, yt1, zt1, xt2, yt2, zt2;
		double xc, yc, zc;
		lidGeo.Geographic2TangentPlane(t[0], t[1], t[2], &xt1, &yt1, &zt1);

		lidGeo.Geodetic_To_WGS84_Geocentric(t[0], t[1], t[2], &xc, &yc, &zc);
		lidGeo.Geocentric_To_TangentPlane(xc, yc, zc, &xt2, &yt2, &zt2);

		
		pDelta[i].X=pos[0]-dstPt.x;
		pDelta[i].Y=pos[1]-dstPt.y;
		pDelta[i].Z=pos[2]-dstPt.z;

		i++;

		dstPt.x=pos[0];
		dstPt.y=pos[1];
		dstPt.z=pos[2];
		atnWriter.write_point(&dstPt);
	}
	
	atnWriter.close();
	atnWriter.open(strOutput.GetBuffer(128));

	header.max_x=xmax;
	header.min_x=xmin;
	header.max_y=ymax;
	header.min_y=ymin;
	header.max_z=zmax;
	header.min_z=zmin;
	atnWriter.write_header(&header);

	atnReader.close();
	atnWriter.close();

	xmax=-1.7e+308;	ymax=-1.7e+308;	zmax=-1.7e+308;
	xmin=1.7e+308;	ymin=1.7e+308;	zmin=1.7e+308;
	//统计误差
	mean_x=mean_y=mean_z=0;
	for(i=0; i<ptNum; i++)
	{//计算点坐标之差

		if(xmax < pDelta[i].X)
			xmax = pDelta[i].X;
		else if(xmin > pDelta[i].X)
			xmin=pDelta[i].X;

		if(ymax < pDelta[i].Y)
			ymax = pDelta[i].Y;
		if(ymin > pDelta[i].Y)
			ymin = pDelta[i].Y;

		if(zmax < pDelta[i].Z)
			zmax = pDelta[i].Z;
		if(zmin > pDelta[i].Z)
			zmin = pDelta[i].Z;
		
		mean_x+=pDelta[i].X;
		mean_y+=pDelta[i].Y;
		mean_z+=pDelta[i].Z;
	}
	//统计均值
	mean_x/=ptNum;
	mean_y/=ptNum;
	mean_z/=ptNum;

	double x_sec, y_sec, z_sec;
	int secNum=256;		//按256分段
	int *x_histogram=NULL, *y_histogram=NULL, *z_histogram=NULL;
	double x_variance, y_variance, z_variance;

	x_sec=(xmax-xmin)/secNum;
	y_sec=(ymax-ymin)/secNum;
	z_sec=(zmax-zmin)/secNum;

	x_histogram=new int[secNum];
	y_histogram=new int[secNum];
	z_histogram=new int[secNum];
	memset(x_histogram, 0, sizeof(int)*secNum);
	memset(y_histogram, 0, sizeof(int)*secNum);
	memset(z_histogram, 0, sizeof(int)*secNum);

	int secID;
	x_variance=y_variance=z_variance=0;
	for(i=0; i<ptNum; i++)
	{
		x_variance+=(pDelta[i].X-mean_x)*(pDelta[i].X-mean_x);
		y_variance+=(pDelta[i].Y-mean_y)*(pDelta[i].Y-mean_y);
		z_variance+=(pDelta[i].Z-mean_z)*(pDelta[i].Z-mean_z);

		//统计直方图
		secID=int((pDelta[i].X-xmin)/x_sec);
		if(secID==secNum)
			secID--;
		x_histogram[secID]++;

		secID=int((pDelta[i].Y-ymin)/y_sec);
		if(secID==secNum)
			secID--;
		y_histogram[secID]++;
		
		secID=int((pDelta[i].Z-zmin)/z_sec);
		if(secID==secNum)
			secID--;
		z_histogram[secID]++;

	}

	//统计方差
	x_variance/=ptNum;
	y_variance/=ptNum;
	z_variance/=ptNum;

	////输出统计值
	TRACE("meanX, meanY, meanZ\n");
	TRACE("%f %f %f\n", mean_x, mean_y, mean_z);
	TRACE("varianceX, varianceY, varianceZ\n");
	TRACE("%f %f %f\n", x_variance, y_variance, z_variance);

	TRACE("x_histogram, y_histogram, z_histogram\n");
	double dx, dy, dz;
	for(i=0; i<secNum; i++)
	{
		dx=xmin+i*x_sec;
		dy=ymin+i*y_sec;
		dz=zmin+i*z_sec;
		TRACE("%.2f %d %.2f %d %.2f %d\n", dx, x_histogram[i], dy, y_histogram[i], dz, z_histogram[i]);
	}

	if(pDelta)
	{
		delete pDelta;
		pDelta=NULL;
	}
	if(x_histogram)
	{
		delete x_histogram;
		x_histogram=NULL;
	}
	if(y_histogram)
	{
		delete y_histogram;
		y_histogram=NULL;
	}
	if(z_histogram)
	{
		delete z_histogram;
		z_histogram=NULL;
	}
}

//切平面坐标转投影坐标
void CCalibProcDoc::OnCalPosTPlane() 
{
	CFileDialog  prjdlg(TRUE,"prj",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"prj(*.prj)|*.prj|(*.*)|*.*||",NULL);
//	CFileDialog  dstdlg(TRUE,"prj",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"prj(*.prj)|*.prj|(*.*)|*.*||",NULL);

	CFileDialog  atndlg(TRUE,"Attune",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"atn文件(*.las)|*.las|(*.*)|*.*||",NULL);	
	CATNReader atnreader;

	if(prjdlg.DoModal()!=IDOK)
		return;
	CString strOrg=prjdlg.GetPathName();

	if(prjdlg.DoModal()!=IDOK)
		return;
	CString strDst=prjdlg.GetPathName();

	orsLidarGeometry lidGeo;
	double lat0, lon0, h0;

	lidGeo.SetORSPlatform(getPlatform());
	lat0=B/180.0*PI;
	lon0=L/180.0*PI;
	h0=H;
	lidGeo.SetTPlaneCoordinate(lat0, lon0, h0);

	lidGeo.SetProjectionFromWkt(strOrg.GetBuffer(128), strDst.GetBuffer(128));

	if(atndlg.DoModal()!=IDOK)
		return;

	CString stratn=atndlg.GetPathName();

	atnreader.open(stratn.GetBuffer(128));

	LASwriter  lasWriter;
	ATNPoint point;

	CString strLasFile;
	int i;
	i=stratn.ReverseFind('_');
	strLasFile=stratn.Left(i);
	strLasFile+="_UTM.LAS";

	FILE *fp=0;
	fp=fopen(strLasFile.GetBuffer(128), "wb");
	if(fp==0)
		return;
	
	lasWriter.open(fp, NULL);

	double Xp, Yp, Zp;
	while(atnreader.read_point(&point))
	{
		lidGeo.TPlane_To_Projection(point.x, point.y, point.z, &Xp, &Yp, &Zp);

		lasWriter.write_point(Xp, Yp, Zp, point.intensity, point.return_number, point.number_of_returns_of_given_pulse,
			point.scan_direction_flag, point.edge_of_flight_line, point.classification, point.scan_angle_rank,
			point.user_data, point.point_source_ID, point.gpstime);
	}

	lasWriter.close();
	fclose(fp);
}


//从ATN数据中提取LAS1.1，不做数据转换
void CCalibProcDoc::OnATNToLAS11() 
{
// 	CFileDialog  atndlg(TRUE,"ATN",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"ATN(*.LAS)|*.LAS|(*.*)|*.*||",NULL);
// 	CFileDialog  lasdlg(FALSE,"导出解算后的LAS文件",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"LAS文件(*.LAS)|*.LAS|(*.*)|*.*||",NULL);
// 	
// 	if(atndlg.DoModal()!=IDOK)
// 		return;
// 
// 	if(lasdlg.DoModal()!=IDOK)
// 		return;

	CSetMisalignDlg setDlg;
	double roll, pitch, heading, tx, ty, tz, sa0, sa_ceof, drho;
	double dXs, dYs, dZs;
	BOOL	bMisalign6Param;
	BOOL	bTranslationParam;
	BOOL bScanAngle0;

	bMisalign6Param=setDlg.m_bMisalign6Param;
	bTranslationParam=setDlg.m_bTranslationParam;

	if(setDlg.DoModal()==IDOK)
	{
		roll=setDlg.m_roll;
		pitch=setDlg.m_pitch;
		heading=setDlg.m_heading;
		tx=setDlg.m_tx;
		ty=setDlg.m_ty;
		tz=setDlg.m_tz;
		dXs=setDlg.m_dTranslationX;
		dYs=setDlg.m_dTranslationY;
		dZs=setDlg.m_dTranslationZ;
		sa0=setDlg.m_sa0;
		sa_ceof=setDlg.m_sa_ceof;
		drho = setDlg.m_drho;

		bMisalign6Param=setDlg.m_bMisalign6Param;
		bTranslationParam=setDlg.m_bTranslationParam;
		bScanAngle0=setDlg.m_bScanAngle0;
	}
	else
	{
		roll=0;
		pitch=0;
		heading=0;
		tx=0;
		ty=0;
		tz=0;
		dXs=0;	
		dYs=0;	
		dZs=0;
		sa0=0;
		sa_ceof=0;
	}

	CalibParam_Type param_type;
	param_type = Calib_rph_drange;
	oriCalibParam oriParam;
	memset(&oriParam, 0, sizeof(oriCalibParam));
	struct  oriCalibParam
	{
		double lever_arm[3];	//偏心分量
		double misalign_angle[3];	//偏心角
		double sa0;				//扫描角误差
		double sac;				//扫描角线性改正系数
		double semiXYZ[3];		//系统平移量
		double drange;			//测距误差
	};

	oriParam.lever_arm[0] = tx;
	oriParam.lever_arm[1] = ty;
	oriParam.lever_arm[2] = tz;
	oriParam.misalign_angle[0] = roll;
	oriParam.misalign_angle[1] = pitch;
	oriParam.misalign_angle[2] = heading;
	oriParam.sa0 = sa0;
	oriParam.sac = sa_ceof;
	oriParam.semiXYZ[0] = dXs;
	oriParam.semiXYZ[1] = dYs;
	oriParam.semiXYZ[2] = dZs;
	oriParam.drange = drho;


	double calib_param[8];
	memset(calib_param, 0, sizeof(double)*8);
	calib_param[0] = roll;
	calib_param[1] = pitch;
	calib_param[2] = heading;
	calib_param[3] = drho;


	CString temp;
	CArray<CString,CString&> pATNFile;
	POSITION pos;
	CFileDialog	Dlg(TRUE,"切平面LAS",NULL,OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT,"ATN_TP(*.LAS)|*.LAS|(*.*)|*.*||",NULL);
	TCHAR *pszFile = new TCHAR[MAX_PATH*MAX_PATH];
	
	
	Dlg.m_ofn.lpstrFilter="ATN_TP(*.LAS)\0*.las\0All Files (*.*)\0*.*\0";
	Dlg.m_ofn.lpstrTitle = "Import ATN_TP Files";
	
	memset(pszFile,0,sizeof(TCHAR)*MAX_PATH*MAX_PATH);
	Dlg.m_ofn.lpstrFile = pszFile;
	Dlg.m_ofn.nMaxFile = MAX_PATH*MAX_PATH; 
	
	Dlg.m_ofn.Flags = OFN_ALLOWMULTISELECT|OFN_EXPLORER|OFN_ENABLEHOOK;
	int i=0;
	if(Dlg.DoModal()==IDOK)
	{
		pos=Dlg.GetStartPosition();
		while(pos!=NULL){	pATNFile.Add(Dlg.GetNextPathName(pos));	i++;}
		temp.Format("%d",i);
		temp="一共打开了"+temp+"个文件";
		AfxMessageBox(temp);
	}
	else 
		return;
	
	CString strFolder;
	if(!_SelectFolderDialog("设置输出目录...", &strFolder, NULL))
		return;

	CATN_TPReader	atnReader;
	LASwriter_fast  lasWriter;
	LASheader  header;
	
	ATNTP_Point pt;
	CString strName;

	ref_ptr<orsIProcessMsgBar> process;
	process = ORS_CREATE_OBJECT( orsIProcessMsgBar, "ors.process.bar.mfc");
	process->InitProgressBar("数据转换...", "wait...", pATNFile.GetSize());
	
	CString strFileName;
	CString	strInput, strOutput;
	for(i=0; i<pATNFile.GetSize(); i++)
	{
		strInput=pATNFile[i];
		strFileName=_ExtractFileName(strInput);
		strOutput=strFolder+"\\"+strFileName+".LAS";
		
		atnReader.open(strInput.GetBuffer(128));
		atnReader.read_header(&header);
		
		header.point_data_format=1;
		
		header.point_data_record_length=28;

		lasWriter.create(strOutput.GetBuffer(128), &header);
//		lasWriter.set_offset_scalefactor(x_offset, y_offset, z_offset, x_scale_factor, y_scale_factor, z_scale_factor);
		lasWriter.write_header();
		
		double xmax, ymax, zmax, xmin, ymin, zmin;
		
		xmax=-1.7e+308;	ymax=-1.7e+308;	zmax=-1.7e+308;
		xmin=1.7e+308;	ymin=1.7e+308;	zmin=1.7e+308;
		while (atnReader.read_point(&pt))
		{
// 			if(bMisalign6Param)
// 				_calLaserPosition(&pt, 1, tx, ty, tz, roll, pitch, heading,sa0, sa_ceof);
// 			else
// 				_calLaserPosition(&pt, 1, 0, 0, 0, roll, pitch, heading,sa0, sa_ceof);
			
			_calLaserPosition(&pt, 1, oriParam, calib_param, param_type);

			if(bTranslationParam)
			{
				pt.x+=dXs;
				pt.y+=dYs;
				pt.z+=dZs;
			}

			if(xmax<pt.x)
				xmax=pt.x;
			if(xmin>pt.x)
				xmin=pt.x;
			
			if(ymax<pt.y)
				ymax=pt.y;
			if(ymin>pt.y)
				ymin=pt.y;
			
			if(zmax<pt.z)
				zmax=pt.z;
			if(zmin>pt.z)
				zmin=pt.z;
			
			lasWriter.write_point(pt.x, pt.y, pt.z, pt.intensity, pt.return_number, pt.number_of_returns_of_given_pulse,
				pt.scan_direction_flag, pt.edge_of_flight_line, pt.classification, pt.scan_angle_rank, pt.user_data,
				pt.point_source_ID, pt.gpstime);
			
		}
		header.min_x=xmin-1;	header.max_x=xmax+1;
		header.min_y=ymin-1;	header.max_y=ymax+1;
		header.min_z=zmin-1;	header.max_z=zmax+1;
		
//		lasWriter.write_header(&header);
		atnReader.close();
		lasWriter.close();

		header.min_x=xmin-1;	header.max_x=xmax+1;
		header.min_y=ymin-1;	header.max_y=ymax+1;
		header.min_z=zmin-1;	header.max_z=zmax+1;

		lasWriter.open(strOutput.GetBuffer(128), &header);
		lasWriter.close();
		process->SetPos(i+1);
	}

	if(pszFile)	delete pszFile;		pszFile=NULL;
}

//批量进行切平面转换


void CCalibProcDoc::OnATN2TPlane()
{
// 	CFileDialog  indlg(TRUE,"ATN",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"ATN(*.LAS)|*.LAS|(*.*)|*.*||",NULL);
// 	CFileDialog  outdlg(FALSE,"ATN",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"ATN(*.LAS)|*.LAS|(*.*)|*.*||",NULL);
 	CString	strInput, strOutput;
	orsLidarGeometry lidGeo;
	CATNReader reader1;
	CATN_TPReader reader2;
	LASheader header1, header2;
	ATNPoint pt1;
	ATNTP_Point pt2;

	CString temp;
	CArray<CString,CString&> pATNFile;
	int i=0;
	POSITION pos;
	CFileDialog	Dlg(TRUE,"LAS",NULL,OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT,"ATN(*.LAS)|*.LAS|(*.*)|*.*||",NULL);
	TCHAR *pszFile = new TCHAR[MAX_PATH*MAX_PATH];
	
		
	Dlg.m_ofn.lpstrFilter="ATN(*.LAS)\0*.las\0All Files (*.*)\0*.*\0";
	Dlg.m_ofn.lpstrTitle = "Import ATN Files";
	
	memset(pszFile,0,sizeof(TCHAR)*MAX_PATH*MAX_PATH);
	Dlg.m_ofn.lpstrFile = pszFile;
	Dlg.m_ofn.nMaxFile = MAX_PATH*MAX_PATH; 
	
	Dlg.m_ofn.Flags = OFN_ALLOWMULTISELECT|OFN_EXPLORER|OFN_ENABLEHOOK;
	if(Dlg.DoModal()==IDOK)
	{
		pos=Dlg.GetStartPosition();
		while(pos!=NULL){	pATNFile.Add(Dlg.GetNextPathName(pos));	i++;}
		temp.Format("%d",i);
		temp="一共打开了"+temp+"个文件";
		AfxMessageBox(temp);
	}
	
	CString strFolder;
	if(!_SelectFolderDialog("设置输出目录...", &strFolder, NULL))
		return;

	lidGeo.SetORSPlatform(getPlatform());
	lidGeo.SetTPlaneCoordinate();

	ref_ptr<orsIProcessMsgBar> process;
	process = ORS_CREATE_OBJECT( orsIProcessMsgBar, "ors.process.bar.mfc");
	process->InitProgressBar("数据转换...", "wait...", pATNFile.GetSize());
	CString strFileName;
	for(i=0; i<pATNFile.GetSize(); i++)
	{
		strInput=pATNFile[i];
		strFileName=_ExtractFileName(strInput);
		strOutput=strFolder+"\\"+strFileName+"_TP.LAS";
		
		//逐条带转换   
		lidGeo.RawATN2TPlane(strInput.GetBuffer(128), strOutput.GetBuffer(128));

		/*  测试安置角
		CATNReader reader;
		reader.open(strInput.GetBuffer(256));
		ATNPoint orgPt; 
		ATNTP_Point dstPt;
		double X_org, Y_org, Z_org;
		double X_dst, Y_dst, Z_dst;
		double Xs, Ys, Zs;
		double phi, omega, kappa;
		double R[9];
		double misalign[3];
		////////////////////出厂标定值/////////////////////////
		misalign[0]=-0.00576609;
		misalign[1]=0.00258278;
		misalign[2]=-0.00138682;
		///////////////////////////////////////////////////////

		while (reader.read_point(&orgPt))
		{
			lidGeo.Geographic2TangentPlane(orgPt.y, orgPt.x, orgPt.z, &X_org, &Y_org, &Z_org);

			lidGeo.POS_Geographic2TangentPlane(orgPt.latitude, orgPt.longitude, orgPt.altitude, 
				orgPt.roll, orgPt.pitch, orgPt.heading, 
				&Xs, &Ys, &Zs, &phi, &omega, &kappa, R);
			
			dstPt.Xs=Xs;
			dstPt.Ys=Ys;
			dstPt.Zs=Zs;
			dstPt.phi=phi;
			dstPt.omega=omega;
			dstPt.kappa=kappa;
			
			lidGeo.calLaserPosition_TPlane(Xs, Ys, Zs, phi, omega, kappa, R, 
							orgPt.range, orgPt.scanAngle, &(dstPt.x), &(dstPt.y), &(dstPt.z),
							misalign);

			lidGeo.calLaserPosition_TPlane(Xs, Ys, Zs, phi, omega, kappa, R, 
				orgPt.range, orgPt.scanAngle, &(X_dst), &(Y_dst), &(Z_dst));
		}*/

		process->SetPos(i+1);
	}
	


// 	reader1.open("G:\\Data\\HYSJ\\LIDAR-raw-data\\anyang\\Calibrate\\ATN\\LDR080424_032057_ATN_TPlane.LAS");
// 	reader2.open(strOutput.GetBuffer(128));
// 	
// 	reader1.read_header(&header1);
// 	reader2.read_header(&header2);
// 
// 	while (reader1.read_point(&pt1))
// 	{
// 		reader2.read_point(&pt2);
// 	}
// 
// 	reader1.close();
// 	reader2.close();
	if(pszFile)	delete pszFile;		pszFile=NULL;
}


//对atn文件建立索引，加快查找速度
#include "..\LidCalib\ATNPointIndex.h"
void CCalibProcDoc::OnCreateATNIdx() 
{
	CString temp;
	CArray<CString,CString&> pATNFile;
	int i=0;
	POSITION pos;
	CFileDialog	Dlg(TRUE,"LAS",NULL,OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT,"ATN(*.LAS)|*.LAS|(*.*)|*.*||",NULL);
	TCHAR *pszFile = new TCHAR[MAX_PATH*MAX_PATH];
	
	
	Dlg.m_ofn.lpstrFilter="ATN(*.LAS)\0*.las\0All Files (*.*)\0*.*\0";
	Dlg.m_ofn.lpstrTitle = "Import ATN Files";
	
	memset(pszFile,0,sizeof(TCHAR)*MAX_PATH*MAX_PATH);
	Dlg.m_ofn.lpstrFile = pszFile;
	Dlg.m_ofn.nMaxFile = MAX_PATH*MAX_PATH; 
	
	Dlg.m_ofn.Flags = OFN_ALLOWMULTISELECT|OFN_EXPLORER|OFN_ENABLEHOOK;
	if(Dlg.DoModal()==IDOK)
	{
		pos=Dlg.GetStartPosition();
		while(pos!=NULL){	pATNFile.Add(Dlg.GetNextPathName(pos));	i++;}
		temp.Format("%d",i);
		temp="一共打开了"+temp+"个文件";
		AfxMessageBox(temp);
	}
	
	CString strFolder;
	if(!_SelectFolderDialog("设置输出目录...", &strFolder, NULL))
		return;
	
	CATNPtIndex		index;
	
	ref_ptr<orsIProcessMsgBar> process;
	process = ORS_CREATE_OBJECT( orsIProcessMsgBar, "ors.process.bar.mfc");
	process->InitProgressBar("数据转换...", "wait...", pATNFile.GetSize());
	
	CString strFileName;
	CString	strInput, strOutput;
	for(i=0; i<pATNFile.GetSize(); i++)
	{
		strInput=pATNFile[i];
		strFileName=_ExtractFileName(strInput);
		strOutput=strFolder+"\\"+strFileName+".idx";
		
		index.CreateQTIdx(strInput.GetBuffer(256), strOutput.GetBuffer(256));
		
		process->SetPos(i+1);
	}
	
	if(pszFile)	delete pszFile;		pszFile=NULL;
}



#include "..\LidCalib\VP_Model.h"
double _cal_z_IDW(double x, double y, std::vector<ATNTP_Point> *pLidPoints);

int LidMC_VP_ascending_tieID(const void *p1, const void  *p2)
{
	LidMC_VP *elem1, *elem2;
	elem1=(LidMC_VP *)p1;
	elem2=(LidMC_VP *)p2;

	if(elem1->tieID < elem2->tieID)
		return -1;
	else 
		if(elem1->tieID > elem2->tieID)
			return 1;
		else
			return 0;
}


//按IDW方法计算虚拟观测值:  距离反比加权法
double _cal_z_IDW(double x, double y, std::vector<ATNTP_Point> *pLidPoints)
{
	int i; 
	double dis2;
	double weight_sum;		//权求和
	double z;
	int num;
	ATNTP_Point point;
	double *pWeight=NULL;

	num=pLidPoints->size();
	pWeight=new double[num];

	weight_sum=0;
	for(i=0; i<num; i++)
	{
		point=(*pLidPoints)[i];

		dis2=(point.x-x)*(point.x-x)+(point.y-y)*(point.y-y);
		
		pWeight[i]=1.0/dis2;
		weight_sum+=pWeight[i];
		
	}
	
	z=0;
	for(i=0; i<num; i++)
	{
		point=(*pLidPoints)[i];

		z+=pWeight[i]*point.z/weight_sum;
	}

	if(pWeight)	delete pWeight;	pWeight=NULL;
	return z;
}

#include "bundler\matrix.h"
#include "levmar.h"
struct myData
{
	double *p;
};

void linefit(double *p, double *x, int m, int n, void *data)
{
	register int i;
	struct myData *dptr;
	double *q;
	dptr=(struct myData *)data; 
	
	q=dptr->p;
	
	for(i=0; i<n; i++)
	{
		x[i]=p[0]*q[i]+p[1];
	}
}

void jaclinefit(double *p, double *jac, int m, int n, void *data)
{
	register int j=0;
	struct myData *dptr;
	double *q;
	dptr=(struct myData *)data; 
	
	q=dptr->p;
	for(j=0; j<n; j++)
	{
		jac[j*2]=q[j];
		jac[j*2+1]=1;
	}
	
}

void condition_adjustment();
//测试levmar库
void CCalibProcDoc::OnTESTlevmar() 
{
	double x[5];
	double y[5];

	x[0]=0;		y[0]=0;
	x[1]=1;		y[1]=1;
	x[2]=-1;	y[2]=-1.1;
	x[3]=1.9;	y[3]=2;
	x[4]=-2;	y[4]=-1.9;

	double p[2];
	double pA[10];

	int i;
	for(i=0; i<5; i++)
	{
		pA[i*2]=x[i];
		pA[i*2+1]=1;
	}

	//间接平差
	dgelsy_driver(pA, y, p, 5, 2, 1);
	TRACE("dgelsy_driver: a=%.9f b=%.9f\n", p[0], p[1]);

	double Param[7];
	double px[5];
	double py[5];
	double *q;
	struct myData data;

	int m, n;
	double opts[LM_OPTS_SZ], info[LM_INFO_SZ];

	opts[0]=LM_INIT_MU; opts[1]=1E-15; opts[2]=1E-15; opts[3]=1E-20;
	opts[4]= LM_DIFF_DELTA;

	px[0]=0;		py[0]=0;
	px[1]=1;		py[1]=1;
	px[2]=-1;		py[2]=-1.1;
	px[3]=1.9;		py[3]=2;
	px[4]=-2;		py[4]=-1.9;
	   
	data.p=px;
	m=2; n=5;
	Param[0]=0;	Param[1]=0; 
	q=Param+2;
	for(i=0; i<5; i++)
		q[i]=px[i];
	  
	//阻尼最小二乘迭代解
	int ret=dlevmar_dif(linefit, Param, py, m, n, 1000, opts, info, NULL, NULL, &data);

	double v[5];
	double rms=0;
	for(i=0; i<5; i++)
	{
		v[i]=y[i]-p[0]*x[i]-p[1];
		rms=v[i]*v[i];
	}
	rms/=5;

	TRACE("rms=%.9f\n", rms);

	condition_adjustment();
	   
//  ret=dlevmar_der(linefit, jaclinefit, Param, py, m, n, 1000, opts, info, NULL, NULL, &data);
}


//AV+Bx~+W=0
void condition_adjustment()
{
	int i, j;
	int m, n, k;	
	double x[15];
	double y[15];
	double *A, *B, *W;
	double p[3], pX[3];

	double AT[450], BT[45];
	double Naa[225], Naa_Inv[225];
	double Nbb[9], Nbb_Inv[9];
	
	
	x[0]=1;			y[0]=0;
	x[1]=0;			y[1]=1.1;
	x[2]=-1.1;		y[2]=0;
	x[3]=0.7071;	y[3]=0.7071;
	x[4]=0;			y[4]=-0.9;

// 	x[0]=2.002;	y[0]=0.597;
// 	x[1]=0.750; y[1]=0.102;
// 	x[2]=0.356; y[2]=0.308;
// 	x[3]=1.039; y[3]=2.031;
// 	x[4]=0.250; y[4]=0.500;
// 	x[5]=0.250; y[5]=1.630;
// 	x[6]=1.587; y[6]=1.903;
// 	x[7]=1.032; y[7]=0.013;
// 	x[8]=2.002; y[8]=1.004;
// 	x[9]=1.945; y[9]=1.520;
// 	x[10]=0.535; y[10]=1.852;
// 	x[11]=0.007; y[11]=1.001;
// 	x[12]=0.513; y[12]=0.232;
// 	x[13]=1.832; y[13]=0.321;
// 	x[14]=1.546; y[14]=0.143;

	m=5; //15;	//条件方程个数
	n=10;   //30;	//观测值个数
	k=3;	//未知数个数

	A=new double[m*n];
	B=new double[m*k];
	W=new double[m];
	
	memset(A, 0, sizeof(double)*m*n);
	p[0]=1.002;	p[1]=1.003;	p[2]=0.998;
	for(i=0; i<m; i++)
	{
		A[i*n+i*2]=(x[i]-p[0])*2;
		A[i*n+i*2+1]=(y[i]-p[1])*2;

		B[i*k]=-2*(x[i]-p[0]);
		B[i*k+1]=-2*(y[i]-p[1]);
		B[i*k+2]=-2*p[2];

		W[i]=(x[i]-p[0])*(x[i]-p[0])+(y[i]-p[1])*(y[i]-p[1])-p[2]*p[2];
	}

	orsMatrixD MA(m,n);
	orsMatrixD MB(m,k);
	orsVectorD MW(m);
	for(i=0; i<m; i++)
	{
		MW[i]=W[i];
		MB[i][0]=B[i*k];	MB[i][1]=B[i*k+1];	MB[i][2]=B[i*k+2];
		for(j=0; j<n; j++)
		{
			MA[i][j]=A[i*n+j];
		}
	}

	ref_ptr<orsIMatrixService> pMatService = ( orsIMatrixService *)getPlatform()->getService( ORS_SERVICE_MATRIX );

	orsMatrixD MAT(n,m), MBT(k,m), MBTNaa(k, m), Mq(k,m);
	orsMatrixD MNaa(m, m), MNaaInv(m, m);
	orsMatrixD MNbb(k, k), MNbbInv(k, k);
	orsVectorD Mp(k);

	pMatService->MatrixTranspose(MA, MAT);
	pMatService->MatrixMultiplyMatrix(MA, MAT, MNaa);
	pMatService->MatrixInverse(MNaa);
	
	pMatService->MatrixTranspose(MB, MBT);
	pMatService->MatrixMultiplyMatrix(MBT, MNaa, MBTNaa);
	pMatService->MatrixMultiplyMatrix(MBTNaa, MB, MNbb);
	pMatService->MatrixInverse(MNbb);

	pMatService->MatrixMultiplyMatrix(MNbb, MBT, MBTNaa);
	pMatService->MatrixMultiplyMatrix(MBTNaa, MNaa, Mq);
	pMatService->MatrixMultiplyVector(Mq, MW, Mp);

	TRACE("a=%.9f b=%.9f c=%.9f\n", Mp[0], Mp[1], Mp[2]);
	
	matrix_transpose(m, n, A, AT);
	matrix_product(m,n,n,m,A,AT,Naa);
	matrix_invert(m, Naa, Naa_Inv);

	matrix_transpose(m, k, B, BT);
			
	double tmp[45], q[45];
	matrix_product(k,m,m,m,BT,Naa_Inv,tmp);
	matrix_product(k, m, m, k, tmp, B, Nbb);
	matrix_invert(k, Nbb, Nbb_Inv);

	matrix_product(k, k, k, m, Nbb_Inv, BT, tmp);
	matrix_product(k, m, m, m, tmp, Naa_Inv, q);
	matrix_product(k, m, m, 1, q, W, pX);

	pX[0]*=-1;
	pX[1]*=-1;
	pX[2]*=-1;

	p[0]+=pX[0];
	p[1]+=pX[1];
	p[2]+=pX[2];

	double v[30];
	double Bx[15];
	double ATNaaInv[450];

	matrix_product(m,k,k,1,B,pX,Bx);
	for(i=0; i<m; i++)
	{
		Bx[i]=W[i]+Bx[i];
	}
	matrix_product(n,m,m,m,AT,Naa_Inv,ATNaaInv);
	matrix_product(n,m,m,1,ATNaaInv,Bx,v);

	double rms=0;
	for(i=0; i<n; i++)
	{
		rms+=v[i]*v[i];
	}
	rms/=(n-4+3);
	TRACE("rms=%.9f\n", rms);
	delete A;
	delete B;
	delete W;
}

#include "..\LidCalib\PlaneConstraint_Model.h"
int LidMC_Plane_ascending_tieID(const void *p1, const void  *p2)
{
	LidMC_Plane *elem1, *elem2;
	elem1=(LidMC_Plane *)p1;
	elem2=(LidMC_Plane *)p2;
	
	if(elem1->tieID < elem2->tieID)
		return -1;
	else 
		if(elem1->tieID > elem2->tieID)
			return 1;
		else
			return 0;
}


void CCalibProcDoc::OnCoplanarAdjustment() 
{
	CFileDialog dlg(TRUE,"apj",NULL,OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT,"align(*.apj)|*.apj|(*.*)|*.*||",NULL);
	CAlignPrj   algPrj;
	CArray<TieObjChain*, TieObjChain*> *pTieList=NULL;
	CArray<Align_LidLine, Align_LidLine> *pLidLines=NULL;
//	orsVector<orsIImageGeometry*> 	imgGeoList;

	int i, j, k;
	TieObjChain *pChain=NULL;
	Align_LidLine lidLine;
	TieObject *pObj=NULL;
	CATNPtIndex	index;
	int srcID;
//	std::vector<ATNTP_Point> ATNpt_List;
	LidMC_Plane *pLidPlanes=NULL;
	int nPatch, iPatch;
//	double x0, y0;
	POINT3D *pt3D;
	int ptNum;
	int nLine, nTies;
	long *pTieLut=NULL;	//连接点的查找表矩阵
//	std::vector<ATNTP_Point> *pLidPoints;

	if(dlg.DoModal()!=IDOK)
		return;

	CString strPath=dlg.GetPathName();

	algPrj.Open(strPath.GetBuffer(256));

	pTieList=algPrj.GetTieObjList();
	pLidLines=algPrj.GetLidList();
	
	nLine=pLidLines->GetSize();
	nTies=pTieList->GetSize();

	pTieLut=new long[nTies*nLine];	//记录每个连接点是否在各条带上可见

	for(i=0; i<nTies*nLine; i++)
	{//不存在连接点时为1
		pTieLut[i]=-1;
	}

	nPatch=0;
	for(i=0; i<pTieList->GetSize(); i++)
	{//统计虚拟连接点数
		pChain=pTieList->GetAt(i);  //遍历每一个点链
		
//		if(pChain->TieID==6) continue;
		ASSERT(pChain->objNum==pChain->pChain->GetSize());
		nPatch+=pChain->pChain->GetSize();	

		for(j=0; j<pChain->objNum; j++)
		{
			pObj=pChain->pChain->GetAt(j);

			if(pObj->sourceType==ST_LiDAR)
			{//记录在链表中的序号
				srcID=pObj->sourceID-1;
				pTieLut[i*nLine+srcID]=j;
			}
		}
	}

	pLidPlanes=new LidMC_Plane[nPatch];
	iPatch=0;
	int tieID=0;
	//按条带顺序初始化连接点，保证不重复用索引
	for(i=0; i<nLine; i++)
	{
		lidLine=pLidLines->GetAt(i);
		if(index.Open(lidLine.AtnName.GetBuffer(256))==false)
			continue;

		tieID=0;
		for(j=0; j<nTies; j++)
		{//遍历当前条带所有连接点
			k=pTieLut[j*nLine+i];
			if(k<0)
				continue;

			pChain=pTieList->GetAt(j);
//			if(pChain->TieID==6) continue;
			pObj=pChain->pChain->GetAt(k);

			ptNum=((TiePatch*)pObj)->ptNum;
			pt3D=((TiePatch*)pObj)->pt3D;
			
			pLidPlanes[iPatch].pLidPoints=new std::vector<ATNTP_Point>;
			pLidPlanes[iPatch].tieID=pChain->TieID-1;
			tieID++;
			
			index.GetPoints(pt3D, ptNum, pLidPlanes[iPatch].pLidPoints);

			//将提取的激光点和包围盒导出，检查点位是否正确
// 			{
// 				TRACE("tie:%d   line:%d\n", j, i);
// 				for(k=0; k<ptNum; k++)
// 				{
// 					TRACE("%.3f %.3f\n", pt3D[k].X, pt3D[k].Y);
// 				}
// 				TRACE("inside points\n");
// 				TRACE("ptNum:%d\n", pLidPlanes[iPatch].pLidPoints->size());
// 				
// 				pLidPoints=pLidPlanes[iPatch].pLidPoints;
// 				for(k=0; k<pLidPlanes[iPatch].pLidPoints->size(); k++)
// 				{
// 					TRACE("%.3f %.3f %.3f\n", (*pLidPoints)[k].x, (*pLidPoints)[k].y, (*pLidPoints)[k].z);
// 				}
// 				
// 			}

			iPatch++;
		}

	}
	
	//对pLidPlanes按连接点ID排序
	qsort(pLidPlanes, nPatch, sizeof(LidMC_Plane), LidMC_Plane_ascending_tieID);
		
	
	double pX[6];
	memset(pX, 0, sizeof(double)*6);

	pX[3]=/*-0.00576609*/0;	pX[4]=/*0.00258278*/0;	pX[5]=/*-0.00138682*/0;
	
	//用初值重新计算点集
	
	CPlaneCalib calib;

	calib.LidMC_Misalign_plane(nPatch, pLidPlanes, pX, 6);

	if(pLidPlanes)	delete[] pLidPlanes;	pLidPlanes=NULL;
	if(pTieLut)		delete[] pTieLut;		pTieLut=NULL;	
}


void CCalibProcDoc::OnSimulateRectangle() 
{
	POINT2D *pData1=NULL, *pData2=NULL;
//	int ptNum1=50, ptNum2;
	double x, y;
//	double sq_r1, sq_r2;
//	POINT2D center;
	int i;
	CATNPtIndex	index;
	
	CFileDialog  dlg(TRUE,"LAS文件",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"LAS文件(*.LAS)|*.LAS|(*.*)|*.*||",NULL);	
		
	pData1=new POINT2D[10000];
	pData2=new POINT2D[10000];
	
	if(dlg.DoModal()!=IDOK)
		return;
	
	CString strName=dlg.GetPathName();
	
	if(index.Open(strName.GetBuffer(128))==false)
	{
		AfxMessageBox("未找到索引!");
		return;
	}

	std::vector<ATNTP_Point> ptDataVec;
	double xc, yc;		//矩形圆心
	double r;
	double gridSize=2, len;
	POINT3D pRect[4];
	POINT2D pSubRect1[4], pSubRect2[4], pSubRect3[4], pSubRect4[4];

/*		_________
		| 2	| 1	|	
		|___|___|
		| 3	| 4	|
		|___|___|
*/
	len=0.5*gridSize;
	r=sqrt(2.0)*len;
	

	int ptNum;
	//////////////////////////////////////////////////////////////////////////
	//通过修改xc，yc和angle值来进行模拟
	double angle=90;   //0~90
	double theta, alpha;

	xc=301;
	yc=416;
	theta=angle/180*PI;

	pRect[0].X=xc+r*cos(theta);
	pRect[0].Y=yc+r*sin(theta);
	pRect[1].X=xc+r*cos(PI/2+theta);
	pRect[1].Y=yc+r*sin(PI/2+theta);
	pRect[2].X=xc+r*cos(PI+theta);
	pRect[2].Y=yc+r*sin(PI+theta);
	pRect[3].X=xc+r*cos(PI*3/2+theta);
	pRect[3].Y=yc+r*sin(PI*3/2+theta);

	index.GetPoints(pRect, 4, &ptDataVec);

	alpha=theta-PI/4;

	pSubRect1[0].x=xc+len*cos(alpha);
	pSubRect1[0].y=yc+len*sin(alpha);
	pSubRect1[1].x=pRect[0].X;
	pSubRect1[1].y=pRect[0].Y;
	pSubRect1[2].x=xc+len*cos(alpha+PI/2);
	pSubRect1[2].y=yc+len*sin(alpha+PI/2);
	pSubRect1[3].x=xc;
	pSubRect1[3].y=yc;

	pSubRect2[0].x=xc;
	pSubRect2[0].y=yc;
	pSubRect2[1].x=xc+len*cos(alpha+PI/2);
	pSubRect2[1].y=yc+len*sin(alpha+PI/2);
	pSubRect2[2].x=pRect[1].X;
	pSubRect2[2].y=pRect[1].Y;
	pSubRect2[3].x=xc+len*cos(alpha+PI);
	pSubRect2[3].y=yc+len*sin(alpha+PI);

	pSubRect3[0].x=xc+len*cos(alpha+PI*3/2);
	pSubRect3[0].y=yc+len*sin(alpha+PI*3/2);
	pSubRect3[1].x=xc;
	pSubRect3[1].y=yc;
	pSubRect3[2].x=xc+len*cos(alpha+PI);
	pSubRect3[2].y=yc+len*sin(alpha+PI);
	pSubRect3[3].x=pRect[2].X;
	pSubRect3[3].y=pRect[2].Y;

	pSubRect4[0].x=pRect[3].X;
	pSubRect4[0].y=pRect[3].Y;
	pSubRect4[1].x=xc+len*cos(alpha);
	pSubRect4[1].y=yc+len*sin(alpha);
	pSubRect4[2].x=xc;
	pSubRect4[2].y=yc;
	pSubRect4[3].x=xc+len*cos(alpha+PI*3/2);
	pSubRect4[3].y=xc+len*sin(alpha+PI*3/2);

	
	CPolygonFilling polygon1(pSubRect1, 4), polygon2(pSubRect2, 4), polygon3(pSubRect3, 4), polygon4(pSubRect4, 4);
	
	ptNum=ptDataVec.size();
	for(i=0; i<ptNum; i++)
	{
		x=ptDataVec[i].x;
		y=ptDataVec[i].y;

		if(polygon1.PointInPolygen(x, y))
		{
			ptDataVec[i].intensity=255;
		}
		else if(polygon2.PointInPolygen(x, y))
		{
			ptDataVec[i].intensity=1;
		}
		else if(polygon3.PointInPolygen(x, y))
		{
			ptDataVec[i].intensity=255;
		}
		else if(polygon4.PointInPolygen(x, y))
		{
			ptDataVec[i].intensity=1;
		}
	}
	
	TRACE("rectangle\n");
	TRACE("%.3f %.3f\n", pRect[0].X, pRect[0].Y);
	TRACE("%.3f %.3f\n", pRect[1].X, pRect[1].Y);
	TRACE("%.3f %.3f\n", pRect[2].X, pRect[2].Y);
	TRACE("%.3f %.3f\n", pRect[3].X, pRect[3].Y);

	TRACE("points:%d\n", ptNum);
	for(i=0; i<ptNum; i++)
	{
		TRACE("%.3f %.3f %d\n", ptDataVec[i].x, ptDataVec[i].y, ptDataVec[i].intensity);
	}

	TRACE("sub rectangle 1\n");
	TRACE("%.3f %.3f\n", pSubRect1[0].x, pSubRect1[0].y);
	TRACE("%.3f %.3f\n", pSubRect1[1].x, pSubRect1[1].y);
	TRACE("%.3f %.3f\n", pSubRect1[2].x, pSubRect1[2].y);
	TRACE("%.3f %.3f\n", pSubRect1[3].x, pSubRect1[3].y);

	TRACE("sub rectangle 2\n");
	TRACE("%.3f %.3f\n", pSubRect2[0].x, pSubRect2[0].y);
	TRACE("%.3f %.3f\n", pSubRect2[1].x, pSubRect2[1].y);
	TRACE("%.3f %.3f\n", pSubRect2[2].x, pSubRect2[2].y);
	TRACE("%.3f %.3f\n", pSubRect2[3].x, pSubRect2[3].y);

	TRACE("sub rectangle 3\n");
	TRACE("%.3f %.3f\n", pSubRect3[0].x, pSubRect3[0].y);
	TRACE("%.3f %.3f\n", pSubRect3[1].x, pSubRect3[1].y);
	TRACE("%.3f %.3f\n", pSubRect3[2].x, pSubRect3[2].y);
	TRACE("%.3f %.3f\n", pSubRect3[3].x, pSubRect3[3].y);

	TRACE("sub rectangle 4\n");
	TRACE("%.3f %.3f\n", pSubRect4[0].x, pSubRect4[0].y);
	TRACE("%.3f %.3f\n", pSubRect4[1].x, pSubRect4[1].y);
	TRACE("%.3f %.3f\n", pSubRect4[2].x, pSubRect4[2].y);
	TRACE("%.3f %.3f\n", pSubRect4[3].x, pSubRect4[3].y);
}

void CCalibProcDoc::OnTp2utm() 
{
	
	CFileDialog  prjdlg(TRUE,"prj",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"prj(*.prj)|*.prj|(*.*)|*.*||",NULL);
	//	CFileDialog  dstdlg(TRUE,"prj",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"prj(*.prj)|*.prj|(*.*)|*.*||",NULL);
	
	//	CFileDialog  atndlg(TRUE,"Attune",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"atn文件(*.las)|*.las|(*.*)|*.*||",NULL);	
//	CATNReader atnreader;
	
	CString temp;
	CArray<CString,CString&> pATNFile;
	POSITION pos;
	CFileDialog	Dlg(TRUE,"切平面LAS",NULL,OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT,"ATN_TP(*.LAS)|*.LAS|(*.*)|*.*||",NULL);
	TCHAR *pszFile = new TCHAR[MAX_PATH*MAX_PATH];
	
	
	Dlg.m_ofn.lpstrFilter="ATN_TP(*.LAS)\0*.las\0All Files (*.*)\0*.*\0";
	Dlg.m_ofn.lpstrTitle = "Import ATN_TP Files";
	
	memset(pszFile,0,sizeof(TCHAR)*MAX_PATH*MAX_PATH);
	Dlg.m_ofn.lpstrFile = pszFile;
	Dlg.m_ofn.nMaxFile = MAX_PATH*MAX_PATH; 
	
	Dlg.m_ofn.Flags = OFN_ALLOWMULTISELECT|OFN_EXPLORER|OFN_ENABLEHOOK;
	int i=0;
	if(Dlg.DoModal()==IDOK)
	{
		pos=Dlg.GetStartPosition();
		while(pos!=NULL){	pATNFile.Add(Dlg.GetNextPathName(pos));	i++;}
		temp.Format("%d",i);
		temp="一共打开了"+temp+"个文件";
		AfxMessageBox(temp);
	}
	
	CString strFolder;
	if(!_SelectFolderDialog("设置输出目录...", &strFolder, NULL))
		return;
	
	
	prjdlg.m_ofn.lpstrTitle="打开源投影";
	if(prjdlg.DoModal()!=IDOK)
		return;
	CString strOrg=prjdlg.GetPathName();
	
	prjdlg.m_ofn.lpstrTitle="打开目标投影";
	if(prjdlg.DoModal()!=IDOK)
		return;
	CString strDst=prjdlg.GetPathName();
	
	orsLidarGeometry lidGeo;
	double lat0, lon0, h0;
	
	lidGeo.SetORSPlatform(getPlatform());
	lat0=B/180.0*PI;
	lon0=L/180.0*PI;
	h0=H;
	lidGeo.SetTPlaneCoordinate();	//设置切平面中心，注意与之前的中心一致
	
	lidGeo.SetProjectionFromWkt(strOrg.GetBuffer(128), strDst.GetBuffer(128));  //投影转换参数
	
	//	if(atndlg.DoModal()!=IDOK)
	//		return;
	
//	CString stratn=atndlg.GetPathName();
	
//	atnreader.open(stratn.GetBuffer(128));
	
	LASreader_fast lasReader;
	LASwriter_fast  lasWriter;
	LASheader  *header, new_header;
	LASpoint *pt;
	double *gpstime;
	double x_offset, y_offset, z_offset;
	double x_scale_factor, y_scale_factor, z_scale_factor;

	ref_ptr<orsIProcessMsgBar> process;
	process = ORS_CREATE_OBJECT( orsIProcessMsgBar, "ors.process.bar.mfc");
	process->InitProgressBar("数据转换...", "wait...", pATNFile.GetSize());

	CString strFileName;
	CString	strInput, strOutput;
	double coord[3];
	for(i=0; i<pATNFile.GetSize(); i++)
	{
		strInput=pATNFile[i];
		strFileName=_ExtractFileName(strInput);
		strOutput=strFolder+"\\"+strFileName+"_UTM.LAS";
		
		lasReader.open(strInput.GetBuffer(128), "r");
		header=&(lasReader.header);

//		memcpy(&new_header, header, sizeof(LASheader));

		double X, Y, Z;
		//计算偏移值
		lidGeo.TPlane_To_Projection(header->max_x, header->max_y, header->max_z, &X, &Y, &Z);
		if(floor(X/100000.0)>0)
		{
			x_offset=floor(X/100000.0)*100000.0;
		}
		else 
		{
			x_offset=header->x_offset;
		}

		if(floor(Y/100000.0)>0)
		{
			y_offset=floor(Y/100000.0)*100000.0;
		}
		else 
		{
			y_offset=header->y_offset;
		}
		
		if(floor(Z/100000.0)>0)
		{
			z_offset=floor(Z/100000.0)*100000.0;
		}
		else 
		{
			z_offset=header->z_offset;
		}

		x_scale_factor=header->x_scale_factor;
		y_scale_factor=header->y_scale_factor;
		z_scale_factor=header->z_scale_factor;


		lasWriter.create(strOutput.GetBuffer(128), header);
		lasWriter.set_offset_scalefactor(x_offset, y_offset, z_offset, x_scale_factor, y_scale_factor, z_scale_factor);
//		lasWriter.write_header();
		
		//	ATNTP_Point  tp_pt;
		double xmax, ymax, zmax, xmin, ymin, zmin;
		
		xmax=-1.7e+308;	ymax=-1.7e+308;	zmax=-1.7e+308;
		xmin=1.7e+308;	ymin=1.7e+308;	zmin=1.7e+308;

				
		while (lasReader.read_point(coord))
		{
			pt=lasReader.point;
			gpstime=lasReader.gps_time;
			lidGeo.TPlane_To_Projection(coord[0], coord[1], coord[2], &X, &Y, &Z);

			if(xmax<X)
				xmax=X;
			if(xmin>X)
				xmin=X;
			
			if(ymax<Y)
				ymax=Y;
			if(ymin>Y)
				ymin=Y;
			
			if(zmax<Z)
				zmax=Z;
			if(zmin>Z)
				zmin=Z;
			
			lasWriter.write_point(X, Y, Z, pt->intensity, pt->return_number, pt->number_of_returns_of_given_pulse,
				pt->scan_direction_flag, pt->edge_of_flight_line, pt->classification, pt->scan_angle_rank, pt->user_data,
				pt->point_source_ID, *gpstime);
			
		}
		
//		lasWriter.write_header(header);
		
		lasReader.close();
		lasWriter.close();

		header->min_x=xmin-1;	header->max_x=xmax+1;
		header->min_y=ymin-1;	header->max_y=ymax+1;
		header->min_z=zmin-1;	header->max_z=zmax+1;
		header->x_offset=x_offset;
		header->y_offset=y_offset;
		header->z_offset=z_offset;

		lasWriter.open(strOutput.GetBuffer(128), header);
		lasWriter.close();

		
		process->SetPos(i+1);
	}
	
	if(pszFile)	delete pszFile;		pszFile=NULL;
}

//将控制点从WGS84系下转到自定义切平面系下
void CCalibProcDoc::OnGcpWgs84ToTplane()
{
	CFileDialog  indlg(TRUE,"控制点",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"txt(*.txt)|*.txt|(*.*)|*.*||",NULL);
	CFileDialog  outdlg(FALSE,"控制点",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "txt文件(*.txt)|*.txt|(*.*)|*.*||",NULL);

	orsLidarGeometry lidGeo;

	lidGeo.SetORSPlatform(getPlatform());
	lidGeo.SetTPlaneCoordinate();	//设置切平面中心，注意与之前的中心一致

	if(indlg.DoModal()!=IDOK)
		return;

	if(outdlg.DoModal()!=IDOK)
		return;

	CString strInput=indlg.GetPathName();

	FILE *fin=NULL;
	FILE *fout=NULL;
	char buf[128];
	double lat, lon, alt;
	std::vector<POINT3D>  ptBuf;
	POINT3D pt;

	fin=fopen(strInput.GetBuffer(128), "rt");
	fgets(buf, 128, fin);
//	fscanf(fin, "%s", buf);
	
	while( !feof(fin) )
	{
		fscanf(fin, "%s%lf%lf%lf", buf, &lat, &lon, &alt);

		lidGeo.Geographic2TangentPlane(lat*PI/180.0, lon*PI/180.0, alt, &(pt.X), &(pt.Y), &(pt.Z));
		double lat1, lon1, alt1;

		lidGeo.TangentPlane2Geographic(pt.X, pt.Y, pt.Z, &lat1, &lon1, &alt1);

		double x, y, z;
		lidGeo.Geographic2TangentPlane(lat1, lon1, alt1, &x, &y, &z);

		lat1=lat1*180.0/PI;
		lon1=lon1*180.0/PI;

		ptBuf.push_back(pt);
	}
	fclose(fin);

	CString strOutput=outdlg.GetPathName();
	fout=fopen(strOutput.GetBuffer(128), "wt");
	fprintf(fout, "## GCP Tangent Plane Coordinate\n");

	std::vector<POINT3D>::iterator iter;
	for(iter = ptBuf.begin(); iter != ptBuf.end(); iter++)
	{
		fprintf(fout, "%.6lf %.6lf %.3lf\n", iter->X, iter->Y, iter->Z);
	}
	fclose(fout);

}

//对真实的激光点，生成虚拟的控制点
void CCalibProcDoc::OnLidarcalibrationTruepoint()
{	
	CFileDialog dlg(TRUE,"apj",NULL,OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT,"align(*.apj)|*.apj|(*.*)|*.*||",NULL);
	CAlignPrj   algPrj;
	CArray<TieObjChain*, TieObjChain*> *pTieList=NULL;
	CArray<Align_LidLine, Align_LidLine> *pLidLines=NULL;
	//	orsVector<orsIImageGeometry*> 	imgGeoList;

	int i, j, k;
	TieObjChain *pChain=NULL;
	Align_LidLine lidLine;
	TieObject *pObj=NULL;
	CATNPtIndex	index;
	int srcID;
	//	std::vector<ATNTP_Point> ATNpt_List;
	int nPatch, iPatch;
//	double x0, y0;
	//	double *pZObs=NULL;
	long *pTieLut=NULL;	//连接点的查找表矩阵
	int nLine, nTies;
	POINT3D* pGCP=NULL, pt3D;
	ATNTP_Point *pATNPoint=NULL;
	int ngcp;
	LidMC_VP *pLidPatch=NULL;
	//	ATNTP_Point *pATNPoints=NULL;

	if(dlg.DoModal()!=IDOK)
		return;

	CString strPath=dlg.GetPathName();

	algPrj.Open(strPath.GetBuffer(256));

	pTieList=algPrj.GetTieObjList();
	pLidLines=algPrj.GetLidList();

	nLine=pLidLines->GetSize();
	nTies=pTieList->GetSize();

	pTieLut=new long[nTies*nLine];	//记录每个连接点是否在各条带上可见
	for(i=0; i<nTies*nLine; i++)
	{//不存在连接点时为1
		pTieLut[i]=-1;
	}

	nPatch=0;
	ngcp=0;
	for(i=0; i<pTieList->GetSize(); i++)
	{//统计虚拟连接点数
		pChain=pTieList->GetAt(i);  //遍历每一个点链

		ASSERT(pChain->objNum==pChain->pChain->GetSize());
		nPatch+=pChain->pChain->GetSize();	

		if(pChain->bGCP) 
			ngcp+=pChain->pChain->GetSize();

		for(j=0; j<pChain->objNum; j++)
		{
			pObj=pChain->pChain->GetAt(j);

			if(pObj->sourceType==ST_LiDAR)
			{//记录在链表中的序号
				srcID=pObj->sourceID-1;
				pTieLut[i*nLine+srcID]=j;
			}
		}
	}

	pLidPatch=new LidMC_VP[nPatch];
	pATNPoint=new ATNTP_Point[nPatch];
	pGCP = new POINT3D[nPatch];
	//	pATNPoints=new ATNTP_Point[nPatch*3];

	//	pZObs=new double[nPatch];

	locateresult   result;
	int tieID=0;
	iPatch=0;
	double radius=2.0;
	//按条带顺序初始化连接点，保证不重复用索引
	for(i=0; i<nLine; i++)
	{
		lidLine=pLidLines->GetAt(i);
		if(index.Open(lidLine.AtnName.GetBuffer(256))==false)
			continue;

		tieID=0;
		for(j=0; j<nTies; j++)
		{//遍历当前条带所有连接点
			k=pTieLut[j*nLine+i];
			if(k<0)  //判断当前连接点是否存在于指定条带上
				continue;

			pChain=pTieList->GetAt(j);
			//			if(pChain->TieID==6) continue;
			pObj=pChain->pChain->GetAt(k);

			//			ptNum=((TiePatch*)pObj)->ptNum;
			pt3D=((TiePoint*)pObj)->pt3D;

			pLidPatch[iPatch].pLidPoints=new std::vector<ATNTP_Point>;
			pLidPatch[iPatch].tieID=pChain->TieID-1;
			tieID++;

			double rmse;
			result=index.GetTriangleVertex(&pt3D, radius, pLidPatch[iPatch].pLidPoints, &rmse); //提取真实LiDAR点
			ASSERT(result!=OUTSIDE);

			if(result==OUTSIDE)
				continue;

			//虚拟观测值
			pLidPatch[iPatch].vx=pt3D.X;
			pLidPatch[iPatch].vy=pt3D.Y;
			pLidPatch[iPatch].vz=TriangleInterpolate(pLidPatch[iPatch].pLidPoints, pt3D.X, pt3D.Y);

			iPatch++;
		}

	}


	//对pLidPlanes按连接点ID排序
	qsort(pLidPatch, nPatch, sizeof(LidMC_VP), LidMC_VP_ascending_tieID);
	
	//将离控制点最近的点作为对应点
	double x, y, z;
	for(i=0; i<nPatch; i++)
	{
		j=pLidPatch[i].tieID;
		
		pChain=pTieList->GetAt(j);
		ASSERT(pChain->bGCP);

		pGCP[i].X=pChain->gcp.X;
		pGCP[i].Y=pChain->gcp.Y;
		pGCP[i].Z=pChain->gcp.Z;

		double maxDis2=100000;
		int ibest;
		for(j=0; j<3; j++)
		{
			x=(*(pLidPatch[i].pLidPoints))[j].x;
			y=(*(pLidPatch[i].pLidPoints))[j].y;
			z=(*(pLidPatch[i].pLidPoints))[j].z;
			double dis2=(x-pGCP[i].X)*(x-pGCP[i].X)+(y-pGCP[i].Y)*(y-pGCP[i].Y);
			if(dis2<maxDis2)
			{
				maxDis2=dis2;
				ibest=j;
			}
		}
		pATNPoint[i]=(*(pLidPatch[i].pLidPoints))[ibest];
	}


	double pX[6];	//线元素，角元素
	memset(pX, 0, sizeof(double)*6);

	pX[3]=/*-0.00576609*/0;	pX[4]=/*0.00258278*/0;	pX[5]=/*-0.00138682*/0;

	//6param
	CVCPCalib calib;
	calib.LidMC_Misalign_VTP_NN_xyzrph_semiXYZ(nPatch, pATNPoint, pGCP, pX, 6);
	//3param
//	LidMC_Misalign_VTP_NN_rph_semiXYZ(nPatch, pATNPoint, pGCP, pX, 6);


	if(pLidPatch)	delete[] pLidPatch;	pLidPatch=NULL;
	if(pATNPoint)	delete[] pATNPoint;	pATNPoint=NULL;
	if(pTieLut)  delete pTieLut;  pTieLut=NULL;
	if(pGCP)	delete pGCP;	pGCP=NULL;
	//	if(pATNPoints)	delete pATNPoints;	pATNPoints=NULL;
}

void CCalibProcDoc::OnCoplanaradjustment3param()
{
	CFileDialog dlg(TRUE,"apj",NULL,OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT,"align(*.apj)|*.apj|(*.*)|*.*||",NULL);
	CAlignPrj   algPrj;
	CArray<TieObjChain*, TieObjChain*> *pTieList=NULL;
	CArray<Align_LidLine, Align_LidLine> *pLidLines=NULL;
	//	orsVector<orsIImageGeometry*> 	imgGeoList;

	int i, j, k;
	TieObjChain *pChain=NULL;
	Align_LidLine lidLine;
	TieObject *pObj=NULL;
	CATNPtIndex	index;
	int srcID;
	//	std::vector<ATNTP_Point> ATNpt_List;
	LidMC_Plane *pLidPlanes=NULL;
	int nPatch, iPatch;
	//	double x0, y0;
	POINT3D *pt3D;
	int ptNum;
	int nLine, nTies;
	long *pTieLut=NULL;	//连接点的查找表矩阵
	//	std::vector<ATNTP_Point> *pLidPoints;

	if(dlg.DoModal()!=IDOK)
		return;

	CString strPath=dlg.GetPathName();

	algPrj.Open(strPath.GetBuffer(256));

	pTieList=algPrj.GetTieObjList();
	pLidLines=algPrj.GetLidList();

	nLine=pLidLines->GetSize();
	nTies=pTieList->GetSize();

	pTieLut=new long[nTies*nLine];	//记录每个连接点是否在各条带上可见

	for(i=0; i<nTies*nLine; i++)
	{//不存在连接点时为-1
		pTieLut[i]=-1;
	}

	nPatch=0;
	for(i=0; i<pTieList->GetSize(); i++)
	{//统计虚拟连接点数
		pChain=pTieList->GetAt(i);  //遍历每一个点链

		//		if(pChain->TieID==6) continue;
		ASSERT(pChain->objNum==pChain->pChain->GetSize());
		nPatch+=pChain->pChain->GetSize();	

		for(j=0; j<pChain->objNum; j++)
		{
			pObj=pChain->pChain->GetAt(j);

			if(pObj->sourceType==ST_LiDAR)
			{//记录在链表中的序号
				srcID=pObj->sourceID-1;
				pTieLut[i*nLine+srcID]=j;
			}
		}
	}

	pLidPlanes=new LidMC_Plane[nPatch];
	iPatch=0;
	int tieID=0;
//	vector<ATNTP_Point> tmpPoints;

	//按条带顺序初始化连接点，保证不重复用索引
	for(i=0; i<nLine; i++)
	{
		lidLine=pLidLines->GetAt(i);
		if(index.Open(lidLine.AtnName.GetBuffer(256))==false)
			continue;

		tieID=0;
		for(j=0; j<nTies; j++)
		{//遍历当前条带所有连接点
			k=pTieLut[j*nLine+i];
			if(k<0)
				continue;

			pChain=pTieList->GetAt(j);
			//			if(pChain->TieID==6) continue;
			pObj=pChain->pChain->GetAt(k);

			ptNum=((TiePatch*)pObj)->ptNum;
			pt3D=((TiePatch*)pObj)->pt3D;

	
			pLidPlanes[iPatch].pLidPoints=new std::vector<ATNTP_Point>;
			index.GetPoints(pt3D, ptNum, pLidPlanes[iPatch].pLidPoints);
			pLidPlanes[iPatch].tieID=pChain->TieID-1;
			tieID++;


			//将提取的激光点和包围盒导出，检查点位是否正确
			// 			{
			// 				TRACE("tie:%d   line:%d\n", j, i);
			// 				for(k=0; k<ptNum; k++)
			// 				{
			// 					TRACE("%.3f %.3f\n", pt3D[k].X, pt3D[k].Y);
			// 				}
			// 				TRACE("inside points\n");
			// 				TRACE("ptNum:%d\n", pLidPlanes[iPatch].pLidPoints->size());
			// 				
			// 				pLidPoints=pLidPlanes[iPatch].pLidPoints;
			// 				for(k=0; k<pLidPlanes[iPatch].pLidPoints->size(); k++)
			// 				{
			// 					TRACE("%.3f %.3f %.3f\n", (*pLidPoints)[k].x, (*pLidPoints)[k].y, (*pLidPoints)[k].z);
			// 				}
			// 				
			// 			}

			iPatch++;
		}

	}

	//对pLidPlanes按连接点ID排序
	qsort(pLidPlanes, nPatch, sizeof(LidMC_Plane), LidMC_Plane_ascending_tieID);

	////////////////////////////////////////////////////////////////////////
	//重新统计连接平面，删除少于两个的面片的连接面
	
	iPatch = 0;
	int ptnumTh = 4;
	tieID = -1;
	int numOfPatch=0;
	int startID, endID;
	int ndummy = 0;
	
	for(i=0, startID=0; i<nPatch; i++)
	{
		if(tieID != pLidPlanes[i].tieID)
		{//开始新的连接面

			tieID = pLidPlanes[i].tieID;
			endID = i;

			if(numOfPatch<2)
			{//连接面不足两个，则删除整个连接面
				for(j=startID; j<endID; j++)
				{
					if(pLidPlanes[j].tieID > 0)
					{//点数大于阈值，但由于其他条带上的连接面片过小，需要删除整个连接面链
						pLidPlanes[j].tieID = -1;
						ndummy++;
					}
				}
			}
			//重新计数
			startID = i;
			numOfPatch = 1;
			if(pLidPlanes[i].pLidPoints->size() < ptnumTh)
			{
				pLidPlanes[i].tieID = -1;
				ndummy++;
				numOfPatch--;
			}
		}
		else
		{
			if(pLidPlanes[i].pLidPoints->size() < ptnumTh)
			{
				pLidPlanes[i].tieID = -1;
				ndummy++;
				numOfPatch--;
			}
			else
				numOfPatch++;
		}
	}

	//补上最后一个
	if(numOfPatch<2)
	{//连接面不足两个，则删除整个连接面
		for(j=startID; j<nPatch; j++)
		{
			if(pLidPlanes[j].tieID > 0)
			{//点数大于阈值，但由于其他条带上的连接面片过小，需要删除整个连接面链
				pLidPlanes[j].tieID = -1;
				ndummy++;
			}
		}
	}

	//重新排序，前ndummy为删除的面片
	qsort(pLidPlanes, nPatch, sizeof(LidMC_Plane), LidMC_Plane_ascending_tieID);

	///////////////////////////////////////////////////////
	//将tieID调整为连续的
	tieID = 0;
	int curID=0;
	for(i=ndummy, startID=ndummy; i<nPatch; i++)
	{
		if(tieID != pLidPlanes[i].tieID)
		{
			tieID = pLidPlanes[i].tieID;
			endID = i;

			for(j=startID; j<endID; j++)
			{
				pLidPlanes[j].tieID =curID;
			}
			startID = i;
			curID++;
		}

	}

	//补上最后一个
	for(i=startID; j<nPatch; j++)
	{
		pLidPlanes[j].tieID =curID;
	}

	
	//ransac筛选出共面点
	LidPlane_ransac(pLidPlanes+ndummy, nPatch-ndummy);

	
	oriCalibParam oriParam;
	memset(&oriParam, 0, sizeof(oriCalibParam));

	CalibParam_Type type;
//	type = Calib_rph;
//	type = Calib_rph_sa0;
	type = Calib_rph_drange;
//	type = Calib_rph_drange_sa0;

	double pX[5];
	int nCalib;
	memset(pX, 0, sizeof(double)*5);
	//用初值重新计算点集
//	pX[3]=/*-0.00576609*/0;	pX[4]=/*0.00258278*/0;	pX[5]=/*-0.00138682*/0;

// 	pX[0] = oriParam.misalign_angle[0] = -0.001961905;
// 	pX[1] = oriParam.misalign_angle[1] = 0.002657077;
// 	pX[2] = oriParam.misalign_angle[2] = -0.001377952;
// 	pX[3] = oriParam.sa0 = -0.003186274;

	switch (type)
	{
	case Calib_rph:
		nCalib = 3;
		break;
	case Calib_rphxyz:
		nCalib = 6;
		break;
	case Calib_rph_appXYZ:
		nCalib = 6;
		break;
	case Calib_rphxyz_sa0:
		nCalib = 7;
		break;
	case Calib_rphxyz_sa0_sac:
		nCalib = 8;
		break;
	case Calib_rph_sa0:
		nCalib = 4;
		break;
	case Calib_rph_drange:
		nCalib = 4;
		break;
	case Calib_rph_drange_sa0:
		nCalib = 5;
		break;
	default:
		nCalib = 0;
	}

	CFileDialog  outdlg(FALSE,"检校结果",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"txt文件(*.txt)|*.txt|(*.*)|*.*||",NULL);	

	if(outdlg.DoModal()!=IDOK)
		return;


	CString strOutput = outdlg.GetPathName();		//导出检校结果


	CPlaneCalib calib;
	calib.LidMC_Misalign_plane(strOutput.GetBuffer(128), nPatch-ndummy, pLidPlanes+ndummy, pX, oriParam, type);



// 	fprintf(fp, "## %d tie_plane param\n", nTies);
// 	for(i=0; i<nTies; i++)
// 	{
// 		for(j=0; j<nPatch; j++)
// 		{
// 			if(pLidPlanes[j].tieID == i)
// 			{
// 				fprintf(fp, "%.6lf\n", pLidPlanes[j].s1);
// 				fprintf(fp, "%.6lf\n", pLidPlanes[j].s2);
// 				fprintf(fp, "%.6lf\n", pLidPlanes[j].s3);
// 				fprintf(fp, "%.6lf\n", pLidPlanes[j].s4);
// 
// 				break;
// 			}
// 		}
// 		
// 	}


// 	for(i=0; i<nPatch; i++)
// 	{
// 		if(pLidPlanes[i].pLidPoints)
// 		{
// 			pLidPlanes[i].pLidPoints->empty();		
// 			delete pLidPlanes[i].pLidPoints;
// 		}
// 	}
	if(pLidPlanes)	delete[] pLidPlanes;	pLidPlanes=NULL;
	if(pTieLut)		delete[] pTieLut;		pTieLut=NULL;	
}

//z值约束的共面平差
void CCalibProcDoc::OnCoplanaradj6pZconstraint()
{
	CFileDialog dlg(TRUE,"apj",NULL,OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT,"align(*.apj)|*.apj|(*.*)|*.*||",NULL);
	CAlignPrj   algPrj;
	CArray<TieObjChain*, TieObjChain*> *pTieList=NULL;
	CArray<Align_LidLine, Align_LidLine> *pLidLines=NULL;
	//	orsVector<orsIImageGeometry*> 	imgGeoList;

	int i, j, k;
	TieObjChain *pChain=NULL;
	Align_LidLine lidLine;
	TieObject *pObj=NULL;
	CATNPtIndex	index;
	int srcID;
	//	std::vector<ATNTP_Point> ATNpt_List;
	LidMC_Plane *pLidPlanes=NULL;
	LidMC_VP *pLidVP=NULL;
	int nPatch, iPatch, nVP, iVP;
	//	double x0, y0;
	POINT3D *pt3D, point;
	int ptNum;
	int nLine, nTies;
	long *pTieLut=NULL;	//连接点的查找表矩阵
	//	std::vector<ATNTP_Point> *pLidPoints;

	if(dlg.DoModal()!=IDOK)
		return;

	CString strPath=dlg.GetPathName();

	algPrj.Open(strPath.GetBuffer(256));

	pTieList=algPrj.GetTieObjList();
	pLidLines=algPrj.GetLidList();

	nLine=pLidLines->GetSize();
	nTies=pTieList->GetSize();

	pTieLut=new long[nTies*nLine];	//记录每个连接点是否在各条带上可见

	for(i=0; i<nTies*nLine; i++)
	{//不存在连接点时为1
		pTieLut[i]=-1;
	}

	nPatch=0;	nVP=0;
	for(i=0; i<pTieList->GetSize(); i++)
	{//统计虚拟连接点数
		pChain=pTieList->GetAt(i);  //遍历每一个点链

		//		if(pChain->TieID==6) continue;
		ASSERT(pChain->objNum==pChain->pChain->GetSize());

		if(pChain->type==TO_PATCH)
			nPatch+=pChain->pChain->GetSize();	

		if(pChain->type==TO_POINT)
			nVP+=pChain->pChain->GetSize();

		for(j=0; j<pChain->objNum; j++)
		{
			pObj=pChain->pChain->GetAt(j);

			if(pObj->sourceType==ST_LiDAR)
			{//记录在链表中的序号
				srcID=pObj->sourceID-1;
				pTieLut[i*nLine+srcID]=j;
			}
		}
	}

	pLidVP=new LidMC_VP[nVP];
	pLidPlanes=new LidMC_Plane[nPatch];
	iPatch=0;	iVP=0;
	int tieID=0;
	//按条带顺序初始化连接点，保证不重复用索引
	for(i=0; i<nLine; i++)
	{
		lidLine=pLidLines->GetAt(i);
		if(index.Open(lidLine.AtnName.GetBuffer(256))==false)
			continue;

		tieID=0;
		for(j=0; j<nTies; j++)
		{//遍历当前条带所有连接点
			k=pTieLut[j*nLine+i];
			if(k<0)
				continue;

			pChain=pTieList->GetAt(j);
			//			if(pChain->TieID==6) continue;
			pObj=pChain->pChain->GetAt(k);

			if(pChain->type==TO_PATCH)
			{
				ptNum=((TiePatch*)pObj)->ptNum;
				pt3D=((TiePatch*)pObj)->pt3D;

				pLidPlanes[iPatch].pLidPoints=new std::vector<ATNTP_Point>;
				pLidPlanes[iPatch].tieID=pChain->TieID-1;
				tieID++;

				index.GetPoints(pt3D, ptNum, pLidPlanes[iPatch].pLidPoints);

				iPatch++;
			}
			else if(pChain->type==TO_POINT)
			{
				point=((TiePoint*)pObj)->pt3D;

				pLidVP[iVP].pLidPoints=new std::vector<ATNTP_Point>;
				pLidVP[iVP].tieID=pChain->TieID-1;
				pLidVP[iVP].eX=pChain->gcp.X;
				pLidVP[iVP].eY=pChain->gcp.Y;
				pLidVP[iVP].eZ=pChain->gcp.Z;
				tieID++;

				double rmse;
				index.GetTriangleVertex(&point, 2, pLidVP[iVP].pLidPoints, &rmse); //提取真实LiDAR点
				iVP++;
			}

		}

	}

	//对pLidPlanes按连接点ID排序
	qsort(pLidPlanes, nPatch, sizeof(LidMC_Plane), LidMC_Plane_ascending_tieID);
	qsort(pLidVP, nVP, sizeof(LidMC_VP), LidMC_VP_ascending_tieID);

	//重新整理tieID
	j=-1;
	tieID=-1;
	for(i=0; i<nPatch; i++)
	{
		if(j!=pLidPlanes[i].tieID)
		{
			tieID++;
			j=pLidPlanes[i].tieID;
			pLidPlanes[i].tieID=tieID;
		}
		else
		{
			pLidPlanes[i].tieID=tieID;
		}
	}

	j=-1;
	tieID=-1;
	for(i=0; i<nVP; i++)
	{
		if(j!=pLidVP[i].tieID)
		{
			tieID++;
			j=pLidVP[i].tieID;
			pLidVP[i].tieID=tieID;
		}
		else
		{
			pLidVP[i].tieID=tieID;
		}
	}

	double pX[6];
	memset(pX, 0, sizeof(double)*6);

	pX[3]=/*-0.00576609*/0;	pX[4]=/*0.00258278*/0;	pX[5]=/*-0.00138682*/0;

	//用初值重新计算点集

	CPlaneCalib calib;
	calib.LidMC_Misalign_plane_zconstraint(nVP, pLidVP,nPatch, pLidPlanes, pX, 6);

	if(pLidPlanes)	delete[] pLidPlanes;	pLidPlanes=NULL;
	if(pLidVP)		delete[] pLidVP;		pLidVP=NULL;
	if(pTieLut)		delete[] pTieLut;		pTieLut=NULL;	
}


void RotateMat_rph(double roll, double pitch, double heading, double *r)
{
	double cosR,sinR;
	double cosP,sinP;
	double cosH,sinH;

	cosR = cos(roll);		sinR = sin(roll);
	cosP = cos(pitch);		sinP = sin(pitch);
	cosH = cos(heading);	sinH = sin(heading);

	*r++ =  cosH*cosP;	*r++ = -sinH*cosR + cosH*sinP*sinR;	*r++ = sinH*sinR + cosH*sinP*cosR;
	*r++ =  sinH*cosP;	*r++ = cosH*cosR + sinH*sinP*sinR;	*r++ = sinH*sinP*cosR - cosH*sinR;
	*r++ =  -sinP;		*r++ = cosP*sinR;					*r   = cosP*cosR;
}

void CCalibProcDoc::OnMountingerror()
{
	double r, p, h, theta, rho;
	double Rm[9], Rs[3];
//	double Rtmp[9];
	double delta[3];
	double radian;

	r=0.1;	p=0.1;	h=0.1;	theta=-75;		rho=1500;

	r=r/180*PI;	p=p/180*PI;	h=h/180*PI;	/*theta=theta/180*PI;*/

	TRACE("%.3f\n", rho);
	for(;theta<80; )
	{
		matrix_zeroes(3,1,delta);
		//	matrix_ident(3, Rs);

		radian=theta/180*PI;
		Rs[0]=0;
		Rs[1]=-sin(radian)*rho;
		Rs[2]=cos(radian)*rho;

		RotateMat_rph(r, p, h, Rm);

		Rm[0]-=1;
		Rm[4]-=1;
		Rm[8]-=1;

		//	matrix_product33(Rm, Rs, Rtmp);
		matrix_product(3,3,3,1,Rm, Rs, delta);

		TRACE("%.3f %.3f %.3f %.3f\n", theta, delta[0],delta[1],delta[2]);
		
		theta+=10;
	}

}

void CCalibProcDoc::OnAdjustmentevaluation()
{
	CFileDialog dlg(TRUE,"apj",NULL,OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT,"align(*.apj)|*.apj|(*.*)|*.*||",NULL);
	CAlignPrj   algPrj;
	CArray<TieObjChain*, TieObjChain*> *pTieList=NULL;
	CArray<Align_LidLine, Align_LidLine> *pLidLines=NULL;
	//	orsVector<orsIImageGeometry*> 	imgGeoList;

	int i, j, k;
	TieObjChain *pChain=NULL;
	Align_LidLine lidLine;
	TieObject *pObj=NULL;
	CATNPtIndex	index;
	int srcID;
	//	std::vector<ATNTP_Point> ATNpt_List;
	LidMC_VP *pLidVP=NULL;
	LidMC_Plane *pLidPlanes=NULL;
	int nPatch, iPatch, nVP, iVP;
	//	double *pZObs=NULL;
	long *pTieLut=NULL;	//连接点的查找表矩阵
	int nLine, nTies;
	POINT3D pt3D, *ptBuf;
	int ngcp;
	//	ATNTP_Point *pATNPoints=NULL;

	if(dlg.DoModal()!=IDOK)
		return;

	CString strPath=dlg.GetPathName();
	algPrj.Open(strPath.GetBuffer(256));

	pTieList=algPrj.GetTieObjList();
	pLidLines=algPrj.GetLidList();

	nLine=pLidLines->GetSize();
	nTies=pTieList->GetSize();

	pTieLut=new long[nTies*nLine];	//记录每个连接点是否在各条带上可见
	for(i=0; i<nTies*nLine; i++)
	{//不存在连接点时为1
		pTieLut[i]=-1;
	}

	nPatch=0;
	nVP=0;
	ngcp=0;
	for(i=0; i<pTieList->GetSize(); i++)
	{//统计虚拟连接点数
		pChain=pTieList->GetAt(i);  //遍历每一个点链

		ASSERT(pChain->objNum==pChain->pChain->GetSize());

		if(pChain->type==TO_PATCH)
			nPatch+=pChain->pChain->GetSize();	

		if(pChain->type==TO_POINT)
			nVP+=pChain->pChain->GetSize();

		// 		if(pChain->type==TO_POINT)
		// 			nVP+=pChain->pChain->GetSize();

		if(pChain->bGCP) 
			ngcp+=pChain->pChain->GetSize();


		for(j=0; j<pChain->objNum; j++)
		{
			pObj=pChain->pChain->GetAt(j);

			if(pObj->sourceType==ST_LiDAR)
			{//记录在链表中的序号
				srcID=pObj->sourceID-1;
				pTieLut[i*nLine+srcID]=j;
			}
		}
	}

	if(nVP>0)
		pLidVP=new LidMC_VP[nVP];
	if(nPatch>0)
		pLidPlanes=new LidMC_Plane[nPatch];
	//	pATNPoints=new ATNTP_Point[nPatch*3];

	//	pZObs=new double[nPatch];
	int ptNum;
	locateresult   result;
	int tieID=0;
	iPatch=0; iVP=0;
	double radius=10.0;
	//按条带顺序初始化连接点，保证不重复用索引
	for(i=0; i<nLine; i++)
	{
		lidLine=pLidLines->GetAt(i);
		if(index.Open(lidLine.AtnName.GetBuffer(256))==false)
			continue;

		tieID=0;
		for(j=0; j<nTies; j++)
		{//遍历当前条带所有连接点
			k=pTieLut[j*nLine+i];
			if(k<0)  //判断当前连接点是否存在于指定条带上
				continue;

			pChain=pTieList->GetAt(j);
			//			if(pChain->TieID==6) continue;
			pObj=pChain->pChain->GetAt(k);

			if(pChain->type==TO_PATCH)
			{
				ptNum=((TiePatch*)pObj)->ptNum;
				ptBuf=((TiePatch*)pObj)->pt3D;

				pLidPlanes[iPatch].pLidPoints=new std::vector<ATNTP_Point>;
				pLidPlanes[iPatch].tieID=pChain->TieID-1;
				tieID++;

				index.GetPoints(ptBuf, ptNum, pLidPlanes[iPatch].pLidPoints);

				iPatch++;
			}
			else if(pChain->type==TO_POINT)
			{
				pt3D=((TiePoint*)pObj)->pt3D;

				pLidVP[iVP].pLidPoints=new std::vector<ATNTP_Point>;
				pLidVP[iVP].tieID=pChain->TieID-1;
				//				pLidVP[iVP].eX=pChain->gcp.X;
				//				pLidVP[iVP].eY=pChain->gcp.Y;
				//				pLidVP[iVP].eZ=pChain->gcp.Z;
				tieID++;

				double rmse;
				result=index.GetTriangleVertex(&pt3D, radius, pLidVP[iVP].pLidPoints, &rmse); 
				ASSERT(result!=OUTSIDE);

				if(result==OUTSIDE)
					continue;

				pLidVP[iVP].vx=pt3D.X;
				pLidVP[iVP].vy=pt3D.Y;
				pLidVP[iVP].vz=TriangleInterpolate(pLidVP[iVP].pLidPoints, pt3D.X, pt3D.Y);
				iVP++;
			}
		}

	}


	//对pLidPlanes按连接点ID排序
	qsort(pLidVP, nVP, sizeof(LidMC_VP), LidMC_VP_ascending_tieID);
	qsort(pLidPlanes, nPatch, sizeof(LidMC_Plane), LidMC_Plane_ascending_tieID);
	//为控制点赋值
	for(i=0; i<nVP; i++)
	{
		j=pLidVP[i].tieID;

		if(i<ngcp)
		{//控制点必须排在最开始
			pChain=pTieList->GetAt(j);
			ASSERT(pChain->bGCP);

			pLidVP[i].eX=pChain->gcp.X;
			pLidVP[i].eY=pChain->gcp.Y;
			pLidVP[i].eZ=pChain->gcp.Z;
		}
	}

	double *pInitX=NULL;
	double *pAdjX=NULL;
	int pnp;

	CSetMisalignDlg setDlg;
	double roll, pitch, heading, tx, ty, tz;
	double dXs, dYs, dZs;
	BOOL	bMisalign6Param;
	BOOL	bTranslationParam;

	bMisalign6Param=setDlg.m_bMisalign6Param;
	bTranslationParam=setDlg.m_bTranslationParam;

	if(setDlg.DoModal()==IDOK)
	{
		roll=setDlg.m_roll;
		pitch=setDlg.m_pitch;
		heading=setDlg.m_heading;
		tx=setDlg.m_tx;
		ty=setDlg.m_ty;
		tz=setDlg.m_tz;
		dXs=setDlg.m_dTranslationX;
		dYs=setDlg.m_dTranslationY;
		dZs=setDlg.m_dTranslationZ;

		bMisalign6Param=setDlg.m_bMisalign6Param;
		bTranslationParam=setDlg.m_bTranslationParam;
	}
	else
	{
		roll=0;
		pitch=0;
		heading=0;
		tx=0;
		ty=0;
		tz=0;
		dXs=0;	
		dYs=0;	
		dZs=0;
	}

	LiDMC_PARAMETER paramtype;
	if(bMisalign6Param)
	{
		pnp=6;
		paramtype=LiDMC_xyzrph;
	}
	else
	{
		pnp=3;
		paramtype=LiDMC_rph;
	}

	if(bTranslationParam)
	{
		pnp+=3;
		paramtype=paramtype+2;
	}

	
	pInitX=new double[pnp];
	pAdjX=new double[pnp];
	
	i=0;
	if(bMisalign6Param)
	{
		pInitX[i++]=tx;
		pInitX[i++]=ty;
		pInitX[i++]=tz;
		pInitX[i++]=roll;
		pInitX[i++]=pitch;
		pInitX[i++]=heading;
	}
	else
	{
		pInitX[i++]=roll;
		pInitX[i++]=pitch;
		pInitX[i++]=heading;
	}

	if(bTranslationParam)
	{
		pInitX[i++]=dXs;
		pInitX[i++]=dYs;
		pInitX[i++]=dZs;
	}

	ASSERT(i==pnp);
	if(setDlg.DoModal()==IDOK)
	{
		roll=setDlg.m_roll;
		pitch=setDlg.m_pitch;
		heading=setDlg.m_heading;
		tx=setDlg.m_tx;
		ty=setDlg.m_ty;
		tz=setDlg.m_tz;
		dXs=setDlg.m_dTranslationX;
		dYs=setDlg.m_dTranslationY;
		dZs=setDlg.m_dTranslationZ;

		bMisalign6Param=setDlg.m_bMisalign6Param;
		bTranslationParam=setDlg.m_bTranslationParam;
	}
	else
	{
		roll=0;
		pitch=0;
		heading=0;
		tx=0;
		ty=0;
		tz=0;
		dXs=0;	
		dYs=0;	
		dZs=0;
	}

	i=0;
	if(bMisalign6Param)
	{
		pAdjX[i++]=tx;
		pAdjX[i++]=ty;
		pAdjX[i++]=tz;
		pAdjX[i++]=roll;
		pAdjX[i++]=pitch;
		pAdjX[i++]=heading;
	}
	else
	{
		pAdjX[i++]=roll;
		pAdjX[i++]=pitch;
		pAdjX[i++]=heading;
	}

	if(bTranslationParam)
	{
		pAdjX[i++]=dXs;
		pAdjX[i++]=dYs;
		pAdjX[i++]=dZs;
	}

	CVCPCalib VCP;
	VCP.LidMC_VPAdj_QA(nVP, 0, pLidVP, pInitX, pAdjX, pnp, paramtype);

	CPlaneCalib plane;
	plane.LidMC_PlaneAdj_QA(nPatch, pLidPlanes, pInitX, pAdjX, pnp, paramtype);


	if(pLidVP)	delete[] pLidVP;	pLidVP=NULL;
	if(pLidPlanes)	delete[] pLidPlanes;	pLidPlanes=NULL;
	if(pTieLut)  delete pTieLut;  pTieLut=NULL;
	if(pInitX)		delete pInitX;		pInitX=NULL;
	if(pAdjX)		delete pAdjX;		pAdjX=NULL;
	//	if(pATNPoints)	delete pATNPoints;	pATNPoints=NULL;
}

#include "..\ImageRectify\orsImageRectify_Inor.h"
void CCalibProcDoc::OnImginorrectify()
{
	ImageRectifyInor_SetPlatform(getPlatform());

	CImageRectify_Inor	imgRect;

	imgRect.Run();
}

void CCalibProcDoc::OnGeodetic2utm()
{
	
	//	CFileDialog  dstdlg(TRUE,"prj",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"prj(*.prj)|*.prj|(*.*)|*.*||",NULL);

	//	CFileDialog  atndlg(TRUE,"Attune",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"atn文件(*.las)|*.las|(*.*)|*.*||",NULL);	
	//	CATNReader atnreader;

	CString temp;
	CArray<CString,CString&> pATNFile;
	POSITION pos;
	CFileDialog	Dlg(TRUE,"经纬度LAS",NULL,OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT,"ATN_TP(*.LAS)|*.LAS|(*.*)|*.*||",NULL);
	Dlg.m_ofn.lpstrTitle="原始LAS";

	TCHAR *pszFile = new TCHAR[MAX_PATH*MAX_PATH];


	Dlg.m_ofn.lpstrFilter="ATN_TP(*.LAS)\0*.las\0All Files (*.*)\0*.*\0";
	Dlg.m_ofn.lpstrTitle = "Import ATN_TP Files";

	memset(pszFile,0,sizeof(TCHAR)*MAX_PATH*MAX_PATH);
	Dlg.m_ofn.lpstrFile = pszFile;
	Dlg.m_ofn.nMaxFile = MAX_PATH*MAX_PATH; 

	Dlg.m_ofn.Flags = OFN_ALLOWMULTISELECT|OFN_EXPLORER|OFN_ENABLEHOOK;
	int i=0;
	if(Dlg.DoModal()==IDOK)
	{
		pos=Dlg.GetStartPosition();
		while(pos!=NULL){	pATNFile.Add(Dlg.GetNextPathName(pos));	i++;}
		temp.Format("%d",i);
		temp="一共打开了"+temp+"个文件";
		AfxMessageBox(temp);
	}

	CString strFolder;
	if(!_SelectFolderDialog("投影转换后输出目录...", &strFolder, NULL))
		return;


	CFileDialog  prjdlg(TRUE,"prj",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"prj(*.prj)|*.prj|(*.*)|*.*||",NULL);
	prjdlg.m_ofn.lpstrTitle="原始Las投影";
	if(prjdlg.DoModal()!=IDOK)
		return;
	CString strOrg=prjdlg.GetPathName();

	CFileDialog  prjdlg1(TRUE,"prj",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"prj(*.prj)|*.prj|(*.*)|*.*||",NULL);
	prjdlg1.m_ofn.lpstrTitle="目标Las投影";
	if(prjdlg1.DoModal()!=IDOK)
		return;
	CString strDst=prjdlg1.GetPathName();

	orsLidarGeometry lidGeo;
//	double lat0, lon0, h0;

 	lidGeo.SetORSPlatform(getPlatform());
// 	lat0=B/180.0*PI;
// 	lon0=L/180.0*PI;
// 	h0=H;
// 	lidGeo.SetTPlaneCoordinate();	//设置切平面中心，注意与之前的中心一致

	lidGeo.SetProjectionFromWkt(strOrg.GetBuffer(128), strDst.GetBuffer(128));  //投影转换参数

	//	if(atndlg.DoModal()!=IDOK)
	//		return;

	//	CString stratn=atndlg.GetPathName();

	//	atnreader.open(stratn.GetBuffer(128));

	FILE *fin=NULL, *fout=NULL;
	LASreader lasReader;
	LASwriter  lasWriter;
//	LASheader  *header, new_header;
	LASpoint *pt;
	double *gpstime;
//	double x_offset, y_offset, z_offset;
//	double x_scale_factor, y_scale_factor, z_scale_factor;

	ref_ptr<orsIProcessMsgBar> process;
	process = ORS_CREATE_OBJECT( orsIProcessMsgBar, "ors.process.bar.mfc");
	process->InitProgressBar("数据转换...", "wait...", pATNFile.GetSize());

	CString strFileName;
	CString	strInput, strOutput;
	double coord[3];
	double X,Y, Z;
	for(i=0; i<pATNFile.GetSize(); i++)
	{
		strInput=pATNFile[i];
		strFileName=_ExtractFileName(strInput);
		strOutput=strFolder+"\\"+strFileName+"_UTM.LAS";

		fin=fopen(strInput, "rb");
		fout=fopen(strOutput, "wb");
		int err=GetLastError();

		lasReader.open(fin);
		lasWriter.open(fout, NULL, 0);

		while (lasReader.read_point(coord))
		{
			pt=&(lasReader.point);
			gpstime=&(lasReader.gps_time);
			lidGeo.ProjectTransform(coord[1]/*/1000*/, coord[0]/*/1000*/, coord[2], &X, &Y, &Z);

			lasWriter.write_point(X, Y, Z, pt->intensity, pt->return_number, pt->number_of_returns_of_given_pulse,
				pt->scan_direction_flag, pt->edge_of_flight_line, pt->classification, pt->scan_angle_rank, pt->user_data,
				pt->point_source_ID, *gpstime);

		}

		//		lasWriter.write_header(header);

		lasReader.close();
		lasWriter.close();

		fclose(fin);
		fclose(fout);

		process->SetPos(i+1);
	}

	if(pszFile)	delete pszFile;		pszFile=NULL;
}


void CCalibProcDoc::OnTest()
{
	CFileDialog dlg(TRUE,"apj",NULL,OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT,"align(*.apj)|*.apj|(*.*)|*.*||",NULL);
	CAlignPrj   algPrj;
	CArray<TieObjChain*, TieObjChain*> *pTieList=NULL;
	CArray<Align_LidLine, Align_LidLine> *pLidLines=NULL;

	if(dlg.DoModal()!=IDOK)
		return;

	CString strPath=dlg.GetPathName();

	algPrj.Open(strPath.GetBuffer(256));

	pTieList=algPrj.GetTieObjList();
	//	pLidLines=algPrj.GetLidList();

	//	int nLine=pLidLines->GetSize();
	int nTies=pTieList->GetSize();

	for(int i=0; i<nTies; i++)
	{//统计虚拟连接点数
		TieObjChain* pTieChain=pTieList->GetAt(i);  //遍历每一个点链

		TRACE("%d\n", pTieChain->TieID);
		TRACE("%d\n", pTieChain->objNum);
		TRACE("%d\n", pTieChain->type);
		TRACE("%x\n", pTieChain->pChain);
		// 		pTieChain->pChain->GetSize();
		//ASSERT(pTieChain->objNum==pTieChain->pChain->GetSize());

		TieObject *pObj=pTieChain->pChain->GetAt(0);

	}

}

//基于虚拟连接点的平差 
void CCalibProcDoc::OnTiePoint()
{
	CFileDialog dlg(TRUE,"apj",NULL,OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT,"align(*.apj)|*.apj|(*.*)|*.*||",NULL);
	dlg.m_ofn.lpstrTitle="打开apj工程";

	CAlignPrj   algPrj;
	CArray<TieObjChain*, TieObjChain*> *pTieList=NULL;
	CArray<Align_LidLine, Align_LidLine> *pLidLines=NULL;
	//	orsVector<orsIImageGeometry*> 	imgGeoList;

	int i, j, k;
	TieObjChain *pTieChain=NULL;
	Align_LidLine lidLine;
	TieObject *pObj=NULL;
	CATNPtIndex	index;
	int srcID;
	//	std::vector<ATNTP_Point> ATNpt_List;
	LidMC_VP *pLidVP=NULL;
	LidMC_Plane *pLidPlanes=NULL;
	int nPatch, iPatch, nVP, iVP;
	//	double *pZObs=NULL;
	long *pTieLut=NULL;	//连接点的查找表矩阵
	int nLine, nTies;
	POINT3D pt3D, *ptBuf;
	int ngcp;
	//	ATNTP_Point *pATNPoints=NULL;

	if(dlg.DoModal()!=IDOK)
		return;

	CString strPath=dlg.GetPathName();

	algPrj.Open(strPath.GetBuffer(256));

	pTieList=algPrj.GetTieObjList();
	pLidLines=algPrj.GetLidList();

	nLine=pLidLines->GetSize();
	nTies=pTieList->GetSize();

//	nTies=20;	//测试用

	pTieLut=new long[nTies*nLine];	//记录每个连接点是否在各条带上可见
	for(i=0; i<nTies*nLine; i++)
	{//不存在连接点时为1
		pTieLut[i]=-1;
	}

	nPatch=0;
	nVP=0;
	ngcp=0;
	for(i=0; i<pTieList->GetSize(); i++)
	{//统计虚拟连接点数
		pTieChain=pTieList->GetAt(i);  //遍历每一个点链

//  		TRACE("%d\n", pTieChain->TieID);
//  		TRACE("%d\n", pTieChain->objNum);
//  		TRACE("%d\n", pTieChain->type);
//  		TRACE("%x\n", pTieChain->pChain);
		// 		pTieChain->pChain->GetSize();
		ASSERT(pTieChain->objNum==pTieChain->pChain->GetSize());

		if(pTieChain->type==TO_PATCH)
		{
			nPatch+=pTieChain->pChain->GetSize();	
//			continue;
		}

		if(pTieChain->type==TO_POINT)
			nVP+=pTieChain->pChain->GetSize();

		// 		if(pChain->type==TO_POINT)
		// 			nVP+=pChain->pChain->GetSize();

		if(pTieChain->bGCP) 
			ngcp+=pTieChain->pChain->GetSize();


		for(j=0; j<pTieChain->objNum; j++)
		{
			pObj=pTieChain->pChain->GetAt(j);

			if(pObj->sourceType==ST_LiDAR)
			{//记录在链表中的序号
				srcID=pObj->sourceID-1;
				pTieLut[i*nLine+srcID]=j;
			}
		}
	}

	if(nVP>0)
		pLidVP=new LidMC_VP[nVP];

	if(nPatch>0)
		pLidPlanes=new LidMC_Plane[nPatch];
	//	pATNPoints=new ATNTP_Point[nPatch*3];

	//	pZObs=new double[nPatch];
	int ptNum;
	locateresult   result;
	int tieID=0;
	iPatch=0; iVP=0;
	double radius=2.0;
	double rmse_th=0.2;
	//按条带顺序初始化连接点，保证不重复用索引
	for(i=0; i<nLine; i++)
	{
		lidLine=pLidLines->GetAt(i);
		if(index.Open(lidLine.AtnName.GetBuffer(256))==false)
			continue;

// 		ref_ptr<orsIImageSourceReader> pImgReader;
// 		pImgReader=getImageService()->openImageFile(lidLine.ImageName.GetBuffer(128));
// 		if (!pImgReader.get())
// 		{//could not open the image
// 			return ;
// 		}
// 		orsRect_i rect;
// 		orsIImageData *pImgData=NULL;
// 		orsBandSet bandSet;
// 		rect=pImgReader->getBoundingRect();
// 		ors_uint32 numBand = pImgReader->getNumberOfInputBands();
// 		for(j = 0; j < numBand; j++)
// 		{
// 			bandSet.push_back(j);
// 		}
// 		pImgData=pImgReader->getImageData(rect, 1, bandSet);

		tieID=0;
		for(j=0; j<nTies; j++)
		{//遍历当前条带所有连接点
			
// 			if(j==521)
// 				printf("");

			k=pTieLut[j*nLine+i];
			if(k<0)  //判断当前连接点是否存在于指定条带上
				continue;

			pTieChain=pTieList->GetAt(j);
// 			if(pTieChain->type==TO_PATCH)
// 				continue;

			//			if(pChain->TieID==6) continue;
			pObj=pTieChain->pChain->GetAt(k);

			if(pTieChain->type==TO_PATCH)
			{
				ptNum=((TiePatch*)pObj)->ptNum;
				ptBuf=((TiePatch*)pObj)->pt3D;

				pLidPlanes[iPatch].pLidPoints=new std::vector<ATNTP_Point>;
				pLidPlanes[iPatch].tieID=pTieChain->TieID-1;
				tieID++;

				index.GetPoints(ptBuf, ptNum, pLidPlanes[iPatch].pLidPoints);

				iPatch++;
			}
			else if(pTieChain->type==TO_POINT)
			{
				char tptype =  ((TiePoint*)pObj)->tpType;

				pt3D=((TiePoint*)pObj)->pt3D;

				pLidVP[iVP].pLidPoints=new std::vector<ATNTP_Point>;
				pLidVP[iVP].tieID=pTieChain->TieID-1;
				//				pLidVP[iVP].eX=pChain->gcp.X;
				//				pLidVP[iVP].eY=pChain->gcp.Y;
				//				pLidVP[iVP].eZ=pChain->gcp.Z;
				tieID++;
				
				double rmse;
				result=index.GetTriangleVertex(&pt3D, radius, pLidVP[iVP].pLidPoints, &rmse); 
//				ASSERT(result!=OUTSIDE);

				if(result==OUTSIDE)
				{//扫描漏洞，剔除
// 					pLidVP[iVP].VP_type=TP_type_horizon;
// 					pLidVP[iVP].vx=pt3D.X;
// 					pLidVP[iVP].vy=pt3D.Y;
// 					pLidVP[iVP].reliability=0;
// 
// 					iVP++;
 					continue;
				}
				
				pLidVP[iVP].vx=pt3D.X;
				pLidVP[iVP].vy=pt3D.Y;
				pLidVP[iVP].vz=TriangleInterpolate(pLidVP[iVP].pLidPoints, pt3D.X, pt3D.Y);

				if(!tptype)
				{//未定义连接点类型，通过粗糙度计算
					if(rmse<rmse_th)
						pLidVP[iVP].VP_type=TP_type_hor_ver;
					else
						pLidVP[iVP].VP_type=TP_type_horizon;
				}
				else
				{//直接使用预先定义的连接点类型
					pLidVP[iVP].VP_type=tptype;
				}
				iVP++;
			}
		}

	}

	if(nVP>iVP)	nVP=iVP;

	//对pLidVP按连接点ID排序
	qsort(pLidVP, nVP, sizeof(LidMC_VP), LidMC_VP_ascending_tieID);

	//对连接点进行检查
	int TieHorNum=0, TieHorVerNum=0, TieVerNum=0;
	int tiePtNum=0;
	for(i=0; i<nVP; )
	{
		int sPos, ePos;
		tieID=pLidVP[i].tieID;
		char vp_type=pLidVP[i].VP_type;
		bool bUpdate=false;

		tiePtNum++;
		sPos=i;
		ePos=i;

		i++;
		while(pLidVP[i].tieID==tieID)
		{
			if(pLidVP[i].VP_type!=vp_type)
			{
				bUpdate=true;
			}
			ePos=i;

			i++;
		}
		
		if(bUpdate)
		{
			vp_type=TP_type_horizon;
			for(j=sPos; j<=ePos; j++)
			{
				pLidVP[j].VP_type=vp_type;
			}
		}

		if(vp_type==TP_type_hor_ver)	TieHorVerNum++;
		else if(vp_type==TP_type_horizon)	TieHorNum++;
		else if(vp_type==TP_type_vertical)	TieVerNum++;

	}

	TRACE("total tie point num: %d\n", tiePtNum);
	TRACE("horizon and vertical tie point num: %d\n", TieHorVerNum);
	TRACE("horizon tie point num: %d\n", TieHorNum);
	TRACE("vertical tie point num: %d\n", TieVerNum);

	//为控制点赋值
	for(i=0; i<ngcp; i++)
	{
		j=pLidVP[i].tieID;
		
//		if(i<ngcp)
//		{//控制点必须排在最开始
			pTieChain=pTieList->GetAt(j);
			ASSERT(pTieChain->bGCP);

			pLidVP[i].eX=pTieChain->gcp.X;
			pLidVP[i].eY=pTieChain->gcp.Y;
			pLidVP[i].eZ=pTieChain->gcp.Z;
//		}
	}

	//调整连接点ID,连续排列 (漏洞点删除后占用的ID)
	for(i=0, j=0; i<nVP; )
	{
		tieID=pLidVP[i].tieID;
		if(tieID!=j)
		{	
			pLidVP[i].tieID=j;	
		}
				
		i++;
		while(pLidVP[i].tieID==tieID)
		{
			if(tieID!=j)
			{	
				pLidVP[i].tieID=j;	
			}
			i++;
		}
		j++;
	}

	oriCalibParam oriParam;
	memset(&oriParam, 0, sizeof(oriCalibParam));

	double pX[8];	//线元素，角元素
	memset(pX, 0, sizeof(double)*8);

//	pX[3]=/*-0.00576609*/0;	pX[4]=/*0.00258278*/0;	pX[5]=/*-0.00138682*/0;

	CFileDialog  outputdlg(FALSE," ",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"txt文件(*.txt)|*.txt|(*.*)|*.*||",NULL);	
	outputdlg.m_ofn.lpstrTitle="保存平差解算参数";

	if(outputdlg.DoModal()!=IDOK)
		goto FUNC_END;

	{
	CString  strOutput=outputdlg.GetPathName();

	CalibParam_Type ctype;
//	ctype=Calib_rph_sa0;
//	ctype=Calib_rph;
//	ctype=Calib_rph_drange;
//	ctype=Calib_rph_drange_sa0;
	ctype=Calib_rphxyz;

	
	pX[0] = 0;
	pX[1] = 0;
	pX[2] = 0;
	pX[3] = 0;
	pX[4] = 0;
	pX[5] = 0;
	

//	ctype=Calib_rphxyz_sa0;
//	ctype=Calib_rphxyz_sa0_sac;

	//有线性系统改正
	//	LidMC_Misalign_VP_Triangle_xyzrph_semiXYZ(nPatch, ngcp, pLidPatch, pX, 6);
	//无系统改正
	CVCPCalib calib;
	calib.LidMC_Misalign_VP_Triangle(strOutput.GetBuffer(128), nVP, ngcp, pLidVP, pX, oriParam, ctype);
	}

FUNC_END:
	if(pLidVP)	delete[] pLidVP;	pLidVP=NULL;
	if(pLidPlanes)	delete[] pLidPlanes;	pLidPlanes=NULL;
	if(pTieLut)  delete pTieLut;  pTieLut=NULL;
	//	if(pATNPoints)	delete pATNPoints;	pATNPoints=NULL;
}

#include "..\LidCalib\ImageCalib.h"
void CCalibProcDoc::OnImgcalib()
{
	CString strImgList;

	{
		CFileDialog dlg(TRUE,"匹配点",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"(*.txt)|*.txt|(*.*)|*.*||",NULL);

		dlg.m_ofn.lpstrTitle="打开影像列表";

		if(dlg.DoModal()!=IDOK)
			return;
		
		strImgList=dlg.GetPathName();
	}

	CString strTracks;
	{
		CFileDialog dlg(TRUE,"匹配点",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"(*.txt)|*.txt|(*.*)|*.*||",NULL);

		dlg.m_ofn.lpstrTitle="打开track文件";
		if(dlg.DoModal()!=IDOK)
			return;
		strTracks=dlg.GetPathName();
	}

	CString posFile;
	{
		CFileDialog dlg(TRUE,"匹配点",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"(*.txt)|*.txt|(*.*)|*.*||",NULL);

		dlg.m_ofn.lpstrTitle="打开POS序列文件";
		if(dlg.DoModal()!=IDOK)	
			return;
		posFile=dlg.GetPathName();
	}

	CString strLidFile;
	{
		CFileDialog dlg(TRUE,"匹配点",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"(*.idx)|*.idx||",NULL);

		dlg.m_ofn.lpstrFilter="*.idx";
		dlg.m_ofn.lpstrTitle="打开LiDAR索引文件";

		if(dlg.DoModal()!=IDOK)	
			return;
		strLidFile=dlg.GetPathName();
	}

//	Img_misalign_calib_delay_ZConstraint(strImgList.GetBuffer(128), strTracks.GetBuffer(128), posFile.GetBuffer(128), strLidFile.GetBuffer(128));
	Img_misalign_calib_ZConstraint( strImgList, strTracks, /*posFile.GetBuffer(128)*/strLidFile );

}

void CCalibProcDoc::OnPosinterpolateformlidar()
{
	CFileDialog dlg(TRUE,"匹配点",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"(*.txt)|*.txt|(*.*)|*.*||",NULL);
	dlg.m_ofn.lpstrTitle="打开影像列表";

	std::vector<imgExOrientParam> exorParam;

	CString strFileName;
	if(dlg.DoModal()!=IDOK)
		return;
	strFileName=dlg.GetPathName();

	double timeArr[256];
	FILE *fp=NULL;
	
	fp=fopen(strFileName.GetBuffer(128), "rt");
	int i=0, j, k;
	while(!feof(fp))
	{
		double Xs, Ys, Zs, f, w, k, lat, lon;
		fscanf(fp, "%lf %lf %lf %lf %lf %lf %lf %lf %lf", timeArr+i, &Xs, &Ys, &Zs, &f, &w, &k, &lat, &lon);

		i++;
	}
	int camNum=i;
	fclose(fp);	fp=NULL;

	double timeInterval=0;
	double sTime, eTime;	//时间起点，终点
// 	for(i=1; i<camNum; i++)
// 	{
// 		if(time[i]-time[i-1]>timeInterval)
// 			timeInterval=time[i]-time[i-1];
// 	}

//	timeInterval*=2;
//	sTime=time[0]-0.001;
//	eTime=time[camNum-1]+0.001;

	timeInterval=42*1e-6;

	CFileDialog  atndlg(TRUE,"Attune",NULL,OFN_HIDEREADONLY|OFN_FILEMUSTEXIST,"atn文件(*.las)|*.las|(*.*)|*.*||",NULL);	
	atndlg.m_ofn.lpstrTitle="打开对应的LiDAR文件";
	CATN_TPReader atnreader;
	ATNTP_Point	point, prePt;;

	if(atndlg.DoModal()!=IDOK)
		return;

	CString strPath=atndlg.GetPathName();

	atnreader.open(strPath.GetBuffer(128));

	double delay=42*1e-6;
	int posNum=atnreader.m_header.number_of_point_records;

	//	double Rt[9]={'1','0','0','0','-1','0','0','0','-1'};	//像空间坐标系到传感器坐标系
	double Rimu[9], R[9];

	double Rt[9];	//像空间坐标系到传感器坐标系
	memset(Rt, 0, sizeof(double)*9);
	Rt[0]=1;	Rt[4]=-1;	Rt[8]=-1;

	for(i=0, j=0; i<camNum; i++)
	{//找到时间最接近的LiDAR点
		double time = timeArr[i]+delay;

		while(atnreader.read_point(&point))
		{

			if(time>point.gpstime)
			{
				j++;
				prePt=point;
			}
			else
			{
				break;
			}

		}

		ASSERT(j>0 && j<posNum);

		double _t=prePt.gpstime;
		double t_=point.gpstime;

		imgExOrientParam exor;
		if(fabs(_t-time)<fabs(time-t_))
		{
			exor.gps_time=_t;
			exor.Xs=prePt.Xs;
			exor.Ys=prePt.Ys;
			exor.Zs=prePt.Zs;
			exor.phi=prePt.phi;
			exor.omega=prePt.omega;
			exor.kappa=prePt.kappa;
		}
		else
		{
			exor.gps_time=t_;
			exor.Xs=point.Xs;
			exor.Ys=point.Ys;
			exor.Zs=point.Zs;
			exor.phi=point.phi;
			exor.omega=point.omega;
			exor.kappa=point.kappa;
		}

		RotateMat_fwk(exor.phi, exor.omega, exor.kappa, Rimu);
		matrix_product(3,3,3,3,Rimu,Rt,R);
		R2fwk(R, &(exor.phi), &(exor.omega), &(exor.kappa));
		exor.kappa-=PI/2;

		exorParam.push_back(exor);
	}

// 	int lidNum=0;
// 	while(atnreader.read_point(&point))
// 	{
// 		if(point.gpstime<sTime)
// 			continue;
// 		if(point.gpstime>eTime)
// 			continue;
// 
// 		lidNum++;
// 	}
// 
// 	if(lidNum==0)
// 	{
// 		ASSERT(FALSE);	
// 		return;
// 	}
// 	imgExOrientParam *exorPara=NULL;
// 	exorPara=new imgExOrientParam[lidNum];
// 
// 
// 	
// 
// // 	RotateMat_rph(180.0/PI, 0, 90.0/PI, Rt);
// // 	matrix_transpose(3, 3, Rt, Rt_T);
// // 
// // 	RotateMat_fwk(180.0/PI, 0, 0, Rt_T);
// 
// 	atnreader.reopen();
// 	i=0;
// 	while(atnreader.read_point(&point))
// 	{
// 		if(point.gpstime<sTime)
// 			continue;
// 		if(point.gpstime>eTime)
// 			continue;
// 
// 		exorPara[i].gps_time=point.gpstime;
// 		exorPara[i].Xs=point.Xs;
// 		exorPara[i].Ys=point.Ys;
// 		exorPara[i].Zs=point.Zs;
// 
// 		RotateMat_fwk(point.phi, point.omega, point.kappa, Rimu);
// 		matrix_product(3,3,3,3,Rimu,Rt,R);
// 		R2fwk(R, &(exorPara[i].phi), &(exorPara[i].omega), &(exorPara[i].kappa));
// 		exorPara[i].kappa-=PI/2;
// 
// //		RotateMat_fwk(f, w, k-PI/2, R);
// //		matrix_product(3, 3, 3, 3, Rt_T, Rimu, R);
// //		R2fwk(R, &(exorPara[i].phi), &(exorPara[i].omega), &(exorPara[i].kappa)); 
// 
// // 		exorPara[i].phi=point.phi;
// // 		exorPara[i].omega=point.omega;
// // 		exorPara[i].kappa=point.kappa;
// 
// 		i++;
// 	}

	CFileDialog  outputdlg(FALSE," ",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"txt文件(*.txt)|*.txt|(*.*)|*.*||",NULL);	
	outputdlg.m_ofn.lpstrTitle="保存相机外方位元素";

	if(outputdlg.DoModal()!=IDOK)
		return;
//		goto FUNC_END;

	strFileName=outputdlg.GetPathName();

	fp=fopen(strFileName.GetBuffer(128), "wt");

//	fprintf(fp, "%d\n", can);
	for( i=0; i<camNum; i++) 
	{
		fprintf(fp, "%lf %lf %lf %lf %lf %lf %lf\n", exorParam[i].gps_time, exorParam[i].Xs, exorParam[i].Ys, exorParam[i].Zs,
			exorParam[i].phi, exorParam[i].omega, exorParam[i].kappa);
		
		RotateMat_fwk(exorParam[i].phi, exorParam[i].omega, exorParam[i].kappa, R);
		
		for( j=0; j<3; j++) 
		{
			for( k=0; k<3; k++)
			{
				fprintf(fp, "%.10lf ", R[j*3+k] );
			}
			fprintf(fp, "\n");
		}
	}
	fclose(fp);	
	fp=NULL;

//FUNC_END:
//	if(exorPara)	delete[] exorPara; exorPara=NULL;
}

void CCalibProcDoc::OnRotate()
{
	double f, w, k;

	double Rimu[9], R[9];

	f=3.139420;
	w=-0.012589;
	k=2.227806;
	RotateMat_fwk(f, w, k, Rimu);

	double Rt[9];	//像空间坐标系到传感器坐标系
	memset(Rt, 0, sizeof(double)*9);
	Rt[0]=1;	Rt[4]=-1;	Rt[8]=-1;

	matrix_product(3,3,3,3,Rimu,Rt,R);
	R2fwk(R, &f, &w, &k);

	RotateMat_fwk(f, w, k-PI/2, R);

	for( int i=0; i<3; i++) 
	{
		for( int j=0; j<3; j++)
		{
			TRACE("%.10lf ", R[i*3+j] );
		}
		TRACE("\n");
	}
}

//ATN_TP文件裁剪
void CCalibProcDoc::OnAtnCrop()
{
	CString temp;
	CArray<CString,CString&> pATNFile;
	POSITION pos;
	CFileDialog	Dlg(TRUE,"",NULL,OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT,"ATN_TP(*.LAS)|*.LAS|(*.*)|*.*||",NULL);
	TCHAR *pszFile = new TCHAR[MAX_PATH*MAX_PATH];


	Dlg.m_ofn.lpstrFilter="ATN_TP(*.LAS)\0*.las\0All Files (*.*)\0*.*\0";
	Dlg.m_ofn.lpstrTitle = "选择裁剪的ATN_TP Files";

	memset(pszFile,0,sizeof(TCHAR)*MAX_PATH*MAX_PATH);
	Dlg.m_ofn.lpstrFile = pszFile;
	Dlg.m_ofn.nMaxFile = MAX_PATH*MAX_PATH; 

	Dlg.m_ofn.Flags = OFN_ALLOWMULTISELECT|OFN_EXPLORER|OFN_ENABLEHOOK;
	int i=0;
	if(Dlg.DoModal()==IDOK)
	{
		pos=Dlg.GetStartPosition();
		while(pos!=NULL){	pATNFile.Add(Dlg.GetNextPathName(pos));	i++;}
		temp.Format("%d",i);
		temp="一共打开了"+temp+"个文件";
		AfxMessageBox(temp);
	}
	else 
		return;

	CFileDialog  outputdlg(FALSE," ",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"las文件(*.las)|*.las|(*.*)|*.*||",NULL);	
	outputdlg.m_ofn.lpstrTitle="裁剪结果";

	if(outputdlg.DoModal()!=IDOK)
		return;

	CString strOutputName=outputdlg.GetPathName();

	CATN_TPReader	atnReader;
	CATN_TPWriter	atnWriter;

	LASheader  header;

	ATNTP_Point pt;
	CString strName;
	D_RECT cropRect;

	cropRect.XMin=-1124.64;
	cropRect.XMax=1082.91;
	cropRect.YMin=-1089.14;
	cropRect.YMax=1039.30;

	ref_ptr<orsIProcessMsgBar> process;
	process = ORS_CREATE_OBJECT( orsIProcessMsgBar, "ors.process.bar.mfc");
	process->InitProgressBar("数据转换...", "wait...", pATNFile.GetSize());

	CString strFileName;
	CString	strInput, strOutput;
	unsigned int cropNum=0;
	for(i=0; i<pATNFile.GetSize(); i++)
	{
		strInput=pATNFile[i];
		// 		strFileName=_ExtractFileName(strInput);
		// 		strOutput=strFolder+"\\"+strFileName+".LAS";

		atnReader.open(strInput.GetBuffer(128));
		atnReader.read_header(&header);

		while (atnReader.read_point(&pt))
		{
			if(pt.x<cropRect.XMax && pt.x>cropRect.XMin && pt.y<cropRect.YMax && pt.y>cropRect.YMin)
				cropNum++;
		}
		atnReader.close();
	}
	
	header.number_of_point_records=cropNum;
	header.min_x=cropRect.XMin;
	header.max_x=cropRect.XMax;
	header.min_y=cropRect.YMin;
	header.max_y=cropRect.YMax;

	atnWriter.create(strOutputName.GetBuffer(128));
	atnWriter.write_header(&header);

	for(i=0; i<pATNFile.GetSize(); i++)
	{
		strInput=pATNFile[i];
// 		strFileName=_ExtractFileName(strInput);
// 		strOutput=strFolder+"\\"+strFileName+".LAS";

		atnReader.open(strInput.GetBuffer(128));
		atnReader.read_header(&header);

		while (atnReader.read_point(&pt))
		{
			if(pt.x<cropRect.XMax && pt.x>cropRect.XMin && pt.y<cropRect.YMax && pt.y>cropRect.YMin)
			{
				atnWriter.write_point(&pt);
			}
		}
		
		atnReader.close();
		process->SetPos(i+1);
	}

	atnWriter.close();

	if(pszFile)	delete pszFile;		pszFile=NULL;
}

#include "../lidQC/lidar_qc.h"
void CCalibProcDoc::OnQcPlane()
{
	CFileDialog dlg(TRUE,"apj",NULL,OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT,"align(*.apj)|*.apj|(*.*)|*.*||",NULL);
	CAlignPrj   algPrj;

	if(dlg.DoModal()!=IDOK)
		return;

	CString strPath=dlg.GetPathName();

	algPrj.Open(strPath.GetBuffer(256));

	oriCalibParam param;

	param.lever_arm[0]=0;
	param.lever_arm[1]=0;
	param.lever_arm[2]=0;

	param.misalign_angle[0] = -0.005170291;
	param.misalign_angle[1] = 0.002516033;
	param.misalign_angle[2] = -0.000042237;

	param.sa0 = 0;
	param.sac = 0;
	param.drange = 0.895160710;

	param.semiXYZ[0]=0;
	param.semiXYZ[1]=0;
	param.semiXYZ[2]=0;

	CFileDialog  outdlg(FALSE,"QC结果",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"txt文件(*.txt)|*.txt|(*.*)|*.*||",NULL);	

	if(outdlg.DoModal()!=IDOK)
		return;

	CString strPathName = outdlg.GetPathName();

	LidQC_Plane(param, &algPrj, strPathName);

}

void CCalibProcDoc::OnQcVp()
{
	CFileDialog dlg(TRUE,"apj",NULL,OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT,"align(*.apj)|*.apj|(*.*)|*.*||",NULL);
	CAlignPrj   algPrj;

	if(dlg.DoModal()!=IDOK)
		return;

	CString strPath=dlg.GetPathName();

	algPrj.Open(strPath.GetBuffer(256));

	oriCalibParam param;

	param.lever_arm[0]=0;
	param.lever_arm[1]=0;
	param.lever_arm[2]=0;

	param.misalign_angle[0] = -0.00522377;
	param.misalign_angle[1] = 0.00238412;
	param.misalign_angle[2] = -0.001019272;

	param.sa0 = 0;
	param.sac = 0;
	param.drange = -0.317940200;

	param.semiXYZ[0]=0;
	param.semiXYZ[1]=0;
	param.semiXYZ[2]=0;

	CFileDialog  outdlg(FALSE,"QC结果",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"txt文件(*.txt)|*.txt|(*.*)|*.*||",NULL);	

	if(outdlg.DoModal()!=IDOK)
		return;

	CString strPathName = outdlg.GetPathName();

	CalibParam_Type ctype; 
	ctype = Calib_rph_drange;

	LidQC_VP(param, ctype, &algPrj, strPathName);

}
//将平面的法向量转换为极坐标
void CCalibProcDoc::OnHonvStat()
{
	CFileDialog dlg(TRUE,"apj",NULL,OFN_FILEMUSTEXIST|OFN_OVERWRITEPROMPT,"align(*.apj)|*.apj|(*.*)|*.*||",NULL);
	CAlignPrj   algPrj;

	if(dlg.DoModal()!=IDOK)
		return;

	CString strPath=dlg.GetPathName();

	algPrj.Open(strPath.GetBuffer(256));


	HONV *pHONV = NULL;
	int nPlane;

	Stat_Plane_HONV( &algPrj, &pHONV, nPlane );

	CFileDialog  outdlg(FALSE,"HONV结果",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"txt文件(*.txt)|*.txt|(*.*)|*.*||",NULL);	

	if(outdlg.DoModal()!=IDOK)
		return;
	
	CString str=outdlg.GetPathName();

	FILE *fp=fopen(str.GetBuffer(128), "wt");

	fprintf(fp, "Stat HONV\n");
	fprintf(fp, "PlaneID, azimuth, zenith, frequency\n");
	for(int i=0; i<nPlane; i++)
	{
		fprintf(fp, "%d %f %f %d\n", i, pHONV[i].azimuth, pHONV[i].zenith, pHONV[i].frequency);
	}

	fclose(fp);
}

//从连接平面周围提取连接点
#include "SelectPointDlg.h"
void CCalibProcDoc::OnSelecttieptbyplane()
{
	CSelectPointDlg dlg;

	dlg.DoModal();
}
