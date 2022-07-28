import { MandatorySubject } from ".";
import { Subject, SubjectWithGradeRestriction } from "./subjects";

export interface PersonID {
    uuid: string;
}

export interface Person extends PersonID {
    id: number;
    createdAt: Date;
    state: string;
    matchRequestCount: number;
    subjects: Subject[];
    excludeMatchesWith: PersonID[];
}

export interface Helper extends Person {
    subjects: SubjectWithGradeRestriction[];
}

export interface Helpee extends Person {
    matchingPriority: number;
    grade: number;
    subjects: MandatorySubject[];
}
