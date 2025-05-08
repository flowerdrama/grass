#include "mainwindow.h"
#include <QFile>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    // 初始化UI组件
    sceneImage = new QLabel(this);
    sceneImage->setAlignment(Qt::AlignCenter);
    sceneImage->setMinimumSize(400, 300);

    sceneText = new QTextEdit(this);
    sceneText->setReadOnly(true);

    option1Button = new QPushButton("选项1", this);
    option2Button = new QPushButton("选项2", this);

    // 布局设置
    QVBoxLayout *mainLayout = new QVBoxLayout();
    QHBoxLayout *buttonLayout = new QHBoxLayout();

    buttonLayout->addWidget(option1Button);
    buttonLayout->addWidget(option2Button);

    mainLayout->addWidget(sceneImage);
    mainLayout->addWidget(sceneText);
    mainLayout->addLayout(buttonLayout);

    QWidget *centralWidget = new QWidget(this);
    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    // 设置游戏场景数据
    setupScenes();

    // 连接信号槽
    connect(option1Button, &QPushButton::clicked, this, &MainWindow::handleOption1);
    connect(option2Button, &QPushButton::clicked, this, &MainWindow::handleOption2);

    // 设置初始场景
    updateGameState("start");
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupScenes()
{
    // 添加游戏场景数据
    scenes.insert("start",
                  qMakePair(":/imag/E:/图片/1.png", "你醒来发现自己在一个陌生的森林里。阳光透过树叶洒在地上。"));
    choices.insert("start",
                   qMakePair("探索周围", "沿着小路走"));
    transitions.insert("start",
                       qMakePair("explore", "follow_path"));

    scenes.insert("explore",
                  qMakePair(":/imag/E:/图片/2.png", "你在附近发现了一个古老的箱子。"));
    choices.insert("explore",
                   qMakePair("打开箱子", "忽略它继续前进"));
    transitions.insert("explore",
                       qMakePair("open_chest", "ignore_chest"));

    // 可以继续添加更多场景...
}

void MainWindow::updateGameState(const QString &nextScene)
{
    currentScene = nextScene;

    // 更新图像
    QPixmap pixmap(scenes.value(currentScene).first);
    if(!pixmap.isNull()) {
        sceneImage->setPixmap(pixmap.scaled(sceneImage->size(), Qt::KeepAspectRatio));
    }

    // 更新文本
    sceneText->setText(scenes.value(currentScene).second);

    // 更新按钮文本
    option1Button->setText(choices.value(currentScene).first);
    option2Button->setText(choices.value(currentScene).second);
}

void MainWindow::handleOption1()
{
    QString nextScene = transitions.value(currentScene).first;
    updateGameState(nextScene);
}

void MainWindow::handleOption2()
{
    QString nextScene = transitions.value(currentScene).second;
    updateGameState(nextScene);
}
