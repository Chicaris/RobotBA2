N=100

Kp=30               % v/deg

Rwheel=0.025          % m
GM0=0.4/180*pi*Rwheel % m/s/v
f0=1.5                % Hz

f=logspace(-2,2,N);

% Set initial amplitude => Vout/Vin = Vout
err=ones(1,N);
% Kp
motor_voltage=Kp*err;
% Motor
speed=motor_voltage*GM0./(1+j*f/f0);
position_m=speed./(j*2*pi*f);
% Encoder
position_deg=360*position_m/(2*pi*Rwheel);
    
% Amplitude
subplot(2,1,1);
semilogx(f,20*log10(abs(position_deg)));
xlabel('Frequency (Hz)')
title('Amplitude (dB)')
grid();
% Phase
subplot(2,1,2);
semilogx(f,180/pi*angle(position_deg));
xlabel('Frequency (Hz)')
title('Phase (deg)')
grid();