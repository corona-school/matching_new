import { Helper, SubjectWithGradeRestriction } from "../../../../types";
import { HelperInputFormat, HelpersInputFormat, HelperSubjectInputFormat } from "../formats";

function helperTransformSubject(subject: SubjectWithGradeRestriction): HelperSubjectInputFormat {
    return {
        name: subject.name,
        grade: subject.gradeRestriction
    };
}
export function helperTransform(helper: Helper): HelperInputFormat {
    return {
        id: helper.id,
        uuid: helper.uuid,
        createdAt: helper.createdAt,
        state: helper.state,
        numberOfOpenMatchRequests: helper.matchRequestCount,
        hasDissolvedMatchesWith: helper.dissolvedMatches,
        subjects: helper.subjects.map(helperTransformSubject)
    };
}
export function helpersTransformToInput(helpers: Helper[]): HelpersInputFormat {
    return helpers.map(helperTransform);
}
