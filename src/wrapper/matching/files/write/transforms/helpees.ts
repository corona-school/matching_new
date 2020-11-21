import { Helpee, Subject } from "../../../../types";
import { HelpeeInputFormat, HelpeesInputFormat, HelpeeSubjectInputFormat } from "../formats";

function helpeeTransformSubject(subject: Subject): HelpeeSubjectInputFormat {
    return {
        name: subject.name
    };
}
export function helpeeTransform(helpee: Helpee): HelpeeInputFormat {
    return {
        id: helpee.id,
        uuid: helpee.uuid,
        createdAt: helpee.createdAt,
        state: helpee.state,
        grade: helpee.grade,
        matchingPriority: helpee.matchingPriority,
        numberOfOpenMatchRequests: helpee.matchRequestCount,
        hasDissolvedMatchesWith: helpee.dissolvedMatches,
        subjects: helpee.subjects.map(helpeeTransformSubject)
    };
}
export function helpeesTransformToInput(helpees: Helpee[]): HelpeesInputFormat {
    return helpees.map(helpeeTransform);
}
