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
  int node_ID;
};

typedef map<char, node> treeEdges;
typedef vector<treeEdges> SuffixTree;


void add_node_to_root(SuffixTree & t,int & i, const string & text,int currentNode,char current_symbol){

  treeEdges newEdge;
  t.push_back(newEdge);
  node newNode;
  newNode.node_ID = t.size()-1;
  newNode.start = i;
  newNode.length = text.size() - i;
  //cout <<"added to root: "<< newNode.start << " " << newNode.length << endl;
  t[currentNode].insert(std::pair<char,node>(current_symbol,newNode));

}

int add_node_to_current_node(SuffixTree & t,const string & text,  int start, int length, int currentNode){
  treeEdges newEdge;
  t.push_back(newEdge);
  node newNode;
  newNode.node_ID = t.size()-1;
  newNode.start = start;
  newNode.length = length;
  //cout <<"added new Node: "<< newNode.start << " " << newNode.length << endl;
  t[currentNode].insert(std::pair<char,node>(text[start],newNode));
  return newNode.node_ID;
}

int add_node_to_new_parent_node(SuffixTree & t,const string & text,  int start, int length, int currentNode, int detached_ID){
  treeEdges newEdge;
  t.push_back(newEdge);
  node newNode;
  newNode.node_ID = detached_ID;
  newNode.start = start;
  newNode.length = length;
  //cout <<"added new Node: "<< newNode.start << " " << newNode.length << endl;
  t[currentNode].insert(std::pair<char,node>(text[start],newNode));
  return newNode.node_ID;
}

int substring_match_length(SuffixTree & t,int i, const string & text,int currentNode,char current_symbol){
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

void no_match(SuffixTree & t,int & i, const string & text,int currentNode,char current_symbol, int nextNode){
  if(currentNode == 0){// root node, just add
    add_node_to_root(t, i, text, currentNode, current_symbol);
    //break;
    i = text.size(); // equivalent of breaking the for loop 

  }else{
    // split the node based on how much of a common substring they have 
    add_node_to_current_node(t, text, i, text.size() - i, currentNode);
    i = text.size(); //ends for this suffix to go to the next one 
  }
}

void match(SuffixTree & t,int & i, const string & text,int currentNode,char current_symbol, int nextNode){
    /** if there is a match, check for how much of substring matches 
      if the full substring matches, then current node becomes child node for   next loop   */
  int k = substring_match_length(t, i, text, currentNode, current_symbol);
  if(k != -1){
    if(t[currentNode].find(current_symbol)->first == t[nextNode].find(current_symbol)->first){// if they are matching, then we switch back one node
      // detach from previous node, add new node and then have the new node point back to the detached guy
      int substring_length = t[currentNode].find(current_symbol)->second.length;
      int substring_start  = t[currentNode].find(current_symbol)->second.start;
      int detached_start =  substring_start;
      int detached_length = substring_length;
      int detached_ID = t[currentNode].find(current_symbol)->second.node_ID; //t[currentNode].find(text[detached_start])->second.node_ID;
      t[currentNode].erase(current_symbol);// erase from map
      int newGuy_ID = add_node_to_current_node(t, text, detached_start, k, currentNode);// 0,1
      add_node_to_new_parent_node(t,text, substring_start+k, substring_length - k, newGuy_ID, detached_ID);// 1,1
      add_node_to_new_parent_node(t,text, i+k, text.size() - i - k, newGuy_ID, detached_ID); // 3, 1

      i = text.size();
    }else{

      // split according to matching substring. Original node only keeps matching substring
      // make new node to the left and old node to the right for the other guy that got cut in two
      int substring_length = t[currentNode].find(current_symbol)->second.length;
      int substring_start  = t[currentNode].find(current_symbol)->second.start;
      add_node_to_current_node(t, text, substring_start+k, substring_length - k, nextNode);// 1, 1 
      // now update the old node with only the matching substring of length k
      t[currentNode].find(current_symbol)->second.length = k;// 0 , 1

      add_node_to_current_node(t, text, i+k, text.size() - i - k, nextNode); // guessing 3,1
      // suffix is now added so update i so that we can start the next suffix 
      i = text.size();
    }
  }else{// the full substring matched, so we update to the next node and keep going down the suffix 
    //currentNode = t[currentNode].find(current_symbol)->second.node_ID;
    i += t[currentNode].find(current_symbol)->second.length -1;// walk i a distance k and subtract 1 because the for loop will advance it
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
      int nextNode = t[currentNode].find(current_symbol)->second.node_ID;

      if( nextNode != 0 ){
        match( t, i, text, currentNode, current_symbol,nextNode);
        currentNode = nextNode;
      }else{
        no_match( t, i, text, currentNode, current_symbol,nextNode);
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




See visualization for help
https://visualgo.net/en/suffixtree



*/

