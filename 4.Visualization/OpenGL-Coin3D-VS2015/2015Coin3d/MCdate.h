// MCdate.h: interface for the CMCdate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCDATE_H__3AC77C58_0E9B_44A6_9181_F3076EE10545__INCLUDED_)
#define AFX_MCDATE_H__3AC77C58_0E9B_44A6_9181_F3076EE10545__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "MC.h"

class CMCdate  
{
public:
	CMCdate();
	virtual ~CMCdate();
	int VectorNum;
	Vector iEdgeVertex;
	CMCdate* iNextMCdate;
private:
	

public:
	void setNextMCdate(	CMCdate* aNextMCdate);	
	void SetMCDate(Vector aEdgeVertex);
};

#endif // !defined(AFX_MCDATE_H__3AC77C58_0E9B_44A6_9181_F3076EE10545__INCLUDED_)
