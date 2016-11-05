// Volume5-6.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#define vtkRenderingCore_AUTOINIT 3(vtkInteractionStyle,vtkRenderingFreeType,vtkRenderingOpenGL)
#define vtkRenderingVolume_AUTOINIT 1(vtkRenderingVolumeOpenGL)
#include"vtkSmartPointer.h"
#include"vtkTIFFReader.h"
#include"vtkImageShrink3D.h"
#include"vtkPiecewiseFunction.h"
#include"vtkColorTransferFunction.h"
#include"vtkVolumeProperty.h"
#include"vtkVolumeRayCastCompositeFunction.h"//合成体绘制函数
#include"vtkVolumeRayCastMapper.h"
#include"vtkVolume.h"
#include"vtkCamera.h"
#include"vtkRenderer.h"
#include"vtkRenderWindow.h"
#include"vtkRenderWindowInteractor.h"
#include"vtkPLYWriter.h"
#include"vtkPolyDataMapper.h"
//std::string filename = "fish1";
int main()
{
	vtkSmartPointer<vtkTIFFReader> reader = vtkSmartPointer<vtkTIFFReader>::New();
	reader->SetFilePrefix("E://Graduate Project//xy_v1234_10_cut//");
	reader->SetFilePattern("%s%d.tif");
	reader->SetDataScalarTypeToChar();
	reader->SetDataByteOrderToLittleEndian();
	reader->SetDataSpacing(0.3, 0.3, 0.3);
	reader->SetDataExtent(0, 381, 0, 381, 20, 380);
	reader->SetDataOrigin(0, 0, 0);
	reader->Update();
	vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
	mapper->SetInputConnection(reader->GetOutputPort());

	//vtkSmartPointer<vtkPLYWriter> writer = vtkSmartPointer<vtkPLYWriter>::New();
	//writer->SetFileName(filename.c_str());
	//writer->SetInputConnection(mapper->GetOutputPort());
	//writer->Write();
	vtkSmartPointer<vtkImageShrink3D> shrink = vtkSmartPointer<vtkImageShrink3D>::New();
	shrink->SetInputConnection(reader->GetOutputPort());
	shrink->SetShrinkFactors(1, 1, 1);
	shrink->Update();
	//给数据点赋不透明值。体绘制中，需要显示三维数据场的内部结构，因而需要生成具有透明度的图像
	vtkSmartPointer<vtkPiecewiseFunction> transfer = vtkSmartPointer<vtkPiecewiseFunction>::New();
	for (int i = 0; i <= 255; i++)
	{
		transfer->AddPoint(i, (double)i / 400);
	}

	//对于不同的组织，涂上不同的颜色
	vtkSmartPointer<vtkColorTransferFunction> colour = vtkSmartPointer<vtkColorTransferFunction>::New();
	for (int i = 0; i <= 255; i++)
	{
		colour->AddRGBPoint(i, 0,0.3+double(i) / 100, 0);
	}

	vtkSmartPointer<vtkVolumeProperty> porperty = vtkSmartPointer<vtkVolumeProperty>::New();
	porperty->SetColor(colour);
	porperty->SetScalarOpacity(transfer);
	porperty->ShadeOn();
	//porperty->SetInterpolationTypeToLinear();//采用线性插值
	//porperty->SetInterpolationTypeToNearest();
	porperty->SetInterpolationType(2);
	vtkSmartPointer<vtkVolumeRayCastCompositeFunction> compositefuc = vtkSmartPointer<vtkVolumeRayCastCompositeFunction>::New();
	vtkSmartPointer<vtkVolumeRayCastMapper>volumemapper = vtkSmartPointer<vtkVolumeRayCastMapper>::New();
	volumemapper->SetVolumeRayCastFunction(compositefuc);//载入体绘制方法
	volumemapper->SetInputConnection(shrink->GetOutputPort());//载入数据
	volumemapper->SetSampleDistance(1);//设置采样间隔
	vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
	volume->SetMapper(volumemapper);
	volume->SetProperty(porperty);
	vtkSmartPointer<vtkCamera>aCamera = vtkSmartPointer<vtkCamera>::New(); //定义摄像机
	aCamera->SetViewUp(0, 0, -1); //取得摄像机方向
	aCamera->SetPosition(0, 1, 0); //光源位置
	aCamera->SetFocalPoint(0, 0, 0); //取焦点坐标
	aCamera->ComputeViewPlaneNormal();
	vtkSmartPointer<vtkRenderer> render = vtkSmartPointer<vtkRenderer>::New();
	render->AddActor(volume);
	render->SetBackground(1, 1, 1);
	vtkSmartPointer<vtkRenderWindow> rewin = vtkSmartPointer<vtkRenderWindow>::New();
	rewin->AddRenderer(render);
	vtkSmartPointer<vtkRenderWindowInteractor> iren = vtkSmartPointer<vtkRenderWindowInteractor>::New();
	iren->SetRenderWindow(rewin);
	iren->Initialize();
	iren->Start();
	return 0;


}
