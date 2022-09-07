#pragma once

#include <map>
#include <string>
#include <vector>

struct node {
  char letter;
  bool is_word;
  std::map<char, node*> children;

  node(char c);
};

struct prefix_node {
  node* node;
  std::string prefix_string;
};

class trie {
  node* root;
  prefix_node get_prefix_node(std::string word);

 public:
  trie(const char* dump_file_path);

  void insert(std::string word);
  bool find(std::string word);
  std::vector<std::string> starts_with(std::string word, size_t limit);
};