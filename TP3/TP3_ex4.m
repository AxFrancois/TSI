close all;
clc;
clear variables;

I = imread('mire.png');
figure()
subplot(2,2,1)
imshow(I)
subplot(2,2,2)
IRouge = zeros(size(I));
IRouge(:,:,1) = I(:,:,1);
imshow(IRouge)
subplot(2,2,3)
IVert = zeros(size(I));
IVert(:,:,2) = I(:,:,2);
imshow(IVert)
subplot(2,2,4)
IBleu = zeros(size(I));
IBleu(:,:,3) = I(:,:,3);
imshow(IBleu)