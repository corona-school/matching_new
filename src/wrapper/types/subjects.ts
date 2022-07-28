export interface Subject {
    name: string;
}

export interface MandatorySubject extends Subject {
    mandatory?: boolean;
}
export interface GradeRestriction {
    min: number;
    max: number;
}

export interface SubjectWithGradeRestriction extends Subject {
    gradeRestriction: GradeRestriction;
}
