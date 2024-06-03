#include <iostream>
#include <vector>
#include <climits>
#include <algorithm>
#include <fstream>

const int INF = INT_MAX;

void print_matrix(std::vector<std::vector<int>> matrix)
{
    int n = matrix.size();
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < n; j++)
        {
            if (matrix[i][j] == INF)
                std::cout << "IN ";
            else
                std::cout << matrix[i][j] << " ";
        }
        std::cout << "\n";
    }
}

void generate_matrix(int n){
    srand(static_cast<unsigned>(time(0)));

    std::ofstream file("matrix.txt");
    if (!file) {
        std::cerr << "Can not open file to write." << std::endl;
        exit(1);
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            if (i == j){
                file << INF << ' ';
            }
            else{
                int random_number = rand() % 10 + 1;
                file << random_number << ' ';
            }
        }
        file << '\n';
    }

    file.close();
    std::cout << "Martix in matrix.txt." << std::endl;

}

std::vector<std::vector<int>> read_matrix(int n) {
    generate_matrix(n);
    const std::string& fileName = "matrix.txt";
    std::vector<std::vector<int>> matrix(n, std::vector<int>(n));
    std::ifstream file(fileName);
    if (!file.is_open()) {
        std::cerr << "Unable to open file " << fileName << std::endl;
        exit(1);
    }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            file >> matrix[i][j];
        }
    }
    return matrix;
}


std::vector<std::vector<int>> find_may_pass(std::vector<std::vector<int>> pieces)
{
    std::vector<std::vector<int>> may_pass;
    int len_pieces = pieces.size();
    for (int i = 0; i < len_pieces; i++)
    {
        int end_curr = pieces[i][pieces[i].size() - 1];
        for (int j = 0; j < len_pieces; j++)
        {
            if (end_curr != pieces[j][pieces[i].size() - 1])
            {
                std::vector<int> new_piece;
                new_piece.push_back(end_curr);
                new_piece.push_back(pieces[j][0]);
                may_pass.push_back(new_piece);
            }
        }
    }
    return may_pass;
}

int min_weight(std::vector<std::vector<int>> pieces, std::vector<std::vector<int>> matrix, std::vector<std::vector<int>> may_pass)
{
    int len_pices = pieces.size();
    int n = may_pass.size();
    int m = pieces.size();
    std::vector<int> min_weight_out(m, INF);
    int k = 0;
    for (int i = 0; i < m; i++)
    {
        for (int j = 0; j < m - 1; j++)
        {
            if (matrix[may_pass[k][0] - 1][may_pass[k][1] - 1] < min_weight_out[i])
            {
                min_weight_out[i] = matrix[may_pass[k][0] - 1][may_pass[k][1] - 1];
            }
            k++;
        }
    }

    std::vector<int> min_weight_in(m, INF);
    for (int i = 0; i < m; i++)
    {
        int curr = pieces[i][0];
        for (int j = 0; j < n; j++)
        {
            if (may_pass[j][1] == curr)
            {
                if (matrix[may_pass[j][0] - 1][curr - 1] < min_weight_in[i])
                {
                    min_weight_in[i] = matrix[may_pass[j][0] - 1][curr - 1];
                }
            }
        }
    }

    int sumi = 0;
    for (int i = 0; i < m; i++)
    {
        sumi += min_weight_in[i];
        sumi += min_weight_out[i];
    }

    return sumi / 2;
}

int weight_MOD(std::vector<std::vector<int>> matrix, std::vector<std::vector<int>> pieces)
{
    int weight = 0;
    std::vector<std::vector<int>> may_pass = find_may_pass(pieces);
    int len = may_pass.size();
    std::vector<std::vector<int>> edges(len, std::vector<int>(3));
    for (int i = 0; i < len; i++)
    {
        edges[i][0] = matrix[may_pass[i][0] - 1][may_pass[i][1] - 1];
        edges[i][1] = may_pass[i][0];
        edges[i][2] = may_pass[i][1];
    }
    std::sort(edges.begin(), edges.end());

    int n = matrix.size();
    std::vector<int> comp(n);
    int i = 0;
    int k = 0;
    while (k < n)
    {
        for (int j = 0; j < pieces[i].size(); j++)
        {
            comp[pieces[i][j] - 1] = i;
            k++;
        }
        i++;
    }

    for (auto &edge : edges)
    {
        int w_edge = edge[0];
        int start = edge[1];
        int end = edge[2];

        if (comp[start] != comp[end])
        {
            weight += w_edge;
            int a = comp[start];
            int b = comp[end];
            for (int i = 0; i < n; i++)
            {
                if (comp[i] == b)
                    comp[i] = a;
            }
        }
    }

    return weight;
}

std::vector<int> find_next(std::vector<std::vector<int>> matrix, std::vector<std::vector<int>> may_pass, std::vector<int> curr_path, std::vector<std::vector<int>> pieces){
    int mini = INF;
    std::vector<int> next_point;
    int curr_end = curr_path[curr_path.size()-1];
    for (int i = 0; i < may_pass.size(); i++)
    {
        if (may_pass[i][0] == curr_end)
        {
            int s = matrix[curr_end - 1][may_pass[i][1] - 1];
            int b1 = min_weight(pieces, matrix, may_pass);

            std::vector<std::vector<int>> new_pieces;
            std::vector<int> may_path = curr_path;
            may_path.push_back(may_pass[i][1]);
            new_pieces.push_back(may_path);
            for (int j = 0; j < matrix.size(); j++)
            {
                int k = 0;
                for (int l = 0; l < may_path.size(); l++)
                {
                    if (j + 1 == may_path[l])
                        k++;
                }
                if (k == 0)
                {
                    std::vector<int> new_piece;
                    new_piece.push_back(j + 1);
                    new_pieces.push_back(new_piece);
                }
            }
            int b2 = weight_MOD(matrix, new_pieces);
            int L = std::max(b1, b2);
            if (s + L < mini)
            {
                mini = s + L;
                next_point.pop_back();
                next_point.push_back(may_pass[i][1]);
            }
            if (s + L == mini){
                next_point.push_back(may_pass[i][1]);
            }
        }
    }
    return next_point;
}

void depth_search(std::vector<std::vector<int>> matrix, std::vector<std::vector<int>> pieces, int weigth, int &total_weight, std::vector<int> &result)
{
    std::vector<int> curr_path = pieces[0];
    int curr_end = curr_path[curr_path.size() - 1];
    std::vector<std::vector<int>> may_pass = find_may_pass(pieces);

    int mini = INF;
    std::vector<int> next_point = find_next(matrix, may_pass, curr_path, pieces);
    std::cout << "next\n";
    for (auto x : next_point)
        std::cout << x << " ";
    std::cout << "\n";

    while (!next_point.empty())
    {   
        curr_path = pieces[0];
        curr_end = curr_path[curr_path.size() - 1];
        curr_path.push_back(next_point[0]);
        std::vector<std::vector<int>> new_pieces;
        new_pieces.push_back(curr_path);
        for (int j = 0; j < matrix.size(); j++)
        {
            int k = 0;
            for (int l = 0; l < curr_path.size(); l++)
            {
                if (j + 1 == curr_path[l])
                    k++;
            }
            if (k == 0)
            {
                std::vector<int> new_piece;
                new_piece.push_back(j + 1);
                new_pieces.push_back(new_piece);
            }
        }
        // std::cout << "new_pieces: ";
        // for (auto x : new_pieces)
        // {
        //     for (int j = 0; j < x.size(); j++)
        //     {
        //         std::cout << x[j];
        //     }
        //     std::cout << " ";
        // }
        // std::cout << "\n";

        weigth += matrix[curr_end - 1][next_point[0] - 1];
        if (total_weight > weigth)
        {
            //std::cout << "weigth " << weigth << "\n";
            //std::cout << "total " << total_weight << "\n";
            std::vector<int> new_next;
            for (int j = 1; j < next_point.size(); j++)
            {
                new_next.push_back(next_point[j]);
            }

            next_point = new_next;
            // std::cout << "next\n";
            // for (auto x : next_point)
            //     std::cout << x << " ";
            // std::cout << "\n";
            if (curr_path.size() == matrix.size())
            {
                weigth += matrix[curr_path[curr_path.size() - 1] - 1][curr_path[0] - 1];
                if (weigth < total_weight && weigth != 0)
                {
                    result = curr_path;
                    result.push_back(curr_path[0]);
                    // std::cout << "result ";
                    // for (auto x : result)
                    //     std::cout << x << " ";
                    // std::cout << "\n";
                    total_weight = weigth;
                }
                //std::cout << "total " << total_weight << "\n";
                weigth -= matrix[curr_path[curr_path.size() - 1] - 1][curr_path[0] - 1];
            }
            else
            {
                depth_search(matrix, new_pieces, weigth, total_weight, result);
                weigth -= matrix[curr_end - 1][curr_path[curr_path.size()-1] - 1];
            }
        }
        else
            break;
        // std::cout<<"total "<<total_weight<<"\n";
    }
    return;
}

int cost_path(std::vector<std::vector<int>> matrix, std::vector<int> result_path){
    int start = result_path[0] - 1;
    int cost = 0;
    for (int i = 1; i < result_path.size(); i++){
        int next = result_path[i] - 1;
        cost += matrix[start][next];
        start = next;
    }
    return cost;
}

void AMP(std::vector<std::vector<int>> matrix, std::vector<int>& result_path, std::vector<std::vector<int>> pieces, int& total){
    int i = 0;
    bool modification = true;
    int n = matrix.size();
    int total_weight = cost_path(matrix, result_path);
    std::vector<int> modify_path;
    modify_path.push_back(result_path[0]);
    int index = 1;
    int new_total = total_weight;
    std::vector<int> new_path;
    for (auto x:result_path) new_path.push_back(x);

    while(modification == true && i < n){
        modification = false;

        std::vector<std::vector<int>> may_pass = find_may_pass(pieces);
        std::vector<int> next_point = find_next(matrix, may_pass, modify_path, pieces);

        // std::cout<<"next: ";
        // for (auto x: next_point) std::cout<<x<<" ";
        // std::cout<<"\n";

        for (int j = 0; j < next_point.size(); j++){
            std::vector<int> curr;
            for (auto x: modify_path) curr.push_back(x);
            curr.push_back(next_point[j]);
            int swap_elem = result_path[index];
            for (int p = index + 1; p < n; p++){
                if (new_path[p] == next_point[j]) curr.push_back(swap_elem);
                else curr.push_back(new_path[p]); 
            }
            curr.push_back(curr[0]);
            int cost_curr = cost_path(matrix, curr);

            if (cost_curr < total_weight && cost_curr < new_total){
                new_total = cost_curr;
                new_path = curr;
            }
        }
        modify_path.push_back(new_path[index]);

        // std::cout<<"modify: ";
        // for (auto x: modify_path) std::cout<<x<<" ";
        // std::cout<<"\n";
                
        std::vector<std::vector<int>> new_pieces;
        new_pieces.push_back(modify_path);
        for (int p = 0; p < n; p++){
            int k = 0;
            for (int l = 0; l < modify_path.size(); l++){
                if (p + 1 == modify_path[l]) k++;
            }
            if (k == 0)
                {
                    std::vector<int> new_piece;
                    new_piece.push_back(p + 1);
                    new_pieces.push_back(new_piece);
                }
        }
        pieces = new_pieces;
        index += 1;
        modification = true;
        i++;
    }
    result_path = modify_path;
    total = new_total;
}


int main()
{
    int n;
    std::cout<<"Matrix size ";
    std::cin >> n;
    std::vector<std::vector<int>> matrix_of_graph = read_matrix(n);
    // for (int i = 0; i < n; i++)
    // {
    //     for (int j = 0; j < n; j++)
    //     {
    //         if (i == j)
    //         {
    //             std::cout << "INF\n";
    //         }
    //         else
    //         {
    //             std::cout << "Write weight " << i << " " << j << ": ";
    //             std::cin >> matrix_of_graph[i][j];
    //         }
    //     }
    // }

    print_matrix(matrix_of_graph);

    int start;
    std::cout<<"Start ";
    std::cin>>start;
    std::vector<std::vector<int>> pieces;
    pieces.push_back({start});
    for (int i = 1; i <= n; i++)
    {
        if (i != start){
            std::vector<int> new_piece;
            new_piece.push_back(i);
            pieces.push_back(new_piece);
        }
    }
    
    std::vector<int> curr_path;
    curr_path.push_back(start);
    int weight = 0;
    int total_weight = INF;
    std::vector<int> result;
    std::vector<int> result_AMP;
    for (int i = 1; i <= n; i++){
        result_AMP.push_back(i);
    }
    result_AMP.push_back(1);
    int total;
    depth_search(matrix_of_graph, pieces, weight, total_weight, result);
    AMP(matrix_of_graph,result_AMP,pieces,total);

    std::cout<<"Kommivoyazhor path1: ";
    for (auto x : result)
        std::cout << x << " ";
    std::cout << "\n";
    std::cout << "cost1 "<<total_weight << "\n";

    std::cout<<"Kommivoyazhor path2: ";
    for (auto x : result_AMP)
        std::cout << x << " ";
    std::cout << "\n";
    std::cout << "cost2 "<<total << "\n";

    return 0;
}
