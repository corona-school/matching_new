#pragma once

#include "./types.h"

namespace CS {
class Pupil;

class CollegeStudent : public Node {
   public:
    std::vector<OfferedSubject> offered_subjects;
    unsigned number_of_possible_matches{0u};

    inline explicit CollegeStudent(ID id) : Node(id) {};

    [[nodiscard]] inline bool accepts(const Pupil &pupil) const {
        if (_acceptance_function) {
            return _acceptance_function(pupil);
        }
        return false;
    }

   private:
    std::function<bool(const Pupil &)> _acceptance_function;
};
}  // namespace CS