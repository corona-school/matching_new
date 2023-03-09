import { DissolvedMatchInputFormat } from "./matches";

export type HelpeeSubjectInputFormat = {
    name: string;
    mandatory?: boolean | null;
};
export type HelpeeInputFormat = {
    id: number;
    uuid: string;
    createdAt: Date;
    state: string;
    grade: number;
    matchingPriority: number;
    subjects: HelpeeSubjectInputFormat[];
    numberOfOpenMatchRequests: number;
    hasDissolvedMatchesWith: DissolvedMatchInputFormat[];
};
export type HelpeesInputFormat = HelpeeInputFormat[];
