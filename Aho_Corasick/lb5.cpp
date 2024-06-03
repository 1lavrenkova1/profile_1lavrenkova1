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
    int patt_ind = 0;
    char char_to_parent;
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
            std::cout<<"-> +new ->"<<sim<<"->";
        }
        else{
            std::cout<<sim<<"->";
        }
        curr = curr->children[sim];
    }
    curr->terminal = true;
    curr->patt_ind = ind_pattern;
    std::cout<<"terminal\n";
}

std::vector<std::pair<int,int>> aho_corasic(Node* root, std::string text, std::vector<std::string> patterns){
    std::vector<std::pair<int,int>> result;
    Node* curr = root;
    int ind = 1;
    for (int i = 0; i < text.length(); i++){
        char sim = text[i];
        Node* new_curr = get_link(curr, sim, root);
        curr = new_curr;
        if (curr->terminal == true){
            result.push_back(std::make_pair(i-patterns[curr->patt_ind-1].length() + 2,curr->patt_ind));
            //ind++;
        }
        while(new_curr != root){
            new_curr = get_zip(new_curr, root);
            if (new_curr->terminal == true){
                result.push_back(std::make_pair(i-patterns[new_curr->patt_ind-1].length() + 2,new_curr->patt_ind));
            }
        }
        //if (new_curr->terminal == true
        //curr = new_curr;
    }
    return result;
}

void read_patterns(std::vector<std::string>& patterns, int n){
    std::string str;
    for (int i = 0; i < n; i++){
        std::cin>>str;
        patterns.push_back(str);
    }
}

void print_result(std::vector<std::pair<int,int>> result){
    for (int i = 0; i < result.size(); i++){
        std::cout<<result[i].first<<" "<<result[i].second<<"\n";
    }
}

int max_suff_chain(Node* root, std::vector<std::string> patterns){
    int max = -1;
    for (auto str: patterns){
        Node* curr = root;
        for (auto s: str){
            if (curr->children[s] != nullptr){
                curr = curr->children[s];
                Node* new_curr = curr;
                int len = 0;
                while(new_curr != root && new_curr->suffix != nullptr){
                    if(new_curr->suffix != nullptr){
                        new_curr = new_curr->suffix;
                        len += 1;
                    }
                }
                max = std::max(max, len);
                len = 0;
            }
        }
    }
    return max;
}

int max_zip_chain(Node* root, std::vector<std::string> patterns){
    int max = -1;
    for (auto str: patterns){
        Node* curr = root;
        for (auto s: str){
            if (curr->children[s] != nullptr){
                curr = curr->children[s];
                Node* new_curr = curr;
                int len = 0;
                while(new_curr != root && new_curr->zip_suffix != nullptr){
                    if(new_curr->zip_suffix != nullptr){
                        new_curr = new_curr->zip_suffix;
                        len += 1;
                    }
                }
                max = std::max(max, len);
                len = 0;
            }
        }
    }
    return max;
}


void initial(Node* root, std::vector<std::string> patterns){
    std::cout<<"avtomat\n";
    for (auto str: patterns){
        Node* curr = root;
        std::cout<<"root->";
        for (auto s: str){
            if (curr->children[s] != nullptr){
                curr = curr->children[s];
                std::cout<<curr->sim<<" ";
                if (curr->suffix != nullptr){
                    Node* suff = curr->suffix;
                    if (suff == root) std::cout<<"suffix: root->";
                    else{
                        std::cout<<"suffix: "<<suff->sim<<" ";
                        if (suff->parent == root) std::cout<<"suff_parent: root"<<"->";
                        else std::cout<<"suff_parent: "<<suff->parent->sim<<"->";
                    }
                }
            }
        }
        std::cout<<"\n";
    }
    
}

int main(){
    std::string text;
    std::cin>>text;
    Node* root = new Node;
    // root->parent = root;
    // root->suffix = root;
    int n;
    std::cin>>n;
    std::vector<std::string> patterns;
    read_patterns(patterns, n);
    for (int i = 0; i < n; i++){
        add_string(root, patterns[i], i+1);
    }
    std::vector<std::pair<int,int>> result = aho_corasic(root, text, patterns);
    initial(root, patterns);
    std::cout<<"length max suffix chain = "<<max_suff_chain(root, patterns)<<"\n";
    std::cout<<"length max suffix chain = "<<max_zip_chain(root, patterns)<<"\n";
    return 0;
}