#include <stdio.h>
#include <fstream>
#include <iostream>
#include <cstdint>
#include <filesystem>

#include <napi.h>

#include "graph_creator.h"
#include "matching.h"
#include "stats.h"

/// The number of arguments expected by the matching algorithm
#define MATCH_ALGO_ARG_COUNT 5


Napi::Value Match(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    //check argument count
    if (info.Length() != MATCH_ALGO_ARG_COUNT) {
        Napi::Error::New(env, "Wrong number of arguments").ThrowAsJavaScriptException();
        return env.Null();
    }

    //check argument values
    for (int i = 0; i < MATCH_ALGO_ARG_COUNT; i++) {
        //check argument i
        auto arg = info[i];

        //make sure it is a string
        if (!arg.IsString()) {
            Napi::TypeError::New(env, "MATCHING-ALGORITHM: Arguments should be strings!").ThrowAsJavaScriptException();
            return env.Null();
        }
    }

    //for the first 3 arguments, check whether this file exists or not
    for (int i = 0; i < MATCH_ALGO_ARG_COUNT - 2; i++) {
        //check if file exist
        std::string fileName = info[i].As<Napi::String>();

        if(!std::filesystem::exists(fileName)) {
            Napi::Error::New(env, "MATCHING-ALGORITHM: Input file " + fileName + " does not exist").ThrowAsJavaScriptException();
            return env.Null();
        }
    }

    //for the last 2 arguments, make sure that the parent folder exists and the path is really a filename path (not a path to a directory)
    for (int i = MATCH_ALGO_ARG_COUNT - 2; i < MATCH_ALGO_ARG_COUNT; i++) {
        std::string outputFileName = info[i].As<Napi::String>();
        std::filesystem::path outputPath = std::filesystem::path(outputFileName);
        std::filesystem::path parentPath = outputPath.parent_path();
        if (!std::filesystem::exists(parentPath)) {
            Napi::Error::New(env, "MATCHING-ALGORITHM: Output folder " + outputFileName + " does not exist. Make sure, it exists, otherwise the algorithm will not work.").ThrowAsJavaScriptException();
            return env.Null();
        }
        if (!outputPath.has_filename()) {
            Napi::Error::New(env, "MATCHING-ALGORITHM: Output file " + outputFileName + " is not a path to a file. Make sure that the output parameter really describes a path to a file.").ThrowAsJavaScriptException();
            return env.Null();
        }
    }

    //get the arguments...
    std::string pupil_file = info[0].As<Napi::String>();
    std::string student_file = info[1].As<Napi::String>();
    std::string balancing_coef = info[2].As<Napi::String>();

    std::string matching_file = info[3].As<Napi::String>();
    std::string stats_file = info[4].As<Napi::String>();

    //perform the algorithm
    try {
        CS::GraphCreator gc;

        std::ifstream pupil_input_file_stream(pupil_file);
        std::ifstream student_input_file_stream(student_file);
        std::ifstream balancing_coefficients(balancing_coef);
    
        gc.init_from_json(pupil_input_file_stream, student_input_file_stream, balancing_coefficients, std::nullopt, std::nullopt);
        
        std::vector<CS::Edge> matching_edges;
        auto const matching_cost = CS::compute_max_cost_matching(gc, matching_edges, CS::MatchingAlgorithm::SuccessiveShortestPath);
        CS::test_matching_valid(matching_edges, gc);
        ///The fourth file should encode the json output file for the matches
        CS::dump_matching_edges_into_json(matching_edges, gc, std::string(matching_file));
        ///The fifth file should encode the json output file for the stats
        CS::dump_stats(matching_edges, gc, matching_cost, std::string(stats_file));
    } catch(std::exception const& e) {
        std::cerr << e.what();
        Napi::Error::New(env, "The actual matching failed due to a C++ exception!").ThrowAsJavaScriptException();
    } catch (...) {
        Napi::Error::New(env, "The actual matching failed due to a C++ exception!").ThrowAsJavaScriptException();
    }
    return env.Null();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "matching"), Napi::Function::New(env, Match));
    return exports;
}

NODE_API_MODULE(matching, Init)

