# Motor de Randare Ray Tracing Optimizat prin Metode de Integrare Numerică

Acest repository conține codul sursă dezvoltat în cadrul lucrării de licență intitulate **„Metode de Integrare Numerică cu Aplicații”**, realizată în cadrul Facultății de Matematică și Informatică, Universitatea Babeș-Bolyai (UBB), Cluj-Napoca.

Proiectul explorează hibridizarea tehnicilor clasice de sinteză a imaginii stocastice (Monte Carlo) prin utilizarea metodelor deterministe de analiză numerică de înaltă precizie (Gauss-Legendre 2D, Simpson 2D și scheme adaptive de rafinare).

---

## 📂 Structura Proiectului

Repository-ul este organizat în două directoare principale:

* 📂 **`Ray Tracer/`** - Motorul de randare scris de la zero în C++17. Conține managementul scenelor, obiectelor geometrice, materialelor și o cameră extensibilă configurată polimorfic.
* 📂 **`MATLAB/`** - Scripturile numerice utilizate în faza de cercetare pentru studiul, testarea și validarea teoretică a metodelor matematice.

---

## 🛠️ Metode de Integrare Implementate

Arhitectura centrală (clasa `camera`) permite comutarea dinamică între 4 moduri distincte de evaluare a ecuației de randare:

1. **Monte Carlo (`RenderMode::MonteCarlo`):** Metoda clasică, stocastică, bazată pe raze aleatoare.
2. **Cuadratura Gauss-Legendre 2D (`RenderMode::Gauss`):** Metodă deterministă bazată pe rădăcinile polinoamelor Legendre, eliminând complet zgomotul vizual în regiunile de continuitate.
3. **Cuadratura Adaptivă 2D (`RenderMode::Adaptive`):** Schemă recursivă care rafinează grila exclusiv în zonele cu variații de culoare (margini/umbre), optimizând costul computațional.
4. **Metoda Simpson 2D Hibridă (`RenderMode::SimpsonHybrid`):** Aplicată pe componentele de iluminare directă prin eșantionarea matricială a surselor de lumină rectangulare.

---

## 🚀 Ghid de Compilare și Utilizare (C++)

Motorul grafic folosește **CMake** (versiunea minimă 3.16) și necesită un compilator compatibil **C++17**.

### 1. Compilarea proiectului

```bash
git clone [https://github.com/SebiB24/Licenta_Anexa.git](https://github.com/SebiB24/Licenta_Anexa.git)
cd "Licenta_Anexa/Ray Tracer"

mkdir build && cd build
cmake ..
cmake --build .
```

### 2. Configurarea Modului de Randare (API)

Alegerea scenei se face interactiv la rularea programului din terminal. Totuși, **metoda de integrare numerică** se configurează direct în codul scenei (în fișierul `main.cpp`), setând atributele camerei înainte de a apela funcția de randare. 

Exemplu de configurare pentru Metoda Gauss cu o grilă de 9 puncte:

```cpp
camera cam;
cam.image_width = 600;
cam.samples_per_pixel = 50;

// Setarea motorului matematic
cam.render_mode = RenderMode::Gauss;
cam.integration_degree = 9; // Valori suportate: 3, 9

// Apelarea randarii (cu sursa de lumina explicită pentru metode deterministe)
cam.render(world, light_source);
```

*Notă: Pentru metoda `RenderMode::Adaptive`, se folosește parametrul `cam.max_adaptive_depth = 2` în loc de `integration_degree`.*

### 3. Rularea și Generarea Imaginii

Deoarece pixelii sunt transmiși către ieșirea standard (`stdout`), rularea trebuie să redirecționeze datele către un fișier `.ppm`:

```bash
./RayTracer > imagine.ppm
```

Meniul interactiv vă va ghida:
```text
Selecteaza scena pe care vrei sa o randezi:
  1. Tree Spheres
  2. Quads
  3. Cornell Box
Optiunea ta: 3

[Randez Cornell Box...]
Scanlines remaining: 0 
Done.                        
```

---

## 📊 Rularea Experimentelor MATLAB

Directorul `MATLAB/` conține codul sursă utilizat pentru analiza teoretică a formulelor de cuadratură, generarea tabelelor de convergență și studiul distribuției nodurilor. Scripturile (`.m`) sunt independente și pot fi deschise și rulate direct în mediul MATLAB pentru a reproduce testele discutate în textul lucrării.

---

## 🎓 Credite

* **Autor:** Buda Sebastian
* **Coordonator Științific:** Conf. univ. dr. Teodora Cătinaș
* **Instituție:** Universitatea Babeș-Bolyai, Facultatea de Matematică și Informatică
* **An Academic:** 2025 - 2026
