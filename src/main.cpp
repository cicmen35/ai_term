#include <iostream>
#include <string>
#include "wrapper.h"

int main() {
    std::cout << "Welcome to intelligent terminal! Type 'exit' to quit.\n";
    AI ai;
    std::string modelPath = "../external/llama.cpp/models/tinyllama-1.1b-chat-v1.0.Q4_K_M.gguf";
    if (!ai.loadModel(modelPath)) {
        std::cerr << "Failed to load model. Exiting.\n";
        return 1;
    }
    std::string input;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, input);
        if (input == "exit") break;
        std::string response = ai.generate(input);
        std::cout << "AI: " << response << std::endl;
    }
    std::cout << "Goodbye!\n";
    return 0;
}
