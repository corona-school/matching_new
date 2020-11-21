import { DissolvedMatchInputFormat } from "./matches";

export type HelperSubjectInputFormat = {
    name: string;
    grade: {
        min: number;
        max: number;
    };
};
export type HelperInputFormat = {
    id: number;
    uuid: string;
    createdAt: Date;
    state: string;
    subjects: HelperSubjectInputFormat[];
    numberOfOpenMatchRequests: number;
    hasDissolvedMatchesWith: DissolvedMatchInputFormat[];
};
export type HelpersInputFormat = HelperInputFormat[];
