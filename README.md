# ai_term

An intelligent terminal with AI-powered assistance, written in C++.

## Features
- Command parsing and execution
- Natural language understanding via Llama.cpp (local LLM)
- Command history and auto-completion (planned)

## Getting Started
1. Clone this repo
2. Install dependencies (CMake, C++17 compiler, Llama.cpp)
3. Build and run:
   ```sh
   mkdir build && cd build
   cmake ..
   make
   ./ai_term
   ```
4. Download a Llama model and follow Llama.cpp instructions to set it up.

## Integrating Llama.cpp
- Llama.cpp is required for AI functionality.
- You can add it as a submodule or build it separately and link it.
- See [Llama.cpp repo](https://github.com/ggerganov/llama.cpp) for setup.

## Roadmap
- [ ] Basic terminal loop
- [ ] Llama.cpp integration
- [ ] Command suggestions and explanations
- [ ] History and completion
