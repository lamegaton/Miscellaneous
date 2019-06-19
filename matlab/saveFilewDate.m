% To continuous long statement
% we can use ellipsis (...)
prompt = 'Type a name you want to use: ';
filename = input(prompt,'s');

% Demo
a = [1 2 3; 4 5 6; 7 8 10];
b = a + 6;

DateString = datestr(now,'_mmddyyyy_HHMM');
filename = strcat(filename,DateString);
% Change the folder path to save our file
% and variables that we want to save 
path = 'C:\Users\mapnh\Google Drive\temp\';
save(strcat(path,filename),'a', ...
    'filename')
