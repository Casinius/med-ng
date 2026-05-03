#include <lsp/connection.h>
#include <lsp/io/standardio.h>
#include <lsp/messagehandler.h>
#include <lsp/messages.h> // Generated message definitions
#include <lsp/process.h>
#include "framework.hpp"
#ifndef __MED__LSP__
#define __MED__LSP__
struct LSP_Process {
  lsp::Process process;
  explicit LSP_Process(std::string args) {
    this->process = lsp::Process(args);
  }
  ~LSP_Process() {
    this->process.terminate();
  }
};

enum Semantic_Tokens {

};
struct Document_Context {
  std::string temp;
  lsp::Connection con_ctx;
  lsp::MessageHandler msg_handle;

  Document_Context(LSP_Process& lsp_p)
      : con_ctx(lsp_p.process.stdIO()), msg_handle(lsp::MessageHandler(con_ctx)) {}
  auto launch_diag(text_editor::Document_Vis& dvis) {
    msg_handle.add<lsp::notifications::TextDocument_PublishDiagnostics>(
        [&](lsp::PublishDiagnosticsParams&& param) {
            for (auto&x : param.diagnostics) {
                dvis.list_diagnostic.emplace_back(x.message);
            }
            
        });
  }
  auto set_workspace() {}
  auto recv() {}
};

#endif
