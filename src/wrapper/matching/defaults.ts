import { Settings } from "../types";

export const defaultSettings: Settings = {
    balancingCoefficients: {
        subjectMatching: 0.65,
        state: 0.05,
        waitingTime: 0.2,
        matchingPriority: 0.1
    }
};
