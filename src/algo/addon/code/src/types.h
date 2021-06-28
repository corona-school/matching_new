#ifndef CORONA_SCHOOL_MATCHING_TYPES_H
#define CORONA_SCHOOL_MATCHING_TYPES_H

#include <functional>
#include <stdexcept>
#include <memory>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <ctime>
#include <iomanip>

namespace CS {

    using ID = std::uint32_t;
    using Grade = std::uint8_t;
    using Subject = std::string;
    using Bundesland = std::string;
    using UUID = std::string;
    static constexpr unsigned MIN_POSSIBLE_GRADE{1u};
    static constexpr unsigned MAX_POSSIBLE_GRADE{13u};
    static constexpr ID InvalidId{std::numeric_limits<ID>::max()};
    const Subject InvalidSubject{"InvalidSubject"};
    const UUID InvalidUUID{"InvalidUUID"};
    const Bundesland InvalidBundesland{"other"}; ///This is how the input file indicates that the user has not specified the state!
    ///Typedefs for the boost graph that will be created. We need to set all the types explicitly:
    typedef boost::adjacency_list_traits < boost::vecS, boost::vecS, boost::directedS > Traits;
    typedef boost::adjacency_list < boost::vecS, boost::vecS, boost::directedS,
    boost::property < boost::vertex_name_t, std::string,
            boost::property < boost::vertex_index_t, long,
            boost::property < boost::vertex_distance_t, double,
            boost::property < boost::vertex_predecessor_t, Traits::edge_descriptor > > > > ,
    boost::property < boost::edge_capacity_t, long,
            boost::property < boost::edge_residual_capacity_t, long,
                    boost::property <boost::edge_weight_t, int,
                    boost::property < boost::edge_reverse_t, Traits::edge_descriptor > > > > > Graph;


    ///Struct for a range of grades that are specified by a college student.
    ///@note: The grades might not be consecutive, i.e. there can be gaps.
    struct GradeRange {
        ///TODO: Check that only valid grades are used...
        std::vector<unsigned> grades;

        [[nodiscard]] inline bool contains(unsigned value) const {
            for (auto const & grade : grades) {
                if (value == grade) return true;
            }
            return false;
        }
    };

    ///Struct for requested subjects (of pupils).
    struct RequestedSubject {
        Subject  subject = InvalidSubject;
        double preference_value = 1.;

        explicit RequestedSubject(Subject && s): subject(std::move(s)){};
    };

    ///Struct for offered subjects (of students).
    struct OfferedSubject {
        Subject subject = InvalidSubject;
        double preference_value = 1.;
        GradeRange grade_range;
    };

    ///Interface for nodes. It serves as a base class for students and pupils.
    class NodeIF {
    public:
        ///Interface for node data. It serves as a base class for student and pupil data.
        ///The idea is that it contains shared properties such as a field for the uuid and one for the state of germany.
        struct DataIF {
            Bundesland bundesland = InvalidBundesland;
            ID input_file_id = InvalidId;
            UUID input_uuid = InvalidUUID;
            std::vector<UUID> dissolved_matches_with;
            double waiting_days = 0; //Want have also fractions of a day, therefore double..
        };

        inline explicit NodeIF(ID id): _id(id){ }
        [[nodiscard]] inline ID id() const {return _id;}

    private:
        const ID _id;
    };

    class Pupil;

    class CollegeStudent : NodeIF {
    public:

        using NodeIF::id;
        struct CollegeStudentData : public NodeIF::DataIF {
            std::vector<OfferedSubject> offered_subjects;
            unsigned number_of_possible_matches{0u};
        };

        inline explicit CollegeStudent(ID id);

        [[nodiscard]] inline bool accepts(const Pupil & pupil) const {
            if (_acceptance_function) {
                return _acceptance_function(pupil);
            }
            return false;
        }
        [[nodiscard]] inline CollegeStudentData const & data() const {return _data;}
        [[nodiscard]] inline CollegeStudentData & data() {return _data;}
    private:
        CollegeStudentData _data;
        std::function<bool(const Pupil &)> _acceptance_function;
    };

    class Pupil : NodeIF {
    public:
        struct PupilData : public NodeIF::DataIF {
            std::vector<RequestedSubject> requested_subjects;
            double matching_priority{0.};
            Grade grade = MIN_POSSIBLE_GRADE;
        };

        using NodeIF::id;
        inline explicit Pupil(ID id) : NodeIF(id) {}

        [[nodiscard]] inline PupilData const & data() const {return _data;}
        [[nodiscard]] inline PupilData & data() {return _data;}
    private:
        PupilData _data;
        std::function<bool(const CollegeStudent &)> _acceptance_function;
    };

    class NodeContainer {
    public:
        [[nodiscard]] inline const CollegeStudent & college_student(ID id) const {
            if (id >= _students.size()) {
                throw std::out_of_range("Tried to access a student with an invalid ID");
            }
            return _students[id];
        }

        [[nodiscard]] inline CollegeStudent & college_student(ID id) {
            if (id >= _students.size()) {
                throw std::out_of_range("Tried to access a student with an invalid ID");
            }
            return _students[id];
        }

        [[nodiscard]] inline const Pupil & pupil(ID id) const {
            if (id >= _pupils.size()) {
                throw std::out_of_range("Tried to access a pupil with an invalid ID");
            }
            return _pupils[id];
        }

        [[nodiscard]] inline Pupil & pupil(ID id) {
            if (id >= _pupils.size()) {
                throw std::out_of_range("Tried to access a pupil with an invalid ID");
            }
            return _pupils[id];
        }

        [[nodiscard]] inline  const std::vector<Pupil> & pupils() const {
            return _pupils;
        }

        [[nodiscard]] inline std::vector<Pupil> & pupils(){
            return _pupils;
        }

        [[nodiscard]] inline  const std::vector<CollegeStudent> & college_students() const {
            return _students;
        }

        [[nodiscard]] inline std::vector<CollegeStudent> & college_students(){
            return _students;
        }

        inline void create_pupils(unsigned num) {
            _pupils.reserve(_pupils.size() + num);
            auto const old_size = _pupils.size();
            for (auto id = old_size; id < old_size + num; ++id) {
                _pupils.emplace_back(id);
            }
        }

        inline void create_college_students(unsigned num) {
            _students.reserve(_students.size() + num);
            auto const old_size = _students.size();
            for (auto id = old_size; id < old_size + num; ++id) {
                _students.emplace_back(id);
            }
        }

        [[nodiscard]] inline std::uint32_t size() const {
            return _students.size() + _pupils.size();
        }


    private:
        std::vector<CollegeStudent> _students;
        std::vector<Pupil> _pupils;
    };

    CollegeStudent::CollegeStudent(ID id) : NodeIF(id) {}

    struct TimeInterval {
        double start;
        double end;

        bool intersects(const TimeInterval & other) const {
            return  !(start >= other.end or end <= other.start);
        }
    };

    struct Schedule {
        std::vector<TimeInterval> times;

        bool has_conflict_with(const Schedule & other) const {
            for (auto const & time_interval :  times) {
                for (auto const & other_time_interval : other.times) {
                    if (time_interval.intersects(other_time_interval)) {
                        return true;
                    }
                }
            }
            return false;
        }
    };

    struct Course {
        const unsigned max_number_of_participants{0u};
        const std::string name;
        const Schedule schedule;
    };

    struct CourseApplicant {
        std::vector<ID> requested_courses;
        //Referring to previous courses here:
        const unsigned number_of_skipped_courses{0u};
        const unsigned number_of_denied_courses{0u};
        const unsigned number_of_accepted_courses{0u};
        std::string const uuid;
        std::vector<ID> assigned_course_ids;
        double score{0.};

        explicit CourseApplicant(unsigned n_skipped_courses, unsigned n_denied_courses, unsigned n_accepted_courses, std::string UUID):
        number_of_skipped_courses(n_skipped_courses), number_of_denied_courses(n_denied_courses),
        number_of_accepted_courses(n_accepted_courses), uuid(UUID) {}
    };

    inline double get_day_difference_from_today(const std::string &date) {
        static constexpr unsigned SECONDS_IN_A_DAY = 86400;
        //Get time now
        std::time_t now = std::time(nullptr);
        //Convert string into time_t
        std::tm t = {};
        std::istringstream ss(date);
        ss >> std::get_time(&t, "%Y-%m-%dT%H:%M:%S");
        //Compute the time difference
        return std::difftime(now, std::mktime(&t)) / SECONDS_IN_A_DAY;
    }

    struct CourseContainer {
        std::vector<Course> courses;

        void init_from_json(std::string const & filename) {
            std::ifstream input_file(filename.c_str());
            nlohmann::json input_json;
            input_file >> input_json;
            for (auto const & entry : input_json) {
                Schedule current_schedule;
                for (auto const & time_entry: entry["schedule"]) {
                    TimeInterval time_interval;
                    time_interval.start = get_day_difference_from_today(time_entry["start"]);
                    ////a minute is 1/(24*60) of a day
                    time_interval.end = time_interval.start + static_cast<double>(time_entry["duration"]) / (24 * 60) ;
                    current_schedule.times.push_back(time_interval);
                }
                courses.push_back({entry["maxNumberOfParticipants"], entry["name"], current_schedule });
            }
        }
    };

    struct CourseApplicantContainer {
        std::vector<CourseApplicant> applicants;

        void init_from_json(std::string const & filename, std::vector<Course> const & courses) {
            std::ifstream input_file(filename.c_str());
            nlohmann::json input_json;
            input_file >> input_json;
            for (auto const & entry : input_json) {
                CourseApplicant applicant(entry["NumberOfSkippedCourses"], entry["NumberOfDeniedCourses"], entry["NumberOfAcceptedCourses"], entry["uuid"]);
                for (auto const & requested_course : entry["requestedCourses"]) {
                    //get the id of the course:
                    ID course_id = 0;
                    for (auto const & course : courses) {
                        if (course.name == requested_course) {
                            applicant.requested_courses.push_back(course_id);
                            break;
                        }
                        course_id++;
                    }
                }
                applicants.push_back(applicant);
            }
        }
    };
} // namespace CS

#endif //CORONA_SCHOOL_MATCHING_TYPES_H
