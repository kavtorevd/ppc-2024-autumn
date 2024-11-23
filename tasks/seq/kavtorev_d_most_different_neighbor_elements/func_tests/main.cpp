// Copyright 2023 Kavtorev Dmitry
#include <gtest/gtest.h>

#include <vector>
#include <memory>
#include <utility>  // Для std::pair

#include "seq/kavtorev_d_most_different_neighbor_elements/include/ops_seq.hpp" // Убедитесь, что путь корректный

using InOutType = int;

// Тест для случая, когда два соседних элемента различаются на 10
TEST(kavtorev_d_most_different_neighbor_elements_seq, TestMostDifferentNeighbors10) {
    std::vector<InOutType> in = {1, 11}; // Два соседних элемента, которые различаются на 10
    std::pair<InOutType, InOutType> out_element; // Для хранения результата

    // Создание TaskData
    auto taskDataSeq = std::make_shared<ppc::core::TaskData>();
    taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(in.data()));
    taskDataSeq->inputs_count.emplace_back(in.size());
    taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(&out_element));
    taskDataSeq->outputs_count.emplace_back(1); // Один выходной элемент (пара)

    // Создание задачи
    ppc::kavtorev_d_most_different_neighbor_elements_seq::testTaskSequential testTaskSequential(taskDataSeq);
    
    // Проверка валидации задачи
    ASSERT_TRUE(testTaskSequential.validation());
    
    // Установка входных данных и выполнение задачи
    testTaskSequential.set_data(in);
    testTaskSequential.pre_processing();
    testTaskSequential.run();
    testTaskSequential.post_processing();

    // Проверка результата
    ASSERT_EQ(out_element.first, 1);
    ASSERT_EQ(out_element.second, 11);
}

// Тест для случая, когда два соседних элемента одинаковы
TEST(kavtorev_d_most_different_neighbor_elements_seq, TestMostDifferentNeighborsSame) {
    std::vector<InOutType> in = {5, 5}; // Два одинаковых соседних элемента
    std::pair<InOutType, InOutType> out_element;

    // Создание TaskData
    auto taskDataSeq = std::make_shared<ppc::core::TaskData>();
    taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(in.data()));
    taskDataSeq->inputs_count.emplace_back(in.size());
    taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(&out_element));
    taskDataSeq->outputs_count.emplace_back(1); // Один выходной элемент (пара)

    // Создание задачи
    ppc::kavtorev_d_most_different_neighbor_elements_seq::testTaskSequential testTaskSequential(taskDataSeq);
    
    // Проверка валидации задачи
    ASSERT_TRUE(testTaskSequential.validation());
    
    // Установка входных данных и выполнение задачи
    testTaskSequential.set_data(in);
    testTaskSequential.pre_processing();
    testTaskSequential.run();
    testTaskSequential.post_processing();

    // Проверка результата
    ASSERT_EQ(out_element.first, 5);
    ASSERT_EQ(out_element.second, 5);
}

// Тест для случая, когда массив содержит несколько различных соседей
TEST(kavtorev_d_most_different_neighbor_elements_seq, TestMostDifferentNeighborsMultiple) {
    std::vector<InOutType> in = {1, 3, 7, 2, 10}; // Массив с различными соседями
    std::pair<InOutType, InOutType> out_element;

    // Создание TaskData
    auto taskDataSeq = std::make_shared<ppc::core::TaskData>();
    taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(in.data()));
    taskDataSeq->inputs_count.emplace_back(in.size());
    taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(&out_element));
    taskDataSeq->outputs_count.emplace_back(sizeof(out_element)); // Один выходной элемент (пара)

    // Создание задачи
    ppc::kavtorev_d_most_different_neighbor_elements_seq::testTaskSequential testTaskSequential(taskDataSeq);
    
    // Проверка валидации задачи
    ASSERT_TRUE(testTaskSequential.validation());
    
    // Установка входных данных и выполнение задачи
    testTaskSequential.set_data(in);
    testTaskSequential.pre_processing();
    testTaskSequential.run();
    testTaskSequential.post_processing();

    // Проверка результата: 2 и 10 должны быть наиболее различающимися соседями
    ASSERT_EQ(out_element.first, 2);
    ASSERT_EQ(out_element.second, 10);
}


// Тест для пустого массива
// TEST(kavtorev_d_most_different_neighbor_elements_seq, TestMostDifferentNeighborsEmpty) {
//     std::vector<int> in; // Пустой массив
//     std::pair<int, int> out_element;

//     // Создание TaskData
//     auto taskDataSeq = std::make_shared<ppc::core::TaskData>();
//     taskDataSeq->inputs.emplace_back(reinterpret_cast<uint8_t*>(in.data()));
//     taskDataSeq->inputs_count.emplace_back(in.size());
//     taskDataSeq->outputs.emplace_back(reinterpret_cast<uint8_t*>(&out_element));
//     taskDataSeq->outputs_count.emplace_back(1); // Один выходной элемент (пара)

//     // Создание задачи
//     ppc::kavtorev_d_most_different_neighbor_elements_seq::testTaskSequential testTaskSequential(taskDataSeq);
    
//     // Проверка валидации задачи
//     ASSERT_TRUE(testTaskSequential.validation());

//     // Установка входных данных и выполнение задачи
//     testTaskSequential.set_data(in);
//     testTaskSequential.pre_processing();
//     testTaskSequential.run();
//     testTaskSequential.post_processing();

//     // Проверка, что выходные элементы инициализированы нулями для пустого ввода
//     ASSERT_EQ(out_element.first, 0);
//     ASSERT_EQ(out_element.second, 0);
// }



