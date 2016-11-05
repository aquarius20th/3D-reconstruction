// MCdate.cpp: implementation of the CMCdate class.
//
//////////////////////////////////////////////////////////////////////
#include"stdafx.h"
#include "MCdate.h"
#define NULL 0;

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMCdate::CMCdate()
{
	iNextMCdate=NULL;
	VectorNum = 0;
}

CMCdate::~CMCdate()
{

}
void CMCdate::setNextMCdate(CMCdate* aNextMCdate)
{
	iNextMCdate = aNextMCdate;
}
void CMCdate::SetMCDate(Vector aEdgeVertex)
{
	iEdgeVertex.fX = aEdgeVertex.fX;
	iEdgeVertex.fY = aEdgeVertex.fY;
	iEdgeVertex.fZ = aEdgeVertex.fZ;
}
