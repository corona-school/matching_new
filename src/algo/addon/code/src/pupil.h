#pragma once

#include "./types.h"

using json = nlohmann::json;

namespace CS {
class CollegeStudent;

class Pupil : public Node {
   public:
    std::vector<RequestedSubject> requested_subjects;
    double matching_priority{0.};
    Grade grade = MIN_POSSIBLE_GRADE;

    inline explicit Pupil(ID id) : Node(id) {}

    static void parse(Pupil& pupil, const json& json) {
        if (json.find("state") != std::end(json)) {
            pupil.bundesland = json["state"];
        }
        /// Id and uuid are needed in the input
        if (json.find("id") == std::end(json)) {
            throw std::invalid_argument(
                "No id of a pupil specified in the input file!");
        }
        if (json.find("uuid") == std::end(json)) {
            throw std::invalid_argument(
                "No uuid of a pupil specified in the input file!");
        }
        pupil.input_file_id = json["id"];
        pupil.input_uuid = json["uuid"];
        /// The grade of the pupil is needed in the input
        if (json.find("grade") == std::end(json)) {
            throw std::invalid_argument(
                "No grade of a pupil specified in the input file!");
        }
        pupil.grade = json["grade"];
        /// Matching priority is optional
        if (json.find("matchingPriority") != std::end(json)) {
            pupil.matching_priority = json["matchingPriority"];
        }
        /// Waiting days are optional
        if (json.find("createdAt") != std::end(json)) {
            pupil.waiting_days =
                get_day_difference_from_today(json["createdAt"]);
        }
        /// Disssolved matches are optional
        if (json.find("hasDissolvedMatchesWith") != std::end(json)) {
            for (auto const& dissolved_matching :
                 json["hasDissolvedMatchesWith"]) {
                pupil.dissolved_matches_with.emplace_back(
                    dissolved_matching["uuid"]);
            }
        }
        /// Subjects are needed in the input
        if (json.find("subjects") == std::end(json)) {
            throw std::invalid_argument(
                "No subjects specified for a pupil in the input file!");
        }

        for (auto const& fach : json["subjects"]) {
            pupil.requested_subjects.emplace_back(fach["name"]);
        }
    }

   private:
    std::function<bool(const CollegeStudent&)> _acceptance_function;
};

}  // namespace CS
