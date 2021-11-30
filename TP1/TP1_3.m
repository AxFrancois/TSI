close all;
clc;
clear variables;

%% Filtrage passe-haut dans l’espace direct : détection de contours

%% 1
I = imread('flower.png');
I = im2double(I);
%X = 0.1*randn(size(I));
%I = X+I;

Derivateur = [-1 0 1];
LissageGaussien = [1; 2; 1];

GammaX = conv2(Derivateur,LissageGaussien);
GammaY = conv2(LissageGaussien',Derivateur');

%% 2
Gh = conv2(GammaX,I);
Gv = conv2(GammaY,I);

%% 3
G = abs(Gh.^2 + Gv.^2);

subplot(2,2,1)
imshow(I, [])
subplot(2,2,2)
imshow(Gh, [])
title('Gh')
subplot(2,2,3)
imshow(Gv, [])
title('Gv')
subplot(2,2,4)
imshow(G, [])
title('G')

%% 4
%Ligne 10 et 11