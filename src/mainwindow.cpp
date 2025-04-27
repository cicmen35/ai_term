#include "mainwindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QApplication>
#include <QProcess>
#include <QTimer>
#include <QDir>
#include <QFileDialog>

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent) {
    QWidget* centralWidget = new QWidget(this);
    QVBoxLayout* mainLayout = new QVBoxLayout(centralWidget);
    
    // Status bar
    QHBoxLayout* statusLayout = new QHBoxLayout();
    statusLabel = new QLabel("AI Model: Loading...");
    statusLabel->setStyleSheet("color: orange;");
    
    statusLayout->addWidget(statusLabel);
    statusLayout->addStretch();
    mainLayout->addLayout(statusLayout);
    
    // Terminal display
    terminalDisplay = new QTextEdit(this);
    terminalDisplay->setReadOnly(true);
    terminalDisplay->setStyleSheet("background-color: #1E1E1E; color: #FFFFFF; font-family: 'Courier New', monospace;");
    mainLayout->addWidget(terminalDisplay, 1);
    
    // Prompt input
    promptInput = new QLineEdit(this);
    promptInput->setPlaceholderText("Enter a command or question...");
    promptInput->setStyleSheet("font-family: 'Courier New', monospace;");
    mainLayout->addWidget(promptInput);
    
    setCentralWidget(centralWidget);
    setWindowTitle("AI Terminal");
    resize(800, 600);
    
    // Connect signals and slots
    connect(promptInput, &QLineEdit::returnPressed, this, &MainWindow::onPromptEntered);
    
    // Welcome message
    appendToTerminal("Welcome to AI Terminal!", true);
    appendToTerminal("Type 'help' for a list of commands.", true);
    
    // Initialize AI automatically
    QTimer::singleShot(100, this, &MainWindow::initializeAI);
}

MainWindow::~MainWindow() {}

void MainWindow::initializeAI() {
    // First try the specific model
    std::string modelPath = "../external/llama.cpp/models/tinyllama-1.1b-chat-v1.0.Q4_K_M.gguf";
    
    // Check if the specific model exists
    QFileInfo specificModel(QString::fromStdString(modelPath));
    if (!specificModel.exists()) {
        appendToTerminal("Specific model not found, looking for alternatives...", true);
        
        // Try to find any suitable model file
        QDir modelsDir("../external/llama.cpp/models");
        QStringList filters;
        filters << "*.gguf";
        QFileInfoList modelFiles = modelsDir.entryInfoList(filters, QDir::Files, QDir::Name);
        
        if (!modelFiles.isEmpty()) {
            // Use the first .gguf file found
            modelPath = modelFiles.first().absoluteFilePath().toStdString();
            appendToTerminal("Found alternative model: " + QString::fromStdString(modelPath), true);
        } else {
            // No models found, prompt user to select one
            appendToTerminal("No model files found in default location.", true);
            QString filePath = QFileDialog::getOpenFileName(this, 
                "Select AI Model", 
                QDir::homePath(), 
                "GGUF Models (*.gguf);;All Files (*)");
                
            if (filePath.isEmpty()) {
                statusLabel->setText("AI Model: Not loaded");
                statusLabel->setStyleSheet("color: red;");
                appendToTerminal("No model selected. The application will work in limited mode.", true);
                QApplication::restoreOverrideCursor();
                return;
            }
            
            modelPath = filePath.toStdString();
        }
    }
    
    appendToTerminal("Loading AI model from: " + QString::fromStdString(modelPath) + "...", true);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    
    if (ai.loadModel(modelPath)) {
        aiInitialized = true;
        statusLabel->setText("AI Model: Loaded");
        statusLabel->setStyleSheet("color: green;");
        appendToTerminal("Model loaded successfully!", true);
    } else {
        statusLabel->setText("AI Model: Failed to load");
        statusLabel->setStyleSheet("color: red;");
        appendToTerminal("Failed to load model. The application will work in limited mode.", true);
    }
    
    QApplication::restoreOverrideCursor();
}

void MainWindow::onPromptEntered() {
    QString input = promptInput->text();
    if (input.isEmpty()) return;
    
    appendToTerminal("> " + input);
    promptInput->clear();
    
    // Handle special commands
    if (input.toLower() == "exit" || input.toLower() == "quit") {
        QApplication::quit();
        return;
    } else if (input.toLower() == "clear") {
        terminalDisplay->clear();
        return;
    } else if (input.toLower() == "help") {
        appendToTerminal("Available commands:\n"
                         "  help - Show this help message\n"
                         "  clear - Clear the terminal\n"
                         "  exit/quit - Exit the application\n"
                         "  Any other input will be sent to the AI", true);
        return;
    }
    
    // Process with AI if initialized
    if (aiInitialized) {
        QApplication::setOverrideCursor(Qt::WaitCursor);
        appendToTerminal("Processing...", true);
        
        // Get AI response
        std::string response = ai.generate(input.toStdString());
        appendToTerminal(QString::fromStdString(response), true);
        
        QApplication::restoreOverrideCursor();
    } else {
        appendToTerminal("AI model not loaded. Please load a model first.", true);
    }
}

void MainWindow::appendToTerminal(const QString& text, bool isAI) {
    // Format differently based on whether it's user input or AI response
    if (isAI) {
        terminalDisplay->append("<span style='color:#4CAF50;'>AI: </span>" + text);
    } else {
        terminalDisplay->append(text);
    }
    
    // Scroll to bottom
    QTextCursor cursor = terminalDisplay->textCursor();
    cursor.movePosition(QTextCursor::End);
    terminalDisplay->setTextCursor(cursor);
}
