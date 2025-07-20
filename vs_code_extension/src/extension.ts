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

    const rangeFormattingProvider = vscode.languages.registerDocumentRangeFormattingEditProvider(
        { language: 'cpp' },
        {
            provideDocumentRangeFormattingEdits(
                document: vscode.TextDocument, 
                range: vscode.Range
            ): vscode.TextEdit[] {
                return formatRangeSync(document, range, context, outputChannel);
            }
        }
    );

    context.subscriptions.push(disposable, formattingProvider, rangeFormattingProvider, outputChannel);
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

        // Apply the formatted content from stdout to the editor
        const editor = vscode.window.activeTextEditor;
        if (editor && stdout) {
            const fullRange = new vscode.Range(
                document.positionAt(0),
                document.positionAt(document.getText().length)
            );
            
            editor.edit(editBuilder => {
                editBuilder.replace(fullRange, stdout);
            }).then(() => {
                vscode.window.showInformationMessage('File formatted with Zensep');
            });
        } else {
            vscode.window.showInformationMessage('File formatted with Zensep');
        }
    });
}

function formatRangeSync(
    document: vscode.TextDocument, 
    range: vscode.Range,
    context: vscode.ExtensionContext, 
    outputChannel: vscode.OutputChannel
): vscode.TextEdit[] {
    outputChannel.appendLine(`Zensep formatRangeSync called for ${document.fileName}, lines ${range.start.line + 1}:${range.end.line + 1}`);
    
    const config = vscode.workspace.getConfiguration('zensep');
    let executablePath = config.get<string>('executablePath');

    if (!executablePath || executablePath.trim() === '') {
        executablePath = getBundledExecutablePath(context);
        
        if (!fs.existsSync(executablePath)) {
            outputChannel.appendLine(`Bundled zensep binary not found at: ${executablePath}`);
            return [];
        }
    }

    try {
        const { execFileSync } = require('child_process');
        
        // Convert VSCode line numbers (0-based) to zensep line numbers (1-based)
        const startLine = range.start.line + 1;
        const endLine = range.end.line + 1;
        const linesArg = `${startLine}:${endLine}`;
        
        // Run zensep with --lines option and capture stdout with the formatted content
        const formattedContent = execFileSync(executablePath, [document.fileName, '--lines', linesArg], { 
            encoding: 'utf8',
            timeout: 10000 // 10 second timeout
        });
        
        outputChannel.appendLine(`Zensep range formatting completed successfully for lines ${linesArg}`);
        
        // Create a TextEdit that replaces the selected range with the formatted content
        return [vscode.TextEdit.replace(range, formattedContent)];
        
    } catch (error) {
        outputChannel.appendLine(`Zensep range formatting error: ${error}`);
        return [];
    }
}

function formatDocumentSync(document: vscode.TextDocument, 
  context: vscode.ExtensionContext, outputChannel: vscode.OutputChannel): vscode.TextEdit[] {
    outputChannel.appendLine(`Zensep formatDocumentSync called for ${document.fileName}`);
    
    const config = vscode.workspace.getConfiguration('zensep');
    let executablePath = config.get<string>('executablePath');

    if (!executablePath || executablePath.trim() === '') {
        executablePath = getBundledExecutablePath(context);
        
        if (!fs.existsSync(executablePath)) {
            outputChannel.appendLine(`Bundled zensep binary not found at: ${executablePath}`);
            return [];
        }
    }

    try {
        const { execFileSync } = require('child_process');
        
        // Run zensep and capture stdout with the formatted content
        const formattedContent = execFileSync(executablePath, [document.fileName], { 
            encoding: 'utf8',
            timeout: 10000 // 10 second timeout
        });
        
        outputChannel.appendLine(`Zensep formatting completed successfully`);
        
        // Create a TextEdit that replaces the entire document with the formatted content
        const fullRange = new vscode.Range(
            document.positionAt(0),
            document.positionAt(document.getText().length)
        );
        
        return [vscode.TextEdit.replace(fullRange, formattedContent)];
        
    } catch (error) {
        outputChannel.appendLine(`Zensep formatting error: ${error}`);
        return [];
    }
}

export function deactivate() {}