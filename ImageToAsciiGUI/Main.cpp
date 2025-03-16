#include <QApplication>
#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QProcess>
#include <QMessageBox>
#include <QFileDialog>
#include <QCoreApplication>
#include <QDir>
#include <QDebug>
#include <QFileInfo>
#include <QComboBox>

class MainWindow : public QWidget {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr) : QWidget(parent) {

        widthEdit = new QLineEdit("320", this);
        aspectRatioEdit = new QLineEdit("1.8", this);
        blockSizeEdit = new QLineEdit("5", this);
        asciiCharsEdit = new QLineEdit("  `.`^,-~:;<>i#MW&8%B@", this);
        pathEdit = new QLineEdit(this);
        browseButton = new QPushButton("Browse", this);
        isColoredCheck = new QCheckBox("Colored Output", this);
        isColoredCheck->setChecked(true);
        
        terminalCombo = new QComboBox(this);
        terminalCombo->addItem("Auto-detect", "auto");
        
        // Lista de terminais suportados
        QStringList terminals = {"xterm", "gnome-terminal", "konsole", "terminator", "xfce4-terminal"};
        
        for (const QString &terminal : terminals) {
            QProcess whichProcess;
            whichProcess.start("which", QStringList() << terminal);
            whichProcess.waitForFinished();
            if (whichProcess.exitCode() == 0) {
                terminalCombo->addItem(terminal, terminal);
            }
        }
        
        playButton = new QPushButton("Play", this);

        QVBoxLayout *layout = new QVBoxLayout(this);
        layout->addWidget(new QLabel("Width:"));
        layout->addWidget(widthEdit);
        layout->addWidget(new QLabel("Aspect Ratio:"));
        layout->addWidget(aspectRatioEdit);
        layout->addWidget(new QLabel("Block Size:"));
        layout->addWidget(blockSizeEdit);
        layout->addWidget(new QLabel("ASCII Chars:"));
        layout->addWidget(asciiCharsEdit);
        layout->addWidget(new QLabel("Video/Image Path:"));
        QHBoxLayout *pathLayout = new QHBoxLayout;
        pathLayout->addWidget(pathEdit);
        pathLayout->addWidget(browseButton);
        layout->addLayout(pathLayout);
        layout->addWidget(isColoredCheck);
        
        QHBoxLayout *terminalLayout = new QHBoxLayout;
        terminalLayout->addWidget(new QLabel("Terminal:"));
        terminalLayout->addWidget(terminalCombo);
        layout->addLayout(terminalLayout);
        
        layout->addWidget(playButton);

        connect(playButton, &QPushButton::clicked, this, &MainWindow::onPlayClicked, Qt::UniqueConnection);
        connect(browseButton, &QPushButton::clicked, this, &MainWindow::onBrowseClicked);
    }

private:

    bool isVideoFile(const QString &filePath) {

        QStringList videoExtensions = {
            "mp4", "avi", "mov", "wmv", "flv", "mkv", "webm", "m4v", 
            "mpg", "mpeg", "3gp", "3g2", "ogv", "ts", "m2ts", "mts", "gif"
        };
        
        QFileInfo fileInfo(filePath);
        QString extension = fileInfo.suffix().toLower();
        
        return videoExtensions.contains(extension);
    }

private slots:
    void onPlayClicked() {
        qDebug() << "onPlayClicked triggered";


        bool widthOk, aspectOk, blockOk;
        widthEdit->text().toInt(&widthOk);
        aspectRatioEdit->text().toFloat(&aspectOk);
        blockSizeEdit->text().toInt(&blockOk);

        if (!widthOk || !aspectOk || !blockOk) {
            QMessageBox::warning(this, "Error", "Invalid numeric values!");
            return;
        }

        if (pathEdit->text().isEmpty()) {
            QMessageBox::warning(this, "Error", "Please specify a video or image path!");
            return;
        }


        QString executablePath = QCoreApplication::applicationDirPath();
        QDir execDir(executablePath);
        

        execDir.cdUp();
        

        QString convertProgram = execDir.absolutePath() + "/ConvertImageToAscii";
        

        if (!QFile::exists(convertProgram)) {

            convertProgram = execDir.absolutePath() + "/../ConvertImageToAscii";
            
            if (!QFile::exists(convertProgram)) {
                QMessageBox::critical(this, "Error", "ConvertImageToAscii executable not found. Please build it first.");
                qDebug() << "Executable not found at:" << convertProgram;
                return;
            }
        }
        
        qDebug() << "Using executable at:" << convertProgram;


        QStringList convertArgs;
        convertArgs << "--width" << widthEdit->text();
        convertArgs << "--aspect-ratio" << aspectRatioEdit->text();
        convertArgs << "--blockSize" << blockSizeEdit->text();
        convertArgs << "--ascii-chars" << "\"" + asciiCharsEdit->text() + "\"";
        
        if (isColoredCheck->isChecked()) {
            convertArgs << "--isColored";
        } else {
            convertArgs << "--isNotColored";
        }
        

        if (isVideoFile(pathEdit->text())) {
            convertArgs << "--video-path" << "\"" + pathEdit->text() + "\"";
        } else {
            convertArgs << "--image-path" << "\"" + pathEdit->text() + "\"";
        }


        QString fullCommand = convertProgram + " " + convertArgs.join(" ");
        qDebug() << "Full command:" << fullCommand;
        
        QString foundTerminal;
        

        if (terminalCombo->currentData().toString() == "auto") {

            QStringList terminals = {"xterm", "gnome-terminal", "konsole", "terminator", "xfce4-terminal"};
            

            for (const QString &terminal : terminals) {
                QProcess whichProcess;
                whichProcess.start("which", QStringList() << terminal);
                whichProcess.waitForFinished();
                if (whichProcess.exitCode() == 0) {
                    foundTerminal = terminal;
                    break;
                }
            }
        } else {

            foundTerminal = terminalCombo->currentData().toString();
        }
        
        if (foundTerminal.isEmpty()) {
            QMessageBox::critical(this, "Error", "No terminal emulator found. Please install xterm, gnome-terminal, or konsole.");
            return;
        }
        
        qDebug() << "Using terminal:" << foundTerminal;
        

        QStringList terminalArgs;
        QString commandToRun = fullCommand + "; echo 'Press Enter to close...'; read";
        
        if (foundTerminal == "xterm") {

            terminalArgs << "-fullscreen" << "-fa" << "Monospace" << "-fs" << "1" << "-bg" << "black" << "-fg" << "white" << "-e" << "bash" << "-c" << commandToRun;
        } else if (foundTerminal == "gnome-terminal") {

            terminalArgs << "--full-screen" << "--font='Monospace 1'" << "--hide-menubar" << "--profile=Default" << "--" << "bash" << "-c" << "echo -e '\\033]10;white\\007\\033]11;black\\007'; " + commandToRun;
        } else if (foundTerminal == "konsole") {


            QString konsoleCommand = "export KONSOLE_FONT='Monospace,1,-1,5,50,0,0,0,0,0'; echo -e '\\033]10;white\\007\\033]11;black\\007'; " + commandToRun;
            terminalArgs << "--fullscreen" << "-e" << "bash" << "-c" << konsoleCommand;
        } else if (foundTerminal == "terminator") {

            terminalArgs << "--fullscreen" << "--font=Monospace 1" << "-e" << "bash" << "-c" << "echo -e '\\033]10;white\\007\\033]11;black\\007'; " + commandToRun;
        } else if (foundTerminal == "xfce4-terminal") {

            terminalArgs << "--fullscreen" << "--font=Monospace 1" << "--color-bg=black" << "--color-fg=white" << "--hide-menubar" << "--hide-toolbar" << "--hide-scrollbar" << "-e" << "bash -c \"" + commandToRun + "\"";
        }
        
        QProcess *process = new QProcess(this);
        

        connect(process, &QProcess::readyReadStandardOutput, this, [=]() {
            qDebug() << "Terminal output:" << process->readAllStandardOutput();
        });
        
        connect(process, &QProcess::readyReadStandardError, this, [=]() {
            qDebug() << "Terminal error:" << process->readAllStandardError();
        });
        
        connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished), this, [=](int exitCode, QProcess::ExitStatus exitStatus) {
            qDebug() << "Terminal process finished with exit code:" << exitCode << "status:" << exitStatus;
            process->deleteLater();
        });
        
        process->start(foundTerminal, terminalArgs);
        
        if (!process->waitForStarted(3000)) {
            QMessageBox::critical(this, "Error", "Failed to start terminal: " + process->errorString());
            qDebug() << "Failed to start terminal process. Error:" << process->errorString();
            process->deleteLater();
        } else {
            qDebug() << "Terminal process started successfully";
        }
    }

    void onBrowseClicked() {

        QString videoFormats = "Video Files (*.mp4 *.avi *.mov *.wmv *.flv *.mkv *.webm *.m4v *.mpg *.mpeg *.3gp *.3g2 *.ogv *.ts *.m2ts *.mts *.gif)";
        QString imageFormats = "Image Files (*.jpg *.jpeg *.png *.bmp *.tiff *.webp)";
        
        QString filePath = QFileDialog::getOpenFileName(
            this, 
            "Select Video or Image", 
            "", 
            videoFormats + ";;" + imageFormats + ";;All Files (*)"
        );
        
        if (!filePath.isEmpty()) {
            pathEdit->setText(filePath);
        }
    }

private:
    QLineEdit *widthEdit;
    QLineEdit *aspectRatioEdit;
    QLineEdit *blockSizeEdit;
    QLineEdit *asciiCharsEdit;
    QLineEdit *pathEdit;
    QPushButton *browseButton;
    QCheckBox *isColoredCheck;
    QComboBox *terminalCombo;
    QPushButton *playButton;
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.setWindowTitle("Image to ASCII Converter GUI");
    window.resize(400, 300);
    window.show();
    return app.exec();
}

#include "Main.moc"
