# Калькулятор матриц на C++17

Лабораторная работа № 3, вариант 19. Библиотека создаёт матрицы типа double, копирует данные из массива, складывает и умножает матрицы, выводит их и вычисляет след квадратной матрицы. Демонстрационная программа использует две матрицы 2 × 2. Ожидаемые следы A и A·B равны 5 и 69.

## Сборка и проверка в Ubuntu

```bash
sudo apt-get update
sudo apt-get install -y cmake g++ make libgtest-dev doxygen graphviz valgrind
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build --parallel
./build/matrix_calculator
ctest --test-dir build --verbose
valgrind --leak-check=full --error-exitcode=1 ./build/tests/matrix_tests
doxygen Doxyfile
```

Откройте `docs/html/index.html` для просмотра документации. Для сборки без Google Test передайте CMake флаг `-DBUILD_TESTING=OFF`.

## Контейнер

```bash
docker build -t matrix-calculator .
docker run --rm matrix-calculator
```

## Интерфейс и автоматизация

Объявления функций и условия их вызова находятся в `include/matrix.h`. Владелец освобождает каждый полученный массив вызовом `free_matrix` ровно один раз. Для некорректных операндов арифметические функции выбрасывают `std::invalid_argument`.

Workflow `.github/workflows/ci.yml` выполняет сборку, тестирование, проверку памяти, генерацию документации и запуск программы в Docker. HTML и журналы доступны в артефакте успешного прогона.
