/*
 * Name: Elle Hanckel
 * Date Submitted: 04/25/2023
 * Lab Section: 005
 * Assignment Name: Lab 10 - Using Breadth-First Search to Solve Puzzles
 */

#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <bitset>
using namespace std;

// Reflects what each node represents...
// int with each bit == 0 left of river, bit == 1 right of river
typedef int state;

// Bit position representation for wolf/goat/cabbage/me
bool bit(int x, int i) { return (x>>i) & 1; }
const int wolf=0, goat=1, cabbage=2, me=3;

// GENERIC -- these shouldn't need to be changed...
map<state, bool> visited;         // have we queued up this state for visitation?
map<state, state> pred;           // predecessor state we came from
map<state, int> dist;             // distance (# of hops) from source node
map<state, vector<state>> nbrs;   // vector of neighboring states

map<pair<state,state>, string> edge_label;

// GENERIC (breadth-first search, outward from curnode)
void search(state source_node)
{
  queue<state> to_visit;
  to_visit.push(source_node);
  visited[source_node] = true;
  dist[source_node] = 0;
  
  while (!to_visit.empty()) {
    state curnode = to_visit.front();
    to_visit.pop();
    for (state n : nbrs[curnode])
      if (!visited[n]) {
	pred[n] = curnode;
	dist[n] = 1 + dist[curnode];
	visited[n] = true;
	to_visit.push(n);
      }
  }
}

string state_string(state s)
{
  string items[4] = { "wolf", "goat", "cabbage", "you" }, result = "";
  for (int i=0; i<4; i++)
    if (!bit(s, i)) result += items[i] + " ";
  result += " |river| ";
  for (int i=0; i<4; i++)
    if (bit(s, i)) result += items[i] + " ";
  return result;
}

// GENERIC
void print_path(state s, state t)
{
  if (s != t) {
    print_path(s, pred[t]);
    cout << edge_label[make_pair(pred[t], t)] << ": " << state_string(t) << "\n";
  } else {
    cout << "Initial state: " << state_string(s) << "\n";
  }
}

string neighbor_label(int s, int t)
{
  string items[3] = { "wolf", "goat", "cabbage" }, which_cross;
  if (bit(s,me) == bit(t,me)) return "";  // must cross river
  int cross_with = 0;
  for (int i=0; i<3; i++) {
    if (bit(s,i) != bit(t,i) && bit(s,i)==bit(s,me)) { cross_with++; which_cross = items[i]; }
    if (bit(s,i) != bit(t,i) && bit(s,i)!=bit(s,me)) return "";
  }
  if (cross_with > 1) return "";
  if (cross_with == 0) return "Cross alone";
  else return "Cross with " + which_cross;
}

void build_graph(void)
{
  //for loop, looping through possible outcomes of the four bitsets
  for (int i = 0; i <= 15; i++) {
    bitset<4> nextState(i);

    //crossing with wolf
    if (nextState[wolf] == nextState[me]) {
      //flipping wolf and me
      nextState.flip(wolf);
      nextState.flip(me);
      
      //if the goat is not with the cabbage and the goat is same side as me
      if (!(nextState[goat] == nextState[cabbage] && nextState[goat] != nextState[me])) {
        //pushing back nextState as a long into nbrs at index i and adding label to it
        nbrs[i].push_back(nextState.to_ulong());
        edge_label[make_pair(i, nextState.to_ulong())] = neighbor_label(i, nextState.to_ulong());
      }
      
      //flipping wolf and me back
      nextState.flip(me);
      nextState.flip(wolf);
    }
    
    //crossing with goat
    if (nextState[goat] == nextState[me]) {
      //flipping goat and me 
      nextState.flip(goat);
      nextState.flip(me);

      //pushing back nextState as a long into nbrs at index i and adding label to it
      nbrs[i].push_back(nextState.to_ulong());
      edge_label[make_pair(i, nextState.to_ulong())] = neighbor_label(i, nextState.to_ulong());
    
      //flipping goat and me back
      nextState.flip(me);
      nextState.flip(goat);
    } 

    //crossing with cabbage
    if (nextState[me] == nextState[cabbage]) { 
      //flipping cabbage and me
      nextState.flip(me);
      nextState.flip(cabbage);

      //if the goat is not with the wolf and the wolf is same side as me
      if (!(nextState[wolf] == nextState[goat] && nextState[wolf] != nextState[me])) {
        //pushing back nextState as a long into nbrs at index i and adding label to it
        nbrs[i].push_back(nextState.to_ulong());
        edge_label[make_pair(i, nextState.to_ulong())] = neighbor_label(i, nextState.to_ulong());
      }

      //flipping cabbage and me back
      nextState.flip(me);
      nextState.flip(cabbage);
    } 

    //crossing myself
    nextState.flip(me);
    //if goat is not with cabbage and goat is with me and wolf is not with goat and wolf is with me
    if (!(nextState[goat] == nextState[cabbage] && nextState[goat] != nextState[me]) && !(nextState[wolf] == nextState[goat] && nextState[wolf] != nextState[me])) {
      //pushing back nextState as a long into nbrs at index i and adding label to it
      nbrs[i].push_back(nextState.to_ulong());
      edge_label[make_pair(i, nextState.to_ulong())] = neighbor_label(i, nextState.to_ulong());
    }

    //flipping me back
    nextState.flip(me);
  }
  
}

// int main(void)
// {
//   build_graph();

//   state start = 0, end = 15;
  
//   search(start);
//   if (visited[end])
//     print_path (start, end);
//   else
//     cout << "No path!\n";
  
//   return 0;
// }
