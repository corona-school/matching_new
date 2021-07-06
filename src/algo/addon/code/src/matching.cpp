#include "matching.h"
#include <boost/graph/cycle_canceling.hpp>
#include <boost/graph/successive_shortest_path_nonnegative_weights.hpp>
#include <boost/graph/find_flow_cost.hpp>

namespace CS {
   /**
    * This function provides a heuristic which computes a matching with high cost.
    * It sorts the edges by costs and adds the most expensive ones to the matching iteratively
    * if they maintain a feasible matching.
    * We use this as a starting point for the min cost matching algorithm in order to speed it up
    * (A good initial matching results in fewer negative cycles that have to be resolved).
    * @param gc The graph creator containing all edges.
    * @param matching_edges A vector in which the result of this heuristic will be stored.
    */
    void heuristic_max_cost_matching(GraphCreator const &gc, std::vector<Edge> &matching_edges) {
        std::vector<Edge> all_edges(gc.edges());
        auto greater_cost_comp = [](const Edge &lhs, const Edge &rhs) {
            return lhs.cost > rhs.cost;
        };
        std::vector<bool> is_pupil_covered(gc.nodes().pupils().size(), false);
        std::vector<bool> is_student_covered(gc.nodes().college_students().size(), false);
        //Sort all edges by cost (starting with the highest cost).
        std::sort(all_edges.begin(), all_edges.end(), greater_cost_comp);
        //Iterate over all edges and accept those where both endpoints are not covered.
        for (auto const &edge : all_edges) {
            if (is_pupil_covered[edge.pupil_id] || is_student_covered[edge.college_student_id]) {
                continue;
            } else {
                matching_edges.push_back(edge);
                is_pupil_covered[edge.pupil_id] = true;
                is_student_covered[edge.college_student_id] = true;
            }
        }
    }

    /**
     * @return The maximum weight of an edge in the graph.
     */
    inline CostValue maximum_edge_weight(const std::vector<Edge> &edges) {
        CostValue retval = 0.;
        for (auto const &edge : edges) {
            retval = std::max(retval, edge.cost);
        }
        return retval;
    }

   /**
    * @return An upper bound on the maximum number of possible matchings, which we use as the max flow value in the algorithm.
    */
    inline unsigned get_max_flow_value(const GraphCreator &gc) {
        //Every pupil can be matched at most once, so the number of pupils is an upper bound on the number of possible matches.
        unsigned const bound1 = gc.nodes().pupils().size();
        //The number of matches that are offered by the students is another upper bound:
        unsigned bound2{0u};
        for (auto const &student : gc.nodes().college_students()) {
            bound2 += student.data().number_of_possible_matches;
        }
        //we return the smaller of the two bounds:
        return std::min(bound1, bound2);
    }

    template<typename Func>
    CostValue compute_max_cost_matching_cycle_canceling(const GraphCreator &gc,
                                                        std::map<std::pair<ID, ID>, Graph::edge_descriptor> &edge_to_edge_descriptor,
                                                        Graph &g,
                                                        boost::property_map<Graph, boost::edge_reverse_t>::type &rev,
                                                        boost::property_map<Graph, boost::edge_residual_capacity_t>::type &residual_capacity,
                                                        Func &add_edge) {
        ///For speedup we try to find a maximum flow that is already close to optimal via a heuristic:
        std::vector<Edge> heuristic_edges;
        heuristic_max_cost_matching(gc, heuristic_edges);
        ///Adapt the flow values on those edges by hand:
        for (auto const &edge : heuristic_edges) {
            auto const s_edge_descriptor = edge_to_edge_descriptor[{gc.s_id(), edge.pupil_id}];
            auto const edge_descriptor = edge_to_edge_descriptor[{edge.pupil_id, edge.college_student_id +
                                                                                 gc.nodes().pupils().size()}];
            auto const t_edge_descriptor = edge_to_edge_descriptor[{
                    edge.college_student_id + gc.nodes().pupils().size(), gc.t_id()}];
            residual_capacity[s_edge_descriptor] = 0;
            residual_capacity[edge_descriptor] = 0;
            residual_capacity[t_edge_descriptor] = 0;
            residual_capacity[rev[s_edge_descriptor]] = 1;
            residual_capacity[rev[edge_descriptor]] = 1;
            residual_capacity[rev[t_edge_descriptor]] = 1;
        }
        unsigned const remaining_flow_value =
                get_max_flow_value(gc) - heuristic_edges.size();
        ///Add a direct edge between s and t of cost 0 with the remaining flow value:
        add_edge(gc.s_id(), gc.t_id(), 0., remaining_flow_value);
        auto const boost_edge = edge_to_edge_descriptor[{gc.s_id(), gc.t_id()}];
        residual_capacity[boost_edge] = 0;
        residual_capacity[rev[boost_edge]] = remaining_flow_value;
        ///Now make the input flow cost optimal:
        boost::cycle_canceling(g);
        ///Retrieve the cost of the flow:
        CostValue flow_cost = boost::find_flow_cost(g);
        return flow_cost;
    }

    template<typename Func>
    CostValue compute_max_cost_matching_successive_shortest_paths(GraphCreator const &gc, Graph &g,
                                                                  Func &add_edge, CostValue additional_cost,
                                                                  unsigned max_flow_value) {
        if (gc.nodes().pupils().size() <= max_flow_value) {
            //Bound is attained for pupils.
            for (auto const &pupil : gc.nodes().pupils()) {
                auto pupil_idx = pupil.id();
                add_edge(pupil_idx, gc.t_id(), additional_cost);
            }
        } else {
            //Bound is attained for students.
            for (auto const &student : gc.nodes().college_students()) {
                auto student_idx = student.id() + gc.nodes().pupils().size();
                add_edge(gc.s_id(), student_idx, additional_cost, student.data().number_of_possible_matches);
            }
        }
        boost::successive_shortest_path_nonnegative_weights(g, gc.s_id(), gc.t_id());
        CostValue flow_cost = boost::find_flow_cost(g);
        return flow_cost;
    }

    CostValue compute_max_cost_matching(GraphCreator &gc,
                                        std::vector<Edge> &matching_edges, MatchingAlgorithm algorithm) {
        ///Initialize edge weights, capacities, residual capacities and the boost graph
        Graph g(gc.num_nodes());
        boost::property_map<Graph, boost::edge_capacity_t>::type capacity = boost::get(boost::edge_capacity, g);
        boost::property_map<Graph, boost::edge_residual_capacity_t>::type
                residual_capacity = get(boost::edge_residual_capacity, g);
        boost::property_map<Graph, boost::edge_reverse_t>::type rev = get(boost::edge_reverse, g);
        boost::property_map<Graph, boost::edge_weight_t>::type weight = get(boost::edge_weight, g);
        std::map<std::pair<ID, ID>, Graph::edge_descriptor> edge_to_edge_descriptor;
        CostValue additional_cost = algorithm == CycleCanceling ? 0. : 2 * maximum_edge_weight(gc.edges());
        ///Lambda function to add an edge in the boost graph (with its residual edge, weight and capacity).
        ///This way we avoid duplicate code. (capacity of the edge is set to 1 as default)
        auto add_edge = [&](ID tail, ID head, double cost, unsigned cap = 1) {
            auto const edge_descriptor = boost::add_edge(tail, head, g).first;
            auto const reverse_edge_descriptor = boost::add_edge(head, tail, g).first;
            rev[edge_descriptor] = reverse_edge_descriptor;
            rev[reverse_edge_descriptor] = edge_descriptor;
            capacity[edge_descriptor] = cap;
            capacity[reverse_edge_descriptor] = 0;
            weight[edge_descriptor] = static_cast<int>(cost);
            //Cost of the reverse edge must be inverted!
            weight[reverse_edge_descriptor] = -static_cast<int>(cost);
            residual_capacity[edge_descriptor] = cap;
            residual_capacity[reverse_edge_descriptor] = 0;
            edge_to_edge_descriptor[{tail, head}] = edge_descriptor;
            return edge_descriptor;
        };
        for (auto const &edge : gc.edges()) {
            auto const pupil_idx = edge.pupil_id;
            auto const college_student_idx = edge.college_student_id + gc.nodes().pupils().size();
            auto const edge_descriptor = add_edge(pupil_idx, college_student_idx, -edge.cost + additional_cost);
        }
        ///Add edges with 0 weight between s and pupils:
        for (auto const &pupil : gc.nodes().pupils()) {
            add_edge(gc.s_id(), pupil.id(), 0.);
        }
        ///Add edges with 0 weight between students and t:
        for (auto const &student : gc.nodes().college_students()) {
            auto const student_index = gc.nodes().pupils().size() + student.id();
            add_edge(student_index, gc.t_id(), 0., student.data().number_of_possible_matches);
        }
        ///Compute the minimum cost flow with the given algorithm:
        unsigned const max_flow_value = get_max_flow_value(gc);
        CostValue inverted_flow_cost = (algorithm  == CycleCanceling) ?
                                       compute_max_cost_matching_cycle_canceling(gc, edge_to_edge_descriptor, g, rev, residual_capacity, add_edge):
                                       compute_max_cost_matching_successive_shortest_paths(gc, g, add_edge, additional_cost, max_flow_value) - max_flow_value * additional_cost;

        ///Store the computed matching edges:
        for (auto const & edge: gc.edges()) {
            ID const college_student_id = edge.college_student_id + gc.nodes().pupils().size();
            if (residual_capacity[edge_to_edge_descriptor[{edge.pupil_id,college_student_id}]] == 0){
                matching_edges.push_back(edge);
            }
        }
        ///Invert the flow cost, because we computed a min cost flow for the negative costs.
        return  -inverted_flow_cost;
    }

    double maximum_applicant_score(std::vector<CourseApplicant> const & applicants) {
        double maximum_score{0.};
        for (auto const & applicant : applicants) {
            maximum_score = std::max(maximum_score, applicant.score);
        }
        return maximum_score;
    }

    CostValue compute_course_assignment(CourseApplicantContainer & applicant_container,
                                        CourseContainer const & course_container) {
        ID const s_id = applicant_container.applicants.size() + course_container.courses.size();
        ID const t_id = s_id + 1;
        ///Initialize edge weights, capacities, residual capacities and the boost graph
        Graph g(t_id + 1);
        boost::property_map<Graph, boost::edge_capacity_t>::type capacity = boost::get(boost::edge_capacity, g);
        boost::property_map<Graph, boost::edge_residual_capacity_t>::type
                residual_capacity = get(boost::edge_residual_capacity, g);
        boost::property_map<Graph, boost::edge_reverse_t>::type rev = get(boost::edge_reverse, g);
        boost::property_map<Graph, boost::edge_weight_t>::type weight = get(boost::edge_weight, g);
        std::map<std::pair<ID, ID>, Graph::edge_descriptor> edge_to_edge_descriptor;
        //Just a large enough constant
        CostValue additional_cost = 2 * maximum_applicant_score(applicant_container.applicants);
        ///Lambda function to add an edge in the boost graph (with its residual edge, weight and capacity).
        ///This way we avoid duplicate code. (capacity of the edge is set to 1 as default)
        auto add_edge = [&](ID tail, ID head, double cost, unsigned cap = 1) {
            auto const edge_descriptor = boost::add_edge(tail, head, g).first;
            auto const reverse_edge_descriptor = boost::add_edge(head, tail, g).first;
            rev[edge_descriptor] = reverse_edge_descriptor;
            rev[reverse_edge_descriptor] = edge_descriptor;
            capacity[edge_descriptor] = cap;
            capacity[reverse_edge_descriptor] = 0;
            weight[edge_descriptor] = static_cast<int>(cost);
            //Cost of the reverse edge must be inverted!
            weight[reverse_edge_descriptor] = -static_cast<int>(cost);
            residual_capacity[edge_descriptor] = cap;
            residual_capacity[reverse_edge_descriptor] = 0;
            edge_to_edge_descriptor[{tail, head}] = edge_descriptor;
            return edge_descriptor;
        };
        ID applicant_index{0};
        //Introduce some cost such that it is preferred to take different applicants.
        double applicant_diversity_factor = 1.;
        for (auto const &applicant : applicant_container.applicants) {
            //Capacity large enough
            unsigned preference_value{1u};
            ///The requested courses are sorted such that the most desired course is first.
            ///TODO: needs to be weighted differently
            for (auto course_id : applicant.requested_courses) {
                add_edge(s_id, applicant_index, additional_cost - applicant_diversity_factor * applicant.score);
                ID course_index = course_id + applicant_container.applicants.size();
                add_edge(applicant_index, course_index, preference_value);
                preference_value++;
                applicant_diversity_factor *= 0.9;
            }
            ++applicant_index;
        }
        ID course_index = applicant_container.applicants.size();
        for (auto const &course : course_container.courses) {
            //Capacity of the edge must be the number of assignable slots
            add_edge(course_index, t_id, 0., course.max_number_of_participants);
            ++course_index;
        }
        boost::successive_shortest_path_nonnegative_weights(g, s_id, t_id);
        ///TODO: recover the correct flow cost
        CostValue flow_cost = boost::find_flow_cost(g);

        applicant_index = 0;
        for (auto & applicant : applicant_container.applicants) {
            for (auto course_id : applicant.requested_courses) {
                course_index = course_id + applicant_container.applicants.size();
                if (residual_capacity[edge_to_edge_descriptor[{applicant_index,course_index}]] == 0){
                    applicant.assigned_course_ids.push_back(course_id);
                }
            }
            ++applicant_index;
        }
        return flow_cost;
    }
}
