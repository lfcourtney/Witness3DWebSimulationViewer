import { describe, expect, it, beforeEach } from "vitest";
import { JSDOM } from "jsdom";

// Represents the corresponding function from the 'startup.ts' file
let getFileUploadElements;

describe("test startup function", () => {
  beforeEach(async () => {
    const dom = await JSDOM.fromFile("index.html");

    global.window = dom.window;
    global.document = dom.window.document;

    const startupModules = await import("./startup");
    getFileUploadElements = startupModules.getFileUploadElements;
  });

  it(`ascertains necessary HTML elements when invoking 'getFileUploadElements' function`, () => {
    let fileUploadElements;

    // Assert no exceptions have been thrown
    expect(() => {
      fileUploadElements = getFileUploadElements();
    }).not.toThrowError();

    // Assert that the elements both exist and are of the type they are meant to be
    expect(fileUploadElements.w3dFileInput.nodeName).toBe("INPUT");
    expect(fileUploadElements.fileUploadSection.nodeName).toBe("DIV");
    expect(fileUploadElements.fileUploadBtn.nodeName).toBe("BUTTON");
    expect(fileUploadElements.formContainer.nodeName).toBe("DIV");
  });
});
