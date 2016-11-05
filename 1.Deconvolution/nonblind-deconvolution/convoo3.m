
I3=imread('E:\\PSF\\measured_PSF_mv_fusion_gray_20.tif');
for i=1:400
fn=strcat('E:\v4_400\',num2str(i),'.tif');
I=imread(fn);
[W3,S3]=deconvblind(I,I3);
figure;
imshow(W3);
f=getframe;
imwrite(f.cdata,strcat('E:\v4_400_deconvo3\',num2str(i),'.tif'));
close all
end