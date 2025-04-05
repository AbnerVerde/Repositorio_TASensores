%clear all
%clc
warning off

pkg load instrument-control

receiveFlag = false;
newData = false;

ts=0.1;
t=0:0.1:10;

maxValue=1023; %valor máximo
minValue=0; %valor minimo

value1=zeros(1,length(t)); %Datos tomados de posición
value2=zeros(1,length(t));

valueN1=zeros(1,length(t)); %Datos tomados de posición para hacer el arreglo
valueN2=zeros(1,length(t));

arduino=serial("COM4",9600) %entrada del arduino COM y velocidad a la que correrá

pause(10); %delay espera de 10

disp('Recolectando Datos....') %Imprime cuando ya se haya conectado el sensor y este por empezar a colectar datosd
for k=1:length(t)
  tic;
  srl_flush(arduino);
  data = srl_read(arduino,11); %leer datos del Arduino
  C=char(data)
  receiveFlag = false;
  for i=1:length(C)
    if receiveFlag == true
      if C(i) != '>'
        Aux(i-1)=C(i);
      else
        newData=true;
      end
    elseif C(i) == '<'
      receiveFlag = true;
    end
  end

  if newData == true
    Data=strsplit(Aux,',');
    value1(k)=str2num(Data{1,1});
    value2(k)=str2num(Data{1,2});
    newData=false;
  else
  value1(k)=value1(k-1);
  value2(k)=value2(k-1);
  end
  valueN1(k)=inputNorm(value1(k),minValue,maxValue);%guarda los datos y llama a el código de InputNorm
  valueN2(k)=inputNorm(value2(k),minValue,maxValue);
  while toc<ts
  end
  tiempo(k)=toc;
end
disp('Datos Recolectados...')%imprime cuando finalizo de recolectar los datos
srl_close(arduino)

%Código para realizar la gráfica de las posciones de pitch y roll recién tomadas
subplot(221)
plot(t,value1);
title('Real pitch angle');
subplot(222)
plot(t,valueN1);
title('Normalized pitch angle');
subplot(223)
plot(t,value2);
title('Real roll angle');
subplot(224)
plot(t,valueN2);
title('Normalized roll angle');

