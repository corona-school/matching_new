#pragma once

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
    class Node {
    public:
        const ID id;
        
        // Shared Properties between Pupils and Students
        Bundesland bundesland = InvalidBundesland;
        ID input_file_id = InvalidId;
        UUID input_uuid = InvalidUUID;
        std::vector<UUID> dissolved_matches_with;
        double waiting_days = 0; //Want have also fractions of a day, therefore double..

        inline explicit Node(ID id): id(id){ }
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


} // namespace CS
