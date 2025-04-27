#include "wrapper.h"
#include <llama.h>
#include <iostream>
#include <vector>
#include <string>

// Simple parameters
#define DEFAULT_N_THREADS 4
#define DEFAULT_N_PREDICT 128

bool AI::loadModel(const std::string& modelPath) {
    // Initialize backend
    llama_backend_init();
    
    // Set model parameters
    llama_model_params model_params = llama_model_default_params();

    // Load the model
    llama_model* model = llama_model_load_from_file(modelPath.c_str(), model_params);
    if (!model) {
        std::cerr << "Failed to load model: " << modelPath << std::endl;
        return false;
    }
    
    // Set context parameters
    llama_context_params ctx_params = llama_context_default_params();
    ctx_params.n_ctx = 2048;  // context window size
    ctx_params.n_threads = DEFAULT_N_THREADS;
    
    // Create context
    llama_context* context = llama_init_from_model(model, ctx_params);
    if (!context) {
        std::cerr << "Failed to create context." << std::endl;
        llama_model_free(model);
        return false;
    }
    
    ctx = context;
    modelLoaded = true;
    return true;
}

std::string AI::generate(const std::string& prompt) {
    if (!modelLoaded || !ctx) {
        return "[Model not loaded]";
    }

    llama_context* context = static_cast<llama_context*>(ctx);
    const llama_model* model = llama_get_model(context);
    const llama_vocab* vocab = llama_model_get_vocab(model);

    // Tokenize the prompt
    std::vector<llama_token> tokens_prompt(2048);
    int n_prompt_tokens = llama_tokenize(
        vocab,
        prompt.c_str(),
        prompt.length(),
        tokens_prompt.data(),
        tokens_prompt.size(),
        true,  // add_special: add BOS/EOS if needed
        false  // parse_special: don't parse special tokens
    );
    if (n_prompt_tokens < 0) {
        return "[Tokenization failed]";
    }
    tokens_prompt.resize(n_prompt_tokens);

    // Feed prompt tokens to model
    llama_batch batch = llama_batch_get_one(tokens_prompt.data(), tokens_prompt.size());
    if (llama_decode(context, batch) != 0) {
        return "[Prompt decoding failed]";
    }

    // Generation parameters
    const int n_predict = DEFAULT_N_PREDICT;
    const int n_ctx = llama_n_ctx(context);
    std::vector<llama_token> output_tokens;
    output_tokens.reserve(n_predict);

    // Sampling setup (greedy for simplicity)
    llama_sampler* sampler = llama_sampler_init_greedy();

    // Start generation loop
    llama_token token = 0;
    for (int i = 0; i < n_predict; ++i) {
        token = llama_sampler_sample(sampler, context, -1);
        if (llama_vocab_is_eog(vocab, token) || llama_vocab_is_control(vocab, token)) {
            break;
        }
        output_tokens.push_back(token);
        llama_sampler_accept(sampler, token);

        // Feed generated token back to model
        llama_batch gen_batch = llama_batch_get_one(&token, 1);
        if (llama_decode(context, gen_batch) != 0) {
            break;
        }
    }
    llama_sampler_free(sampler);

    // Detokenize output
    std::string result;
    std::vector<char> result_buf(4096);
    int n_chars = llama_detokenize(
        vocab,
        output_tokens.data(),
        output_tokens.size(),
        result_buf.data(),
        result_buf.size(),
        true,   // remove_special
        false   // unparse_special
    );
    if (n_chars > 0) {
        result.assign(result_buf.data(), n_chars);
    } else {
        result = "[Detokenization failed]";
    }
    return result;
}

AI::~AI() {
    if (ctx) {
        llama_free(static_cast<llama_context*>(ctx));
        ctx = nullptr;
    }
    llama_backend_free();
}
