#include "mainwindow.h"
#include <QPalette>
#include <QPixmap>
#include <QFileDialog>
#include <QResizeEvent>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), currentSceneIndex(0)
{
    // 设置窗口初始大小为16:9比例(例如1280x720)
    this->resize(1280, 720);

    // 初始化游戏场景
    scenes = {
        {
            ":/imag/E:/图片/1.png", // 背景图片路径
            "你醒来发现自己在一片陌生的森林中。\n\n阳光透过树叶间的缝隙洒落下来，四周鸟鸣声不断。",
            {"探索周围", "留在原地"}
        },
        {
            ":/imag/E:/图片/2.png",
            "你决定探索周围。\n\n走了一段路后，你发现了一条小溪和一座破旧的小木屋。",
            {"调查小溪", "进入木屋"}
        },
        {
            ":/images/scene3.jpg",
            "你选择进入木屋。\n\n屋内灰尘满布，角落里有张桌子，上面放着一本发黄的日记。",
            {"阅读日记", "离开木屋"}
        }
    };

    setupUI();
    loadScene(currentSceneIndex);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupUI()
{
    // 创建中央部件
    centralWidget = new QWidget(this);
    this->setCentralWidget(centralWidget);

    // 主布局
    mainLayout = new QVBoxLayout(centralWidget);
    mainLayout->setContentsMargins(0, 0, 0, 0);
    mainLayout->setSpacing(0);

    // 背景标签(用于显示图片)
    backgroundLabel = new QLabel(centralWidget);
    backgroundLabel->setScaledContents(true);
    backgroundLabel->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainLayout->addWidget(backgroundLabel);

    // 创建文本容器(半透明)
    textContainer = new QWidget(centralWidget);
    textContainer->setStyleSheet("background-color: rgba(0, 0, 0, 150);");

    // 文本容器布局
    QVBoxLayout *textLayout = new QVBoxLayout(textContainer);
    textLayout->setContentsMargins(20, 20, 20, 20);

    // 文本显示区域
    textDisplay = new QTextEdit(textContainer);
    textDisplay->setReadOnly(true);
    textDisplay->setStyleSheet(
        "QTextEdit {"
        "background: transparent;"
        "border: none;"
        "color: white;"
        "font-size: 18px;"
        "font-family: 'Microsoft YaHei', sans-serif;"
        "}"
        );

    // 下一步按钮
    nextButton = new QPushButton("下一步", textContainer);
    nextButton->setStyleSheet(
        "QPushButton {"
        "background-color: rgba(70, 70, 70, 200);"
        "color: white;"
        "border: 1px solid #555;"
        "padding: 8px 16px;"
        "font-size: 16px;"
        "}"
        "QPushButton:hover {"
        "background-color: rgba(90, 90, 90, 200);"
        "}"
        );

    // 选择按钮布局
    choicesLayout = new QHBoxLayout();
    choicesLayout->setSpacing(10);

    // 将部件添加到布局
    textLayout->addWidget(textDisplay, 1);
    textLayout->addLayout(choicesLayout);
    textLayout->addWidget(nextButton, 0, Qt::AlignRight);

    // 将文本容器覆盖在背景上
    textContainer->setParent(backgroundLabel);
    textContainer->setGeometry(0, backgroundLabel->height()/2,
                               backgroundLabel->width(), backgroundLabel->height()/2);

    // 连接按钮信号
    connect(nextButton, &QPushButton::clicked, this, &MainWindow::nextButtonClicked);
}

void MainWindow::loadScene(int sceneIndex)
{
    if(sceneIndex < 0 || sceneIndex >= scenes.size()) return;

    currentSceneIndex = sceneIndex;
    const GameScene &scene = scenes[sceneIndex];

    // 加载背景图片
    QPixmap pixmap(scene.backgroundImage);
    if(!pixmap.isNull()) {
        backgroundLabel->setPixmap(pixmap);
    } else {
        backgroundLabel->setStyleSheet("background-color: #333;");
        qDebug() << "Failed to load image:" << scene.backgroundImage;
    }

    // 设置场景文本
    textDisplay->setText(scene.sceneText);

    // 设置选择按钮
    setupChoices();
}

void MainWindow::setupChoices()
{
    // 清除现有选择按钮
    for(QPushButton* button : choiceButtons) {
        choicesLayout->removeWidget(button);
        delete button;
    }
    choiceButtons.clear();

    // 如果没有选择项，显示下一步按钮
    if(scenes[currentSceneIndex].choices.isEmpty()) {
        nextButton->show();
        return;
    }

    // 隐藏下一步按钮，显示选择按钮
    nextButton->hide();

    // 创建选择按钮
    for(int i = 0; i < scenes[currentSceneIndex].choices.size(); ++i) {
        QPushButton *button = new QPushButton(scenes[currentSceneIndex].choices[i], textContainer);
        button->setStyleSheet(
            "QPushButton {"
            "background-color: rgba(70, 70, 70, 200);"
            "color: white;"
            "border: 1px solid #555;"
            "padding: 8px 16px;"
            "font-size: 16px;"
            "min-width: 120px;"
            "}"
            "QPushButton:hover {"
            "background-color: rgba(90, 90, 90, 200);"
            "}"
            );

        // 使用lambda捕获按钮索引
        connect(button, &QPushButton::clicked, this, [this, i]() {
            choiceButtonClicked(i);
        });

        choicesLayout->addWidget(button);
        choiceButtons.append(button);
    }
}

void MainWindow::nextButtonClicked()
{
    // 简单示例：切换到下一个场景
    if(currentSceneIndex + 1 < scenes.size()) {
        loadScene(currentSceneIndex + 1);
    } else {
        textDisplay->append("\n\n游戏结束！感谢游玩！");
        nextButton->setEnabled(false);
    }
}

void MainWindow::choiceButtonClicked(int index)
{
    // 根据选择更新场景
    QString choice = scenes[currentSceneIndex].choices[index];
    qDebug() << "玩家选择了:" << choice;

    // 这里可以根据选择决定下一个场景
    // 简单示例：线性推进
    if(currentSceneIndex + 1 < scenes.size()) {
        loadScene(currentSceneIndex + 1);
    }
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
    QMainWindow::resizeEvent(event);

    // 保持16:9比例
    int width = event->size().width();
    int height = width * 9 / 16;
    this->resize(width, height);

    // 调整文本容器大小和位置
    if(textContainer && backgroundLabel) {
        textContainer->setGeometry(0, backgroundLabel->height()/2,
                                   backgroundLabel->width(), backgroundLabel->height()/2);
    }
}
