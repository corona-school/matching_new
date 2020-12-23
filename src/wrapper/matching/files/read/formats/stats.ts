export type StatsOutputFormat = {
    "Average waiting days of a matched pupil": number;
    "Matching Cost": number;
    "Most waiting days of an unmatched pupil": number;
    "Number of pupils": number;
    "Number of students": number;
    "Total number of covered subjects": number;
    "Total number of edges in the graph": number;
    "Total number of found matches": number;
    "Total number of matching edges with matching bundesland": number;
    "Total number of offered subjects": number;
    "Total number of uncovered subjects": number;
    subjects: {
        name: string;
        stats: {
            offered: number;
            requested: number;
            "requests fulfilled": number;
        };
    }[];
};
