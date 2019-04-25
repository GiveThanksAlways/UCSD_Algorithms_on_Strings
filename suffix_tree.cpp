#include <algorithm>
#include <cassert>
#include <cstdio>
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

struct node{
  int start;
  int length;
  int current_node;
};

typedef map<char, node> treeEdges;
typedef vector<treeEdges> SuffixTree;


void add_node_to_root(SuffixTree & t,int & i, const string & text,int currentNode,char current_symbol){

  treeEdges newEdge;
  t.push_back(newEdge);
  node newNode;
  newNode.current_node = t.size()-1;
  newNode.start = i;
  newNode.length = text.size() - i;
  cout << newNode.start << " " << newNode.length << endl;
  t[currentNode].insert(std::pair<char,node>(current_symbol,newNode));

}

int substring_match_length(SuffixTree & t,int & i, const string & text,int currentNode,char current_symbol){
  int substring_length = t[currentNode].find(current_symbol)->second.length;
  int substring_start  = t[currentNode].find(current_symbol)->second.start;
  for(int k = 0; k < substring_length; k++){
    char nodeSubStringChar = text[substring_start + k];
    char currentStringChar = text[i+k];
    if(nodeSubStringChar != currentStringChar){

      return k; // return k being the length of the matching substring
      // k will be the number that determines how we split the node 
    }
  }
  return -1;// -1 means you have a matching substring ie each char matched so far 
}

void no_match(SuffixTree & t,int & i, const string & text,int currentNode,char current_symbol){
  if(currentNode == 0){// root node, just add
    add_node_to_root
  (t, i, text, currentNode, current_symbol);
    //break;
    i = text.size(); // equivalent of breaking the for loop 

  }else{
    // split the node based on how much of a common substring they have 
    int k = substring_match_length(t, i, text, currentNode, current_symbol);
    /** ___--__---@#$%@#$%@#%4
        ---__--___@$%@#$%@#$%@#$
         ___--__---@$%@#$%@#$%
        ---__--___@#$%@#$%@#$%@
         ___--__---@#$%@#$%@#$%
        ---__--___@$#%@#$%@#$%@
    */
  }
}

void match(SuffixTree & t,int & i, const string & text,int currentNode,char current_symbol){
    /** if there is a match, check for how much of substring matches 
      if the full substring matches, then current node becomes child node for   next loop   */
  int k = substring_match_length(t, i, text, currentNode, current_symbol);
  if(k != -1){
    // split according to matching substring 
    

  }else{
    currentNode = t[currentNode].find(current_symbol)->second.current_node;
    i += k;// walk i a distance k 
  }
}

SuffixTree build_suffix_tree(const string & text) {
  SuffixTree t;
  treeEdges root;
  t.push_back(root);
  
  for(int j = 0; j < text.size(); j++){// go through each suffix 
    int currentNode = 0;// root node

    for(int i = j; i < text.size(); i++){// go from beginning of each suffix till the end of text 
      //cout << text.at(i); // unit test, run through each suffix and print 
      char current_symbol = text[i];
      int nextNode = t[currentNode].find(current_symbol)->second.current_node;

      if( nextNode != 0 ){
        match( t, i, text, currentNode, current_symbol);
      }else{
        no_match( t, i, text, currentNode, current_symbol);
      }
    }
  }
  return t;
}


int main (void){
	string text;
	cin >> text;

	SuffixTree ans;
  ans = build_suffix_tree(text);

  cout <<endl << endl << "answer:" << endl;
	for(treeEdges & vect : ans){
    for(auto & elem : vect){
      for(int i = 0; i < elem.second.length; i++){
        cout << text[elem.second.start + i];
      }
      cout << endl;
    }
  }
	return 0;
}

/**

panamabananas$

A$

ACA$












*/


/**
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
          if(t[current_node].find('$')->first == '$'){
            // we reached the end of a pattern
            char nextLetter = text[j+1];// get the next letter to look ahead to see if there is overlap 
            if(t[current_node].find(nextLetter)->second == 0){// nextLetter not in trie 
              cout << j << " ";// no next letter, we hit the end anyways so lets keep going 
              //cout << text[j] << text[j+1] << text[j+2] << endl;
            }
          }
          break;// we found a match, end for loop running through the nodes 
        }
      }
      if(!hit)// no match in the trie, so break second for loop 
        break;
    }
    
    j++;
  }


	return result;
}*/

/**
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
      // note if the node is the end of a pattern in the trie 
      if(CurrentPattern.at(CurrentPattern.length()-1) == currentSymbol){
        // this is the end of pattern, mark the node with a $ symbol 
        //cout << "end of pattern: " << daPattern << endl;
        t[currentNode].insert(std::pair<char,int>('$',t.size()-1));

      }
          
    }
  }
  return t;
}*/