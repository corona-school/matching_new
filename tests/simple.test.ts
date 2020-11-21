import test from "tape";
import { Helpee, Helper, Match, match } from "../src";
import * as TestData from "./simple.test.data";

////////////////////////////////////////////////////////////////////////////////
// Use this file to test some simple and very basic cases
////////////////////////////////////////////////////////////////////////////////

function matchFromUUIDs(helperUUID: string, helpeeUUID: string): Match {
    return {
        helper: {
            uuid: helperUUID
        },
        helpee: {
            uuid: helpeeUUID
        }
    };
}

test("Only one simple possible match combination", t => {
    const resultSingleSubject = match([TestData.helperWithOneSubjectA], [TestData.helpeeWithOneSubjectA]);
    t.deepEqual(resultSingleSubject.matches, [
        matchFromUUIDs(TestData.helperWithOneSubjectA.uuid, TestData.helpeeWithOneSubjectA.uuid)
    ]);

    const resultMultipleSubjects = match([TestData.helperA], [TestData.helpeeA]);
    t.deepEqual(resultMultipleSubjects.matches, [matchFromUUIDs(TestData.helperA.uuid, TestData.helpeeA.uuid)]);

    t.end();
});

test("No one to match", t => {
    const result = match([], []);
    t.deepEqual(result.matches, []);

    t.end();
});

test("Empty subject name", t => {
    const result = match([TestData.unmatchableHelper1], [TestData.helpeeA]);
    t.deepEqual(result.matches, []);

    t.end();
});

test("No helpee or no helper", t => {
    const resultNoHelper = match([], [TestData.helpeeA]);
    t.deepEqual(resultNoHelper.matches, []);

    const resultNoHelpee = match([TestData.helperA], []);
    t.deepEqual(resultNoHelpee.matches, []);

    t.end();
});

test("A single subject name is empty", t => {
    const result = match([TestData.helperA], [TestData.helpeeWithOneEmptySubjectName1]);
    t.deepEqual(result.matches, [matchFromUUIDs(TestData.helperA.uuid, TestData.helpeeWithOneEmptySubjectName1.uuid)]);

    t.end();
});

test("No matches due to dissolvedMatches", t => {
    const helperANew: Helper = { ...TestData.helperA, dissolvedMatches: [{ uuid: TestData.helpeeA.uuid }] };
    const helpeeANew: Helpee = { ...TestData.helpeeA, dissolvedMatches: [{ uuid: TestData.helperA.uuid }] }; //note: both need to have the dissolved matches property!
    const result = match([helperANew], [helpeeANew]);
    t.deepEqual(result.matches, []);

    t.end();
});

test("Two with disjunct subjects, i.e. unmatchable", t => {
    const result = match([TestData.helperB], [TestData.helpeeC]);
    t.deepEqual(result.matches, []);

    t.end();
});
