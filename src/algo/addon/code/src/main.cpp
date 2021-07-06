#include <iostream>
#include "graph_creator.h"
#include "matching.h"
#include "stats.h"
#include "courses_postoptimization.h"
using json = nlohmann::json;





void run_course_assignment(std::string const & course_input_file_name, std::string const & applicant_file_name,
                           std::string const & output_file_name) {
    CS::CourseContainer course_container;
    CS::CourseApplicantContainer applicant_container;
    course_container.init_from_json(course_input_file_name);
    applicant_container.init_from_json(applicant_file_name, course_container.courses);
    CS::compute_course_assignment(applicant_container, course_container);
    CS::remove_overlapping_courses(applicant_container.applicants, course_container.courses);
    CS::assign_free_slots_greedily(applicant_container.applicants, course_container.courses);
    CS::dump_course_assignment(output_file_name, applicant_container.applicants, course_container.courses);
}

int main(int argc, char * argv[]) {
    if (argc == 4) {
        //Run the course matching
        std::string course_input_file_name(argv[1]);
        std::string applicant_file_name(argv[2]);
        std::string output_file_name(argv[3]);
        run_course_assignment(course_input_file_name, applicant_file_name, output_file_name);
    }
    //Run normal matching otherwise
    //READ DATA AND CONSTRUCT GRAPH
    else if (argc != 6) {
        std::cout<<"Wrong number of input files, we need 5 of them!"<<std::endl;
        return EXIT_FAILURE;
    } else {
        CS::GraphCreator gc;
        ///We assume that the first file encodes the pupils, the second one the students and the third one the balancing coefficients.
        std::ifstream pupil_input_file_stream(argv[1]);
        std::ifstream student_input_file_stream(argv[2]);
        std::ifstream balancing_coefficients(argv[3]);
        gc.init_from_json(pupil_input_file_stream, student_input_file_stream, balancing_coefficients, std::nullopt,
                          std::nullopt);
        std::vector<CS::Edge> matching_edges;
        auto const matching_cost = CS::compute_max_cost_matching(gc, matching_edges,
                                                                 CS::MatchingAlgorithm::SuccessiveShortestPath);
        CS::test_matching_valid(matching_edges, gc);
        ///The fourth file should encode the json output file for the matches
        CS::dump_matching_edges_into_json(matching_edges, gc, std::string(argv[4]));
        ///The fifth file should encode the json output file for the stats
        CS::dump_stats(matching_edges, gc, matching_cost, std::string(argv[5]));
    }
    return 0;
}