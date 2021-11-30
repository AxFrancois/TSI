close all;
clc;
clear variables;

%% Filtrage à partir d’histogramme : seuillage par K-means
%% 1
I = imread('flower.png');
I = im2double(I);
figure()
imshow(I, [])

%% 2
K = 10;
[idx,C] = Kmeans(I,K);

V = I(:);
%{
[idx,C] = kmeans(V,K);
%}
Csorted = sort(C);
[counts,centers] = hist(V',0:1/255:1);
Zones = zeros(1,K+1);
Zones(end) = 1;
MatZone = zeros(1,length(Csorted)-1);
for k = 1:length(MatZone)
    MatZone(k) = mean(Csorted(k:k+1));
end
Zones(2:K) = MatZone;
figure()
hold on

for k = 1:K
    index = find(centers>=Zones(k) & centers<Zones(k+1));
    bar(centers(index),counts(index))
    line([Csorted(k) Csorted(k)],[0 2500],'Color','red','LineStyle','--')
end
