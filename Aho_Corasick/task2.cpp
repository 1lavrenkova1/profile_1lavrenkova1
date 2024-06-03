#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>

const int k = 5;

struct Node{
    bool terminal = false;
    std::map<char, Node*> children;
    std::map<char, Node*> moves;
    char sim;
    Node* suffix = nullptr;
    Node* parent = nullptr;
    Node* zip_suffix = nullptr;
    std::vector<int> patt_ind;
    char char_to_parent;
    //int position = 0;
};

Node* get_suff_link(Node* curr, Node* root);

Node* get_link(Node* curr, char simb, Node* root){
    if (curr->moves[simb] == nullptr){
        if (curr->children[simb] != nullptr){
            curr->moves[simb] = curr->children[simb];
        }
        else if (curr == root){
            curr->moves[simb] = root;
        }
        else{
            curr->moves[simb] = get_link(get_suff_link(curr,root),simb,root);
        }
    }
    return curr->moves[simb];
}


Node* get_suff_link(Node* curr, Node* root){
    if (curr->suffix == nullptr){
        if (curr == root || curr->parent == root){
            curr->suffix = root;
        }
        else{
            curr->suffix = get_link(get_suff_link(curr->parent, root), curr->char_to_parent, root);
        }
    }
    return curr->suffix;
}

Node* get_zip(Node* curr, Node* root){
    if (curr->zip_suffix == nullptr){
        Node* suff_link = get_suff_link(curr, root);
        if (suff_link->terminal == true){
            curr->zip_suffix = suff_link;
        }
        else if (suff_link == root){
            curr->zip_suffix = root;
        }
        else{
            curr->zip_suffix = get_zip(suff_link, root);
        }
    }
    return curr->zip_suffix;
}

void add_string(Node* root, std::string new_s, int ind_pattern){
    Node* curr = root;
    for (int i = 0; i < new_s.length(); i++){
        char sim = new_s[i];
        if (curr->children[sim] == nullptr){
            curr->children[sim] = new Node;
            curr->children[sim]->sim = sim;
            curr->children[sim]->parent = curr;
            curr->children[sim]->char_to_parent = sim;
        }
        curr = curr->children[sim];
    }
    curr->terminal = true;
    curr->patt_ind.push_back(ind_pattern);
}

void aho_corasic(Node* root, std::string text, std::vector<std::string> patterns, std::vector<std::pair<int,int>>& result){
    Node* curr = root;
    for (int i = 0; i < text.length(); i++){
        char sim = text[i];
        Node* new_curr = get_link(curr, sim, root);
        curr = new_curr;
        if (curr->terminal == true){
            int ind = i - patterns[curr->patt_ind[0]].length() + 2;
            for (auto j: curr->patt_ind) {
                result.push_back(std::make_pair(ind, j + 1));
            }
        }
        while(new_curr != root){
            new_curr = get_zip(new_curr, root);
            if (new_curr->terminal == true){
                int ind = i - patterns[new_curr->patt_ind[0]].length() + 2;
                for (auto j: new_curr->patt_ind){
                    result.push_back(std::make_pair(ind, j + 1));
                }
            }
        }
    }
}


void print_result(std::vector<int> result){
    std::sort(std::begin(result), std::end(result));
    for (int i = 0; i < result.size(); i++){
        std::cout<<result[i]<<"\n";
    }
}

int main(){
    std::string text;
    std::cin>>text;
    std::string pattern;
    std::cin>>pattern;
    char wild_card;
    std::cin>>wild_card;
    std::vector<std::string> patterns;
    std::string s = "";
    std::vector<int> start_pos;
    int ind = 1;
    for (int i = 0; i < pattern.length(); i++){
        if (pattern[i] != wild_card) {
            s += pattern[i];
        }
        else if (!s.empty()){
            start_pos.push_back(i + 1 - s.length());
            patterns.push_back(s);
            s = "";
        }
    }
    if (!s.empty()){
        start_pos.push_back(pattern.length() - s.length() + 1);
        patterns.push_back(s);
    }
    Node* root = new Node;

    for (auto x:patterns) std::cout<<x<<"\n";
    std::cout<<"start_pos ";
    for (auto x:start_pos) std::cout<<x<<" ";
    std::cout<<"\n";

    for (int i = 0; i < patterns.size(); i++){
        add_string(root, patterns[i], i);
    }

    std::vector<std::pair<int,int>> pos_mask;
    aho_corasic(root, text, patterns, pos_mask);

    std::cout<<"pos_mask\n";
    for (auto x: pos_mask){
        std::cout<<x.first<<" "<<x.second<<"\n";
    }

    std::vector<int> pattern_in_text(text.length()+1, 0);
    for (int i = 0; i < pos_mask.size(); i++){
        int pos = pos_mask[i].first - start_pos[pos_mask[i].second - 1] + 1;
        if (pos > 0) pattern_in_text[pos] += 1;
    }
    
    std::cout<<"c[i]\n";
    for (int i = 0; i < pattern_in_text.size(); i++){
        std::cout<<i<<" "<<pattern_in_text[i]<<"\n";
    }
    
    int k = patterns.size();
    std::vector<int> result;
    for (int i = 0; i < pattern_in_text.size(); i++){
        if (pattern_in_text[i] == k && i + pattern.length() - 1 <= text.length()){
            result.push_back(i);
        }
    }
    print_result(result);
    return 0;
}