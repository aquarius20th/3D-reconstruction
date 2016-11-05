
a=zeros(400,400,400);
b=zeros(400,400,400);
for i=1:400
now=imread(strcat('E:\v1_400\',num2str(i),'.tif'));
for j=1:400
    for k=4:100
        if(now(j,k)>=5)
        a(i,j,k)=now(j,k);
        end
    end              
end
end
for i=1:400
now=imread(strcat('E:\v2_400\',num2str(i),'.tif'));
for j=1:400
    for k=1:400
        if(now(j,k)>=5)
        b(401-k,j,i)=now(j,k);
        end
    end
end
end
% for i=1:100
%     for j=1:100
%        for k=1:100
%            result(i,j,k)=max(a(i,j,k),b(i,j,k));
%        end
%     end
% end
        
wavelet='haar';
%·Ö½â²ãÊý
level = 5;
Ca = dwt3(a,'haar');
Cb = dwt3(b,'haar');
% cellnum=size(Ca.dec,1);
% 
% Ra=Ca;
% Ra.dec{1}=(Ca.dec{1}+Cb.dec{1})/2;
% for i=2:cellnum
%     x=size(Ca.dec{i});
%     ma=Ca.dec{i};
%     mb=Cb.dec{i};
%     mc=zeros(x(1),x(2),x(3));
%     for j=1:x(1)*x(2)*x(3)
%         if abs(ma(j))>abs(mb(j))
%             mc(j)=ma(j);
%         else
%             mc(j)=mb(j);
%         end
%     end
%     Ra.dec{i}=mc;
% end
% result = waverec3(Ra);
% for i=1:400
%     imwrite(mat2gray(squeeze(result(i,:,:))),strcat('E:\test\',num2str(i),'.tif'));
% end

% for p=1:x(1)*x(2)*x(3)
%     Ra.dec{1}=(Ca.dec(p)+Cb.dec(p))/2;
% end
% for p=x(1)*x(2)*x(3):x(1)*x(2)*x(3)
%     Ra(p)=(Ca.dec(p)+Cb.dec(p))/2;
% end

    