close all;
clc;
clear variables;

%% 2 Transformations d’histogramme et segmentation

%% 1 
I = imread('pieces.png');
%I = imread('images-trames\9-trame.png');
I = im2double(I);
figure()
imshow(I)
[h,w] = size(I);

%% 2 
K = 2;
[idx,C] = Kmeans(I,K);


V = I(:);
Csorted = sort(C);
[counts,centers] = imhist(I);

counts = counts./(h*w);
Zones = zeros(1,K+1);
Zones(end) = 1;
MatZone = zeros(1,length(Csorted)-1);
for k = 1:length(MatZone)
    MatZone(k) = mean(Csorted(k:k+1));
end
Zones(2:K) = MatZone;

I2 = I;
for k = 1:K
    I2(Zones(k)<I & I<Zones(k+1)) = (k-1)/(K-1);
end
figure()
subplot(2,1,1)
imshow(I2)

%% 3
subplot(2,1,2)
hold on

for k = 1:K
    index = find(centers>=Zones(k) & centers<Zones(k+1));
    bar(centers(index),counts(index))
    line([Csorted(k) Csorted(k)],[0 1],'Color','red','LineStyle','--')
end

cumul = cumsum(counts);
plot(centers,cumul)

%% 4 

image_egalise = zeros(size(I));
for k = 1:h
    for j = 1:w
        index = find(centers<I(k,j));
        image_egalise(k,j) = 1/(h*w) * sum(counts(1:index(end)));
    end
end

image_egalise = histeq(I);
figure()
imshow(image_egalise);
[idx,C] = Kmeans(image_egalise,K);

V = image_egalise(:);
Csorted = sort(C);
[counts,centers] = imhist(image_egalise);
counts = counts./(h*w);
Zones = zeros(1,K+1);
Zones(end) = 1;
MatZone = zeros(1,length(Csorted)-1);
for k = 1:length(MatZone)
    MatZone(k) = mean(Csorted(k:k+1));
end
Zones(2:K) = MatZone;

I3 = image_egalise;
for k = 1:K

    I3(Zones(k)<image_egalise & image_egalise<Zones(k+1)) = (k-1)/(K-1);
end
figure()
subplot(2,1,1)
imshow(I3)

subplot(2,1,2)
hold on

for k = 1:K
    index = find(centers>=Zones(k) & centers<Zones(k+1));
    bar(centers(index),counts(index))
    line([Csorted(k) Csorted(k)],[0 1],'Color','red','LineStyle','--')
end

cumul = cumsum(counts);
plot(centers,cumul)

%% 3 Morphologie mathématique : granulométrie

%% 1
se = strel('disk',2);
closeBW = logical(imopen(I3,se));
figure, imshow(closeBW)

%% 2 et 3 
% On utilise un marker sur tout le bord pour éliminer les pièces
[h,w] = size(closeBW);
marker = false(size(closeBW));
marker(1,:) = true;
marker(:,1) = true;
marker(h,:) = true;
marker(:,w) = true;
im = imreconstruct(marker,closeBW);
%{
im_buff = marker;
i=0;
while i<30
    i = i+1;
	marker = imdilate(marker,strel('disk',i));
    im = closeBW .* ~marker;
    figure, imshow(im)
    if im == im_buff
        i
        break
    else
        im_buff = im;
    end
    
end
%}

figure, imshow(im)
im_3_2 = closeBW - im;
figure, imshow(im_3_2)

%{
buff = bweuler(im_3_2);
histo = zeros(1,min(size(im_3_2)));
for k = 1:min(size(im_3_2))
    se = strel('disk',k);
    test = logical(imopen(im_3_2,se));
    if bweuler(test) ~= buff
        disp([num2str(buff-bweuler(test)), ' pièce de taille ' , num2str(k)]);
        histo(k) = buff-bweuler(test);
        buff = bweuler(test);
        figure,imshow(test);
    end
    if bweuler(test) == 0
        break
    end
end 

figure,bar(histo)
%}