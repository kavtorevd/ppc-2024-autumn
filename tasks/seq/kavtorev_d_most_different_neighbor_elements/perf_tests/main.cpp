// Copyright 2023 Kavtorev Dmitry
#include <gtest/gtest.h>
#include <vector>
#include <chrono>
#include <numeric>
#include <algorithm>

#include "core/perf/include/perf.hpp"
#include "seq/kavtorev_d_most_different_neighbor_elements/include/ops_seq.hpp"

using InOutType = int;
using IndexType = int;

TEST(kavtorev_d_most_different_neighbor_elements_seq, test_pipeline_run) {
    const int vector_size = 100;
    std::vector<int> input_vec(vector_size);
    std::vector<int> output_elements(2, 0);
    std::vector<int> output_indices(2, 0);

    // Заполнение случайными значениями
    std::generate(input_vec.begin(), input_vec.end(), []() { return rand() % 50 + 1; });

    // Создание TaskData
    auto taskDataSeq = std::make_shared<ppc::core::TaskData>();

    // Проверяем адреса и корректность заполнения
    std::cout << "Input vector address: " << static_cast<void*>(input_vec.data()) << std::endl;

    taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(input_vec.data()));
    taskDataSeq->inputs_count.emplace_back(input_vec.size());
    taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(output_elements.data()));
    taskDataSeq->outputs_count.emplace_back(output_elements.size());
    taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(output_indices.data()));
    taskDataSeq->outputs_count.emplace_back(output_indices.size());

    // Отладка taskDataSeq
    ASSERT_FALSE(taskDataSeq->inputs.empty());
    ASSERT_NE(taskDataSeq->inputs[0], nullptr);
    std::cout << "TaskData input address: " << static_cast<void*>(taskDataSeq->inputs[0]) << std::endl;

    // Создание задачи
    auto testTaskSequential = std::make_shared<ppc::kavtorev_d_most_different_neighbor_elements_seq::testTaskSequential>(taskDataSeq);

    // Проверяем результат pre_processing
    ASSERT_TRUE(testTaskSequential->pre_processing()) << "Pre-processing failed";

    // Запуск задачи
    ASSERT_TRUE(testTaskSequential->run()) << "Run failed";

    // Постобработка
    ASSERT_TRUE(testTaskSequential->post_processing()) << "Post-processing failed";

    // Проверка, что результаты совпадают
    ASSERT_EQ(output_elements[0], testTaskSequential->get_most_different_neighbors().first);
    ASSERT_EQ(output_elements[1], testTaskSequential->get_most_different_neighbors().second);

    // Проверка разницы
    int calculated_diff = std::abs(output_elements[0] - output_elements[1]);
    int expected_diff = 0;
    for (size_t i = 0; i < input_vec.size() - 1; ++i) {
        int diff = std::abs(input_vec[i] - input_vec[i + 1]);
        expected_diff = std::max(expected_diff, diff);
    }
    ASSERT_EQ(calculated_diff, expected_diff);
}





// Тест для производительности, который запускает задачу
TEST(kavtorev_d_most_different_neighbor_elements_seq, test_task_run) {
    const int vector_size = 100;
    std::vector<InOutType> input_vec(vector_size);
    std::vector<InOutType> output_elements(2, 0);  // Массив для хранения двух значений
    std::vector<IndexType> output_indices(2, 0);   // Массив для хранения индексов этих значений

    // Заполнение вектора фиксированными значениями для предсказуемости
    std::iota(input_vec.begin(), input_vec.end(), 1); // Заполняет 1, 2, 3, ..., 100

    // Создание TaskData
    auto taskDataSeq = std::make_shared<ppc::core::TaskData>();
    taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(input_vec.data()));
    taskDataSeq->inputs_count.emplace_back(input_vec.size());
    taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(output_elements.data()));
    taskDataSeq->outputs_count.emplace_back(output_elements.size());
    taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(output_indices.data()));
    taskDataSeq->outputs_count.emplace_back(output_indices.size());

    // Создание задачи
    auto testTaskSequential = std::make_shared<ppc::kavtorev_d_most_different_neighbor_elements_seq::testTaskSequential>(taskDataSeq);
    
    // Установка входных данных и выполнение задачи
    ASSERT_TRUE(testTaskSequential->validation()); // Проверка валидации задачи через указатель
    testTaskSequential->set_data(input_vec);
    testTaskSequential->pre_processing();
    ASSERT_TRUE(testTaskSequential->run()); // Проверка, что run() возвращает true
    testTaskSequential->post_processing();

    // Проверка, что выходные данные соответствуют ожидаемым
    ASSERT_NE(output_elements[0], output_elements[1]);
}


