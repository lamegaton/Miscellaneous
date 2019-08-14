arduino=serial('COM3','BaudRate',9600);
fopen(arduino)
fprintf(arduino,'1');
fclose(arduino);
