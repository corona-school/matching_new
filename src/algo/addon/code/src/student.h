#pragma once

#include "./types.h"

using json = nlohmann::json;

namespace CS {
class Pupil;

class CollegeStudent : public Node {
   public:
    std::vector<OfferedSubject> offered_subjects;
    unsigned number_of_possible_matches{0u};

    inline explicit CollegeStudent(ID id) : Node(id){};

    [[nodiscard]] inline bool accepts(const Pupil &pupil) const {
        if (_acceptance_function) {
            return _acceptance_function(pupil);
        }
        return false;
    }

    bool offers_subject(const Subject& subject) const {
        for (const OfferedSubject& offered_subject: this->offered_subjects) {
            if (offered_subject.subject == subject) {
                return true;
            }
        }
        return false;
    }

    static void parse(CollegeStudent &student, const json &json) {
        if (json.find("state") != std::end(json)) {
            student.bundesland = json["state"];
        }
        /// Waiting days are optional:
        if (json.find("createdAt") != std::end(json)) {
            student.waiting_days =
                get_day_difference_from_today(json["createdAt"]);
        }
        /// Id is needed in the input file
        if (json.find("id") == std::end(json)) {
            throw std::invalid_argument(
                "No id of a student specified in the input file!");
        }
        student.input_file_id = json["id"];
        /// UUid is needed in the input file
        if (json.find("uuid") == std::end(json)) {
            throw std::invalid_argument(
                "No uuid of a student specified in the input file!");
        }
        student.input_uuid = json["uuid"];
        /// Number of possible matches is needed
        if (json.find("numberOfOpenMatchRequests") == std::end(json)) {
            throw std::invalid_argument(
                "Number of open match requests unspecified for a student in "
                "the input file!");
        }
        student.number_of_possible_matches = json["numberOfOpenMatchRequests"];
        /// Dissolved matches are optional
        if (json.find("hasDissolvedMatchesWith") != std::end(json)) {
            for (auto const &dissolved_matching :
                 json["hasDissolvedMatchesWith"]) {
                student.dissolved_matches_with.emplace_back(
                    dissolved_matching["uuid"]);
            }
        }
        /// Offered subjects are needed in the input:
        if (json.find("subjects") == std::end(json)) {
            throw std::invalid_argument(
                "Subjects not specified for a student in the input file!");
        }
        for (auto const &offered_sub : json["subjects"]) {
            /// Subject name is needed
            if (offered_sub.find("name") == std::end(offered_sub)) {
                throw std::invalid_argument(
                    "Subject name not specified for a student in the input "
                    "file!");
            }
            Subject const subject = offered_sub["name"];
            GradeRange range;
            /// Grade range is optional:
            unsigned min, max;
            if (offered_sub.find("grade") == std::end(offered_sub)) {
                // Assume that in this case we have all grades
                min = MIN_POSSIBLE_GRADE;
                max = MAX_POSSIBLE_GRADE;
            } else {
                min = offered_sub["grade"]["min"];
                max = offered_sub["grade"]["max"];
            }
            for (unsigned grade = min; grade <= max; ++grade) {
                range.grades.push_back(grade);
            }
            student.offered_subjects.push_back({subject, 1., range});
        }
    }

   private:
    std::function<bool(const Pupil &)> _acceptance_function;
};
}  // namespace CS