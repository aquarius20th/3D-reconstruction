
//Read Binvox file////////////////////////////////////////////////////////////////
#include <string>
#include <fstream>
#include <iostream>
#include <stdlib.h>
#include<cv.h>
#include<opencv2\opencv.hpp>

using namespace std;
typedef unsigned char byte;

static int version;
static int depth, height, width;
static int size1;
static byte *voxels = 0;
static float tx, ty, tz;
static float scale;


int read_binvox(string filespec)
{

  ifstream *input = new ifstream(filespec.c_str(), ios::in | ios::binary);

  //
  // read header
  //
  string line;
  *input >> line;  // #binvox
  if (line.compare("#binvox") != 0) {
    cout << "Error: first line reads [" << line << "] instead of [#binvox]" << endl;
    delete input;
    return 0;
  }
  *input >> version;
  cout << "reading binvox version " << version << endl;

  depth = -1;
  int done = 0;
  while(input->good() && !done) {
    *input >> line;
    if (line.compare("data") == 0) done = 1;
    else if (line.compare("dim") == 0) {
      *input >> depth >> height >> width;
    }
    else if (line.compare("translate") == 0) {
      *input >> tx >> ty >> tz;
    }
    else if (line.compare("scale") == 0) {
      *input >> scale;
    }
    else {
      cout << "  unrecognized keyword [" << line << "], skipping" << endl;
      char c;
      do {  // skip until end of line
        c = input->get();
      } while(input->good() && (c != '\n'));

    }
  }
  if (!done) {
    cout << "  error reading header" << endl;
    return 0;
  }
  if (depth == -1) {
    cout << "  missing dimensions in header" << endl;
    return 0;
  }

  size1 = width * height * depth;
  voxels = new byte[size1];
  if (!voxels) {
    cout << "  error allocating memory" << endl;
    return 0;
  }

  //
  // read voxel data
  //
  byte value;
  byte count;
  int index = 0;
  int end_index = 0;
  int nr_voxels = 0;
  
  input->unsetf(ios::skipws);  // need to read every byte now (!)
  *input >> value;  // read the linefeed char

  while((end_index < size1) && input->good()) {
    *input >> value >> count;

    if (input->good()) {
      end_index = index + count;
      if (end_index > size1) return 0;
      for(int i=index; i < end_index; i++) voxels[i] = value;
      
      if (value) nr_voxels += count;
      index = end_index;
    }  // if file still ok
    
  }  // while

  input->close();
  cout << "  read " << nr_voxels << " voxels" << endl;
  cout<< depth<<"x"<<height<<"x"<<width<<endl;
  return 1;

}
///////////////////////////////////////////////////////////////////////



#include <Inventor/Win/SoWin.h>
#include <Inventor/Win/viewers/SoWinExaminerViewer.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCone.h>
#include <Inventor/nodes/SoMaterial.h>
#include <Inventor/nodes/SoTranslation.h>
#include <Inventor/nodes/SoFaceSet.h>
#include <Inventor/nodes/SoVertexProperty.h> 
#include <Inventor/nodes/SoShapeHints.h>

#include <VolumeViz/nodes/SoVolumeData.h>
#include <VolumeViz/nodes/SoVolumeSkin.h>
#include <VolumeViz/nodes/SoVolumeRender.h>
#include <VolumeViz/nodes/SoTransferFunction.h>

#include "MCdate.h"
#include "ICP.h"

void MCVoxl(CMCdate* iMCdate);
void vMarchCube(int iX,int iY,int iZ,float fX, float fY, float fZ, float fScale,CMCdate* iMCdate);
int GetCubeValue(int a, int b, int c);
float GetOffset(int iX,int iY,int iZ, int iEdgeIndex);
void vNormalizeVector(Vector &rfVectorResult, Vector &rfVectorSource);
void vGetNormal(Vector &rfNormal, float fX, float fY, float fZ);
SoSeparator* AddFaceItems(int Color, unsigned int* VectorNum, float (*vertices)[3] ,int Num);

//Variable;
int dimX=512;
int dimY=512;
int dimZ=512;
unsigned char *VolumeData;
unsigned char *VolumeData_src;
SoSeparator * root;
SoWinExaminerViewer * viewer;
SoMaterial *m_material;
SoVolumeData* pVolData;	//体数据指针
SoVolumeRender *pVolRender;
int MCStep = 4;
unsigned int* numvertices;	


void SweptVolumeTest(int dx, int dy, int dz);

double p[9] = { 0.9953,0.0077,-0.0966,-0.0068,0.9999,0.0098,0.0967,-0.0091,0.9953 };
double m[3], mul[3];

int main(int, char ** argv)
{
  //Read Binvox////////////////////////////////////////////////
	if (!read_binvox("C://Voxelizer//Engine2.binvox")) {
    cout << "Error reading [" << argv[1] << "]" << endl << endl;
    exit(1);
  }


  HWND window = SoWin::init(argv[0]);
  if (window==NULL) exit(1);

   viewer = new SoWinExaminerViewer(window);
   root = new SoSeparator;
    root->ref();

  
  ////////////////////////////////////////////////////////////////Volume data
  	SoVolumeRendering::init();
	//初始化体数据和临时体数据
	
	dimX=512;
	dimY=512;
	dimZ=512;
	VolumeData=new unsigned char[dimX * dimY * dimZ];
	VolumeData_src=new unsigned char[dimX * dimY * dimZ];

	/*for(int i=0; i<dimZ; i++ ) 
	for(int j=0; j<dimY; j++)
	for(int k=0; k<dimX; k++)
	{
		
		VolumeData[i*dimY*dimX+j*dimX+k]=VolumeData_src[i*dimY*dimX+j*dimX+k]=voxels[i*dimY*dimX+k*dimX+j];
	}*/
	memset(VolumeData, 0, sizeof(unsigned char)*dimX*dimY*dimZ);
	/*for (int i = 1; i <400; i++)
	{
		char str[30];
		sprintf(str, "E:\\slices\\v1_400\\%d.tif", i);
		IplImage *src = cvLoadImage(str, 0);
		for (int j = 1; j <= dimY; j++)
		{
			uchar *ptr = (uchar*)src->imageData + j * dimX;
			for (int cols = 1; cols <= dimX; cols++)
			{
				if (ptr[cols] >=5)
				{
					VolumeData[(i-1)*dimY*dimX + j*dimX + cols] = ptr[cols];
				}
				else
				{
					VolumeData[(i-1)*dimY*dimX + j*dimX + cols] = 0;
				}
				
			}
		}
	}*/
	for (int i = 1; i < 400; i++)
	{
		char str[30];
		sprintf(str, "E:\\slices\\v3_400\\%d.tif", 400-i);
		IplImage *src = cvLoadImage(str, 0);
		for (int j = 1; j <= dimY; j++)
		{
			uchar *ptr = (uchar*)src->imageData + j * dimX;
			for (int cols = 1; cols <= 100; cols++)
			{
				m[0] = (double)cols, m[1] = (double)j, m[2] = (double)(400 - i - 1);
				printf("%lf %lf %lf\n",m[0],m[1],m[2]);
				MatrixMul(m, p, mul, 1, 3, 3, 3);
				printf("%lf %lf %lf\n", mul[0], mul[1], mul[2]);
				mul[0] += (double)58, mul[1] -=(double)9, mul[2] += (double)21;
				printf("%lf %lf %lf\n", mul[0], mul[1], mul[2]);
				int a, b, c;

				a = (int)(mul[0]+512)%512, b = (int)(mul[1]+512)%400, c = (int)(mul[2]+512)%400;
				printf("a=%d b=%d c=%d\n",a,b,c);
				if (ptr[cols] >=5)
				{
					VolumeData[c*dimY*dimX + b*dimX + a]|= 1;
				}
				else
				{
					VolumeData[c*dimY*dimX + b*dimX + a]|= 0;
				}

			}
		}
	}
	
	/*for (int i = 1; i <250; i++)
	{
		char str[30];
		sprintf(str, "E:\\slices\\v2_512\\%d.tif", i);
		IplImage *src = cvLoadImage(str, 0);
		for (int j = 1; j <= dimY; j++)
		{
			uchar *ptr = (uchar*)src->imageData + j * dimX;
			for (int cols = 1; cols <= dimX; cols++)
			{
				if (ptr[cols] >= 5)
				{
					VolumeData[(cols-1)*dimY*dimX + j*dimX + 400-i]|= 1;
				}
				else
				{
					VolumeData[(cols-1)*dimY*dimX + j*dimX + 400-i]|= 0;
				}

			}
		}
	}
	for (int i = 1; i <250; i++)
	{
		char str[30];
		sprintf(str, "E:\\slices\\v4_512\\%d.tif", 400-i);
		IplImage *src = cvLoadImage(str, 0);
		for (int j = 1; j <= dimY; j++)
		{
			uchar *ptr = (uchar*)src->imageData + j * dimX;
			for (int cols = 1; cols <= dimX; cols++)
			{
				if (ptr[cols] >= 5)
				{
					VolumeData[(cols - 1)*dimY*dimX + j*dimX + i]|= 1;
				}
				else
				{
					VolumeData[(cols - 1)*dimY*dimX + j*dimX + i]|= 0;
				}

			}
		}
	}*/



	delete voxels;

	//Swept trail
	/*for(int i=0;i<160;i++)
	{
		if(i<=80)
			SweptVolumeTest(0,2*i,0);
		else
			SweptVolumeTest(-2*(i-80),160,0);
	}*/
	
	
	m_material = new SoMaterial();
	/*m_material->ambientColor.setValue(1.0f,1.0f,1.0f);
	m_material->diffuseColor.setValue(0.9,0.9,0.9);*/

	
	pVolData = new SoVolumeData();
	pVolData->setVolumeData( SbVec3s(dimX, dimY, dimZ), VolumeData);
	pVolData->setVolumeSize( SbBox3f(-dimX, -dimY, -dimZ, dimX, dimY, dimZ) );

	pVolData->storageHint = SoVolumeData::TEX3D;
	pVolData->usePalettedTexture = FALSE;
	
	SoTransferFunction* pTransFunc = new SoTransferFunction;
	pTransFunc->predefColorMap = SoTransferFunction::NONE;
	pTransFunc->reMap(1,254);
	pTransFunc->colorMapType = SoTransferFunction::RGBA;

	
	for (int i=0;i<256;i++)
	  {
			
		  if (i==1)
		  {
			  pTransFunc->colorMap.set1Value(4*i,0.5);
			  pTransFunc->colorMap.set1Value(4*i+1,0.5);
			  pTransFunc->colorMap.set1Value(4*i+2,1.0);
			  pTransFunc->colorMap.set1Value(4*i+3,1.0);
		  }

		  else if (i==3)
		  {
			  pTransFunc->colorMap.set1Value(4*i,0.8);
			  pTransFunc->colorMap.set1Value(4*i+1,0.8);
			  pTransFunc->colorMap.set1Value(4*i+2,0.8);
			  pTransFunc->colorMap.set1Value(4*i+3,1.0);
		  }

		  else if (i==2 || i==12 || i==22)
		  {
			  pTransFunc->colorMap.set1Value(4*i,0.7);
			  pTransFunc->colorMap.set1Value(4*i+1,0.7);
			  pTransFunc->colorMap.set1Value(4*i+2,0.7);
			  pTransFunc->colorMap.set1Value(4*i+3,1.0);
		  }

		  else if (i==5 || i==15 || i==25)
		  {
			  pTransFunc->colorMap.set1Value(4*i,0.73);
			  pTransFunc->colorMap.set1Value(4*i+1,0.23);
			  pTransFunc->colorMap.set1Value(4*i+2,0.13);
			  pTransFunc->colorMap.set1Value(4*i+3,1.0);
		  }

		  else if (i==150)
		  {
			  pTransFunc->colorMap.set1Value(4*i,1.0);
			  pTransFunc->colorMap.set1Value(4*i+1,1.0);
			  pTransFunc->colorMap.set1Value(4*i+2,0.5);
			  pTransFunc->colorMap.set1Value(4*i+3,1.0);
		  }

		  else if (i==101)
		  {
			  pTransFunc->colorMap.set1Value(4*i,0.5);
			  pTransFunc->colorMap.set1Value(4*i+1,0.5);
			  pTransFunc->colorMap.set1Value(4*i+2,1.0);
			  pTransFunc->colorMap.set1Value(4*i+3,0.01);
		  }

		  else if (i==102)
		  {
			  pTransFunc->colorMap.set1Value(4*i,0.5);
			  pTransFunc->colorMap.set1Value(4*i+1,0.5);
			  pTransFunc->colorMap.set1Value(4*i+2,1.0);
			  pTransFunc->colorMap.set1Value(4*i+3,0.1);
		  }

		  else
		  {
			  pTransFunc->colorMap.set1Value(4*i,0.0);
			  pTransFunc->colorMap.set1Value(4*i+1,0.0);
			  pTransFunc->colorMap.set1Value(4*i+2,0.0);
			  pTransFunc->colorMap.set1Value(4*i+3,0.0);
		  }
		 
	  }
	pVolRender = new SoVolumeRender;
	pVolRender->lighting = TRUE;

	root->addChild(m_material);
	root->addChild( pVolData );
	root->addChild( pTransFunc );
	root->addChild( pVolRender );

/*	
	//Marching Cubes////////////////////////////////////////////////////////////////////////////////
	 root->removeAllChildren();
	root->ref();

	 SoShapeHints *sh = new SoShapeHints;
	sh->creaseAngle.setValue(1.0);
	root->addChild(sh);


		CMCdate* iMCdate = new CMCdate();
		CMCdate* temp = new CMCdate();
		MCVoxl(iMCdate);
		//add point to array
		//add a point
		numvertices = new unsigned int[iMCdate->VectorNum];	
		temp = iMCdate->iNextMCdate;

		float  (*iVectorArray)[3];
		iVectorArray = new float[iMCdate->VectorNum] [3];
	

		for(int j=0; j < iMCdate->VectorNum; j++)
		{
			iVectorArray[j][0] = temp->iEdgeVertex.fX;
			iVectorArray[j][1] = temp->iEdgeVertex.fY;
			iVectorArray[j][2] = temp->iEdgeVertex.fZ;
			numvertices[j] = 3;
			temp = temp->iNextMCdate;
		}

		SoTranslation * SubTrans2 = new SoTranslation;
		 SubTrans2->translation.setValue(-dimX/2,-dimY/2,-60-dimZ);

		root->addChild(SubTrans2);
		root->addChild(AddFaceItems(1.0f, numvertices, iVectorArray ,iMCdate->VectorNum));

*/
   viewer->setSceneGraph(root);
  viewer->show();

  SoWin::show(window);
  SoWin::mainLoop();
  delete viewer;
  root->unref();
  return 0;
}




void MCVoxl(CMCdate* iMCdate)
{
	// a cube is 2*2*2 voxels
	int iX, iY, iZ;
	for(iX = 0; iX < (dimX-1)/MCStep; iX++)
		for(iY = 0; iY <(dimY-1)/MCStep; iY++)
			for(iZ = 0; iZ < (dimZ-1)/MCStep; iZ++)
			{
				vMarchCube(iX,iY,iZ,iX*MCStep, iY*MCStep, iZ*MCStep, MCStep , iMCdate);
			}	
}
//search intersectant point on each edge and draw a triangle 
void vMarchCube(int iX,int iY,int iZ,float fX, float fY, float fZ, float fScale,CMCdate* iMCdate)
{
	int iCorner, iVertex,  iTriangle, iFlagIndex, iEdgeFlags;
	int iEdge;
	float fOffset;//intersectant point's offset value
	float aCubeValue[8];//8 vertexs' value
	Vector aEdgeVertex[12];//intersectant point on each edge
	
	CMCdate* Vector;
	
	//Make a local copy of the values at the cube's corners
	for(iVertex = 0; iVertex < 8; iVertex++)
	{
		aCubeValue[iVertex] = GetCubeValue(iX + a2fVertexOffset[iVertex][0],
											 iY + a2fVertexOffset[iVertex][1], 
												iZ + a2fVertexOffset[iVertex][2]);
	}
	//Find which vertices are inside of the surface and which are outside

	iFlagIndex = 0;
	for(iVertex = 0; iVertex < 8; iVertex++)
	{
		if(aCubeValue[iVertex] !=0) 
			iFlagIndex |= 1<<iVertex;
	}
	
	//Find which edges are intersected by the surface
	iEdgeFlags = aiCubeEdgeFlags[iFlagIndex];

	//If the cube is entirely inside or outside of the surface, then there will be no intersections
  if(iEdgeFlags == 0) 
	{
		return;
	}	

  //Find the point of intersection of the surface with each edge
  //Then find the normal to the surface at those points
	for(iEdge = 0; iEdge < 12; iEdge++)
	{
		//if there is an intersection on this edge
		if(iEdgeFlags & (1<<iEdge))
		{
			fOffset = GetOffset(iX,iY,iZ, iEdge);
			
			aEdgeVertex[iEdge].fX = fX + (a2fVertexOffset[ a2iEdgeConnection[iEdge][0] ][0]  +  fOffset * a2fEdgeDirection[iEdge][0]) * fScale;
			aEdgeVertex[iEdge].fY = fY + (a2fVertexOffset[ a2iEdgeConnection[iEdge][0] ][1]  +  fOffset * a2fEdgeDirection[iEdge][1]) * fScale;
			aEdgeVertex[iEdge].fZ = fZ + (a2fVertexOffset[ a2iEdgeConnection[iEdge][0] ][2]  +  fOffset * a2fEdgeDirection[iEdge][2]) * fScale;
		}
	}	

	//Draw the triangles that were found.  There can be up to five per cube
	for(iTriangle = 0; iTriangle < 5; iTriangle++)
	{
		if(a2iTriangleConnectionTable[iFlagIndex][3*iTriangle] < 0)
			break;
		
		for(iCorner = 0; iCorner < 3; iCorner++)
		{
			iVertex = a2iTriangleConnectionTable[iFlagIndex][3*iTriangle+iCorner];
			
			Vector=new CMCdate();
			Vector->SetMCDate(aEdgeVertex[iVertex]);
			Vector->setNextMCdate(iMCdate->iNextMCdate);
			iMCdate->VectorNum ++;
			iMCdate->setNextMCdate(Vector);			
		}
		/////////////////////////////////////
	}
}
int GetCubeValue(int a, int b, int c)
{
 	int Index=(MCStep*c)*dimY*dimX+(MCStep*b)*dimX+(MCStep*a);
	return VolumeData[Index]; 
}
float GetOffset(int iX,int iY,int iZ, int iEdgeIndex)
{
	float i;
	double OffsetValue;
	int TmpValue;
	//////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////
	//edge 0
	if (iEdgeIndex==0)
	{
		TmpValue=VolumeData[MCStep*iZ*dimX*dimY+MCStep*iY*dimX+MCStep*iX];
		for (i = 1;i <= MCStep;i ++)
		{
			OffsetValue=VolumeData[MCStep*iZ*dimX*dimY+MCStep*iY*dimX+MCStep*iX+(int)i];
			if (OffsetValue != TmpValue)
				return OffsetValue=(i-0.5)/MCStep;//go back 1/4
		}
	}
	//edge 1
	else if (iEdgeIndex==1)
	{
		TmpValue=VolumeData[MCStep*iZ*dimX*dimY+MCStep*iY*dimX+MCStep*iX+MCStep];
		for (i = 1;i <= MCStep;i ++)
		{
			OffsetValue=VolumeData[MCStep*iZ*dimX*dimY+MCStep*iY*dimX+MCStep*iX+MCStep+(int)i*dimX];
			if (OffsetValue != TmpValue)
				return OffsetValue=(i-0.5)/MCStep;
		}
	}
	//edge 2
	else if (iEdgeIndex==2)
	{
		TmpValue=VolumeData[MCStep*iZ*dimX*dimY+MCStep*(iY+1)*dimX+MCStep*iX+MCStep];
		for (i = 1;i <= MCStep;i ++)
		{
			OffsetValue=VolumeData[MCStep*iZ*dimX*dimY+MCStep*(iY+1)*dimX+MCStep*iX+MCStep-(int)i];
			if (OffsetValue != TmpValue)
			return OffsetValue=(i-0.5)/MCStep;
		}
	}
	//edge 3
	else if (iEdgeIndex==3)
	{
		TmpValue=VolumeData[MCStep*iZ*dimX*dimY+MCStep*(iY+1)*dimX+MCStep*iX];
		for (i = 1;i <= MCStep;i ++)
		{
			OffsetValue=VolumeData[MCStep*iZ*dimX*dimY+MCStep*(iY+1)*dimX+MCStep*iX-(int)i*dimX];
			if (OffsetValue != TmpValue)
			return OffsetValue=(i-0.5)/MCStep;
		}
	}
	////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////////////////////////
	//edge 4
	else if (iEdgeIndex==4)
	{
		TmpValue=VolumeData[MCStep*(iZ+1)*dimX*dimY+MCStep*iY*dimX+MCStep*iX];
		for (i = 1;i <= MCStep;i ++)
		{
			OffsetValue=VolumeData[MCStep*(iZ+1)*dimX*dimY+MCStep*iY*dimX+MCStep*iX+(int)i];
			if (OffsetValue != TmpValue)
			return OffsetValue=(i-0.5)/MCStep;
		}
	}		
	//edge 5
	else if (iEdgeIndex==5)
	{
		TmpValue=VolumeData[MCStep*(iZ+1)*dimX*dimY+MCStep*iY*dimX+MCStep*iX+MCStep];
		for (i = 1;i <= MCStep;i ++)
		{
			OffsetValue=VolumeData[MCStep*(iZ+1)*dimX*dimY+MCStep*iY*dimX+MCStep*iX+MCStep+(int)i*dimX];
			if (OffsetValue != TmpValue)
			return OffsetValue=(i-0.5)/MCStep;
		}
	}
	//edge 6
	else if (iEdgeIndex==6)
	{
		TmpValue=VolumeData[MCStep*(iZ+1)*dimX*dimY+MCStep*(iY+1)*dimX+MCStep*iX+MCStep];
		for (i = 1;i <= MCStep;i ++)
		{
			OffsetValue=VolumeData[MCStep*(iZ+1)*dimX*dimY+MCStep*(iY+1)*dimX+MCStep*iX+MCStep-(int)i];
			if (OffsetValue != TmpValue)
			return OffsetValue=(i-0.5)/MCStep;
		}
	}
	//edge 7
	else if (iEdgeIndex==7)
	{
		TmpValue=VolumeData[MCStep*(iZ+1)*dimX*dimY+MCStep*(iY+1)*dimX+MCStep*iX];
		for (i = 1;i <= MCStep;i ++)
		{
			OffsetValue=VolumeData[MCStep*(iZ+1)*dimX*dimY+MCStep*(iY+1)*dimX+MCStep*iX-(int)i*dimX];
			if (OffsetValue != TmpValue)
			return OffsetValue=(i-0.5)/MCStep;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////
	//edge 8
	else if (iEdgeIndex == 8)
	{
		TmpValue=VolumeData[MCStep*iZ*dimX*dimY+MCStep*iY*dimX+MCStep*iX];
		for (i = 1;i <= MCStep;i ++)
		{
			OffsetValue=VolumeData[MCStep*iZ*dimX*dimY+MCStep*iY*dimX+MCStep*iX+int(i)*dimX*dimY];
			if (OffsetValue != TmpValue)
				return OffsetValue=(i-0.5)/MCStep;
		}
	}
	//edge 9
	else if ( iEdgeIndex == 9)
	{
		TmpValue=VolumeData[MCStep*iZ*dimX*dimY+MCStep*iY*dimX+MCStep*iX+MCStep];
		for (i = 1;i <= MCStep;i ++)
		{
			OffsetValue=VolumeData[MCStep*iZ*dimX*dimY+MCStep*iY*dimX+MCStep*iX+MCStep+ int(i)*dimX*dimY];
			if (OffsetValue != TmpValue)
				return OffsetValue=(i-0.5)/MCStep;
		}
	}
	//edge 10
	else if( iEdgeIndex == 10)
	{
		TmpValue=VolumeData[MCStep*iZ*dimX*dimY+MCStep*(iY+1)*dimX+MCStep*iX+MCStep];
		for (i = 1;i <= MCStep;i ++)
		{
			OffsetValue=VolumeData[MCStep*iZ*dimX*dimY+MCStep*(iY+1)*dimX+MCStep*iX+MCStep +int(i)*dimX*dimY];
			if (OffsetValue != TmpValue)
				return OffsetValue=(i-0.5)/MCStep;
		}
	}
	//edge 11
	else //( iEdgeIndex == 11)
	{
		TmpValue=VolumeData[MCStep*iZ*dimX*dimY+MCStep*(iY+1)*dimX+MCStep*iX];
		for (i = 1;i <= MCStep;i ++)
		{
			OffsetValue=VolumeData[MCStep*iZ*dimX*dimY+MCStep*(iY+1)*dimX+MCStep*iX +int(i)*dimX*dimY];
			if (OffsetValue != TmpValue)
				return OffsetValue=(i-0.5)/MCStep;
		}
 	}
	return 0.5;
}


void vNormalizeVector(Vector &rfVectorResult, Vector &rfVectorSource)
{
	float fOldLength;
	float fScale;
	
	fOldLength = sqrtf( (rfVectorSource.fX * rfVectorSource.fX) +
		(rfVectorSource.fY * rfVectorSource.fY) +
		(rfVectorSource.fZ * rfVectorSource.fZ) );
	
	if(fOldLength == 0.0)
	{
		rfVectorResult.fX = rfVectorSource.fX;
		rfVectorResult.fY = rfVectorSource.fY;
		rfVectorResult.fZ = rfVectorSource.fZ;
	}
	else
	{
		fScale = 1.0/fOldLength;
		rfVectorResult.fX = rfVectorSource.fX*fScale;
		rfVectorResult.fY = rfVectorSource.fY*fScale;
		rfVectorResult.fZ = rfVectorSource.fZ*fScale;
	}
}

//vGetNormal() finds the gradient of the scalar field at a point
//This gradient can be used as a very accurate vertx normal for lighting calculations
void vGetNormal(Vector &rfNormal, float fX, float fY, float fZ)
{
	rfNormal.fX = GetCubeValue(fX-1, fY, fZ) - GetCubeValue(fX+1, fY, fZ);
	rfNormal.fY = GetCubeValue(fX, fY-1, fZ) - GetCubeValue(fX, fY+1, fZ);
	rfNormal.fZ = GetCubeValue(fX, fY, fZ-1) - GetCubeValue(fX, fY, fZ+1);
	vNormalizeVector(rfNormal, rfNormal);
}

SoSeparator* AddFaceItems(int Color, unsigned int* VectorNum, float (*vertices)[3] ,int Num)
{
	SoSeparator *obelisk = new SoSeparator();
	obelisk->ref();

	// Using the new SoVertexProperty node is more efficient
	SoVertexProperty *myVertexProperty = new SoVertexProperty;
	
	// Define material for obelisk
	myVertexProperty->orderedRGBA.setValue(SbColor(0.8, 0.8f, 0.8f).getPackedValue());

	// Define coordinates for vertices
	myVertexProperty->vertex.setValues(0, Num, (const class SbVec3f *)vertices);

	// Define the FaceSet
	SoFaceSet *myFaceSet = new SoFaceSet;
	myFaceSet->numVertices.setValues(0, Num/3, (const int *)numvertices);
	
	myFaceSet->vertexProperty.setValue(myVertexProperty);
	obelisk->addChild(myFaceSet);
	obelisk->unrefNoDelete();
	return obelisk;
}


void SweptVolumeTest(int dx,int dy, int dz)
{
	int x=dimX;
	int y=dimY;
	int z=dimZ;

	unsigned char *VolumeData_Tmp=new unsigned char[x * y * z];
	
	for(int i=0; i<z; i++ ) 
	for(int j=0; j<y; j++)
	for(int k=0; k<x; k++)
	{
		if((i-dz<0)||(j-dy<0)||(k-dx<0) ||(i-dz>z)||(j-dy>y)||(k-dx>x) )
			VolumeData_Tmp[i*y*x+j*x+k]= 0;
		else
			VolumeData_Tmp[i*y*x+j*x+k]= VolumeData_src[(i-dz)*y*x+(j-dy)*x+(k-dx)];
	}
	

	for(int i=0; i<z; i++ ) 
	for(int j=0; j<y; j++)
	for(int k=0; k<x; k++)
	{
		if(VolumeData[i*y*x+j*x+k]==0 && VolumeData_Tmp[i*y*x+j*x+k]==0)
			VolumeData[i*y*x+j*x+k]=0;
		else
			VolumeData[i*y*x+j*x+k]=1;	
	}

	delete VolumeData_Tmp;
}