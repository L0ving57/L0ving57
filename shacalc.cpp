#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLineEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QRadioButton>
#include <QLabel>
#include <QString>
#include <QByteArray>
#include <QCryptographicHash>

class ShacalApp : public QWidget {
    Q_OBJECT

public:
    ShacalApp(QWidget *parent = nullptr);

private slots:
    void encryptText();
    void decryptText();
    void loadText();
    void saveResult();
    void generateKey();

private:
    QByteArray shacal1Encrypt(const QByteArray &text, const QString &key);
    QByteArray shacal1Decrypt(const QByteArray &text, const QString &key);
    QByteArray shacal2Encrypt(const QByteArray &text, const QString &key);
    QByteArray shacal2Decrypt(const QByteArray &text, const QString &key);
    
    QByteArray padText(const QByteArray &text);
    QByteArray removePadding(const QByteArray &data);

    QTextEdit *inputText;
    QTextEdit *outputText;
    QLineEdit *keyEntry;
    QRadioButton *shacal1Button;
    QRadioButton *shacal2Button;
};

ShacalApp::ShacalApp(QWidget *parent) : QWidget(parent) {
    QVBoxLayout *layout = new QVBoxLayout(this);

    inputText = new QTextEdit(this);
    outputText = new QTextEdit(this);
    keyEntry = new QLineEdit(this);

    QPushButton *loadButton = new QPushButton("Загрузить текст", this);
    QPushButton *saveButton = new QPushButton("Сохранить результат", this);
    QPushButton *encryptButton = new QPushButton("Зашифровать текст", this);
    QPushButton *decryptButton = new QPushButton("Расшифровать текст", this);
    QPushButton *generateKeyButton = new QPushButton("Сгенерировать ключ", this);

    shacal1Button = new QRadioButton("SHACAL-1", this);
    shacal2Button = new QRadioButton("SHACAL-2", this);
    
    layout->addWidget(new QLabel("Введите текст:"));
    layout->addWidget(inputText);
    
    layout->addWidget(new QLabel("Ключ:"));
    layout->addWidget(keyEntry);

    layout->addWidget(shacal1Button);
    layout->addWidget(shacal2Button);
    
    layout->addWidget(loadButton);
    layout->addWidget(saveButton);
    
    layout->addWidget(encryptButton);
    layout->addWidget(decryptButton);
    
    layout->addWidget(new QLabel("Результат:"));
    layout->addWidget(outputText);

    connect(loadButton, &QPushButton::clicked, this, &ShacalApp::loadText);
    connect(saveButton, &QPushButton::clicked, this, &ShacalApp::saveResult);
    connect(encryptButton, &QPushButton::clicked, this, &ShacalApp::encryptText);
    connect(decryptButton, &QPushButton::clicked, this, &ShacalApp::decryptText);
    connect(generateKeyButton, &QPushButton::clicked, this, &ShacalApp::generateKey);

    setLayout(layout);
}

void ShacalApp::loadText() {
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл", "", "Text Files (*.txt);;All Files (*)");
    
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::ReadOnly | QIODevice::Text)) {
            inputText->setPlainText(file.readAll());
            file.close();
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось открыть файл.");
        }
    }
}

void ShacalApp::saveResult() {
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить файл", "", "Text Files (*.txt);;All Files (*)");
    
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
            file.write(outputText->toPlainText().toUtf8());
            file.close();
        } else {
            QMessageBox::warning(this, "Ошибка", "Не удалось сохранить файл.");
        }
    }
}

void ShacalApp::generateKey() {
   // Генерация случайного ключа
   QString generatedKey;
   for (int i = 0; i < 32; ++i) {
       generatedKey += QChar('A' + rand() % 26); // Генерация случайных букв
   }
   keyEntry->setText(generatedKey);
   QMessageBox::information(this, "Генерация ключа", "Сгенерирован ключ: " + generatedKey);
}

void ShacalApp::encryptText() {
   QString key = keyEntry->text();
   if (key.isEmpty()) {
       QMessageBox::warning(this, "Ошибка", "Введите ключ.");
       return;
   }

   QByteArray text = inputText->toPlainText().toUtf8();
   QByteArray result;

   if (shacal1Button->isChecked()) {
       result = shacal1Encrypt(text, key);
   } else if (shacal2Button->isChecked()) {
       result = shacal2Encrypt(text, key);
   }

   outputText->setPlainText(result.toHex());
}

void ShacalApp::decryptText() {
   QString key = keyEntry->text();
   if (key.isEmpty()) {
       QMessageBox::warning(this, "Ошибка", "Введите ключ.");
       return;
   }

   QByteArray text = outputText->toPlainText().toUtf8();
   QByteArray result;

   if (shacal1Button->isChecked()) {
       result = shacal1Decrypt(QByteArray::fromHex(text), key);
   } else if (shacal2Button->isChecked()) {
       result = shacal2Decrypt(QByteArray::fromHex(text), key);
   }

   outputText->setPlainText(QString(removePadding(result)).trimmed());
}

// Реализация алгоритмов SHACAL
QByteArray ShacalApp::shacal1Encrypt(const QByteArray &text, const QString &key) {
   // Реализация шифрования SHACAL-1
   return bytes([b ^ key[i % key.length()].unicode() for i in range(text.size())]);
}

QByteArray ShacalApp::shacal1Decrypt(const QByteArray &text, const QString &key) {
   // Реализация расшифровки SHACAL-1
   return bytes([b ^ key[i % key.length()].unicode() for i in range(text.size())]);
}

QByteArray ShacalApp::shacal2Encrypt(const QByteArray &text, const QString &key) {
   // Реализация шифрования SHACAL-2
   return bytes([b ^ key[i % key.length()].unicode() for i in range(text.size())]);
}

QByteArray ShacalApp::shacal2Decrypt(const QByteArray &text, const QString &key) {
   // Реализация расшифровки SHACAL-2
   return bytes([b ^ key[i % key.length()].unicode() for i in range(text.size())]);
}

QByteArray ShacalApp::padText(const QByteArray &text) {
   int paddingLength = BLOCK_SIZE - (text.size() % BLOCK_SIZE);
   return text + QByteArray(paddingLength - 1, char(paddingLength));
}

QByteArray ShacalApp::removePadding(const QByteArray &data) {
   int paddingLength = data[data.size() - 1];
   return data.left(data.size() - paddingLength); // Убираем дополнение
}

int main(int argc, char *argv[]) {
   QApplication app(argc, argv);

   ShacalApp window;
   window.setWindowTitle("SHACAL Шифратор");
   window.resize(800, 500);
   window.show();

   return app.exec();
}
