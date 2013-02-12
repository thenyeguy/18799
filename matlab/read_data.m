function data = read_data(file)
    fid = fopen(file,'r');
    tline = fgets(fid);
    data_line = strread(tline,'%s','delimiter',' ');
    data = [];
    
    while tline ~= -1,
        if(strcmp(data_line(1), 'nan') == 0)
            data = horzcat(data,data_line);
        end
        tline = fgets(fid);
        if tline==-1
            break;
        end
        data_line = strread(tline,'%s','delimiter',' ');
    end
    data = str2double(data);
end