#include "TSP.hpp"
#include "gtest/gtest.h"
/*
 * Przykładowy test
TEST(NazwaGrupyTestow, NazwaPojedynczegoTestu){
    EXPECT_EQ(1, 1); #oczekujemy że obie zmienne będą sobie równe.
}
*/

TEST(CostMatrix_tests, get_min_values_in_rows_test){
    cost_matrix_t cm = {{INF, 10, 8,   19, 12},
                        {10, INF, 20,  6,  3},
                        {8,   20, INF, 4,  2},
                        {19,  6,  4, INF,  7},
                        {12,  3,  2,   7, INF}};

    CostMatrix t = CostMatrix(cm);
    std::vector<cost_t> v = t.get_min_values_in_rows();
    std::vector<cost_t> v2 = {8, 3, 2, 4, 2};
    EXPECT_EQ(v, v2);
}

TEST(CostMatrix_tests, reduce_rows_test){
    cost_matrix_t cm = {{INF, 10, 8,   19, 12},
                        {10, INF, 20,  6,  3},
                        {8,   20, INF, 4,  2},
                        {19,  6,  4, INF,  7},
                        {12,  3,  2,   7, INF}};

    CostMatrix t = CostMatrix(cm);
    cost_t v = t.reduce_rows();
    EXPECT_EQ(v, 19);
}

TEST(CostMatrix_tests, reduce_rows_test_wINF){
    cost_matrix_t cm = {{INF, INF, INF,   INF, INF},
                        {INF, INF, 20,  6,  3},
                        {INF,   20, INF, 4,  2},
                        {INF,  6,  4, INF,  7},
                        {INF,  3,  2,   7, INF}};

    CostMatrix t = CostMatrix(cm);
    cost_t v = t.reduce_rows();
    EXPECT_EQ(v, 11);
}

TEST(CostMatrix_tests, reduce_rows_test_asymetric){
    cost_matrix_t cm = {{INF, 10, 1,   19, 12},
                        {10, INF, 20,  6,  3},
                        {8,   20, INF, 4,  2},
                        {19,  6,  4, INF,  7},
                        {12,  3,  2,   7, INF}};

    CostMatrix t = CostMatrix(cm);
    cost_t v = t.reduce_rows();
    EXPECT_EQ(v, 12);
}

TEST(CostMatrix_tests, get_min_values_in_cols_test){
cost_matrix_t cm = {{INF, 10, 8,   19, 12},
                    {10, INF, 20,  6,  3},
                    {8,   20, INF, 4,  2},
                    {19,  6,  4, INF,  7},
                    {12,  3,  2,   7, INF}};

CostMatrix t = CostMatrix(cm);
std::vector<cost_t> v = t.get_min_values_in_cols();
std::vector<cost_t> v2 = {8, 3, 2, 4, 2};
EXPECT_EQ(v, v2);
}

TEST(CostMatrix_tests, reduce_cols_test){
cost_matrix_t cm = {{INF, 10, 8,   19, 12},
                    {10, INF, 20,  6,  3},
                    {8,   20, INF, 4,  2},
                    {19,  6,  4, INF,  7},
                    {12,  3,  2,   7, INF}};

CostMatrix t = CostMatrix(cm);
cost_t v = t.reduce_cols();
EXPECT_EQ(v, 19);
}

TEST(CostMatrix_tests, reduce_cols_test_wINF){
    cost_matrix_t cm = {{INF, INF, INF,   INF, INF},
                        {INF, INF, 20,  6,  3},
                        {INF,   20, INF, 4,  2},
                        {INF,  6,  4, INF,  7},
                        {INF,  3,  2,   7, INF}};

    CostMatrix t = CostMatrix(cm);
    cost_t v = t.reduce_cols();
    EXPECT_EQ(v, 11);
}

TEST(CostMatrix_tests, reduce_cols_test_asymetric){
    cost_matrix_t cm = {{INF, 10, 8,   19, 12},
                        {10, INF, 20,  6,  3},
                        {1,   20, INF, 4,  2},
                        {19,  6,  4, INF,  7},
                        {12,  3,  2,   7, INF}};

    CostMatrix t = CostMatrix(cm);
    cost_t v = t.reduce_cols();
    EXPECT_EQ(v, 12);
}

TEST(CostMatrix_tests, get_vertex_cost_test){
cost_matrix_t cm = {{INF, 2, 1,   11, 4},
                    {7, INF, 17,  3,  0},
                    {5,   18, INF, 2,  0},
                    {15,  2,  0, INF,  3},
                    {10,  1,  1,   5, INF}};


CostMatrix t = CostMatrix(cm);
cost_t v = t.get_vertex_cost(3, 2);
EXPECT_EQ(v, 3);
v = t.get_vertex_cost(2, 4);
EXPECT_EQ(v, 2);
}

TEST(StageState_tests, reduce_cost_matrix_test){
    cost_matrix_t cm = {{INF, 10, 8,   19, 12},
                        {10, INF, 20,  6,  3},
                        {8,   20, INF, 4,  2},
                        {19,  6,  4, INF,  7},
                        {12,  3,  2,   7, INF}};

    StageState t = StageState(CostMatrix(cm));
    cost_t v = t.reduce_cost_matrix();
    CostMatrix z = CostMatrix(cm);
    cost_t v1 = z.reduce_cols();
    cost_t v2 = z.reduce_rows();
    EXPECT_EQ(v, v1 + v2);
}

TEST(StageState_tests, update_cost_matrix_test){
    cost_matrix_t cm = {{INF, 10, 8,   19, 12},
                        {10, INF, 20,  6,  3},
                        {8,   20, INF, 4,  2},
                        {19,  6,  4, INF,  7},
                        {12,  3,  2,   7, INF}};

    vertex_t v(0, 2);

    cost_matrix_t cm2 = {{INF, INF, INF, INF, INF},
                        {  10, INF, INF,   6,   3},
                        { INF,  20, INF,   4,   2},
                        {  19,   6, INF, INF,   7},
                        {  12,   3, INF,   7, INF}};

    StageState t1 = StageState(CostMatrix(cm));
    StageState t2 = StageState(CostMatrix(cm2));
    t1.update_cost_matrix(v);

    //EXPECT_EQ(t1.get_matrix(), t2.get_matrix());
    //EXPECT_THAT()

    for (int i = 0; i < t1.get_matrix().size(); ++i) {
        for (int j = 0; j < t1.get_matrix().size(); ++j) {
            EXPECT_EQ(t1.get_matrix()[i][j], t2.get_matrix()[i][j]);
        }
    }

}

TEST(NewVertex_tests, choose_new_vertex_test){
    cost_matrix_t cm = {{INF, 2, 1,   11, 4},
                        {7, INF, 17,  3,  0},
                        {5,   18, INF, 2,  0},
                        {15,  2,  0, INF,  3},
                        {10,  1,  1,   5, INF}};
    StageState t = StageState(CostMatrix(cm));
    NewVertex v1({1, 4}, 3);
    NewVertex v2 = t.choose_new_vertex();

    EXPECT_EQ(v1.cost, v2.cost);
    EXPECT_EQ(v1.coordinates.col, v2.coordinates.col);
    EXPECT_EQ(v1.coordinates.row, v2.coordinates.row);
    EXPECT_EQ(v1.cost, t.get_matrix().get_vertex_cost(1 , 4));
}


TEST(General_tests, solve_tsp_test1){
    cost_matrix_t cm = {{INF, 10, 8,   19, 12},
                        {10, INF, 20,  6,  3},
                        {8,   20, INF, 4,  2},
                        {19,  6,  4, INF,  7},
                        {12,  3,  2,   7, INF}};

    tsp_solutions_t t = solve_tsp(cm);
    tsp_solution_t s1;
    s1.lower_bound = 32;
    s1.path = {2, 3, 4, 1, 0};

    tsp_solution_t s2;
    s2.lower_bound = 32;
    s2.path = {1, 4, 3, 2, 0};
    tsp_solutions_t t1 = {s1, s2};

    for (int i = 0; i < solve_tsp(cm).size(); ++i) {
        EXPECT_EQ(t[i].lower_bound, t1[i].lower_bound);
        for (int j = 0; j < t[i].path.size(); ++j) {
            EXPECT_EQ(t[i].path[j],t1[i].path[j])  ;
        }
    }

    /* Rozwiązania:
     * 32 : 2 3 4 1 0
     * 32 : 1 4 3 2 0
     */
}


//TODO - sprawdzić dlaczego nie przechodzi
TEST(General_tests, solve_tsp_test2){
    cost_matrix_t cm {
            {INF, 12,   3,  45,   6},
            {78, INF,  90,  21,   3},
            { 5,  56, INF,  23,  98},
            {12,   6,   8, INF,  34},
            { 3,  98,   3,   2, INF}
    };

    tsp_solutions_t t = solve_tsp(cm);
    tsp_solution_t s1;
    s1.lower_bound = 30;
    s1.path = {4, 3, 2, 0, 1};

    tsp_solutions_t t1 = {s1};

    for (int i = 0; i < solve_tsp(cm).size(); ++i) {
        EXPECT_EQ(t[i].lower_bound, t1[i].lower_bound);
        for (int j = 0; j < t[i].path.size(); ++j) {
            EXPECT_EQ(t[i].path[j],t1[i].path[j])  ;
        }
    }

    /* Rozwiązania:
     * 32 : 2 3 4 1 0
     * 32 : 1 4 3 2 0
     */
}

//TODO  - Same
TEST(General_tests, solve_tsp_test3){
    cost_matrix_t cm {
            {INF,  3,  4,  2,  7},
            {3,  INF,  4,  6,  3},
            {4,  4,  INF,  5,  8},
            {2,  6,  5,  INF,  6},
            {7,  3,  8,  6,  INF},
    };

    tsp_solutions_t t = solve_tsp(cm);
    tsp_solution_t s1;
    s1.lower_bound = 19;
    s1.path = {4, 3, 0, 2, 1};

    tsp_solution_t s2;
    s2.lower_bound = 19;
    s2.path = {1, 2, 0, 3, 4};

    tsp_solutions_t t1 = {s1, s2};

    for (int i = 0; i < solve_tsp(cm).size(); ++i) {
        EXPECT_EQ(t[i].lower_bound, t1[i].lower_bound);
        for (int j = 0; j < t[i].path.size(); ++j) {
            EXPECT_EQ(t[i].path[j],t1[i].path[j])  ;
        }
    }

    /* Rozwiązania:
     * 32 : 2 3 4 1 0
     * 32 : 1 4 3 2 0
     */
}


