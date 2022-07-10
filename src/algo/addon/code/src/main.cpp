#include <iostream>

#include "courses_postoptimization.h"
#include "graph_creator.h"
#include "matching.h"
#include "stats.h"
using json = nlohmann::json;

int main(int argc, char* argv[]) {
    // Run normal matching otherwise
    // READ DATA AND CONSTRUCT GRAPH
    if (argc != 6) {
        std::cout << "Wrong number of input files, we need 5 of them!"
                  << std::endl;
        return EXIT_FAILURE;
    }

    CS::GraphCreator gc;
    /// We assume that the first file encodes the pupils, the second one the
    /// students and the third one the balancing coefficients.
    std::ifstream pupil_input_file_stream(argv[1]);
    std::ifstream student_input_file_stream(argv[2]);
    std::ifstream balancing_coefficients(argv[3]);
    gc.init_from_json(pupil_input_file_stream, student_input_file_stream,
                      balancing_coefficients, std::nullopt, std::nullopt);
    std::vector<CS::Edge> matching_edges;
    auto const matching_cost = CS::compute_max_cost_matching(
        gc, matching_edges, CS::MatchingAlgorithm::SuccessiveShortestPath);
    CS::test_matching_valid(matching_edges, gc);
    /// The fourth file should encode the json output file for the matches
    CS::dump_matching_edges_into_json(matching_edges, gc, std::string(argv[4]));
    /// The fifth file should encode the json output file for the stats
    CS::dump_stats(matching_edges, gc, matching_cost, std::string(argv[5]));

    return 0;
}