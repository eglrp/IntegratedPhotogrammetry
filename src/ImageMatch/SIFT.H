#ifndef _IPLR_SIFT_Feature_Detector_H__
#define _IPLR_SIFT_Feature_Detector_H__
// Scale Invariant Feature Transform class
// �Կ�Դ��SiftGPU��ĵ��ýӿ�	2010.3	zj
//	2010.4.28  �Դ�Ӱ��ֿ���ȡsift����  zj
//  2010.5.4	�����ļ�bin�Ķ�д
//

#ifdef _IMAGE_MATCH_
#define  _Image_Match_  __declspec(dllexport)
#else
#define  _Image_Match_  __declspec(dllimport)	
#endif

#include "\openrs\IntegratedPhotogrammetry\external\SiftGPU\SiftGPU\src\SIFTGPU.h"
#include "orsImage/orsIImageService.h"
#include <vector>
using namespace std;
#include "IPhBaseDef.h"

//#define __BUNDLER_DISTR__
//#include "..\..\BA\bundler-v0.3\src\keys.h"

//#include "keys2a.h"

class orsIImageData;

class _Image_Match_ CSIFT
{
public:
	CSIFT();
	virtual ~CSIFT();
	
	bool	Startup(LPSTR cmd);	//ͨ����������SIFT

	void	SetBlock(int blockWid=1024, int blockHei=1024);	//����Ӱ��ֿ��С
	
	bool	RunSift_Integrated(orsIImageSourceReader *ImgReader, int maxd=2560);	//������Ӱ����ȡsift�����ֿ�

	//2011.8.24 zj ��ָ��������ȡ
	bool	RunSift(orsIImageSourceReader *ImgReader, int nband=0);	//������Ӱ����ȡsift, �Զ��Դ�Ӱ��ֿ�
	
	bool	RunSift(orsIImageSourceReader *ImgReader, orsRect_i rect);	//��ָ����Ӱ�����ȡsift  todo 4.30

	//ȡ��ǰ��ȡ��SIFT�������������
	std::vector<KEYPOINT> GetKeypointList();
	std::vector<unsigned char> GetDescriptors();



	bool 	SaveSift_txt(const char * szFileName);	//txt
	bool	SaveSift_bin(const char * szFileName);	//bin
	
	std::vector<KEYPOINT> ReadSiftKeyFile(const char * szFileName);	//todo 4.30
	std::vector<KEYPOINTWithDesc> ReadSiftKeyFileWithDesc(const char *szFileName, bool descriptor);

private:

	void	TransImageData(orsIImageData *imgData, ors_byte *pBuf);
	void	TransImageData(orsIImageData *imgData, ors_byte *pBuf, int nBand);

protected:

	ors_byte		*m_dataBuf;
	ors_uint32		m_BufLen;

private:

	SiftGPU		*m_pSiftEx;
	int		m_blockWid;
	int		m_blockHei;

	vector<SiftGPU::SiftKeypoint> m_SiftGPU_keys;
	vector<float> m_SiftGPU_descriptors;

	vector<KEYPOINT> m_keypoints;
	vector<unsigned char> m_desc;
};




#ifndef _IMAGE_MATCH_
#ifdef _DEBUG
#pragma comment(lib,"ImageMatchD.lib")
#else
#pragma comment(lib,"ImageMatch.lib")
#endif
#endif

#endif