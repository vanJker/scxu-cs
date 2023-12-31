#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QTextCodec>
#include<QDebug>
#include <QTextStream>
#include <string>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->file_path = "";
    this->file_content = "";
}

MainWindow::~MainWindow()
{
    delete ui;
}

//open file
void MainWindow::on_open_clicked()
{
    //打开选取路径
    QString path = QFileDialog::getOpenFileName(this,"打开文件","C:");
    //读取内容
    //设置utf-8的类型
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QFile file(path);
    //打开文件以只读的形式打开
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream a(&file);
    //读取内容
    QByteArray arry = file.readAll();
    //显示在文本框中
    ui->textEdit->setText(codec->toUnicode(arry));
    //关闭文件
    file.close();

    // 保存文件路径
    this->file_path = path.toStdString();
    // 保存文件内容
    this->file_content = ui->textEdit->toPlainText();
}


void MainWindow::on_save_clicked()
{
    QString text = ui->textEdit->toPlainText();
    //打开选取路径
    QString path = QFileDialog::getOpenFileName(this,"保存文件","C:");
    //读取内容，并设置utf-8的类型
    QTextCodec *codec = QTextCodec::codecForName("utf-8");
    QFile savefile(path);
    if(savefile.open(QIODevice::WriteOnly)){
        QByteArray a = text.toLatin1();
        savefile.write(a);
        savefile.close();
    }
}

void MainWindow::on_scan_clicked()
{
    using std::string;

    if (this->file_path == "" || this->file_content != ui->textEdit->toPlainText()) {
        this->file_path = this->temp_file();
        this->file_content = ui->textEdit->toPlainText();
    }
    const char *pgm = this->file_path.c_str(); /* source code file name */
    source = fopen(pgm, "r");
    listing = stdout;
    lineno = 0;

    string text;
    text += "\nTINY COMPILATION: " + file_path + "\n";
    text += "\nTokens:\n";
    TokenType token;
    while ((token = getToken()) != ENDFILE) {
      text += strToken(token, tokenString);
    }
    fclose(source);

    QString qtext = QString::fromStdString(text);
    ui->textBrowser->setText(qtext);
}

void MainWindow::on_parse_clicked()
{
    using std::string;

    if (this->file_path == "" || this->file_content != ui->textEdit->toPlainText()) {
        this->file_path = this->temp_file();
        this->file_content = ui->textEdit->toPlainText();
    }
    const char* pgm = this->file_path.c_str(); /* source code file name */
    source = fopen(pgm, "r");
    listing = stdout;
    lineno = 0;

    string text;
    text += "\nTINY COMPILATION: " + file_path + "\n";
    TreeNode* syntaxTree = parse();
    text += "\nSyntax Tree:\n";
    text += strSyntaxTree(syntaxTree);
    fclose(source);

    QString qtext = QString::fromStdString(text);
    ui->textBrowser->setText(qtext);
}

std::string MainWindow::temp_file()
{
    QString text = ui->textEdit->toPlainText();
    //打开选取路径
    QString path = QString::fromStdString("D:\\temp.tny");
    QFile savefile(path);
    if(savefile.open(QIODevice::WriteOnly)){
        QByteArray a = text.toLatin1();
        savefile.write(a);
        savefile.close();
    }
    return path.toStdString();
}
