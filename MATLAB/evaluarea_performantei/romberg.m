function [I, nr_eval] = romberg(f, a, b, tol, n_max)
    % ROMBERG Calculeaza integrala definita folosind extrapolarea Romberg adaptiva.
    % f      - functia de integrat (function handle)
    % a, b   - limitele de integrare
    % tol    - toleranta dorita pentru oprire
    % n_max  - dimensiunea maxima a tabelului Romberg
    % Returneaza: I (valoarea integralei), nr_eval (numarul de apeluri ale functiei f)
    
    % Matricea tabelului Romberg
    R_std = zeros(n_max, n_max);

    h = b - a;
    R_std(1,1) = (h/2) * (f(a) + f(b));
    T_vechi = R_std(1,1);
    % Total evaluari (+ 2 pentru f(a) si f(b))
    nr_eval = 2;

    for j = 2:n_max
        % Completarea primei coloane la fiecare schimbare de rand
        h_j = (b - a) / (2^(j-1)); 
        numar_noduri_noi = 2^(j-2);
        suma = 0;
        for i = 1:numar_noduri_noi
            x_nou = a + (2*i - 1) * h_j;
            suma = suma + f(x_nou);
            nr_eval = nr_eval + 1;
        end
        R_std(j,1) = 0.5 * R_std(j-1,1) + h_j * suma;
        
        % Completarea randului
        for k = 2:j
            % Aplicarea formulei bazate pe extrapolarea Richardson
            R_std(j,k) = (4^(k-1) * R_std(j,k-1) - R_std(j-1,k-1)) / (4^(k-1) - 1);
        end
        T_nou = R_std(j,j);
        
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