import { Helpee, Helper, Settings } from "../../../types";
import { InputFiles } from "../types";
import { writeToTemporaryFile } from "../utils";
import { BalancingCoefficientsInputFormat, HelpeesInputFormat, HelpersInputFormat } from "./formats";
import { balancingCoefficientsTransformToInput, helpeesTransformToInput, helpersTransformToInput } from "./transforms";

type WriteInputs = {
    helpers: HelpersInputFormat;
    helpees: HelpeesInputFormat;
    balancingCoefficients: BalancingCoefficientsInputFormat;
};

function createInputToWrite(helpers: Helper[], helpees: Helpee[], settings: Settings): WriteInputs {
    return {
        helpers: helpersTransformToInput(helpers),
        helpees: helpeesTransformToInput(helpees),
        balancingCoefficients: balancingCoefficientsTransformToInput(settings.balancingCoefficients)
    };
}

function writeInput(input: WriteInputs): InputFiles {
    return {
        helpers: writeToTemporaryFile(input.helpers),
        helpees: writeToTemporaryFile(input.helpees),
        balancingCoefficients: writeToTemporaryFile(input.balancingCoefficients)
    };
}

export function writeInputFiles(helpers: Helper[], helpees: Helpee[], settings: Settings): InputFiles {
    const toWrite = createInputToWrite(helpers, helpees, settings);
    return writeInput(toWrite);
}
