files = dir('analysis/*-cepstrum.out');
names = char(files.name);

for file = names'
    c_raw = read_data(strcat('analysis/',file'));
    h = figure(1);
    imagesc(idct(c_raw,40));
    [path,name,ext] = fileparts(file');
    saveas(h,strcat('spectrograms/',name,'.png'));
end


files = dir('analysis/*-log-mel.out');
names = char(files.name);

for file = names'
    c_raw = read_data(strcat('analysis/',file'));
    h = figure(1);
    imagesc(c_raw);
    [path,name,ext] = fileparts(file');
    saveas(h,strcat('spectrograms/',name,'.png'));
end