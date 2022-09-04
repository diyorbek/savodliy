#include <fstream>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>

#include "trie.h"

using std::ifstream;
using std::stack;
using std::string;
using std::vector;

node::node(char c) : letter(c) {}

trie::trie(const char* dump_file_path) : root(new node('\0')) {
  ifstream dump(dump_file_path);
  string word;

  while (dump >> word) {
    insert(word);
  }
}

void trie::insert(string word) {
  auto current = root;

  for (char c : word) {
    if (!current->children.count(c)) {
      current->children[c] = new node(c);
    }

    current = current->children[c];
  }

  current->is_word = true;
}

bool trie::find(string word) {
  auto current = root;

  for (char c : word) {
    if (!current->children.count(c)) {
      return false;
    }

    current = current->children[c];
  }

  return current->is_word;
}

prefix_node* trie::get_prefix_node(string word) {
  auto current = root;
  string prefix;

  for (char c : word) {
    if (!current->children.count(c)) {
      return current == root ? nullptr : new prefix_node{current, prefix};
    }

    current = current->children[c];
    prefix += c;
  }

  return new prefix_node{current, prefix};
}

vector<string> trie::starts_with(string word, size_t limit) {
  auto prefix = get_prefix_node(word);

  if (!prefix)
    return {};

  vector<string> words;
  stack<prefix_node*> node_stack;
  node_stack.push(prefix);

  while (node_stack.empty() == false) {
    prefix_node* pref_node = node_stack.top();
    node_stack.pop();

    if (pref_node->node->is_word)
      words.push_back(pref_node->prefix_string);

    if (words.size() == limit)
      break;

    auto& children = pref_node->node->children;

    for (auto iter = children.rbegin(); iter != children.rend(); iter++) {
      auto trie_node = iter->second;
      auto prefix_string = pref_node->prefix_string + iter->first;

      node_stack.push(new prefix_node{trie_node, prefix_string});
    }
  }

  return words;
}
