/** @file matrix.h
 *  @brief Операции над вещественными матрицами.
 */
#pragma once

/** @brief Динамическая матрица с отдельным массивом для каждой строки.
 *  @warning Присваивание Matrix копирует указатель. Освобождайте память один раз.
 */
struct Matrix {
    double** data; ///< Массив указателей на строки.
    int rows;      ///< Количество строк.
    int cols;      ///< Количество столбцов.
};

/** @brief Создаёт нулевую матрицу.
 * @param rows Число строк, больше нуля.
 * @param cols Число столбцов, больше нуля.
 * @return Новая матрица. Освободите её вызовом free_matrix().
 * @throws std::invalid_argument Размер неположителен.
 * @throws std::bad_alloc Недостаточно памяти.
 */
Matrix create_matrix(int rows, int cols);

/** @brief Освобождает все строки и массив указателей.
 * @param m Матрица, созданная библиотекой, или пустая структура.
 * @warning После вызова присвойте владельцу {} перед повторным использованием.
 */
void free_matrix(Matrix m) noexcept;

/** @brief Копирует значения из массива построчно.
 * @param data Массив как минимум из rows × cols чисел.
 * @param rows Число строк.
 * @param cols Число столбцов.
 * @return Независимая матрица.
 * @throws std::invalid_argument Пустой указатель или неположительный размер.
 */
Matrix matrix_from_array(double* data, int rows, int cols);

/** @brief Складывает две матрицы поэлементно.
 * @param a Первое слагаемое.
 * @param b Второе слагаемое той же формы.
 * @return Новая матрица сумм.
 * @throws std::invalid_argument Некорректная матрица или несовпадение размеров.
 */
Matrix matrix_add(Matrix a, Matrix b);

/** @brief Выполняет матричное умножение.
 * @param a Левый множитель размера m × n.
 * @param b Правый множитель размера n × p.
 * @return Новая матрица размера m × p.
 * @throws std::invalid_argument Некорректная матрица или несовместимые размеры.
 */
Matrix matrix_multiply(Matrix a, Matrix b);

/** @brief Вычисляет след квадратной матрицы.
 * @param m Квадратная матрица размера n × n.
 * @return Сумма элементов главной диагонали.
 * @throws std::invalid_argument Некорректная или прямоугольная матрица.
 * @note Время O(n), дополнительная память O(1).
 */
double matrix_trace(Matrix m);

/** @brief Печатает матрицу с двумя знаками после точки.
 * @param m Матрица для вывода. Для пустой матрицы выводится [Empty matrix].
 * @note Показывает до десяти строк и десяти столбцов.
 */
void print_matrix(Matrix m);
