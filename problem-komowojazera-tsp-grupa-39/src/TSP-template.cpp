//
// Created by Dominika on 01.11.2020.
// Jakub Mazur, Piotr Wojciechowski, Michał Spinczyk
//

#include "TSP.hpp"

#include <algorithm>
#include <stack>
#include <optional>
#include <iomanip>

std::ostream& operator<<(std::ostream& os, const CostMatrix& cm) {
    for (std::size_t r = 0; r < cm.size(); ++r) {
        for (std::size_t c = 0; c < cm.size(); ++c) {
            const auto& elem = cm[r][c];
            os << std::setw(3)<< (is_inf(elem) ? "INF" : std::to_string(elem)) << " ";
        }
        os << "\n";
    }
    os << std::endl;

    return os;
}

/* PART 1 */

/**
 * Create path from unsorted path and last 2x2 cost matrix.
 * @return The vector of consecutive vertex.
 */
path_t StageState::get_path() {
    std::vector<size_t > v_cons;
    std::vector<size_t> start;
    std::vector<size_t> end;
    for(auto i = 0; i < matrix_.size(); i++){
        for(auto j = 0; j < matrix_[i].size(); j++) {
            auto el = matrix_[i][j];
            if(!is_inf(el)){
                start.push_back(i);
                end.push_back(j);
            }
        }
    }

    append_to_path({start[0],end[0]});
    append_to_path({start[1],end[1]});
    auto sorted = get_unsorted_path();
    sort(sorted.begin(),sorted.end(),[](vertex_t a,vertex_t b){ return (a.row < b.row);});

    auto b_s = 0;
    for (auto i=0;i<matrix_.size();i++){
        v_cons.push_back(sorted[b_s].col);
        b_s = sorted[b_s].col;
    }

    return v_cons;
}
/**
 * Get minimum values from each row and returns them.
 * @return Vector of minimum values in row.
 */
std::vector<cost_t> CostMatrix::get_min_values_in_rows() const {
    std::vector<int> w;
    for(int i = 0; i< size(); i++){
        int m = matrix_[i][0];
        for(int j =1; j< matrix_[i].size(); j++){
            if (matrix_[i][j] < m) m = matrix_[i][j];
        }
        w.push_back(m);
    }
    return w;
}

/**
 * Reduce rows so that in each row at least one zero value is present.
 * @return Sum of values reduced in rows.
 */
cost_t CostMatrix::reduce_rows() {
    std::vector<cost_t> z = get_min_values_in_rows();
    for(int i = 0; i < size(); i++){
        for(int j = 0; j < matrix_[i].size(); j++){
            if (matrix_[i][j] != INF){
                matrix_[i][j] -= z[i];
            }
        }
    }

    int s =0;
    for (int k = 0; k < z.size(); ++k) {
        if (z[k]!= INF) s+=z[k];
    }

    return s;
}

/**
 * Get minimum values from each column and returns them.
 * @return Vector of minimum values in columns.
 */
std::vector<cost_t> CostMatrix::get_min_values_in_cols() const {
    std::vector<cost_t> min_values;
    for(int i = 0; i < size();i++){
        int m = matrix_[0][i];
        for (int j = 1; j < matrix_[i].size(); j++){
            if (matrix_[j][i] < m) m = matrix_[j][i];
        }
        min_values.push_back(m);
    }
    return min_values;
}

/**
 * Reduces rows so that in each column at least one zero value is present.
 * @return Sum of values reduced in columns.
 */
cost_t CostMatrix::reduce_cols() {
    // TODO: Sprawdzić!
    std::vector<cost_t> v_min_cols;
    v_min_cols = get_min_values_in_cols();
    for (int i = 0; i < size();i++){
        for (int j = 0; j < matrix_[i].size(); j++) {
            if (matrix_[j][i] != INF){
                matrix_[j][i] -= v_min_cols[i];
            }
        }
    }

    int s =0;
    for (int k = 0; k < v_min_cols.size(); ++k) {
        if (v_min_cols[k]!= INF) s+=v_min_cols[k];
    }

    return s;
}

/**
 * Get the cost of not visiting the vertex_t (@see: get_new_vertex())
 * @param row
 * @param col
 * @return The sum of minimal values in row and col, excluding the intersection value.
 */
cost_t CostMatrix::get_vertex_cost(std::size_t row, std::size_t col) const {
    //[row][col]
    int row_min = INF;
    int col_min = INF;
    //Po komórkach w rzędzie row:
    for(int i=0; i<matrix_.size(); i++){
        if(i != col){
            if(matrix_[row][i] < row_min){
                row_min = matrix_[row][i];
            }
        }
    }
    //po komórkach w kolumnie col:
    for(int i=0; i<matrix_.size(); i++){
        if(i != row){
            if(matrix_[i][col] <col_min){
                col_min = matrix_[i][col];
            }
        }
    }
    return col_min + row_min;
}

/* PART 2 */

/**
 * Choose next vertex to visit:
 * - Look for vertex_t (pair row and column) with value 0 in the current cost matrix.
 * - Get the vertex_t cost (calls get_vertex_cost()).
 * - Choose the vertex_t with maximum cost and returns it. (First one found.)
 * @return The coordinates of the next vertex.
 */
NewVertex StageState::choose_new_vertex() {
    NewVertex v({0, 0}, -INF);

    for(size_t i = 0; i<matrix_.size(); i++){
        for (size_t j = 0; j < matrix_.size(); j++) {
            if(!matrix_[i][j]){
                cost_t t = matrix_.get_vertex_cost(i, j);
                if(t > v.cost){
                    v = NewVertex({i, j}, t);
                }
            }
        }
    }

    return v;
}

/**
 * Update the cost matrix with the new vertex.
 * @param new_vertex
 */
void StageState::update_cost_matrix(vertex_t new_vertex) {
    for (int i = 0; i < matrix_.size(); ++i) {
        matrix_[new_vertex.row][i] = INF;
        matrix_[i][new_vertex.col] = INF;
    }

    matrix_[new_vertex.col][new_vertex.row] = INF;


    for (int k = 0; k < unsorted_path_.size(); ++k) {
        std::vector<vertex_t> cur_path;
        cur_path.push_back(unsorted_path_[k]);

        int s = unsorted_path_[k].col;
        int l = 0;
        while (l < unsorted_path_.size()) {
            if (unsorted_path_[l].row == s) {
                s = unsorted_path_[l].col;
                cur_path.push_back(unsorted_path_[l]);
                l = -1;

                for (int j = 0; j < cur_path.size(); ++j) {
                    matrix_[s][cur_path[j].row] = INF;
                }

            }
            l++;
        }


    }

}

/**
 * Reduce the cost matrix.
 * @return The sum of reduced values.
 */
cost_t StageState::reduce_cost_matrix() {
    cost_t have_zero = 0;
    cost_t reduced_values_sum = matrix_.reduce_rows();
    for(int i = 0;i < matrix_.size() and have_zero == 0; i++){
        for (int j = 0; j < matrix_.size(); j++){
            if (matrix_[j][i] == 0) {
                have_zero++;
            }
        }
        if (have_zero > 0){
            have_zero = 0;
        }
        else{
            reduced_values_sum += matrix_.reduce_cols();
        }
    }
    return reduced_values_sum;
}

/**
 * Given the optimal path, return the optimal cost.
 * @param optimal_path
 * @param m
 * @return Cost of the path.
 */
cost_t get_optimal_cost(const path_t& optimal_path, const cost_matrix_t& m) {
    cost_t cost = 0;

    for (std::size_t idx = 1; idx < optimal_path.size(); ++idx) {
        cost += m[optimal_path[idx - 1]][optimal_path[idx]];
    }

    // Add the cost of returning from the last city to the initial one.
    cost += m[optimal_path[optimal_path.size() - 1]][optimal_path[0]];

    return cost;
}

/**
 * Create the right branch matrix with the chosen vertex forbidden and the new lower bound.
 * @param m
 * @param v
 * @param lb
 * @return New branch.
 */
StageState create_right_branch_matrix(cost_matrix_t m, vertex_t v, cost_t lb) {
    CostMatrix cm(m);
    cm[v.row][v.col] = INF;
    return StageState(cm, {}, lb);
}

/**
 * Retain only optimal ones (from all possible ones).
 * @param solutions
 * @return Vector of optimal solutions.
 */
tsp_solutions_t filter_solutions(tsp_solutions_t solutions) {
    cost_t optimal_cost = INF;
    for (const auto& s : solutions) {
        optimal_cost = (s.lower_bound < optimal_cost) ? s.lower_bound : optimal_cost;
    }

    tsp_solutions_t optimal_solutions;
    std::copy_if(solutions.begin(), solutions.end(),
                 std::back_inserter(optimal_solutions),
                 [&optimal_cost](const tsp_solution_t& s) { return s.lower_bound == optimal_cost; }
    );

    return optimal_solutions;
}

/**
 * Solve the TSP.
 * @param cm The cost matrix.
 * @return A list of optimal solutions.
 */
tsp_solutions_t solve_tsp(const cost_matrix_t& cm) {

    StageState left_branch(cm);

    // The branch & bound tree.
    std::stack<StageState> tree_lifo;

    // The number of levels determines the number of steps before obtaining
    // a 2x2 matrix.
    std::size_t n_levels = cm.size() - 2;

    tree_lifo.push(left_branch);   // Use the first cost matrix as the root.

    cost_t best_lb = INF;
    tsp_solutions_t solutions;

    while (!tree_lifo.empty()) {

        left_branch = tree_lifo.top();
        tree_lifo.pop();

        while (left_branch.get_level() != n_levels && left_branch.get_lower_bound() <= best_lb) {
            // Repeat until a 2x2 matrix is obtained or the lower bound is too high...

            if (left_branch.get_level() == 0) {
                left_branch.reset_lower_bound();
            }

            // 1. Reduce the matrix in rows and columns.
            cost_t new_cost = left_branch.reduce_cost_matrix(); // @TODO (KROK 1)

            // 2. Update the lower bound and check the break condition.
            left_branch.update_lower_bound(new_cost);
            if (left_branch.get_lower_bound() > best_lb) {
                break;
            }

            // 3. Get new vertex and the cost of not choosing it.
            NewVertex new_vertex = left_branch.choose_new_vertex(); // @TODO (KROK 2)

            // 4. @TODO Update the path - use append_to_path method.
            left_branch.append_to_path(new_vertex.coordinates);
            // 5. @TODO (KROK 3) Update the cost matrix of the left branch.

            left_branch.update_cost_matrix(new_vertex.coordinates);
            // 6. Update the right branch and push it to the LIFO.
            cost_t new_lower_bound = left_branch.get_lower_bound() + new_vertex.cost;
            tree_lifo.push(create_right_branch_matrix(cm, new_vertex.coordinates,
                                                      new_lower_bound));
        }

        if (left_branch.get_lower_bound() <= best_lb) {
            // If the new solution is at least as good as the previous one,
            // save its lower bound and its path.
            best_lb = left_branch.get_lower_bound();

            path_t new_path = left_branch.get_path();
            auto el1 = get_optimal_cost(new_path, cm);
            auto el2 = new_path;
            for (auto i=0;i<new_path.size();i++){
                el2[i] += 1;
            }
            solutions.push_back({el1, el2});
    }
    }

    return filter_solutions(solutions); // Filter solutions to find only optimal ones.
}
