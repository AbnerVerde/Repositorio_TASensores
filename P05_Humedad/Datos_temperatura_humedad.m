% Configurar puerto serie
puerto = "COM7"; % <-- Cambia al puerto correcto
baudrate = 9600;

s = serialport(puerto, baudrate);
flush(s);

% Inicializar arrays
N = 500;
humedad = zeros(1, N);
temp_lm35 = zeros(1, N);
temp_term = zeros(1, N);
temp_aht  = zeros(1, N);

disp("Iniciando adquisición de datos...");

for i = 1:N
    linea = readline(s);
    % Eliminar espacios y saltos de línea
    linea = strtrim(linea);

    try
        % Extraer valores con expresiones regulares
        h = sscanf(linea, 'H:%f T_LM35:%f T_TERM:%f T_AHT:%f');
        if numel(h) == 4
            humedad(i)     = h(1);
            temp_lm35(i)   = h(2);
            temp_term(i)   = h(3);
            temp_aht(i)    = h(4);
        else
            disp("Lectura inválida, reintentando...");
            i = i - 1; % Reintentar lectura
        end
    catch
        disp("Error de lectura, reintentando...");
        i = i - 1;
    end

    pause(1); % Espera de 1 segundo entre muestras
end

% Cerrar puerto
clear s;

% Graficar humedad vs temperaturas
figure;
hold on;
plot(temp_lm35, humedad, 'r', 'DisplayName', 'LM35');
plot(temp_term, humedad, 'b', 'DisplayName', 'Termistor');
plot(temp_aht, humedad,  'g', 'DisplayName', 'AHT20');
xlabel('Temperatura (°C)');
ylabel('Humedad (%)');
title('Humedad vs Temperatura');
legend show;
grid on;
