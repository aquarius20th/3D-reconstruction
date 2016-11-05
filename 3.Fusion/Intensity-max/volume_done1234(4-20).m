

p31=[ 0.999738, -0.0164256, -0.0161053;
		0.0165795, 0.999819, 0.00948525;
		0.0159466, -0.00974987, 0.999826];
p32=[0.999967, 0.001114429, -0.00837694;
		-0.00114611, 1, -0.000169035;
		0.00837672, 0.000178437, 0.999966];
p3=[0.999588,-0.015314,	-0.02447672;
    0.0155125,0.999839,	0.009177;
    0.02433251,	-0.00955369,0.999660];
s31 =[0.996194698, 0.08715574;
    -0.08715574, 0.996194698];
p4=[0.999384, 0.0346903, 0.00565249;
		-0.0347993, 0.999183, 0.0206506;
		-0.00493163, -0.0208353, 0.999773];
s3 =[0.990268, -0.1391731;
    0.1391731, 0.990268];
s40=[0.7071, -0.7071;
	0.7071, 0.7071];
s41=[0.939692, 0.342020;
	-0.342020, 0.939692];
volume=zeros(400,400,400);
volume1=zeros(400,400,400);
volume2=zeros(400,400,400);
volume3=zeros(400,400,400);
volume4=zeros(400,400,400);

for i=1:400
now=imread(strcat('E:\v3_400_deconv\',num2str(401-i),'.tif'));
for j=1:400
    for k=1:400
        array=zeros(1,3);
        array(1)=k;array(2)=j;array(3)=i;
        %mul=array;
         mul=array*p3;
%         m1(1)=mul(2)/2;m1(2)=mul(3)/2;
%          mul1=m1*s3;
%          mul(2)=mul1(1)+mul(2)/2;mul(3) = mul1(2) + mul(3) / 2;
        %array(1)=mul(1)-36;array(2)=mul(2)-15;%array(3)=mul(3)+2;
        
%          m1(1)=mul(1)/2;m1(2)=mul(2)/2;
%          mul1=m1*s31;
%          mul(1)=mul1(1)+mul(1)/2;mul(2) = mul1(2) + mul(2) / 2;
        array(1)=mul(1)-32;array(2)=mul(2);array(3)=mul(3)-7;
        array=round(array);
            if  (array(1)>0&&array(2)>0&&array(3)>0&&array(1)<=400&&array(2)<=400&&array(3)<=400&&now(j,k)&&now(j,k)>=5)
                    volume3(array(3),array(2),array(1))=now(j,k);
            end
   end
end
end



for i=1:400
now=imread(strcat('E:\v4_400_deconv\',num2str(i),'.tif'));
for j=1:400
    for k=1:400
        m=zeros(1,3);
        m(1)=401-i;m(2)=j;m(3)=k;
        mul=m*p4;
%          m1(1)=mul(3)/2;m1(2) = mul(2)/2;
%  		mul1 = m1*s40;
%  		mul(3) = mul1(1) + mul(3) / 2; mul(2) = mul1(2) + mul(2) / 2;
% 		m1(1) = mul(1) / 2; m1(2) = mul(2)/ 2;
% 		mul1 = m1*s41;
% 		mul(1) = mul1(1)+ mul(1) / 2; mul(2) = mul1(2) + mul(2) / 2;
%  		mul(3)= mul(3)-50; 
         mul(2)=mul(2)-4; 
         mul(1)= mul(1)-17;
        m=round(mul);
        if  (m(1)>0&&m(2)>0&&m(3)>0&&m(1)<=400&&m(2)<=400&&m(3)<=400&&now(j,k)>=5)
                volume4(m(3),m(2),m(1))=now(j,k);
        end
     end
end
end

for i=1:400
now=imread(strcat('E:\v1_400_deconv\',num2str(i),'.tif'));
for j=1:400
    for k=1:400
        if(now(j,k)>=5)
        volume1(i,j,k)=now(j,k);
        end
    end              
end
end
for i=1:394
now=imread(strcat('E:\v2_400_deconv\',num2str(i),'.tif'));
for j=1:400
    for k=1:390
        if(now(j,k)>=5)
        volume2(391-k,j,i+6)=now(j,k);
        end
    end
end
end

volume=max(max(volume1,volume2),max(volume3,volume4));
% for i=1:400
%     for j=1:400
%         for k=1:400
%             volume(i,j,k)=max(





for i=1:400
    imwrite(mat2gray(squeeze(volume(i,:,:))),strcat('E:\xy_V1234_deconv_biger5\',num2str(i),'.tif'));
end

