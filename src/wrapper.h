#pragma once
#include <string>

class AI {
public:
    // Loads the model from the given path
    bool loadModel(const std::string& modelPath);

    // Generates a response given a prompt
    std::string generate(const std::string& prompt);

    ~AI();
private:
    void* ctx = nullptr; // Pointer to llama_context
    bool modelLoaded = false;
};
