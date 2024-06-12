# Projekt Zespołowy na zaliczenie AiSD2

## skład zespołu

- **Mateusz Sobkowiak** (kapitan)
- **Paulina Grabowska**
- **Mikołaj Juda**

## zależności

cmake, make, g++ (lub inny kompilator ze wsparciem c++17),  
catch2 (instalowany automatycznie przez cmake)

## budowanie

```bash
mkdir build # jeżeli folder build nie istnieje
cmake -S . -B build
cmake --build build
```

## uruchamianie

Po zbudowaniu projektu pliki wykonywalne będą znajdowały się w katalogu `build/bin`

## testowanie

```bash
cmake --build build # przed testowaniem jeśli zostały wprowadzone zmiany
ctest --test-dir build
```

## dokumentacja

Doxygen musi być zainstalowany

```bash
cmake -S . -B build
cmake --build build --target doxygen
```

Wynikowa strona internetowa powstanie w katalogu `build/docs/html`

## sprawozdanie

```bash
cd report
latexmk -pdf report.tex
```
