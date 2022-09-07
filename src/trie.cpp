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

prefix_node trie::get_prefix_node(string word) {
  auto current = root;
  string prefix;

  for (char c : word) {
    if (!current->children.count(c)) {
      return prefix_node{current == root ? nullptr : current, prefix};
    }

    current = current->children[c];
    prefix += c;
  }

  return prefix_node{current, prefix};
}

void dfs(prefix_node& start, size_t limit, vector<string>& words) {
  if (start.node->is_word && words.size() < limit)
    words.push_back(start.prefix_string);

  if (words.size() == limit) {
    return;
  }

  auto& children = start.node->children;

  for (auto iter = children.begin(); iter != children.end(); iter++) {
    auto trie_node = iter->second;
    auto prefix_string = start.prefix_string + iter->first;
    auto next = prefix_node{trie_node, prefix_string};

    dfs(next, limit, words);
  }
}

vector<string> trie::starts_with(string word, size_t limit) {
  auto prefix = get_prefix_node(word);

  if (!prefix.node)
    return {};

  vector<string> words;

  dfs(prefix, limit, words);

  return words;
}
