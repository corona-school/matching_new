import { Stats } from "../../../../types";
import { StatsOutputFormat } from "../formats";

export function statsTransformFromOutput(statsOutput: StatsOutputFormat): Stats {
    return {
        helpeeCount: statsOutput["Number of pupils"],
        helperCount: statsOutput["Number of students"],
        edgeCount: statsOutput["Total number of edges in the graph"],
        matchCount: statsOutput["Total number of found matches"],
        matchingCost: statsOutput["Matching Cost"],
        averageWaitingDaysMatchedHelpee: statsOutput["Average waiting days of a matched pupil"],
        mostWaitingDaysUnmatchedHelpee: statsOutput["Most waiting days of an unmatched pupil"],
        numberOfCoveredSubjects: statsOutput["Total number of covered subjects"],
        numberOfUncoveredSubjects: statsOutput["Total number of uncovered subjects"],
        numberOfOfferedSubjects: statsOutput["Total number of offered subjects"],
        numberOfMatchingEdgesWithMatchingState: statsOutput["Total number of matching edges with matching bundesland"],
        subjectStats: statsOutput.subjects?.map(s => ({
            name: s.name,
            stats: {
                offered: s.stats.offered,
                requested: s.stats.requested,
                fulfilledRequests: s.stats["requests fulfilled"]
            }
        }))
    };
}
