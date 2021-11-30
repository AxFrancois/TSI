function [labels,mi] = Kmeans(I,K)
[h,w] = size(I);
mi = rand(1,K);
mi = sort(mi);
mibuff = zeros(1,K);
iteration = 0;
V = I(:);

while iteration < 100
    labels = zeros(1,h*w);
    misum = zeros(1,K);
    micount = zeros(1,K);
    for pixel = 1:length(V)
        [minValue,closestIndex] = min(abs(V(pixel) - mi));
        labels(pixel) = closestIndex;
        misum(closestIndex) = misum(closestIndex) + V(pixel);
        micount(closestIndex) = micount(closestIndex)+1;
    end
    mibuff = mi;
    mi = misum./micount;
    iteration = iteration+1;
    if sum(abs(mibuff-mi)) == 0
        break
    end
end
iteration
end

