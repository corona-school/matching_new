export type BalancingCoefficients = {
    subjectMatching: number;
    state: number;
    waitingTime: number;
    matchingPriority: number;
};
export type Settings = {
    balancingCoefficients: BalancingCoefficients;
};
