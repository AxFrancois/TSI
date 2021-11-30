close all;
clc;
clear variables;

%% Filtrage coupe-bande dans l’espace de Fourier
%% 1
I = imread('journal.png');
%I = imread('images-trames\9-trame.png');
figure()
imshow(I)
[h,w] = size(I);

%% 2
[U,V] = meshgrid(-w/2+1/2:w/2-1/2,-h/2+1/2:h/2-1/2);
D = sqrt(U.^2 + V.^2);
p=2;
nc = 100;
B = 120;
H = 1./(1+(B*D./(D.^2 - nc^2)).^(2*p));
figure()
imshow(H)

%% 3
TF = fftshift(fft2(I));
figure()
imshow(10*log10(abs(TF)), [])
figure()

%% 4
Filtrage = TF.*H;
imshow(10*log10(abs(Filtrage)), [])
figure()

%5
ImFiltre = abs(ifft2(Filtrage));
imshow(ImFiltre, [])

%6
%I = imread('images-trames\9-trame.png');