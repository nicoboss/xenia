/**
 ******************************************************************************
 * Xenia : Xbox 360 Emulator Research Project                                 *
 ******************************************************************************
 * Copyright 2013 Ben Vanik. All rights reserved.                             *
 * Released under the BSD license - see LICENSE in the root for more details. *
 ******************************************************************************
 */

#ifndef ALLOY_RUNTIME_RUNTIME_H_
#define ALLOY_RUNTIME_RUNTIME_H_

#include <memory>
#include <mutex>
#include <vector>

#include <alloy/backend/backend.h>
#include <alloy/frontend/frontend.h>
#include <alloy/memory.h>
#include <alloy/runtime/debugger.h>
#include <alloy/runtime/entry_table.h>
#include <alloy/runtime/module.h>
#include <alloy/runtime/symbol_info.h>
#include <alloy/runtime/thread_state.h>

namespace alloy {
namespace runtime {

class Runtime {
 public:
  explicit Runtime(Memory* memory, uint32_t debug_info_flags = 0,
                   uint32_t trace_flags = 0);
  virtual ~Runtime();

  Memory* memory() const { return memory_; }
  Debugger* debugger() const { return debugger_.get(); }
  frontend::Frontend* frontend() const { return frontend_.get(); }
  backend::Backend* backend() const { return backend_.get(); }

  int Initialize(std::unique_ptr<frontend::Frontend> frontend,
                 std::unique_ptr<backend::Backend> backend = 0);

  int AddModule(std::unique_ptr<Module> module);
  Module* GetModule(const char* name);
  Module* GetModule(const std::string& name) { return GetModule(name.c_str()); }
  std::vector<Module*> GetModules();

  Module* builtin_module() const { return builtin_module_; }
  FunctionInfo* DefineBuiltin(const std::string& name,
                              FunctionInfo::ExternHandler handler, void* arg0,
                              void* arg1);

  std::vector<Function*> FindFunctionsWithAddress(uint64_t address);

  int LookupFunctionInfo(uint64_t address, FunctionInfo** out_symbol_info);
  int LookupFunctionInfo(Module* module, uint64_t address,
                         FunctionInfo** out_symbol_info);
  int ResolveFunction(uint64_t address, Function** out_function);

  // uint32_t CreateCallback(void (*callback)(void* data), void* data);

 private:
  int DemandFunction(FunctionInfo* symbol_info, Function** out_function);

 protected:
  Memory* memory_;

  uint32_t debug_info_flags_;
  uint32_t trace_flags_;

  std::unique_ptr<Debugger> debugger_;

  std::unique_ptr<frontend::Frontend> frontend_;
  std::unique_ptr<backend::Backend> backend_;

  EntryTable entry_table_;
  std::mutex modules_lock_;
  std::vector<std::unique_ptr<Module>> modules_;
  Module* builtin_module_;
  uint64_t next_builtin_address_;
};

}  // namespace runtime
}  // namespace alloy

#endif  // ALLOY_RUNTIME_RUNTIME_H_
