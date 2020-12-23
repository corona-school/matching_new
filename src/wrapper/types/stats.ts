export interface Stats {
    helperCount: number;
    helpeeCount: number;
    edgeCount: number;
    matchCount: number;
    matchingCost: number;
    averageWaitingDaysMatchedHelpee: number;
    mostWaitingDaysUnmatchedHelpee: number;
    numberOfCoveredSubjects: number;
    numberOfUncoveredSubjects: number;
    numberOfOfferedSubjects: number;
    numberOfMatchingEdgesWithMatchingState: number;
    subjectStats: {
        name: string;
        stats: {
            offered: number;
            requested: number;
            fulfilledRequests: number;
        };
    }[];
}
