#include <napi.h>

#include "trie.h"

namespace bridge {

trie* prefix_tree;

void Init(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  if (!info.Length() || !info[0].IsString() || info[0].IsEmpty()) {
    Napi::TypeError::New(env, "Invalid argument").ThrowAsJavaScriptException();
    return;
  }

  if (prefix_tree) {
    Napi::TypeError::New(env, "Already initialized")
        .ThrowAsJavaScriptException();
    return;
  }

  auto file_path = info[0].As<Napi::String>().Utf8Value();
  prefix_tree = new trie(file_path.c_str());
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
  return exports;
}

NODE_API_MODULE(dictionary, ModuleInit)