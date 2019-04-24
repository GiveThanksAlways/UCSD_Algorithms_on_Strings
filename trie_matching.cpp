#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

typedef map<char, int> edges;
typedef vector<edges> trie;


trie build_trie(const vector<string> & patterns) {
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

vector <int> solve (const string& text, int n, const vector <string>& patterns)
{
	vector <int> result;

	trie t = build_trie(patterns);
  int j=0;
  
  for(auto & letter : text){// go through each letter in the text 
    int current_node = 0;
    for(int i = j; i < text.size(); i++){// go from current letter to end of text 
      char currentLetter = text[i];
      // search all children nodes to see if one of the chilren match the current letter
      bool hit = false;
      for(auto & node : t[current_node]){
        if(node.first == currentLetter){// hit, we have a match 
          hit = true;
          current_node = node.second;// go to the child
          if(t[current_node].empty())// we reached a leaf, so the full pattern matches 
            cout << j << " ";

          break;
        }
      }
      if(!hit)
        break;
    }
    
    j++;
  }


	return result;
}

int main (void){
	string text;
	cin >> text;

	int n;
	cin >> n;

	vector <string> patterns (n);
	for (int i = 0; i < n; i++){
		cin >> patterns[i];
	}

	vector <int> ans;
	ans = solve (text, n, patterns);

	for (int i = 0; i < (int) ans.size (); i++){
		cout << ans[i];
		if (i + 1 < (int) ans.size ()){
			cout << " ";
		}else{
			cout << endl;
		}
	}

	return 0;
}
