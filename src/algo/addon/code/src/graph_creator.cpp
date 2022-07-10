#include <iostream>
#include "graph_creator.h"
#include <nlohmann/json.hpp>
#include <algorithm>
#include <iomanip>

using json = nlohmann::json;

namespace CS {


    CostType parse_cost_type(std::string s) {
        std::for_each(s.begin(), s.end(), [](char &c) {
            c = std::tolower(c);
        });
        if (s == "bundeslandbonus") return BundeslandBonus;
        if (s == "fachuebereinstimmung") return FachUebereinstimmung;
        if (s == "wartezeitbonus") return WaitingTimeBonus;
        if (s == "matchingprioritybonus") return MatchingPriorityBonus;
        return InvalidCostType;
    }


    template <typename T>
    inline bool contains(const std::vector<T> &vec, T const &elem) {
        for (auto const & t : vec) {
            if (t == elem) {
                return true;
            }
        }
        return false;
    }


    void GraphCreator::init_from_json(std::ifstream &pupil_file, std::ifstream &student_file,
                                      std::ifstream &balancing_coefficients,
                                      std::optional<std::vector<std::string>> const & pupil_uuids,
                                      std::optional<std::vector<std::string>> const & student_uuids) {
        json pupil_data_file, student_data_file;
        //Initialize the json object:
        pupil_file >> pupil_data_file;
        student_file >> student_data_file;
        //Build pupils and students with the data in the json file.
        _nodes.pupils().reserve(pupil_data_file.size());
        for (auto const &pupil_json_data : pupil_data_file) {
            if (pupil_uuids && (!contains<std::string>(*pupil_uuids, pupil_json_data["uuid"]))) {
                //This pupil cannot be matched currently..
                continue;
            }
            _nodes.create_pupils(1u);
            auto &pupil_data = _nodes.pupils().back();
            ///Bundesland data is optional
            if (pupil_json_data.find("state") != std::end(pupil_json_data)) {
                pupil_data.bundesland = pupil_json_data["state"];
            }
            ///Id and uuid are needed in the input
            if (pupil_json_data.find("id") == std::end(pupil_json_data)) {
                throw std::invalid_argument("No id of a pupil specified in the input file!");
            }
            if (pupil_json_data.find("uuid") == std::end(pupil_json_data)) {
                throw std::invalid_argument("No uuid of a pupil specified in the input file!");
            }
            pupil_data.input_file_id = pupil_json_data["id"];
            pupil_data.input_uuid = pupil_json_data["uuid"];
            ///The grade of the pupil is needed in the input
            if (pupil_json_data.find("grade") == std::end(pupil_json_data)) {
                throw std::invalid_argument("No grade of a pupil specified in the input file!");
            }
            pupil_data.grade = pupil_json_data["grade"];
            ///Matching priority is optional
            if (pupil_json_data.find("matchingPriority") != std::end(pupil_json_data)) {
                pupil_data.matching_priority = pupil_json_data["matchingPriority"];
            }
            ///Waiting days are optional
            if (pupil_json_data.find("createdAt") != std::end(pupil_json_data)) {
                pupil_data.waiting_days = get_day_difference_from_today(pupil_json_data["createdAt"]);
            }
            ///Disssolved matches are optional
            if (pupil_json_data.find("hasDissolvedMatchesWith") != std::end(pupil_json_data)) {
                for (auto const & dissolved_matching: pupil_json_data["hasDissolvedMatchesWith"]) {
                    pupil_data.dissolved_matches_with.emplace_back(dissolved_matching["uuid"]);
                }
            }
            ///Subjects are needed in the input
            if (pupil_json_data.find("subjects") == std::end(pupil_json_data)) {
                throw std::invalid_argument("No subjects specified for a pupil in the input file!");
            }

            for (auto const &fach : pupil_json_data["subjects"]) {
                pupil_data.requested_subjects.emplace_back(fach["name"]);
            }
        }

        _nodes.college_students().reserve(student_data_file.size());
        for (auto const &student_json_data : student_data_file) {
            if ( student_uuids && (!contains<std::string>(*student_uuids, student_json_data["uuid"]))) {
                //This student cannot be matched currently
                continue;
            }
            _nodes.create_college_students(1u);
            auto &student_data = _nodes.college_students().back();
            ///Bundesland data is optional:
            if (student_json_data.find("state") != std::end(student_json_data)) {
                student_data.bundesland = student_json_data["state"];
            }
            ///Waiting days are optional:
            if (student_json_data.find("createdAt") != std::end(student_json_data)) {
                student_data.waiting_days = get_day_difference_from_today(student_json_data["createdAt"]);
            }
            ///Id is needed in the input file
            if (student_json_data.find("id") == std::end(student_json_data)) {
                throw std::invalid_argument("No id of a student specified in the input file!");
            }
            student_data.input_file_id = student_json_data["id"];
            ///UUid is needed in the input file
            if (student_json_data.find("uuid") == std::end(student_json_data)) {
                throw std::invalid_argument("No uuid of a student specified in the input file!");
            }
            student_data.input_uuid = student_json_data["uuid"];
            ///Number of possible matches is needed
            if (student_json_data.find("numberOfOpenMatchRequests") == std::end(student_json_data)) {
                throw std::invalid_argument("Number of open match requests unspecified for a student in the input file!");
            }
            student_data.number_of_possible_matches = student_json_data["numberOfOpenMatchRequests"];
            ///Dissolved matches are optional
            if (student_json_data.find("hasDissolvedMatchesWith") != std::end(student_json_data)) {
                for (auto const &dissolved_matching : student_json_data["hasDissolvedMatchesWith"]) {
                    student_data.dissolved_matches_with.emplace_back(dissolved_matching["uuid"]);
                }
            }
            ///Offered subjects are needed in the input:
            if (student_json_data.find("subjects") == std::end(student_json_data)) {
                throw std::invalid_argument("Subjects not specified for a student in the input file!");
            }
            for (auto const &offered_sub : student_json_data["subjects"]) {
                ///Subject name is needed
                if (offered_sub.find("name") == std::end(offered_sub)) {
                    throw std::invalid_argument("Subject name not specified for a student in the input file!");
                }
                Subject const subject = offered_sub["name"];
                GradeRange range;
                ///Grade range is optional:
                unsigned min, max;
                if (offered_sub.find("grade") == std::end(offered_sub)) {
                    //Assume that in this case we have all grades
                    min = MIN_POSSIBLE_GRADE;
                    max = MAX_POSSIBLE_GRADE;
                } else {
                    min = offered_sub["grade"]["min"];
                    max = offered_sub["grade"]["max"];
                }
                for (unsigned grade = min; grade <= max; ++grade) {
                    range.grades.push_back(grade);
                }
                student_data.offered_subjects.push_back({subject, 1., range});
            }
        }
        //Create edges:
        create_edges();
        //Initialize the cost components that are used currently:
        edge_cost_computer.add_bundesland_bonus();
        edge_cost_computer.add_fachuebereinstimmung();
        edge_cost_computer.add_waiting_time_bonus();
        edge_cost_computer.add_matching_priority_bonus();
        //balance the edge costs
        balance_edge_costs(balancing_coefficients);
        //Cache the edge costs:
        init_edge_costs();
    }

    void GraphCreator::init_edge_costs() {

        //Iterate over all edges and cache the costs which are computed by the edge cost computer.
        for (auto &edge : _edges) {
            edge.cost = edge_cost_computer.compute_edge_cost(
                    _nodes.college_student(edge.college_student_id), _nodes.pupil(edge.pupil_id));
        }
    }

    void GraphCreator::create_edges() {
        for (auto const &pupil : _nodes.pupils()) {
            for (auto const &student : _nodes.college_students()) {
                if (is_possible_pairing(student.id, pupil.id)) {
                    _edges.emplace_back(pupil.id, student.id);
                }
            }
        }
    }

    bool GraphCreator::is_possible_pairing(ID student_id, ID pupil_id) const {
        auto const & student = nodes().college_student(student_id);
        auto const & pupil = nodes().pupil(pupil_id);
        //Check whether they already had a dissolved matching:
        for (auto const & uuid : pupil.dissolved_matches_with) {
            if (uuid == student.input_uuid) {
                //This pair was already matched at some point, but they dissolved their matching.
                //So we do not want to match them again!
                return false;
            }
        }
        //Check whether there exists a subject requested by the pupil that the student offers and whether the pupil
        // is contained in the specified grade range.
        for (auto const & offered_subject : student.offered_subjects) {
            if (!offered_subject.grade_range.contains(pupil.grade)) {
                //Grade does not fit for this subject!
                continue;
            }
            //Else check whether the pupil requested this subject
            for (auto const & requested_subject : pupil.requested_subjects) {
                if (requested_subject.subject == offered_subject.subject) {
                    //Nice, we found a subject that is offered and requested (and the grade range fits)
                    //This is a possible pairing!
                    return true;
                }
            }
        }
        //Otherwise we did not find matching subjects, this is not a possible pairing.
        return false;
    }

    void GraphCreator::balance_edge_costs(std::ifstream &balancing_coefficient_file) {
        json balancing_coeff_json;
        //Initialize the json object:
        balancing_coefficient_file >> balancing_coeff_json;
        std::map<CostType, double> cost_coefficients_by_type;
        std::map<CostType, double> total_edge_costs_by_type;
        //Collect the coefficients in the json file:
        for (auto it = balancing_coeff_json.begin(); it != balancing_coeff_json.end(); ++it) {
            CostType const cost_type = parse_cost_type(it.key());
            if (cost_type != CostType::InvalidCostType) {
                cost_coefficients_by_type.emplace(cost_type, it.value());
            }
        }
        //Compute the fraction of the total cost that is currently consumed by each cost component:
        double total_cost{0.};
        std::map<CostType, unsigned> num_edges_with_type_data;
        for (auto const &[type, coeff] : cost_coefficients_by_type) {
            double specific_cost_total{0.};
            unsigned edges_with_type_data = 0;
            for (auto const &edge : edges()) {
                auto const &student = nodes().college_student(edge.college_student_id);
                auto const &pupil = nodes().pupil(edge.pupil_id);
                auto const current_cost = edge_cost_computer.get_specific_edge_cost(student, pupil, type);
                total_cost += current_cost;
                specific_cost_total += current_cost;
                if (current_cost > 0.) ++edges_with_type_data;
            }
            num_edges_with_type_data[type] = edges_with_type_data;
            total_edge_costs_by_type[type] = specific_cost_total;
        }
        if (total_cost == 0.) return;
        //Now adapt the coefficients:
        for (auto const &[type, cost] : total_edge_costs_by_type) {
            if (cost == 0.) continue;
            auto const current_coefficient = cost / total_cost;
            auto adaption_factor = cost_coefficients_by_type[type] / current_coefficient;
            auto const relative_num_edges_with_type_data = double(num_edges_with_type_data[type]) / edges().size();
            adaption_factor *= relative_num_edges_with_type_data;
            edge_cost_computer.set_cost_coefficient(type, adaption_factor * edge_cost_computer.cost_coefficient(type));
        }
    }
}


