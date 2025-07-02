%clear all
%clc
warning off
pkg load instrument-control

receiveFlag = false;
newData = false;

ts = 0.1;
t = 0:ts:10;

maxValue = 1023;
minValue = 0;

value1 = zeros(1, length(t)); % pitch 1
value2 = zeros(1, length(t)); % roll 1
value3 = zeros(1, length(t)); % pitch 2
value4 = zeros(1, length(t)); % roll 2

valueN1 = zeros(1, length(t));
valueN2 = zeros(1, length(t));
valueN3 = zeros(1, length(t));
valueN4 = zeros(1, length(t));

arduino = serial("COM5", 9600);
pause(3);

disp('Recolectando Datos....')

for k = 1:length(t)
    tic;
    srl_flush(arduino);
    C = '';         % Cadena de caracteres recibida
    charRead = '';  % Caracter leído

    % Leer carácter por carácter hasta formar una trama <...>
    while true
        charRead = char(srl_read(arduino, 1));
        if charRead == '<'
            C = ''; % Reinicia cadena
        elseif charRead == '>'
            break; % Finaliza lectura de una trama completa
        else
            C(end+1) = charRead;
        end
    end

    % Intentar dividir en 4 partes
    Data = strsplit(C, ',');
    if length(Data) == 4
        temp1 = str2num(Data{1});
        temp2 = str2num(Data{2});
        temp3 = str2num(Data{3});
        temp4 = str2num(Data{4});

        if ~isempty(temp1) && ~isempty(temp2) && ~isempty(temp3) && ~isempty(temp4)
            value1(k) = temp1;
            value2(k) = temp2;
            value3(k) = temp3;
            value4(k) = temp4;
        else
            if k > 1
                value1(k) = value1(k-1);
                value2(k) = value2(k-1);
                value3(k) = value3(k-1);
                value4(k) = value4(k-1);
            end
        end
    else
        if k > 1
            value1(k) = value1(k-1);
            value2(k) = value2(k-1);
            value3(k) = value3(k-1);
            value4(k) = value4(k-1);
        end
    end

    % Normalización
    valueN1(k) = inputNorm(value1(k), minValue, maxValue);
    valueN2(k) = inputNorm(value2(k), minValue, maxValue);
    valueN3(k) = inputNorm(value3(k), minValue, maxValue);
    valueN4(k) = inputNorm(value4(k), minValue, maxValue);

    % Espera para cumplir con el tiempo de muestreo
    while toc < ts
    end
    tiempo(k) = toc;
end

disp('Datos Recolectados...')
srl_close(arduino)

% Graficación
figure
subplot(321)
plot(t, value1); title('Real pitch MPU1');

subplot(322)
plot(t, valueN1); title('Normalized pitch MPU1');

subplot(323)
plot(t, value2); title('Real roll MPU1');

subplot(324)
plot(t, valueN2); title('Normalized roll MPU1');

subplot(325)
plot(t, value3); title('Real pitch MPU2');

subplot(326)
plot(t, valueN3); title('Normalized pitch MPU2');

% Si quieres graficar también value4 y valueN4, puedes usar otro `figure` adicional
