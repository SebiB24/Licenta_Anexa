% --- DATELE PROBLEMEI ---
% Integrala 1
f1 = @(x) sqrt(1+(cos(x)).^2);
a1 = 0;
b1 = pi;
exact_val_1 = integral(f1, a1, b1, 'AbsTol', eps, 'RelTol', eps);
% Integrala 2
f2 = @(x) abs(cos(x));
a2 = 0;
b2 = 3;
exact_val_2 = 2 - sin(3);
% Toleranta
tol = 1e-8;

% 1. Rezultate metoda Trapezului repetata ----------------------------------
[val_trap_1, eval_trap_1] = trapez_repetat(f1, a1, b1, tol, 10);
[val_trap_2, eval_trap_2]= trapez_repetat(f2, a2, b2, tol, 10);
fprintf('Eroarea trapez repetat I1: %.15f\n', abs(exact_val_1 - val_trap_1));
fprintf('Nr de evaluari I1: %d\n', eval_trap_1);
fprintf('Eroarea trapez repetat I2: %.15f\n', abs(exact_val_2 - val_trap_2));
fprintf('Nr de evaluari I2: %d\n', eval_trap_2);

% 2. Rezultate metoda Romberg ----------------------------------------------
% OBS: nu se poata aplica optimizarea de la ex anteriror si aici datorita
% intervalului diferit !!
[val_romb_1, eval_romb_1]= romberg(f1, a1, b1, tol, 10);
[val_romb_2, eval_romb_2]= romberg(f2, a2, b2, tol, 10);
fprintf('Eroarea metodei Romberg I1: %.15f\n', abs(exact_val_1 - val_romb_1));
fprintf('Nr de evaluari I1: %d\n', eval_romb_1);
% OBS: Romberg obtine o eroare mai mica decat trap. rep, dar e in continuare
% considerata un esec
fprintf('Eroarea metodei Romberg I2: %.15f\n', abs(exact_val_2 - val_romb_2));
fprintf('Nr de evaluari I2: %d\n', eval_romb_2);

% 3. Rezultate metoda Simpson Adaptiva -------------------------------------
[val_adapt_1, eval_adapt_1] = simpson_adaptiv(f1, a1, b1, tol, 100);
[val_adapt_2, eval_adapt_2] = simpson_adaptiv(f2, a2, b2, tol, 100);
fprintf('Eroarea metodei Adaptive I1: %.15f\n', abs(exact_val_1 - val_adapt_1));
fprintf('Nr de evaluari I1: %d\n', eval_adapt_1);

fprintf('Eroarea metodei Adaptive I2: %.15f\n', abs(exact_val_2 - val_adapt_2));
fprintf('Nr de evaluari I2: %d\n', eval_adapt_2);