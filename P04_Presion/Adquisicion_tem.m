% Configuración
puerto = 'COM7';       % Cambia al puerto de tu Arduino
baudRate = 9600;
muestras = 500;        % 500 muestras
frecuencia = 1;        % 1 Hz (1 muestra/segundo)
nombreArchivo = 'datos_temperatura2.txt';

% Abrir archivo .txt
fid = fopen(nombreArchivo, 'w');
fprintf(fid, 'Tiempo(s)\tTemperatura(C)\n');  % Encabezado

% Iniciar comunicación serial
try
    s = serial(puerto, 'BaudRate', baudRate);
    fopen(s);
    disp('Comenzando adquisición...');
    
    tStart = tic;  % Cronómetro inicio
    
    for i = 1:muestras
        % Leer temperatura desde Arduino
        linea = fgetl(s);
        temperatura = str2double(linea);
        
        % Calcular tiempo actual
        tiempo = toc(tStart);
        
        % Guardar en .txt
        fprintf(fid, '%.2f\t%.2f\n', tiempo, temperatura);
        
        % Mostrar en consola
        fprintf('Muestra %d: Tiempo = %.2f s, Temp = %.2f °C\n', i, tiempo, temperatura);
        
        % Esperar hasta el próximo segundo (sincronización precisa)
        pause(1 - mod(tiempo, 1));
    end
    
catch ME
    disp('Error:');
    disp(ME.message);
end

% Cerrar conexión y archivo
fclose(s);
delete(s);
clear s;
fclose(fid);
disp('Datos guardados en datos_temperatura.txt');