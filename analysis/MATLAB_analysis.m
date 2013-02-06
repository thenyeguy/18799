%% close all heatmap windows

close all force

%% Code
number_names = {'one' 'two' 'three' 'four'};

number_of_number_names = length(number_names);
number_of_recordings_per_number = 3;
subplot_count = 1;

for number_index = 1:length(number_names)
    %number_names{number_index}
    
    for recording_index = 1:number_of_recordings_per_number
        filename = strcat(number_names{number_index},int2str(recording_index),'-cepstrum.out')
        fid = fopen(filename,'r');  % Open text file

        tline=fgets(fid);
        cepstrum_13 = strread(tline,'%s','delimiter',' ');
        cepstrum_string_array = [];

        while tline~=-1,
            cepstrum_string_array = horzcat(cepstrum_string_array , cepstrum_13);

            % Get next line
            tline = fgets(fid); 
            if tline==-1
                break;
            end
            cepstrum_13 = strread(tline,'%s','delimiter',' ');
        end

        idct_pad_length= 128;
        cepstrum_double_array = str2double(cepstrum_string_array);
        idct_of_cepstrum_double_array = idct(cepstrum_double_array,idct_pad_length);

        %size(idct_of_cepstrum_double_array)

        %spectrogram(str2double(cepstrum_13))

        %take each 13, pad to 128, then inverse IDCT

        %%subplot(number_of_number_names,number_of_recordings_per_number,subplot_count);
        hm = HeatMap(idct_of_cepstrum_double_array),title(filename);
        subplot_count = subplot_count+1;
    end
end