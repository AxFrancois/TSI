close all;
clc;
clear variables;

snrMat = zeros(1,8);
isoMat = zeros(1,8);
figure()
for i = 1:8
    Iso = 100*2^(i-1);
    fname = sprintf('iso%d.jpg', Iso);
    I = imread(fname);
    subplot(8,3,1+3*(i-1))
    imshow(I)
    subplot(8,3,2+3*(i-1))
    IGris = rgb2gray(I);
    imshow(IGris)
    
    reg = IGris(1025:1130, 1330:1545);
    snr = 20*log10(mean2(reg)/std2(reg));
    subplot(8,3,3+3*(i-1))
    imshow(reg)
    isoMat(i) = Iso;
    snrMat(i) = snr;
end

figure()
plot(isoMat,snrMat)

figure()
subplot(2,2,1)
imshow(IGris, [])
title('I')
% Moyenneur
n = 5;
Moyenneur = 1/((2*n+1)^2) * ones(n);
I_moyenneur = conv2(Moyenneur,IGris);

%Butterworth
[h,w] = size(IGris);
[U,V] = meshgrid(-w/2+1/2:w/2-1/2,-h/2+1/2:h/2-1/2);
D = sqrt(U.^2 + V.^2);
nc = 100;
p = 5;
H = 1./(1+(sqrt(D/nc)).^(2*p));

I_butterworth = abs(ifft2(fftshift(fft2(IGris)).*H));

%Médian
I_median = ordfilt2(IGris,round(n^2/2),ones(n));

%Affichage
subplot(2,2,2)
imshow(I_moyenneur,[])
title('I moyenneur')

subplot(2,2,3)
imshow(I_butterworth,[])
title('I butterworth')

subplot(2,2,4)
imshow(I_median,[])
title('I median')
