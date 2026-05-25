#ifndef NMSDK_REGISTER_MOTION_CONTROL_LIB_LLM_TOOLS_H
#define NMSDK_REGISTER_MOTION_CONTROL_LIB_LLM_TOOLS_H

namespace RDK::LLM {
class ILLMProjectContextProvider;
class ULLMToolRegistry;
}

void RegisterMotionControlLibLlmTools(RDK::LLM::ULLMToolRegistry& registry,
                                      RDK::LLM::ILLMProjectContextProvider* project_context);

#endif
