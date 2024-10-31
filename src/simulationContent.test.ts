import { describe, expect, it } from "vitest";
import { SimulationContents } from "./simulationContent";

const instantiationErrorMessage =
  "'_tagStore' argument is not an array of XML parsed JavaScript objects";

describe("test SimulationContents class", () => {
  it(`throws appropriate exception if it is tried to be instantiated with an array that is not an object`, () => {
    const incorrectTagStore = [{}];

    expect(() => {
      new SimulationContents(incorrectTagStore);
    }).toThrowError(instantiationErrorMessage);
  });
});
