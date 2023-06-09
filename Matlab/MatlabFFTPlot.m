close all; clear all; delete(instrfind); clc

disp('Iniciando Comunicación con ESP32...')
puerto='COM3';
BAUDRATE=115200;
ENCABEZADO_TRAMA = -11;
FIN_TRAMA = -22;
SAMPLES = 1024;
FREQ = 12600;
VECTOR_FREQ = (0:1:SAMPLES-1)* FREQ ./ SAMPLES /2;

ESP32=serial(puerto,'BaudRate',BAUDRATE); 

set(ESP32,'InputBufferSize',2048);
fopen(ESP32);
vReal = zeros(SAMPLES,1);
if(get(ESP32,'BytesAvailable')>0)
    disp(getl(s))
end

try
    %%Plot of Original and OMP Signals
    figure(1); clf;
    axis([1 SAMPLES -3 1000]);
    h=xlabel('index [1 to m]');
    %legend('FFT')
    set(h,'FontSize',11);
    ylabel('Amplitude');
    set(h,'FontSize',11);
    set(gca,'FontSize',11);
       
    while(1) %j = 1: 10
        i = 1;
        encabezadoEncontrado = false;
        finEncontrado = false;
        %fprintf('Iteracion %d\n', j);
        fprintf(ESP32,'S'); % Pedir a la ESP que mande datos.
        pause(0.1);
        while(get(ESP32,'BytesAvailable')>0)    %% Levanta datos que pueden haber quedado colgados en el buffer
            if(fscanf(ESP32,'%d') == ENCABEZADO_TRAMA)
                encabezadoEncontrado = true;
                for i = 1:1:SAMPLES
                    dato = fscanf(ESP32,'%d');
                    vReal(i) = dato;
                    
                end
                
            end
                        
        end
        vReal(1) = 0;
        vReal(2) = 0;
        vReal(1024) = 0;
        if(encabezadoEncontrado)
            h2 = stem(VECTOR_FREQ(1:SAMPLES/2),vReal(1:SAMPLES/2),'.r');
        end
        

        
        
    end
    
catch exception % Si durante la ejecución se produce algún error aparece acá.
    disp('Ups Loop')
    disp(strcat('Error: ',exception.message))
end
    fclose(ESP32);
    disp('Terminado')
    