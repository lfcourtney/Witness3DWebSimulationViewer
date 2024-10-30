import { App } from "./app";
import { XMLParser } from "fast-xml-parser";

// interface W3dFileStructure {
//   create: object[];
//   delete: object[];
//   end: object[];
//   load: object[];
//   new: object;
//   start: object[];
//   update: object[];
//   version: object;
// }

/**
 * Class responsible for managing the state of the initial w3d file input form
 * and the operations of this form
 */
export class FileUpload {
  private readonly fileInputElement: HTMLInputElement;
  private readonly fileUploadSection: HTMLElement;
  private readonly fileUploadBtn: HTMLButtonElement;
  private readonly formContainer: HTMLElement;

  private w3dFile?: File;

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

  private loadFile(event: ProgressEvent<FileReader>, file: File): void {
    if (!event.target?.result || typeof event.target.result !== "string")
      return;
    const contents = event.target.result;

    const isolatedComments: string[] = [];

    const lines = contents.split("\n");
    let currentCommandName = "";
    let elementStore = "";
    lines.forEach((line) => {
      if (currentCommandName) {
        elementStore += line;
        if (!this.checkCurrentLineMatchesClosingTag(line, currentCommandName)) {
          return;
        }
      }

      const elementMatches = this.matchClosingTag(line);

      if (elementMatches && elementMatches.length > 1) {
        const commandName = elementMatches[1];
        if (commandName === "root") return;
        if (currentCommandName === commandName) {
          currentCommandName = "";
          isolatedComments.push(elementStore);
          return;
        }

        elementStore += line;
        currentCommandName = commandName;
        return;
      }

      const selfClosingElementMatches = this.matchSelfClosingTag(line);
      if (selfClosingElementMatches && selfClosingElementMatches.length > 1) {
        isolatedComments.push(line);
      }
    });

    const options = {
      ignoreAttributes: false, // To ensure attributes are parsed
      attributeNamePrefix: "", // To avoid prefixing attribute names
    };
    const parser = new XMLParser(options);

    console.log(isolatedComments);

    let integer = 0;

    isolatedComments.forEach((isolatedComment) => {
      if (integer > 20) return;
      const jsonObj = parser.parse(isolatedComment);
      console.log(jsonObj); // Display the parsed JavaScript object in the comments
      integer++;
    });

    // const options = {
    //   ignoreAttributes: false, // To ensure attributes are parsed
    //   attributeNamePrefix: "", // To avoid prefixing attribute names
    // };

    // const parser = new XMLParser(options);
    // const jsonObj: W3dFileStructure = parser.parse(contents);
    // console.log(jsonObj); // Display the parsed JavaScript object in the comments
    const para = document.createElement("p");
    para.textContent = `File name ${file.name}, file size ${this.returnFileSize(file.size)}`;
    this.fileUploadSection.appendChild(para);

    this.w3dFile = file;
  }

  private checkCurrentLineMatchesClosingTag(
    currentLine: string,
    currentCommand,
  ): boolean {
    const elementMatches = this.matchClosingTag(currentLine);

    if (elementMatches && elementMatches.length > 1) {
      const commandFromLine = elementMatches[1];
      return commandFromLine === currentCommand;
    }

    return false;
  }

  private matchSelfClosingTag(matchString: string) {
    const selfClosingElementRegex = /<([a-zA-Z]+)[^>]*\/>/;

    return selfClosingElementRegex.exec(matchString);
  }

  private matchClosingTag(matchString: string) {
    const elementRegex = /<\/?([a-zA-Z]+)[^>/]*>/;

    return elementRegex.exec(matchString);
  }

  private submitFile(): void {
    if (!this.w3dFile || this.hasW3dExtension(this.w3dFile.name) === false) {
      if (
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
    new App(this.formContainer);
  }

  private reset(): void {
    this.w3dFile = undefined;

    this.resetHtmlElements();

    this.fileInputElement.files = null;
    this.fileInputElement.value = "";

    const para = document.createElement("p");
    para.textContent = "No w3d file selected for upload";
    this.fileUploadSection.appendChild(para);
  }

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

  private hasW3dExtension(filename: string): boolean {
    const w3dRegex = /\.w3d$/;
    return w3dRegex.test(filename);
  }

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
