#ifndef CORONA_SCHOOL_MATCHING_MATCHING_H
#define CORONA_SCHOOL_MATCHING_MATCHING_H

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

    CostValue compute_course_assignment(CourseApplicantContainer & applicant_container,
                                        CourseContainer const & course_container);
}

#endif //CORONA_SCHOOL_MATCHING_MATCHING_H
