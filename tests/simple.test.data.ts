import { Helper, Helpee } from "../src";

//////////////////////////// convention for the test data in this file ////////////////////////////
// 1. `helperA` and `helpeeA` form a match (use the *letter* to recognize that)
// 2. `unmatchableHelper1` is the first unmatchable helper (use the *number* to recognize the number)
///////////////////////////////////////////////////////////////////////////////////////////////////

export const helperA: Helper = {
    id: 1,
    uuid: "HelperA",
    createdAt: new Date(),
    state: "nw",
    matchRequestCount: 1,
    excludeMatchesWith: [],
    subjects: [
        {
            name: "Deutsch",
            gradeRestriction: {
                min: 1,
                max: 8
            }
        },
        {
            name: "Mathematik",
            gradeRestriction: {
                min: 3,
                max: 10
            }
        }
    ]
};
export const helpeeA: Helpee = {
    id: 2,
    uuid: "HelpeeA",
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
            name: "Mathematik"
        }
    ]
};

export const helperB: Helper = {
    id: 3,
    uuid: "HelperB",
    createdAt: new Date(),
    state: "nw",
    matchRequestCount: 1,
    excludeMatchesWith: [],
    subjects: [
        {
            name: "Englisch",
            gradeRestriction: {
                min: 7,
                max: 13
            }
        },
        {
            name: "Physik",
            gradeRestriction: {
                min: 3,
                max: 7
            }
        }
    ]
};
export const helpeeC: Helpee = {
    id: 4,
    uuid: "HelpeeC",
    createdAt: new Date(),
    state: "nw",
    grade: 8,
    matchRequestCount: 1,
    matchingPriority: 1,
    excludeMatchesWith: [],
    subjects: [
        {
            name: "Deutsch"
        },
        {
            name: "Mathematik"
        }
    ]
};

export const helperWithOneSubjectA: Helper = {
    id: 5,
    uuid: "helperWithOneSubjectA",
    createdAt: new Date(),
    state: "nw",
    matchRequestCount: 1,
    excludeMatchesWith: [],
    subjects: [
        {
            name: "Deutsch",
            gradeRestriction: {
                min: 1,
                max: 8
            }
        }
    ]
};
export const helpeeWithOneSubjectA: Helpee = {
    id: 6,
    uuid: "helpeeWithOneSubjectA",
    createdAt: new Date(),
    state: "nw",
    grade: 1,
    matchRequestCount: 1,
    matchingPriority: 1,
    excludeMatchesWith: [],
    subjects: [
        {
            name: "Deutsch"
        }
    ]
};

export const unmatchableHelper1: Helper = {
    //is unmatchable due to subjects with empty names
    id: 7,
    uuid: "unmatchableHelper1",
    createdAt: new Date(),
    state: "nw",
    matchRequestCount: 1,
    excludeMatchesWith: [],
    subjects: [
        {
            name: "",
            gradeRestriction: {
                min: 1,
                max: 8
            }
        },
        {
            name: "",
            gradeRestriction: {
                min: 3,
                max: 10
            }
        }
    ]
};

export const unmatchableHelpee1: Helpee = {
    //in this test file, no match for that helpee can be found
    id: 8,
    uuid: "unmatchableHelpee1",
    createdAt: new Date(),
    state: "nw",
    grade: 4,
    matchRequestCount: 1,
    matchingPriority: 1,
    excludeMatchesWith: [],
    subjects: [
        {
            name: "Altgriechisch"
        },
        {
            name: "Französisch"
        }
    ]
};

export const helpeeWithOneEmptySubjectName1: Helpee = {
    id: 9,
    uuid: "helpeeWithOneEmptySubjectName1",
    createdAt: new Date(),
    state: "by",
    grade: 4,
    matchRequestCount: 1,
    matchingPriority: 1,
    excludeMatchesWith: [],
    subjects: [
        {
            name: ""
        },
        {
            name: "Mathematik"
        }
    ]
};
