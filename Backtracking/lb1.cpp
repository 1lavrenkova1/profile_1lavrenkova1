#include <iostream>
#include <vector>


class Square{
    public:
        int x_coord;
        int y_coord;
        int size;
        Square(int x, int y, int size): x_coord{x}, y_coord{y}, size(size){} 
};


void print_result(std::vector<Square>& table, int min){
    std::cout<<table.size()<<"\n";
    for(size_t i = 0; i < table.size(); i++){
        std::cout<<(table[i].x_coord*min)+1<<" "<<(table[i].y_coord*min)+1<<" "<<table[i].size*min<<"\n";
    }
}

bool check_new_square(int x, int y, std::vector<Square>& table){
    for (size_t i = 0; i < table.size(); i++){
        if ((table[i].x_coord <= x && x < table[i].x_coord + table[i].size) && (table[i].y_coord <= y && y < table[i].y_coord + table[i].size)){
            return true;
        }
    }
    return false;
}

void backtraking(std::vector<Square>& result, std::vector<Square> curr_table, int curr_area, int count, int min_x, int min_y, int& k, int n){
    for (int x = min_x; x < n; x++){
        for (int y = min_y; y < n; y++){
            if(!check_new_square(x,y,curr_table)){
                int limit = std::min(n-x, n-y);
                for (int i = 0; i < curr_table.size(); i++){
                    if (curr_table[i].x_coord + curr_table[i].size > x && curr_table[i].y_coord > y){
                        limit = std::min(limit, curr_table[i].y_coord - y);
                    }
                }

                for(int sq_size = limit; sq_size > 0; sq_size--){
                    Square new_square = Square(x,y,sq_size);
                    curr_table.push_back(new_square);

                    if (curr_area + new_square.size*new_square.size == n*n){
                        if (count + 1 < k){
                            k = count + 1;
                            result = curr_table;
                        }
                    }
                    else {
                        if (count + 1 < k){
                            backtraking(result, curr_table, curr_area + new_square.size*new_square.size, count + 1, x, y + sq_size, k, n);
                        }
                    }
                    curr_table.pop_back();
                }
                return;

            }
        }
        min_y = n / 2;
    }
}

int main(){
    int n;
    std::cin>>n;
    int k;
    std::vector<Square> square_table;
    std::vector<Square> result;
    int min_simple = 1;

    if (n % 2 == 0){
        k = 4;
        square_table.push_back(Square(0,0,n/2));
        square_table.push_back(Square(n/2,0,n/2));
        square_table.push_back(Square(0,n/2,n/2));
        square_table.push_back(Square(n/2,n/2,n/2));
        result = square_table;
    }

    else{
        k = n * 2 + 1;
        for (int i = 2; i < n; i++){
            if (n % i == 0) min_simple = i;
        }

        n /= min_simple;
        square_table.push_back(Square(0,0,(n + 1)/2));
        square_table.push_back(Square(0,(n + 1)/2, n / 2));
        square_table.push_back(Square((n + 1)/2,0, n / 2));

        int curr_area = (((n + 1)/2)*((n + 1)/2) + 2 * (n/2) * (n/2));
        int new_count = 3;
        int min_x = n / 2;
        int min_y = (n + 1)/2;
        backtraking(result, square_table, curr_area, new_count, min_x, min_y, k, n);
    }

    print_result(result, min_simple);
    return 0;
}


