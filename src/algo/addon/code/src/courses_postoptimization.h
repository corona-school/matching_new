#ifndef CORONA_SCHOOL_MATCHING_COURSES_POSTOPTIMIZATION_H
#define CORONA_SCHOOL_MATCHING_COURSES_POSTOPTIMIZATION_H

#include "types.h"
#include <numeric>

namespace CS {
    //TODO: Could be bad to do this greedily (order is bad for some courses), also some might be removed without need
    void remove_overlapping_courses(std::vector<CourseApplicant> &applicants,
                                    const std::vector<Course> &courses) {
        for (auto &applicant : applicants) {
            for (unsigned i = 0; i < applicant.assigned_course_ids.size(); ++i) {
                auto const & current_schedule = courses[applicant.assigned_course_ids[i]].schedule;
                for (unsigned j = i+1; j < applicant.assigned_course_ids.size(); ++j) {
                    if (current_schedule.has_conflict_with(courses[applicant.assigned_course_ids[j]].schedule)) {
                        //Mark entry i with a special value
                        applicant.assigned_course_ids[i] = std::numeric_limits<unsigned>::max();
                        break;
                    }
                }
            }
            //Erase the marked courses
            applicant.assigned_course_ids.erase(std::remove(applicant.assigned_course_ids.begin(),
                                                            applicant.assigned_course_ids.end(), std::numeric_limits<unsigned>::max()), applicant.assigned_course_ids.end());
        }

    };

    void assign_free_slots_greedily(std::vector<CourseApplicant> &applicants,
                                          const std::vector<Course> &courses) {
        std::vector<int> free_slots_for_course(courses.size(),0);
        for (ID id = 0 ; id < courses.size(); ++id) {
            free_slots_for_course[id] = courses[id].max_number_of_participants;
        }
        for (auto const & applicant : applicants) {
            for (auto course_id : applicant.assigned_course_ids) {
                free_slots_for_course[course_id]--;
            }
        }
        //Sort applicants by score and assign them to courses
        std::vector<ID> sorted_applicants(applicants.size());
        //Fill with 0,1,...,applicants.size()-1
        std::iota(sorted_applicants.begin(), sorted_applicants.end(), 0);
        std::sort(sorted_applicants.begin(), sorted_applicants.end(), [&](ID lhs, ID rhs){
            return applicants[lhs].score > applicants[rhs].score;
        });

        //Assign remaining slots greedily:
        for (ID course_id = 0; course_id < courses.size(); ++course_id) {
            auto const & course_schedule = courses[course_id].schedule;
            for (auto applicant_id : sorted_applicants) {
                if (free_slots_for_course[course_id] == 0) {break;}
                //Check whether applicant is interested in the course and whether there are intersections with his
                //currently assigned courses.
                //Is the applicant interested in the course?
                if (std::find(applicants[applicant_id].requested_courses.begin(), applicants[applicant_id].requested_courses.end(), course_id)
                    != applicants[applicant_id].requested_courses.end()) {
                    //Can he take it without conflicts with his schedule?
                    bool has_conflict = false;
                    for (auto assigned_course_id : applicants[applicant_id].assigned_course_ids) {
                        if (course_schedule.has_conflict_with(courses[assigned_course_id].schedule)) {
                            has_conflict = true;
                            break;
                        }
                    }
                    //Add it if there is no conflict!
                    if (not has_conflict) {
                        //One place less
                        free_slots_for_course[course_id]--;
                        applicants[applicant_id].assigned_course_ids.push_back(course_id);
                    }
                }
            }
        }

    }
}


#endif //CORONA_SCHOOL_MATCHING_COURSES_POSTOPTIMIZATION_H
