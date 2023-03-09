export interface Subject {
    name: string;
}

export interface MandatorySubject extends Subject {
    mandatory?: boolean | null;
}
export interface GradeRestriction {
    min: number;
    max: number;
}

export interface SubjectWithGradeRestriction extends Subject {
    gradeRestriction: GradeRestriction;
}
