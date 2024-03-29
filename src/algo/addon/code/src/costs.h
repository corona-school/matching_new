#pragma once

#include <map>
#include "types.h"
#include "pupil.h"
#include "student.h"

namespace CS{
    ///Using double for costs
    using CostValue = double;
    ///Raw Cost Components are functions returning an unweighted cost between a student and a pupil.
    using RawCostComponent = std::function<CostValue(const CollegeStudent & student, const Pupil & pupil)>;


    /**
     * Enum for different cost components.
     */
    enum CostType {
        BundeslandBonus,
        FachUebereinstimmung,
        WaitingTimeBonus,
        MatchingPriorityBonus,
        InvalidCostType
    };

    struct CostComponent {
        double coefficient{0.};
        RawCostComponent raw_cost_func;
        inline explicit CostComponent(double c, RawCostComponent && r): coefficient(c), raw_cost_func(std::move(r)){};
    };

    /**
     * Class that computes the cost of an edge (between a pupil and a college student)
     * This is done by initializing different raw cost components and setting weighting coefficients (1 as default)
     * The cost of an edge will be the weighted sum of the cost components applied to the given edge.
     */
    class EdgeCostComputer {
    public:
        /**
         * Main function of the class.
         * @brief Computes the edge cost of an edge between @param student and @param pupil.
         */
        [[nodiscard]] inline CostValue compute_edge_cost(const CollegeStudent & student, const Pupil & pupil) const{
            CostValue total_edge_cost{0.};
            ///Iterate over all cost components and compute the weighted sum of the costs
            ///by multiplying each raw cost component with its coefficient.
            for (auto const & cost_comp : cost_components) {
                total_edge_cost += cost_comp.coefficient * cost_comp.raw_cost_func(student, pupil);
            }
            return total_edge_cost;
        }

        /**
         * @return The coefficient of the cost component of type @param type.
         */
        [[nodiscard]] inline double cost_coefficient(CostType type) const {
            auto it = cost_component_by_type.find(type);
            if (it == std::end(cost_component_by_type)) {
                return 0.;
            } else {
                return cost_components[(*it).second].coefficient;
            }
        }

        /**
         * @return The cost of type @param type on an edge between @param student and @param pupil.
         */
        [[nodiscard]] inline CostValue get_specific_edge_cost(const CollegeStudent & student, const Pupil & pupil,
                CostType type) const {
            auto it = cost_component_by_type.find(type);
            if (it == std::end(cost_component_by_type)) {
                return 0.;
            } else {
                auto const & cost_comp= cost_components[(*it).second];
                return cost_comp.coefficient * cost_comp.raw_cost_func(student, pupil);
            }
        }

        /**
         * Function to set/adapt the coefficient of a cost component of type @param type
         * @param coefficient The new coefficient of the cost component.
         */
        inline void set_cost_coefficient(CostType type, double coefficient) {
            //If no cost component with the given type exists, we throw an exception.
            if (cost_component_by_type.find(type) == std::end(cost_component_by_type)) {
                throw std::invalid_argument("Tried to change the coefficient of a cost type that was not yet created!");
            }
            //Set the coefficient:
            cost_components[cost_component_by_type[type]].coefficient = coefficient;
        }

        /**
         * Function to add a bonus cost if the student and pupil live in the same state.
         * @note This function should only be called once, multiple calls will not result in creation of
         * multiple cost components.
         */
        inline void add_bundesland_bonus() {
            //The cost component returns 1 if the states match, otherwise it returns 0
            add_cost_component(CostType::BundeslandBonus,
                    [](const CollegeStudent & student, const Pupil & pupil){
                if ((student.bundesland == InvalidBundesland)  || (pupil.bundesland == InvalidBundesland)) {
                    return 0.;
                }
                return (student.bundesland == pupil.bundesland) ? 1. : 0.;});
        }

        /**
         * Function to add a bonus cost if the pupil has a high matching priority.
         * @note This function should only be called once, multiple calls will not result in creation of
         * multiple cost components.
         */
        inline void add_matching_priority_bonus() {
            //The raw cost component just returns the priority value of the pupil.
            add_cost_component(CostType::MatchingPriorityBonus,
                    [](const CollegeStudent & student, const Pupil & pupil){
               return pupil.matching_priority;
            });
        }

        /**
         * Function to add costs for matching subjects.
         * @note This function should only be called once, multiple calls will not result in creation of
         * multiple cost components.
         */
        inline void add_fachuebereinstimmung() {
            //The cost component computes values for matching subjects with given preference values.
            //More specifically, if the pupil has set preference value x to a subject and the college stundent
            // has set preference value y for the same subject, we add a value of x*y. The total cost is
            // generated by taking the sum over all matching subjects.
            add_cost_component(CostType::FachUebereinstimmung,
                    [](const CollegeStudent & student, const Pupil & pupil){
                double retval{0.};
                //Could be done faster, but it doesn't matter so much here...
                for (auto const & offered_subject : student.offered_subjects) {
                    for (auto const & requested_subject : pupil.requested_subjects) {
                        if (requested_subject.subject == offered_subject.subject) {
                            //Nice! We have a matching subject. Add the sum of the preference values.
                            retval += offered_subject.preference_value + requested_subject.preference_value;
                        }
                    }
                }
                return retval;
            });
        }

        /**
         * Function to add costs for waiting time.
         * @note This function should only be called once, multiple calls will not result in creation of
         * multiple cost components.
         */
        inline void add_waiting_time_bonus() {
            add_cost_component(CostType::WaitingTimeBonus,
                    [] (const CollegeStudent & student, const Pupil & pupil) {
                //Add the sum of the waiting days.
                return static_cast<CostValue>(student.waiting_days + pupil.waiting_days);
            });
        }
    private:

        /**
         * Fuction to add a cost component of type @param type with weight @param coefficient.
         * @param cost_component The function that returns the raw cost
         */
        inline void add_cost_component(CostType type, RawCostComponent && cost_component, double coefficient = 1.) {
            //If there is already a cost component of the given type we throw an exception.
            if (cost_component_by_type.find(type) != std::end(cost_component_by_type)) {
                //Maybe better to use a log file
                throw std::invalid_argument("Tried to add a cost component for a type that already exists!");
            }
            //Store the index of this cost component:
            cost_component_by_type.emplace(type, cost_components.size());
            //Add the cost component
            cost_components.emplace_back(coefficient, std::move(cost_component));
        }

        //Container that holds the different cost components which are used ot generate the total cost.
        std::vector<CostComponent> cost_components;
        //Index map for the different cost components (by type)
        std::map<CostType, ID> cost_component_by_type;
    };

}
