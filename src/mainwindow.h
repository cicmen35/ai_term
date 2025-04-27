#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTextEdit>
#include <QLineEdit>
#include <QLabel>
#include "wrapper.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();

private slots:
    void onPromptEntered();

private:
    QTextEdit* terminalDisplay;
    QLineEdit* promptInput;
    QLabel* statusLabel;
    
    AI ai;  // AI instance
    bool aiInitialized = false;
    
    void initializeAI();
    void appendToTerminal(const QString& text, bool isAI = false);
};

#endif // MAINWINDOW_H
