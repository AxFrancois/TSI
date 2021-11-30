close all;
clc;
clear variables;

I = imread('mire.png');
IHSV = rgb2hsv(I);
figure()
imshow(IHSV)
