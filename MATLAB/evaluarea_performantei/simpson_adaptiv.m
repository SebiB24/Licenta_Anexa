function [I, nr_eval] = simpson_adaptiv(f, a, b, tol, n_max)
    % SIMPSON_ADAPTIV Calculeaza integrala definita folosind Cuadratura Simpson Adaptiva (locala).
    % f      - functia de integrat (function handle)
    % a, b   - limitele de integrare globale
    % tol    - toleranta dorita pentru oprire
    % n_max  - adancimea maxima permisa pentru recursivitate (limita de siguranta)
    % Returneaza: I (valoarea integralei), nr_eval (numarul total de apeluri f(x))
    
    % Initializarea primei estimari Simpson
    c = (a + b) / 2;
    fa = f(a); 
    fc = f(c); 
    fb = f(b);
    
    S_initial = (b - a) / 6 * (fa + 4 * fc + fb);
    [I, eval_rec] = recursie_simpson(f, a, b, tol, S_initial, fa, fc, fb, 1, n_max);
    % Total evaluari (+ 3 pentru f(a), f(c) si f(b))
    nr_eval = 3 + eval_rec;
end


function [I, evals] = recursie_simpson(f, a, b, tol, S, fa, fc, fb, n, n_max)
    % RECURSIE_SIMPSON Functie interna ce implementeaza logica divide-et-impera.
    % Foloseste extrapolarea Richardson (impartirea la 15) pentru a estima eroarea locala.
    
    % c este vechiul mijloc, d si e sunt noile sferturi de interval
    c = (a + b) / 2;
    h = b - a;
    d = (a + c) / 2;
    e = (c + b) / 2;

    fd = f(d); 
    fe = f(e);
    evals = 2;
    
    % Aplicarea metodei Simpson pe jumatatea stanga si jumatatea dreapta
    S_stanga = (h / 2) / 6 * (fa + 4 * fd + fc);
    S_dreapta = (h / 2) / 6 * (fc + 4 * fe + fb);
    
    % Verificarea preciziei
    eroare = abs(S_stanga + S_dreapta - S) / 15;
    if eroare < tol || n >= n_max   
        % Returnam rezultatul 
        I = S_stanga + S_dreapta + eroare;
        return;
    end
    
    % Daca NU e precis, TAIEM intervalul in doua si trimitem bucatile mai departe.
    [I_stanga, evals_stanga]  = recursie_simpson(f, a, c, tol / 2, S_stanga, fa, fd, fc, n+1, n_max);
    [I_dreapta, evals_dreapta] = recursie_simpson(f, c, b, tol / 2, S_dreapta, fc, fe, fb, n+1, n_max);
    
    % Adunam rezultatele si evaluarile de pe ambele ramuri
    I = I_stanga + I_dreapta;
    evals = evals + evals_stanga + evals_dreapta;
end