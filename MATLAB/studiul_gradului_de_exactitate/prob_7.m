% =========================================================================
% Validarea Gradului de Exactitate (Studiu Comparativ Simpson vs Gauss)
% Ambele metode utilizeaza EXACT 3 noduri.
% Comparam rezultatele pe un polinom de grad 3 si unul de grad 5.
% Interval de integrare asimetric: [0, 2]
% =========================================================================


% --- DATELE PROBLEMEI ---
a = 0;
b = 2;

% Polinomul 1 (Gradul 3) si analiticul sau pe [0, 2]
f1 = @(x) x.^3 + 2*x.^2 - x + 1;
F1 = @(x) (x.^4)/4 + (2*x.^3)/3 - (x.^2)/2 + x;
I_exact_1 = F1(b) - F1(a);

% Polinomul 2 (Gradul 5) si analiticul sau pe [0, 2]
f2 = @(x) 3*x.^5 - 2*x.^4 + x.^3 - 5*x.^2 + 1;
F2 = @(x) (1/2)*x.^6 - (2/5)*x.^5 + (1/4)*x.^4 - (5/3)*x.^3 + x;
I_exact_2 = F2(b) - F2(a);

% --- 1. METODA SIMPSON (3 Noduri) ---
h = (b-a)/2;
x0 = a;
x1 = (a+b)/2;
x2 = b;

% Aplicam Simpson pe ambele functii
I_simp_1 = (h/3) * (f1(x0) + 4*f1(x1) + f1(x2));
I_simp_2 = (h/3) * (f2(x0) + 4*f2(x1) + f2(x2));

% Calculam erorile
Err_simp_1 = abs(I_exact_1 - I_simp_1);
Err_simp_2 = abs(I_exact_2 - I_simp_2);

% --- 2. METODA GAUSS-LEGENDRE (3 Noduri, m=2) ---
% Radacinile si ponderile standard pe [-1, 1] pentru 3 noduri
t0 = -sqrt(3/5);  A0 = 5/9;
t1 = 0;           A1 = 8/9;
t2 = sqrt(3/5);   A2 = 5/9;

% Schimbarea de interval [-1, 1] -> [0, 2]
xg0 = ((b-a)/2)*t0 + (a+b)/2;
xg1 = ((b-a)/2)*t1 + (a+b)/2;
xg2 = ((b-a)/2)*t2 + (a+b)/2;

% Aplicam Gauss pe ambele functii
I_gauss_1 = ((b-a)/2) * (A0 * f1(xg0) + A1 * f1(xg1) + A2 * f1(xg2));
I_gauss_2 = ((b-a)/2) * (A0 * f2(xg0) + A1 * f2(xg1) + A2 * f2(xg2));

% Calculam erorile
Err_gauss_1 = abs(I_exact_1 - I_gauss_1);
Err_gauss_2 = abs(I_exact_2 - I_gauss_2);

% --- AFISARE REZULTATE PENTRU TABELUL DIN LATEX ---
fprintf('===================================================================\n');
fprintf(' REZULTATE EXPERIMENT (3 NODURI DE EVALUARE)\n');
fprintf('===================================================================\n');
fprintf('%-18s | %-20s | %-20s\n', 'Metoda', 'Eroare (Grad 3)', 'Eroare (Grad 5)');
fprintf('-------------------|----------------------|----------------------\n');
fprintf('%-18s | %-20.6e | %-20.6e\n', 'Simpson clasic', Err_simp_1, Err_simp_2);
fprintf('%-18s | %-20.6e | %-20.6e\n', 'Gauss-Legendre', Err_gauss_1, Err_gauss_2);
fprintf('===================================================================\n');