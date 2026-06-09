% --- DATELE PROBLEMEI ---
f = @(x) sqrt(x);
a = 0; b = 1;
exact_val = 2/3; 
% Numarul de iteratii pentru Romberg
n_max = 5; 


% --- 1. METODA ROMBERG STANDARD ---
R_std = zeros(n_max, n_max);

% Pasul initial
h = b - a; 
R_std(1,1) = (h/2) * (f(a) + f(b));

% Completarea primei coloane
for j = 2:n_max
    h_j = (b - a) / (2^(j-1)); 
    numar_noduri_noi = 2^(j-2);
    
    % calculearea sumei
    suma = 0;
    for i = 1:numar_noduri_noi
        x_nou = a + (2*i - 1) * h_j;
        suma = suma + f(x_nou);
    end
    
    % aplicarea ecuației
    R_std(j,1) = 0.5 * R_std(j-1,1) + h_j * suma;
end

% Aplicarea Extrapolării Romberg clasică pe restul tabelului
for k = 2:n_max
    for j = k:n_max
        R_std(j,k) = (4^(k-1) * R_std(j,k-1) - R_std(j-1,k-1)) / (4^(k-1) - 1);
    end
end


% --- 2. METODA ROMBERG MODIFICATĂ ---
R_mod = zeros(n_max, n_max);
zeta_val = zeta(-0.5); 

% Generarea primei coloane aplicand corectia matematica
for j = 1:n_max
    h = (b-a)/(2^(j-1));
    % Scaderea termenul care blocheaza convergenta (singularitatea) !
    R_mod(j,1) = R_std(j,1) - zeta_val * h^(1.5);
end

% 2. Aplicarea extrapolarii Romberg pe noile valori curatate
for k = 2:n_max
    for j = k:n_max
        R_mod(j,k) = (4^(k-1) * R_mod(j,k-1) - R_mod(j-1,k-1)) / (4^(k-1) - 1);
    end
end

% --- AFISARE REZULTATE ---
disp('--- Eroarea Metoda STANDARD ---');
disp(abs(R_std - exact_val));
disp(' ');
disp('--- Eroarea Metoda MODIFICATA ---');
disp(abs(R_mod - exact_val));