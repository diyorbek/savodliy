#include <napi.h>

#include "trie.h"

namespace bridge {

trie* prefix_tree;

void Init(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (prefix_tree) {
    Napi::TypeError::New(env, "Already initialized")
        .ThrowAsJavaScriptException();
    return;
  }

  if (!info.Length() || !info[0].IsString() || info[0].IsEmpty()) {
    prefix_tree = new trie();
  } else {
    auto file_path = info[0].As<Napi::String>().Utf8Value();
    prefix_tree = new trie(file_path.c_str());
  }
}

Napi::Value Find(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (!info.Length() || !info[0].IsString()) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!prefix_tree) {
    Napi::TypeError::New(env, "Not initialized").ThrowAsJavaScriptException();
    return env.Null();
  }

  auto word = info[0].As<Napi::String>().Utf8Value();
  bool result = prefix_tree->find(word);

  return Napi::Boolean::New(env, result);
}

void Insert(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (!info.Length() || !info[0].IsString()) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return;
  }

  if (!prefix_tree) {
    Napi::TypeError::New(env, "Not initialized").ThrowAsJavaScriptException();
    return;
  }

  auto word = info[0].As<Napi::String>().Utf8Value();
  prefix_tree->insert(word);
}

Napi::Value StartsWith(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (!info.Length() || !info[0].IsString()) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return env.Null();
  }

  if (!prefix_tree) {
    Napi::TypeError::New(env, "Not initialized").ThrowAsJavaScriptException();
    return env.Null();
  }

  int default_limit = 5;
  auto word = info[0].As<Napi::String>().Utf8Value();
  auto limit = info.Length() > 1 ? info[1].As<Napi::Number>().Int32Value()
                                 : default_limit;
  auto results = prefix_tree->starts_with(word, limit);
  auto return_value = Napi::Array::New(env, results.size());
  size_t i = 0;

  for (auto& result : results) {
    return_value[i++] = Napi::String::New(env, result);
  }

  return return_value;
}

void ReadFromDumpFile(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (!info.Length() || !info[0].IsString()) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return;
  }

  if (!prefix_tree) {
    Napi::TypeError::New(env, "Not initialized").ThrowAsJavaScriptException();
    return;
  }

  auto file_path = info[0].As<Napi::String>().Utf8Value();
  prefix_tree->read_from_dump_file(file_path.c_str());
}

void DumpToFile(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (!info.Length() || !info[0].IsString()) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return;
  }

  if (!prefix_tree) {
    Napi::TypeError::New(env, "Not initialized").ThrowAsJavaScriptException();
    return;
  }

  auto file_path = info[0].As<Napi::String>().Utf8Value();
  prefix_tree->dump_to_file(file_path.c_str());
}

}  // namespace bridge

Napi::Object ModuleInit(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "init"),
              Napi::Function::New(env, bridge::Init));
  exports.Set(Napi::String::New(env, "find"),
              Napi::Function::New(env, bridge::Find));
  exports.Set(Napi::String::New(env, "insert"),
              Napi::Function::New(env, bridge::Insert));
  exports.Set(Napi::String::New(env, "startsWith"),
              Napi::Function::New(env, bridge::StartsWith));
  exports.Set(Napi::String::New(env, "readFromDumpFile"),
              Napi::Function::New(env, bridge::ReadFromDumpFile));
  exports.Set(Napi::String::New(env, "dumpToFile"),
              Napi::Function::New(env, bridge::DumpToFile));
  return exports;
}

NODE_API_MODULE(dictionary, ModuleInit)