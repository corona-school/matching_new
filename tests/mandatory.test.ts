import test from "tape";
import { Helpee, Helper, Match, match } from "../src";

export const helperMath: Helper = {
    id: 1,
    uuid: "HelperMath",
    createdAt: new Date(),
    state: "nw",
    matchRequestCount: 1,
    excludeMatchesWith: [],
    subjects: [
        {
            name: "Mathematik",
            gradeRestriction: {
                min: 3,
                max: 10
            }
        }
    ]
};

export const helperNoMath: Helper = {
    id: 2,
    uuid: "HelperNoMath",
    createdAt: new Date(),
    state: "nw",
    matchRequestCount: 1,
    excludeMatchesWith: [],
    subjects: [
        {
            name: "Deutsch",
            gradeRestriction: {
                min: 3,
                max: 10
            }
        }
    ]
};

export const helpeeMandatoryMath: Helpee = {
    id: 3,
    uuid: "Helpee",
    createdAt: new Date(),
    state: "nw",
    grade: 4,
    matchRequestCount: 1,
    matchingPriority: 10,
    excludeMatchesWith: [],
    subjects: [
        {
            name: "Deutsch"
        },
        {
            name: "Mathematik",
            mandatory: true
        }
    ]
};


test("Shall not match if mandatory subject not offered", t => {
    const result = match([helperNoMath], [helpeeMandatoryMath]);
    t.deepEqual(result.matches, []);

    t.end();
});

test("Shall match if mandatory subject is offered", t => {
    const result = match([helperMath], [helpeeMandatoryMath]);
    t.deepEqual(result.matches, [ <Match> { helpee: { uuid: "Helpee" }, helper: { uuid: "HelperMath" }} ]);
    t.end();
})