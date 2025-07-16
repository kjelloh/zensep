import * as vscode from 'vscode';
import { exec } from 'child_process';
import * as path from 'path';
import * as fs from 'fs';
import { execFile } from 'child_process';

export function activate(context: vscode.ExtensionContext) {

    const outputChannel = vscode.window.createOutputChannel("Zensep");
    outputChannel.appendLine("Zensep extension activated");
    outputChannel.show(true);

    const disposable = vscode.commands.registerCommand('zensep.format', () => {
        const editor = vscode.window.activeTextEditor;
        if (!editor) {
            vscode.window.showErrorMessage('No active editor');
            return;
        }

        const document = editor.document;
        if (!isCppFile(document)) {
            vscode.window.showErrorMessage('Zensep only formats C++ files');
            return;
        }

        formatDocument(document, context, outputChannel);
    });

    const formattingProvider = vscode.languages.registerDocumentFormattingEditProvider(
        { language: 'cpp' },
        {
            provideDocumentFormattingEdits(document: vscode.TextDocument): vscode.TextEdit[] {
                return formatDocumentSync(document, context, outputChannel);
            }
        }
    );

    context.subscriptions.push(disposable, formattingProvider, outputChannel);
}

function isCppFile(document: vscode.TextDocument): boolean {
    const cppExtensions = ['.cpp', '.cc', '.cxx', '.c++', '.cp', '.hpp', '.hh', '.hxx', '.h++', '.hp', '.h'];
    const fileExtension = path.extname(document.fileName).toLowerCase();
    return cppExtensions.includes(fileExtension);
}

function getBundledExecutablePath(context: vscode.ExtensionContext): string {
    const platform = process.platform; // 'darwin', 'linux', 'win32'
    const arch = process.arch; // 'x64', 'arm64'
    
    let platformDir = 'darwin-arm64/zensep';
    if (platform === 'darwin') {
        platformDir = arch === 'arm64' ? 'darwin-arm64' : 'darwin-x64';
    } else if (platform === 'linux') {
        platformDir = 'linux-x64';
    } else if (platform === 'win32') {
        platformDir = 'win32-x64';
    }
    
    const executableName = platform === 'win32' ? 'zensep.exe' : 'zensep';
    return path.join(context.extensionPath, 'bin', platformDir, executableName);
}

function formatDocument(
    document: vscode.TextDocument,
    context: vscode.ExtensionContext,
    outputChannel: vscode.OutputChannel
) {
    outputChannel.appendLine(`Zensep formatDocument called for ${document.fileName}`);
    outputChannel.show(true);

    const config = vscode.workspace.getConfiguration('zensep');

    let executablePath = config.get<string>('executablePath');

    console.log('Executable path from config:', executablePath);
    outputChannel.appendLine(`Executable path from config: ${executablePath}`);

    // If no custom path is configured, use the bundled binary
    if (!executablePath || executablePath.trim() === '') {
        executablePath = getBundledExecutablePath(context);
        
        // Check if bundled binary exists
        if (!fs.existsSync(executablePath)) {
            vscode.window.showErrorMessage(`Bundled zensep binary not found at: ${executablePath}`);
            return;
        }
    }
    
    console.log('Zensep executable path::', executablePath);
    
    const tempFile = document.fileName;
    const command = `"${executablePath}" "${tempFile}"`;

    execFile(executablePath, [tempFile], (error, stdout, stderr) => {
        if (error) {
            vscode.window.showErrorMessage(`Zensep formatting failed: ${error.message}\nUsed path: ${executablePath}`);
            return;
        }

        if (stderr) {
            vscode.window.showWarningMessage(`Zensep warning: ${stderr}`);
        }

        vscode.window.showInformationMessage('File formatted with Zensep');
    });
}

function formatDocumentSync(document: vscode.TextDocument, 
  context: vscode.ExtensionContext,outputChannel: vscode.OutputChannel): vscode.TextEdit[] {
    // For now, return empty array - this would need to be implemented
    // to actually format the document content and return the changes
    return [];
}

export function deactivate() {}