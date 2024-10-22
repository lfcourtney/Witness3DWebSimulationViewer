import { describe, expect, it, beforeEach, vi, afterEach } from "vitest";
import { FileUpload } from "./fileUpload";
import { JSDOM } from "jsdom";

let fileUpload: FileUpload;

const textFileName = "test.txt";
const w3dFileName = "test.w3d";

const submitFileButtonMsg = "No w3d file to submit";
const noFileMsg = "No w3d file selected for upload";
const fileNotW3d = "Error: w3d file type required";

describe("test FileUpload class", () => {
  beforeEach(async () => {
    const dom = await JSDOM.fromFile("index.html");

    global.window = dom.window;
    global.document = dom.window.document;

    const startupModules = await import("./startup");
    const fileUploadElements = startupModules.getFileUploadElements();
    fileUpload = new FileUpload(
      fileUploadElements.w3dFileInput,
      fileUploadElements.fileUploadSection,
      fileUploadElements.fileUploadBtn,
      fileUploadElements.formContainer,
    );
  });

  afterEach(() => {
    vi.restoreAllMocks();
  });

  it("should return false when a non .w3d file is checked against 'hasW3dExtension' function", () => {
    const isw3dFile = fileUpload["hasW3dExtension"](textFileName);

    expect(isw3dFile).toBe(false);
  });

  it("should return true a .w3d file is checked against 'hasW3dExtension' function", () => {
    const isw3dFile = fileUpload["hasW3dExtension"](w3dFileName);

    expect(isw3dFile).toBe(true);
  });

  it(`should prevent file from being submitted if form is attempted to be submitted
    without w3d file`, () => {
    // Arrange
    const fileUploadBtnSection = document.getElementById(
      "fileUploadBtnSection",
    );

    // Act
    fileUpload["submitFile"]();

    const errorMessage = fileUploadBtnSection
      ?.getElementsByTagName("p")
      .item(0);

    // Assert warning paragraph element has been added
    expect(errorMessage).toBeTruthy();

    // Assert warning paragraph element has correct message
    expect(errorMessage?.textContent).toBe(submitFileButtonMsg);
  });

  it(`should display appropriate error message if no file has been uploaded when 
    'addFile' function has been called`, () => {
    // Arrange
    const fileUploadSection = document.getElementById("fileUploadSection");

    // Act
    fileUpload["addFile"]();

    const paragraphMessage = fileUploadSection
      ?.getElementsByTagName("p")
      .item(0);

    // Assert warning paragraph element has been added
    expect(paragraphMessage).toBeTruthy();

    // Assert warning paragraph element has correct message
    expect(paragraphMessage?.textContent).toBe(noFileMsg);
  });

  it(`should display appropriate error message if file has been uploaded when 
    'addFile' function has been called, but it is not w3d file`, () => {
    // Arrange
    const fileUploadSection = document.getElementById("fileUploadSection");
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (fileUpload as any).fileInputElement = {
      files: [new File([], textFileName)],
    };

    // Act
    fileUpload["addFile"]();

    const paragraphMessage = fileUploadSection
      ?.getElementsByTagName("p")
      .item(0);

    // Assert warning paragraph element has been added
    expect(paragraphMessage).toBeTruthy();

    // Assert warning paragraph element has correct message
    expect(paragraphMessage?.textContent).toBe(fileNotW3d);
  });

  it(`should parse to FileReader when 'addFile' function has been called with
     appropriate w3d file`, () => {
    // Arrange
    const readAsTextSpy = vi.spyOn(FileReader.prototype, "readAsText");
    const w3dFile: File = new File([], w3dFileName);
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (fileUpload as any).fileInputElement = {
      files: [w3dFile],
    };

    // Act
    fileUpload["addFile"]();

    expect(readAsTextSpy).toHaveBeenCalledWith(w3dFile);
  });

  it(`should display message information if loadFile method has been called with appropriate w3d file`, () => {
    // Arrange
    const fileUploadSection = document.getElementById("fileUploadSection");
    const mockEvent = { target: { result: "exampleXML" } };
    const w3dFile: File = new File([], w3dFileName);
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (fileUpload as any).fileInputElement = {
      files: [w3dFile],
    };

    // Act
    fileUpload["resetHtmlElements"]();
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    fileUpload["loadFile"](mockEvent as any, w3dFile);

    const paragraphMessage = fileUploadSection
      ?.getElementsByTagName("p")
      .item(0);

    // Assert warning paragraph element has been added
    expect(paragraphMessage).toBeTruthy();

    // Assert warning paragraph element has correct message
    expect(paragraphMessage?.textContent).toBe(
      `File name ${w3dFile.name}, file size ${fileUpload["returnFileSize"](w3dFile.size)}`,
    );
  });

  it(`should reset state of FileUpload object when 'reset' method is called`, () => {
    // Arrange
    const w3dFile: File = new File([], w3dFileName);
    // eslint-disable-next-line @typescript-eslint/no-explicit-any
    (fileUpload as any).w3dFile = w3dFile;

    // Assert that w3d file has not been reset
    expect(fileUpload["w3dFile"]).equal(w3dFile);

    // Act
    fileUpload["reset"]();

    // Assert that w3d file has been reset
    expect(fileUpload["w3dFile"]).toBe(undefined);
  });
});
