#include <fstream>
#include <iostream>
#include <map>
#include <stack>
#include <string>
#include <vector>

#include "trie.h"

using std::ifstream;
using std::istream;
using std::noskipws;
using std::ofstream;
using std::ostream;
using std::stack;
using std::string;
using std::to_string;
using std::vector;

node::node(char c) : letter(c), is_word(false) {}
node::~node() {
  for (auto it : children) {
    delete it.second;
  }
}

trie::trie() : root(new node('\0')) {}

trie::trie(const char* word_list_file_path) : root(new node('\0')) {
  ifstream dump(word_list_file_path);
  string word;

  while (dump >> word) {
    insert(word);
  }
}

trie::~trie() {
  delete root;
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

  if (words.size() >= limit) {
    return;
  }

  auto& children = start.node->children;

  for (auto iter = children.begin(); iter != children.end(); iter++) {
    if (words.size() >= limit) {
      return;
    }

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

void trie::dump_to_file(const char* dump_file_path) {
  ofstream dump_file;
  dump_file.open(dump_file_path);
  serialize(root, dump_file);
}

void trie::read_from_dump_file(const char* dump_file_path) {
  ifstream dump_file(dump_file_path);
  delete root;
  root = deserialize(dump_file);
}

ushort trie::serialize(node* root, ostream& out) {
  if (root->letter != 0) {
    out << root->letter;

    if (root->is_word) {
      out << '+';
    }
  }

  ushort r = 0;
  ushort i = 1;
  ushort n = root->children.size();

  for (auto it : root->children) {
    r = serialize(it.second, out);

    if (i++ < n)
      out << r;
  }

  return r + 1;
}

node* trie::deserialize(istream& in) {
  node* root = new node(0);
  stack<node*> s;
  s.push(root);
  char c;

  while (in >> noskipws >> c) {
    int pop_count = 0;

    while (c >= '0' && c <= '9') {
      pop_count = pop_count * 10 + (c - '0');
      in >> noskipws >> c;
    }

    while (pop_count--) {
      s.pop();
    }

    if (c == '+') {
      s.top()->is_word = true;
      continue;
    }

    auto new_node = new node(c);
    s.top()->children[c] = new_node;
    s.push(new_node);
  }

  return root;
}
