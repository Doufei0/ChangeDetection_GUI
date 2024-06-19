#include "ChangeDetectionApplication.h"
#include "config.h"

QPixmap startDetection(const QString& beforeImagePath, const QString& afterImagePath, const QString& resultImagePath)
{
    QProcess process;
    process.setProcessChannelMode(QProcess::MergedChannels);
    QString appDirPath = QCoreApplication::applicationDirPath();
    QDir dir(appDirPath);
    dir.cdUp(); // 上一级目录
    dir.cdUp(); // 再上一级目录
    QString projectRootPath = dir.absolutePath();
    // this "activate DL" shoule be your python environment name
    QString command = "activate DL && cd /d " + projectRootPath + "/PyFiles && python predict.py" + " " +
        beforeImagePath + " " + afterImagePath + " " + resultImagePath + "\r\n";

    process.start("cmd.exe");
    process.waitForStarted();
    process.write(command.toLatin1());
    process.closeWriteChannel();
    process.waitForFinished(-1);

    std::cout << "脚本执行完成!" << std::endl;

    QPixmap resultPixmap(resultImagePath);
    return resultPixmap;
}



ChangeDetectionApplication::ChangeDetectionApplication(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
    // 设置主窗口大小
    resize(800, 400);

    QWidget* mainWidget = new QWidget(this);
    setCentralWidget(mainWidget);

    QHBoxLayout* mainLayout = new QHBoxLayout(mainWidget);
    mainLayout->setContentsMargins(40, 20, 40, 20); // 左上右下
    mainLayout->setSpacing(20);

    // 左边子窗口及其布局
    QWidget* leftContainer = new QWidget(this);
    QVBoxLayout* leftLayout = new QVBoxLayout(leftContainer);
    leftLayout->setContentsMargins(0, 0, 0, 0);
    leftLayout->setSpacing(10); // 控件之间的距离
    leftLabel = new QLabel("Before Change", this);
    leftLabel->setAlignment(Qt::AlignCenter);
    leftLabel->setFixedSize(WIDGETLENGTH, LABELHEIGHT);
    leftLayout->addWidget(leftLabel);

    leftWidget = new QLabel("Choose Picture", this);
    leftWidget->setStyleSheet("background-color: lightgray; border: 1px solid black");
    leftWidget->setAlignment(Qt::AlignCenter);
    leftWidget->setFixedSize(WIDGETLENGTH, WIDGETLENGTH);
    leftLayout->addWidget(leftWidget);
    leftWidget->installEventFilter(this);

    leftButton = new QPushButton("Select File", this);
    leftLayout->addWidget(leftButton);
    connect(leftButton, &QPushButton::clicked, this, &ChangeDetectionApplication::openFileDialogForLeftWidget);
    mainLayout->addWidget(leftContainer);

    // 中间子窗口及其布局
    QWidget* centerContainer = new QWidget(this);
    QVBoxLayout* centerLayout = new QVBoxLayout(centerContainer);
    centerLayout->setContentsMargins(0, 0, 0, 0);
    centerLayout->setSpacing(10); // 控件之间的距离
    centerLabel = new QLabel("After Change", this);
    centerLabel->setAlignment(Qt::AlignCenter);
    centerLabel->setFixedSize(WIDGETLENGTH, LABELHEIGHT);
    centerLayout->addWidget(centerLabel);

    centerWidget = new QLabel("Choose Picture", this);
    centerWidget->setStyleSheet("background-color: lightgray; border: 1px solid black");
    centerWidget->setAlignment(Qt::AlignCenter);
    centerWidget->setFixedSize(WIDGETLENGTH, WIDGETLENGTH);
    centerLayout->addWidget(centerWidget);
    centerWidget->installEventFilter(this);

    centerButton = new QPushButton("Select File", this);
    centerLayout->addWidget(centerButton);
    connect(centerButton, &QPushButton::clicked, this, &ChangeDetectionApplication::openFileDialogForCenterWidget);
    mainLayout->addWidget(centerContainer);

    // 右边子窗口及其布局
    QWidget* rightContainer = new QWidget(this);
    QVBoxLayout* rightLayout = new QVBoxLayout(rightContainer);
    rightLayout->setContentsMargins(0, 0, 0, 0);
    rightLayout->setSpacing(10); // 控件之间的距离
    rightLabel = new QLabel("Before Change", this);
    rightLabel->setAlignment(Qt::AlignCenter);
    rightLabel->setFixedSize(WIDGETLENGTH, LABELHEIGHT);
    rightLayout->addWidget(rightLabel);

    rightWidget = new QLabel("Choose Picture", this);
    rightWidget->setStyleSheet("background-color: lightgray; border: 1px solid black");
    rightWidget->setAlignment(Qt::AlignCenter);
    rightWidget->setFixedSize(WIDGETLENGTH, WIDGETLENGTH);
    rightLayout->addWidget(rightWidget);
    rightWidget->installEventFilter(this);

    rightButton = new QPushButton("Change Detection", this);
    rightLayout->addWidget(rightButton);
    connect(rightButton, &QPushButton::clicked, this, &ChangeDetectionApplication::performChangeDetection);
    mainLayout->addWidget(rightContainer);

    
}

ChangeDetectionApplication::~ChangeDetectionApplication()
{
    /* 释放空间 */
    delete process;
}

bool ChangeDetectionApplication::eventFilter(QObject* obj, QEvent* event)
{
    if ((obj == leftWidget || obj == centerWidget) && event->type() == QEvent::MouseButtonPress) {
        clickedWidget = qobject_cast<QLabel*>(obj);
        openFileDialog();
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}

void ChangeDetectionApplication::openFileDialog() 
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "", tr("Image Files (*.png *.jpg *.bmp)"));
    if (!fileName.isEmpty()) {
        // 尝试加载图片
        QPixmap pixmap(fileName);
        if (pixmap.isNull())
            qDebug() << "Failed to load image!";
        else
            qDebug() << "Image loaded successfully!";

        // 尝试在对应的 QLabel 中显示图片
        if (clickedWidget) {
            clickedWidget->setPixmap(pixmap.scaled(clickedWidget->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            if (clickedWidget == leftWidget){
                beforeImage = pixmap;
                beforeImagePath = fileName;
                std::cout << "变化前图像路径：";
                qDebug() << beforeImagePath;
            }
            else if (clickedWidget == centerWidget) {
                afterImage = pixmap;
                afterImagePath = fileName;
                std::cout << "变化后图像路径：";
                qDebug() << afterImagePath;
            }
        }
        else {
            qDebug() << "No clickedWidget set!";
        }
    }
}


void ChangeDetectionApplication::openFileDialogForLeftWidget() 
{
    clickedWidget = leftWidget;
    openFileDialog();
}


void ChangeDetectionApplication::openFileDialogForCenterWidget()
{
    clickedWidget = centerWidget;
    openFileDialog();
}


void ChangeDetectionApplication::performChangeDetection()
{
    if (!beforeImage.isNull() && !afterImage.isNull()) {
        // 提取 beforeImagePath 的文件命，加到resultImagePath中
        QFileInfo fileInfo(beforeImagePath);
        QString appDirPath = QCoreApplication::applicationDirPath();
        QDir dir(appDirPath);
        dir.cdUp(); // 上一级目录
        dir.cdUp(); // 再上一级目录
        QString projectRootPath = dir.absolutePath();
        QString resultImagePath = projectRootPath + "/PyFiles/predict_results/" + fileInfo.fileName();

        qDebug() << beforeImagePath;
        qDebug() << afterImagePath;
        qDebug() << resultImagePath;

        QPixmap resultPixmap = startDetection(beforeImagePath, afterImagePath, resultImagePath);
        rightWidget->setPixmap(resultPixmap.scaled(rightWidget->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
    }
    else
    {
        qDebug() << "One or both images are not loaded sucessfully!";
    }

}