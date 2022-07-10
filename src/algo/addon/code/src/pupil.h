#pragma once

#include "./types.h"

namespace CS {
    class CollegeStudent;

    class Pupil : public Node {
        public:
            std::vector<RequestedSubject> requested_subjects;
            double matching_priority{0.};
            Grade grade = MIN_POSSIBLE_GRADE;
            
            inline explicit Pupil(ID id) : Node(id) {}
        private:
            std::function<bool(const CollegeStudent &)> _acceptance_function;
    };
}
