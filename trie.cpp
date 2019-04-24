#include <string>
#include <iostream>
#include <vector>
#include <map>

using std::map;
using std::vector;
using std::string;
using std::cout;
using std::endl;

typedef map<char, int> edges;
typedef vector<edges> trie;

trie build_trie(vector<string> & patterns) {
  trie t;
  edges root;
  t.push_back(root);
  // go through all patterns
  for(auto & daPattern : patterns){

    int currentNode = 0;// root node
    string CurrentPattern = daPattern;
    //go through each letter of the pattern 
    for(auto & currentSymbol : CurrentPattern){
      //looks at the current node of mapped edges, finds if the letter key is in the map and returns 0 if it is not in the edges from the currentNode
      // if the current Symbol is in the trie then don't add, just go to the next child in the tree
      if( t[currentNode].find(currentSymbol)->second != 0){
        currentNode = t[currentNode].find(currentSymbol)->second;
      }else{
        // currentSymbol not a child of the current node. We add the currentSymbol as a child of the node 
        edges newNode;
        t.push_back(newNode);
        t[currentNode].insert(std::pair<char,int>(currentSymbol,t.size()-1));
        currentNode = t.size()-1;
      }
    }
  }
  return t;
}

int main() {
  size_t n;
  std::cin >> n;
  vector<string> patterns;
  for (size_t i = 0; i < n; i++) {
    string s;
    std::cin >> s;
    patterns.push_back(s);
  }

  trie t = build_trie(patterns);
  for (size_t i = 0; i < t.size(); ++i) {
    for (const auto & j : t[i]) {
      std::cout << i << "->" << j.second << ":" << j.first << "\n";
    }
  }

  return 0;
}
