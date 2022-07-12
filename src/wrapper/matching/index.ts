import { Helpee, Helper, Match, Settings, Stats } from "../types";
import { defaultSettings } from "./defaults";
import { createOutputFiles, readOutputFiles } from "./files/read";
import { deleteTemporaryFiles } from "./files/utils";
import { writeInputFiles } from "./files/write";
import matchingAlgo from "../../algo";

export type MatchingResult = {
    matches: Match[];
    stats: Stats;
};

export function match(helpers: Helper[], helpees: Helpee[], settings: Settings = defaultSettings): MatchingResult {
    console.log(`\n --------------- MATCHING STARTED ------------------ \n`);
    console.log(`Matching ${helpers.length} helpers with ${helpees.length} helpees`);

    console.time("pre-matching");
    //write matching algorithm input files
    const inputFiles = writeInputFiles(helpers, helpees, settings);

    //generate outputFiles
    const outputFiles = createOutputFiles();
    console.timeEnd("pre-matching");
    //perform matching
    matchingAlgo(
        inputFiles.helpees.name,
        inputFiles.helpers.name,
        inputFiles.balancingCoefficients.name,
        outputFiles.matches.name,
        outputFiles.stats.name
    );
    
    console.time("post-matching");
    //parse matching results
    const results = readOutputFiles(outputFiles);

    //delete tmp files
    deleteTemporaryFiles([
        //matching algorithm INPUT files
        inputFiles.helpers,
        inputFiles.helpees,
        inputFiles.balancingCoefficients,
        //matching algorithm OUTPUT files
        outputFiles.matches,
        outputFiles.stats
    ]);

    console.timeEnd("post-matching");
    console.log("Matching done", results.stats);
    console.log(`\n --------------- MATCHING DONE ------------------ \n\n`);
    return results;
}
