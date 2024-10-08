import { describe, expect, it, beforeEach } from "vitest";
import { JSDOM } from "jsdom";

// FUNCTION from the 'startup.ts' file
let getFileUploadElements;

const w3dFileInputErrorMessage =
  "Element of id 'w3dFileInput' was unable to be found within index.html";
const fileUploadSectionErrorMessage =
  "Element of id 'fileUploadSection' was unable to be found within index.html";
const fileUploadBtnErrorMessage =
  "Element of id 'fileUploadBtn' was unable to be found within index.html";
const formContainerErrorMessage =
  "Element of id 'formContainer' was unable to be found within index.html";
const loaderContainerErrorMessage =
  "Element of id 'loaderContainer' was unable to be found within index.html";

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

  it(`should throw exception if w3dFileInput element cannot be found`, () => {
    const w3dFileInput = document.getElementById("w3dFile");
    w3dFileInput?.remove();

    // Correct exception has been thrown
    expect(() => getFileUploadElements()).toThrowError(
      w3dFileInputErrorMessage,
    );
  });

  it(`should throw exception if fileUploadSection element cannot be found`, () => {
    const fileUploadSection = document.getElementById("fileUploadSection");
    fileUploadSection?.remove();

    // Correct exception has been thrown
    expect(() => getFileUploadElements()).toThrowError(
      fileUploadSectionErrorMessage,
    );
  });

  it(`should throw exception if fileUploadBtn element cannot be found`, () => {
    const fileUploadBtn = document.getElementById("fileUploadBtn");
    fileUploadBtn?.remove();

    // Correct exception has been thrown
    expect(() => getFileUploadElements()).toThrowError(
      fileUploadBtnErrorMessage,
    );
  });

  it(`should throw exception if formContainer element cannot be found`, () => {
    const formContainer = document.getElementById("formContainer");
    formContainer?.remove();

    // Correct exception has been thrown
    expect(() => getFileUploadElements()).toThrowError(
      formContainerErrorMessage,
    );
  });

  it(`should throw exception if loaderContainer element cannot be found`, () => {
    const loaderContainer = document.getElementById("loaderContainer");
    loaderContainer?.remove();

    // Correct exception has been thrown
    expect(() => getFileUploadElements()).toThrowError(
      loaderContainerErrorMessage,
    );
  });
});
