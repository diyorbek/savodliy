#pragma once

#include <map>
#include <string>
#include <vector>

typedef unsigned short ushort;

struct node {
  char letter;
  bool is_word;
  std::map<char, node*> children;

  node(char c);
  ~node();
};

struct prefix_node {
  node* node;
  std::string prefix_string;
};

class trie {
  node* root;

  prefix_node get_prefix_node(std::string word);
  ushort serialize(node* root, std::ostream& out);
  node* deserialize(std::istream& in);

 public:
  trie();
  trie(const char* word_list_file_path);
  ~trie();

  void read_from_dump_file(const char* dump_file_path);
  void dump_to_file(const char* dump_file_path);

  void insert(std::string word);
  bool find(std::string word);
  std::vector<std::string> starts_with(std::string word, size_t limit);
};