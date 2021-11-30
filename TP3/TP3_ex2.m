close all;
clc;
clear variables;

I = imread('iso100.jpg');
I = rgb2gray(I);
figure()
reg = I;%(920:950,570:1715);
subplot(3,3,2)
imshow(reg)
subplot(3,3,4)
Iresized = imresize(reg, 0.25);
imshow(Iresized)
subplot(3,3,5)
IresizedBili = imresize(reg, 0.25, 'bilinear');
imshow(IresizedBili)
subplot(3,3,6)
IresizedBicu = imresize(reg, 0.25, 'bicubic');
imshow(IresizedBicu)
subplot(3,3,7)
Iresized2 = imresize(Iresized, 4);
imshow(Iresized2)
subplot(3,3,8)
Iresized2Bili = imresize(IresizedBili, 4, 'bilinear');
imshow(Iresized2Bili)
subplot(3,3,9)
Iresized2Bicu = imresize(IresizedBicu, 4, 'bicubic');
imshow(Iresized2Bicu)