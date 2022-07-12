import { Helpee, Helper, match } from "../src";
import test from "tape";

const PUPIL_COUNT = 1_000;
const STUDENT_COUNT = 1_000;

test(`Match ${STUDENT_COUNT} Students with ${PUPIL_COUNT} Pupils`, t => {
    const pupils = [];
    for (let i = 0; i < PUPIL_COUNT; i++) {
        pupils.push(<Helpee> {
                id: i,
                uuid: "Helpee" + i,
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
        });
    }

    const students = [];
    for (let i = 0; i < STUDENT_COUNT; i++) {
        students.push(<Helper> {
            id: i,
            uuid: "Helper" + i,
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
        });
    }
    
    match(students, pupils);
    t.end();
});