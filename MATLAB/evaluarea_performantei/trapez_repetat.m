function [I, nr_eval] = trapez_repetat(f, a, b, tol, n_max)
    % TRAPEZ_REPETAT Calculeaza integrala definita folosind Regula Trapezului Adaptiva.
    % f      - functia de integrat (function handle)
    % a, b   - limitele de integrare
    % tol    - toleranta dorita pentru oprire
    % n_max  - numarul maxim de înjumătățiri ale pasului
    % Returneaza: I (valoarea integralei), nr_eval (numarul de apeluri ale functiei f)
    
    h = b - a;
    T_vechi = (h/2) * (f(a) + f(b));
    % Total evaluari (+ 2 pentru f(a) si f(b))
    nr_eval = 2;

    for j = 2:n_max
        h = h/2;
        nr_noduri_noi = 2^(j-2);

        % Calculul sumei
        suma = 0;
        for i = 1:nr_noduri_noi
            x_nou = a + (2*i - 1) * h;
            suma = suma + f(x_nou);
            nr_eval = nr_eval + 1;
        end
        
       % Calculul urmatorului element adaugand doar contributia noilor noduri
       T_nou = 0.5 * T_vechi + h * suma;
       
       % Verificarea preciziei
       eroare = abs(T_nou - T_vechi);
       if eroare < tol
            I = T_nou;
            return;
        end

        T_vechi = T_nou;
    end
    I = T_nou;
end