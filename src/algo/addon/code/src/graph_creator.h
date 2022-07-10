#pragma once

#include <fstream>

#include "types.h"
#include "costs.h"
#include "pupil.h"
#include "student.h"

namespace CS {

    /**
     * Simple struct for edges between pupils and students.
     */
    struct Edge {
        ID  pupil_id;
        ID  college_student_id;
        CostValue cost = 0.;
        Edge(ID p, ID c) : pupil_id{p}, college_student_id{c} {}
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

    class GraphCreator {
    public:
        /**
         * Main function of the Graph Creator.
         * @param pupil_file A json file which describes the pupils of the instance
         * @param student_file A json file which described the students of the instance
         * @param balancing_coefficients A json file that describes the fraction of the total cost that
         * each cost component should contribute.
         * Given the json files this function constructs all students and pupils and
         * edges between them. Further, costs are initialized and balanced.
         * @note The coefficients in the last file should all be between 0 and 1 and add up to 1.
         */
        void
        init_from_json(std::ifstream &pupil_file, std::ifstream &student_file, std::ifstream &balancing_coefficients,
                std::optional<std::vector<std::string>> const & pupil_uuids, std::optional<std::vector<std::string>> const & student_uuids);

        bool is_possible_pairing(ID student_id, ID pupil_id) const;

        /**
         * @return The id of the virtual source node s in the min cost flow instance.
         */
        [[nodiscard]] inline ID s_id() const { return _nodes.size(); }

        /**
         * @return The id of the virtual source node t in the min cost flow instance.
         */
        [[nodiscard]] inline ID t_id() const { return _nodes.size() + 1; }

        /**
         * @return The total numbe of nodes, that is number of students + number of pupils + 2 virtual nodes (s and t)
         */
        [[nodiscard]] inline std::uint32_t num_nodes() const { return _nodes.size() +2; }

        /**
         * @return A const reference to the container of edges.
         */
        [[nodiscard]] inline std::vector<Edge> const & edges() const {return _edges;}

        /**
         * @return A const reference to the container of nodes.
         */
        [[nodiscard]] inline NodeContainer const & nodes() const {return _nodes;}

        /**
         * @return A const reference to the cost computer.
         */
        [[nodiscard]] inline EdgeCostComputer const & cost_computer() const {return edge_cost_computer;}


    private:

        /**
         * @brief: Creates the edges between students and pupils and stores them.
         */
        void create_edges();

        /**
         * @brief: Initializes the costs of all edges between students and pupils.
         */
        void init_edge_costs();

        /**
         * This function adapts the edge costs in order to achieve given coefficients.
         * E.g. if the input file specifies that 90% of the total costs should be reserved for matching subjects,
         * then the coefficients of the edge cost computer will be adapted accordingly.
         * @param balancing_coefficients Input file stream which specifies the fraction of the total
         * that should be achieved by every cost component.
         */
        void balance_edge_costs(std::ifstream & balancing_coefficients);

        //Container for nodes and edges
        std::vector<Edge> _edges;
        NodeContainer _nodes;
        EdgeCostComputer edge_cost_computer;
    };
}
