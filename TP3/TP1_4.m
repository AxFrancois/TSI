close all;
clc;
clear variables;

%% Filtrage linéaire vs non linéaire : débruitage
%% 1
I = imread('flower.png');
I = im2double(I);
GNoise = 0.1*randn(size(I));
I_g = GNoise+I;

SPNoise = imnoise(I,'salt & pepper',0.5);
I_sp = SPNoise+I;

figure()
subplot(3,4,1)
imshow(I, [])
title('I')
subplot(3,4,5)
imshow(I_g, [])
title('I_g')
subplot(3,4,9)
imshow(I_sp, [])
title('I_{sp}')

%% 2
% Moyenneur
n = 5;
Moyenneur = 1/((2*n+1)^2) * ones(n);
I_moyenneur = conv2(Moyenneur,I);
I_g_moyenneur = conv2(Moyenneur,I_g);
I_sp_moyenneur = conv2(Moyenneur,I_sp);
%imshow(I_g_moyenneur,[])
%Butterworth
[h,w] = size(I);
[U,V] = meshgrid(-w/2+1/2:w/2-1/2,-h/2+1/2:h/2-1/2);
D = sqrt(U.^2 + V.^2);
nc = 100;
p = 5;
H = 1./(1+(sqrt(D/nc)).^(2*p));

I_butterworth = abs(ifft2(fftshift(fft2(I)).*H));
I_g_butterworth = abs(ifft2(fftshift(fft2(I_g)).*H));
I_sp_butterworth = abs(ifft2(fftshift(fft2(I_sp)).*H));

%Médian
I_median = ordfilt2(I,round(n^2/2),ones(n));
I_g_median = ordfilt2(I_g,round(n^2/2),ones(n));
I_sp_median = ordfilt2(I_sp,round(n^2/2),ones(n));

%Affichage
subplot(3,4,2)
imshow(I_moyenneur,[])
title('I moyenneur')
subplot(3,4,6)
imshow(I_g_moyenneur,[])
title('I_g moyenneur')
subplot(3,4,10)
imshow(I_sp_moyenneur,[])
title('I_{sp} moyenneur')

subplot(3,4,3)
imshow(I_butterworth,[])
title('I butterworth')
subplot(3,4,7)
imshow(I_g_butterworth,[])
title('I_g butterworth')
subplot(3,4,11)
imshow(I_sp_butterworth,[])
title('I_{sp} butterworth')

subplot(3,4,4)
imshow(I_median,[])
title('I median')
subplot(3,4,8)
imshow(I_g_median,[])
title('I_g median')
subplot(3,4,12)
imshow(I_sp_median,[])
title('I_{sp} median')

% Calculs erreur

