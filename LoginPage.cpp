#include <QMessageBox>
#include "LoginPage.h"

LoginPage::LoginPage(DataBase* db, QWidget* parent)
    : database(db), QWidget(parent) {
    setWindowTitle("Login");

    usernameLineEdit = new QLineEdit(this);
    usernameLineEdit->setPlaceholderText("Username");

    passwordLineEdit = new QLineEdit(this);
    passwordLineEdit->setPlaceholderText("Password");
    passwordLineEdit->setEchoMode(QLineEdit::Password);

    loginButton = new QPushButton("Login", this);
    loginButton->setEnabled(false);

    registerButton = new QPushButton("Register", this);
    registerButton->setEnabled(false);

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(usernameLineEdit);
    layout->addWidget(passwordLineEdit);
    layout->addWidget(loginButton);
    layout->addWidget(registerButton);

    connect(loginButton, &QPushButton::clicked, this, &LoginPage::onLoginClicked);
    connect(registerButton, &QPushButton::clicked, this, &LoginPage::onRegisterClicked);
    connect(usernameLineEdit, &QLineEdit::textChanged, this, &LoginPage::checkInputFields);
    connect(passwordLineEdit, &QLineEdit::textChanged, this, &LoginPage::checkInputFields);
}

void LoginPage::onLoginClicked() {
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();

    if (database->authenticateUser(username, password)) {
        int userId = database->getUserIdByUsername(username);
        int role = database->getUserRole(username);
        if (role == 0) {
            emit userLoginSuccessful(userId);
        } else if (role == 1) {
            emit adminLoginSuccessful();
        }
    } else {
        QMessageBox::critical(this, "Error", "Invalid username or password.");
    }
}

void LoginPage::onRegisterClicked() {
    QString username = usernameLineEdit->text();
    QString password = passwordLineEdit->text();
    if (!username.isEmpty() && !password.isEmpty()) {
        database->registerUser(username, password, 0);
        QMessageBox::information(this, "Success", "Registration successful!");
    } else {
        QMessageBox::critical(this, "Error", "Please enter valid username and password.");
    }
}

void LoginPage::checkInputFields() {
    bool isUsernameEmpty = usernameLineEdit->text().trimmed().isEmpty();
    bool isPasswordEmpty = passwordLineEdit->text().trimmed().isEmpty();
    loginButton->setEnabled(!isUsernameEmpty && !isPasswordEmpty);
    registerButton->setEnabled(!isUsernameEmpty && !isPasswordEmpty);
}
