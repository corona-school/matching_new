#pragma once

#include "graph_creator.h"

namespace CS {

    enum MatchingAlgorithm {
        CycleCanceling,
        SuccessiveShortestPath
    };

    /**
     * Main routine to compute a matching of maximum cost.
     * @param gc The graph creator that describes the edges
     * @param matching_edges A vector (possibly empty) in which we write the matching edges.
     * @return The cost of the maximum cost matching that was found.
     */
    CostValue compute_max_cost_matching(GraphCreator &gc,
            std::vector<Edge> &matching_edges, MatchingAlgorithm algorithm);
}

