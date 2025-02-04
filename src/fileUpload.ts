import { App } from "./app";
import { SimulationContents } from "./simulationContent/simulationContent";
import { XMLParser } from "fast-xml-parser";

/**
 * Class responsible for managing the state of the initial w3d file input form
 * and the operations of this form.
 * @see {@link https://developer.mozilla.org/en-US/docs/Web/HTML/Element/input/file|<input type="file"> - HTML: HyperText Markup Language | MDN} for
 * the inspiration for this class. Specifically, look to the source code of the example at the very bottom of the page.
 */
export class FileUpload {
  private readonly fileInputElement: HTMLInputElement;
  private readonly fileUploadSection: HTMLElement;
  private readonly fileUploadBtn: HTMLButtonElement;
  private readonly formContainer: HTMLElement;

  private w3dFile?: File;
  private simulationContents?: SimulationContents;

  /**
   * Create the object representing the TypeScript representation of the HTML w3d file input form
   * @param _fileInputElement HTML <input> of type file
   * @param _fileUploadSection HTML <div> for displaying form information
   * @param _fileUploadBtn HTML <button> to submit the form
   * @param _formContainer HTML <div> that is the parent of the entire form. Hiding this will hide the form in HTML
   */
  constructor(
    _fileInputElement: HTMLInputElement,
    _fileUploadSection: HTMLElement,
    _fileUploadBtn: HTMLButtonElement,
    _formContainer: HTMLElement,
  ) {
    this.fileInputElement = _fileInputElement;
    this.fileUploadSection = _fileUploadSection;
    this.fileUploadBtn = _fileUploadBtn;
    this.formContainer = _formContainer;

    this.fileInputElement.addEventListener("change", this.addFile.bind(this));
    this.fileUploadBtn.addEventListener("click", this.submitFile.bind(this));
  }

  /**
   * Event listener that is called whenever the file upload button is clicked on.
   * Will check the uploaded file, and if it is correct, will pass the file contents
   * on to another method so that its contents can be passed into text.
   */
  private addFile() {
    this.resetHtmlElements();

    const currFiles = this.fileInputElement.files;

    if (!currFiles || currFiles.length === 0) {
      const para = document.createElement("p");
      para.classList.add("error");
      para.textContent = "No w3d file selected for upload";
      this.fileUploadSection.appendChild(para);
      return;
    }

    const w3dFile = currFiles[0];

    if (this.hasW3dExtension(w3dFile.name) === false) {
      const para = document.createElement("p");
      para.classList.add("error");
      para.textContent = "Error: w3d file type required";
      this.fileUploadSection.appendChild(para);
      return;
    }

    const reader = new FileReader();
    const loadFile = this.loadFile.bind(this);
    reader.onload = function (e) {
      loadFile(e, w3dFile);
    };
    reader.readAsText(w3dFile);
  }

  /**
   * Event listener for the FileReader 'load' event. Takes the text content
   * created by the FileReader, parses this into XML and passes the XML into
   * an instance of 'SimulationContents' class.
   * @param event FileReader event. Contains the text contents.
   * @param file JavaScript File object that contains text content.
   */
  private loadFile(event: ProgressEvent<FileReader>, file: File): void {
    if (!event.target?.result || typeof event.target.result !== "string")
      return;
    const contents = event.target.result;

    const parsedTextContent = this.removeRootElement(contents);

    const options = {
      ignoreAttributes: false, // To ensure attributes are parsed
      attributeNamePrefix: "", // To avoid prefixing attribute names
      preserveOrder: true,
    };
    const parser = new XMLParser(options);

    const arrayOfParsedTags = parser.parse(parsedTextContent);

    // TODO: Wrap the instantiation of this class within a try...catch statement:
    // currently, if the client attempts to upload a file that is indeed a '.w3d'
    // file yet does not adhere to the expected XML format for a simulation trace
    // output file produced from Witness, the 'loadFile' method will end here due
    // to the corresponding exception thrown from the constructor of the `SimulationContents` class.
    // This means the rest of the method will not be executed, and so the user will not be able to submit the
    // form because no simulation contents has been stored. Correspondingly, there is nothing Witness 3D Web Simulation Viewer
    // is displaying to the user to let them know that the given exception has occurred. Therefore, a try...catch statement
    // should be added so that the file input form can react by displaying the proper exception message.
    const simulationContents: SimulationContents = new SimulationContents(
      arrayOfParsedTags,
    );

    this.simulationContents = simulationContents;

    const para = document.createElement("p");
    para.textContent = `File name ${file.name}, file size ${this.returnFileSize(file.size)}`;
    this.fileUploadSection.appendChild(para);

    this.w3dFile = file;
  }

  /**
   * Removes opening and closing parent <root> element given text contents of w3d file.
   * @param {string} fileContents Text content of w3d file
   * @returns {string} If w3d file contained opening and closing parent <root> element, the text contents of w3d file with these tags removed.
   * Otherwise, the original, unchanged text contents of w3d file.
   */
  private removeRootElement(fileContents: string): string {
    const rootElementRegEx = /<\/?root[^>]*>/;

    // Split the text contents into individual lines
    const individualLines = fileContents.trim().split("\n");

    if (rootElementRegEx.test(individualLines[0])) {
      individualLines.shift();
    }

    if (rootElementRegEx.test(individualLines[individualLines.length - 1])) {
      individualLines.pop();
    }

    // Join the individual lines back into a string
    return individualLines.join("\n");
  }

  /**
   * Event listener for user clicking on form submit button. If the file has been uploaded correctly
   * and parsed into XML without errors, will instantiate an instance of 'App'
   */
  private submitFile(): void {
    if (
      !this.w3dFile ||
      this.hasW3dExtension(this.w3dFile.name) === false ||
      this.simulationContents === undefined
    ) {
      if (
        // Only insert warning above submit button if no such warning has already been inserted into HTML.
        !(
          this.fileUploadBtn.previousElementSibling &&
          this.fileUploadBtn.previousElementSibling.nodeName
        )
      ) {
        const para = document.createElement("p");
        para.textContent = "No w3d file to submit";
        this.fileUploadBtn.parentNode?.insertBefore(para, this.fileUploadBtn);
      }
      // There is no w3d file to upload
      return;
    }
    // Remove 'formContainer' so that canvas is the only element in the document body
    this.formContainer.style.display = "none";
    this.reset();

    // Initialise app, so canvas element will be added to the document body
    new App(this.formContainer, this.simulationContents);
  }

  /**
   * Resets the file input form back to its original state.
   */
  private reset(): void {
    this.w3dFile = undefined;

    this.resetHtmlElements();

    this.fileInputElement.files = null;
    this.fileInputElement.value = "";

    const para = document.createElement("p");
    para.textContent = "No w3d file selected for upload";
    this.fileUploadSection.appendChild(para);
  }

  /**
   * See {@link  FileUpload#reset} for companion method. Does the same as that method, only it deals with the HTML elements only and does not affect the
   * state of the input form as managed by {@link  FileUpload} class
   */
  private resetHtmlElements(): void {
    while (this.fileUploadSection.firstChild) {
      this.fileUploadSection.removeChild(this.fileUploadSection.firstChild);
    }

    while (
      this.fileUploadBtn.previousElementSibling &&
      this.fileUploadBtn.previousElementSibling.nodeName == "P"
    ) {
      this.fileUploadBtn.previousElementSibling.remove();
    }
  }

  /**
   * Checks if the given filename has the '.w3d' file extension.
   * @param filename The given filename.
   * @returns True if the filename has a '.w3d' extension. Otherwise, false.
   */
  private hasW3dExtension(filename: string): boolean {
    const w3dRegex = /\.w3d$/;
    return w3dRegex.test(filename);
  }

  /**
   * Parses the 'size' property of a JavaScript File into a readable format.
   * @param number 'size' property of JavaScript File.
   * @returns 'size' of JavaScript file in readable format.
   * @see {@link https://developer.mozilla.org/en-US/docs/Web/HTML/Element/input/file|<input type="file"> - HTML: HyperText Markup Language | MDN} for source.
   * Specifically, look to the source code of the example at the very bottom of the page.
   */
  private returnFileSize(number) {
    if (number < 1e3) {
      return `${number} bytes`;
    } else if (number >= 1e3 && number < 1e6) {
      return `${(number / 1e3).toFixed(1)} KB`;
    } else {
      return `${(number / 1e6).toFixed(1)} MB`;
    }
  }
}
