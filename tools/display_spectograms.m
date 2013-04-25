files = dir('recordings/*.wav');
names = char(files.name);

for file = names'
    [data,fs] = wavread(strcat('recordings/',file'));
    h = figure(1);
    specgram(data);
    [path,name,ext] = fileparts(file');
    saveas(h,strcat('spectrograms/',name,'.png'));
end