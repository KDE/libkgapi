/*
    SPDX-FileCopyrightText: 2020 David Barchiesi <david@barchie.si>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/


#include "mainwindow.h"
#include "ui_main.h"

#include <drive/file.h>
#include <authjob.h>
#include <account.h>

#include <QFileDialog>
#include <QDebug>

MainWindow::MainWindow(QWidget * parent):
    QMainWindow(parent),
    m_ui(new Ui::MainWindow)
{
    /* Initialize GUI */
    m_ui->setupUi(this);
    connect(m_ui->authButton, &QAbstractButton::clicked,
            this, &MainWindow::authenticate);
    connect(m_ui->browseButton, &QAbstractButton::clicked,
            this, &MainWindow::browseFiles);
    connect(m_ui->uploadButton, &QAbstractButton::clicked,
            this, &MainWindow::uploadFile);
    setInputsEnabled(false);
}

MainWindow::~MainWindow()
{
    delete m_ui;
}

void MainWindow::authenticate()
{
    KGAPI2::AccountPtr account(new KGAPI2::Account);
    account->setScopes( QList<QUrl>() << KGAPI2::Account::driveScopeUrl() );

    /* Create AuthJob to retrieve OAuth tokens for the account */
    KGAPI2::AuthJob *authJob = new KGAPI2::AuthJob(
        account,
        QStringLiteral("554041944266.apps.googleusercontent.com"),
        QStringLiteral("mdT1DjzohxN3npUUzkENT0gO"));
    connect(authJob, &KGAPI2::Job::finished,
             this, &MainWindow::slotAuthJobFinished);
}

void MainWindow::slotAuthJobFinished(KGAPI2::Job *job)
{
    KGAPI2::AuthJob *authJob = qobject_cast<KGAPI2::AuthJob*>(job);
    Q_ASSERT(authJob);
    /* Always remember to delete the jobs, otherwise your application will
     * leak memory. */
    authJob->deleteLater();

    if (authJob->error() != KGAPI2::NoError) {
        m_ui->statusbar->showMessage(QStringLiteral("Error: %1").arg(authJob->errorString()));
        return;
    }

    m_account = authJob->account();

    m_ui->authStatusLabel->setText(QStringLiteral("Authenticated"));
    m_ui->authButton->setEnabled(false);
    setInputsEnabled(true);
}

void MainWindow::browseFiles()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Select file"));
    m_ui->sourceLineEdit->setText(fileName);
}

void MainWindow::uploadFile()
{
    if (m_ui->sourceLineEdit->text().isEmpty()) {
        m_ui->statusbar->showMessage(QStringLiteral("Error: must specify source file."));
        return;
    }
    
    uploadingFile = new QFile(m_ui->sourceLineEdit->text());
    if (!uploadingFile->open(QIODevice::ReadOnly)) {
        m_ui->statusbar->showMessage(QStringLiteral("Error: source file not valid."));
        return;
    }
    
    KGAPI2::Drive::FilePtr uploadFile = KGAPI2::Drive::FilePtr::create();
    QFileInfo fileInfo(uploadingFile->fileName());
    uploadFile->setTitle(fileInfo.fileName());
    
    KGAPI2::Drive::FileResumableCreateJob *fileCreateJob;
    if (m_ui->writeOnSignalRadioButton->isChecked()) {
        fileCreateJob = new KGAPI2::Drive::FileResumableCreateJob(uploadFile, m_account, this);
        connect(fileCreateJob, &KGAPI2::Drive::FileResumableCreateJob::readyWrite,
                this, &MainWindow::slotFileCreateJobReadyWrite);
    } else {
        fileCreateJob = new KGAPI2::Drive::FileResumableCreateJob(uploadingFile, uploadFile, m_account, this);
    }
    fileCreateJob->setUploadSize(uploadingFile->size());
    connect(fileCreateJob, &KGAPI2::Drive::FileResumableCreateJob::finished,
            this, &MainWindow::slotFileCreateJobFinished);
    connect(fileCreateJob, &KGAPI2::Drive::FileResumableCreateJob::progress,
            this, &MainWindow::slotFileCreateJobProgress);
    
    m_ui->statusbar->clearMessage();
    bytesUploaded = 0;
    if (m_ui->writeOnSignalRadioButton->isChecked()) {
        fileUploadProgressBar = new QProgressBar(m_ui->statusbar);
        fileUploadProgressBar->setFormat(QStringLiteral("Sent to Job: %v bytes [%p%]"));
        fileUploadProgressBar->setMaximum(uploadingFile->size());
        m_ui->statusbar->addWidget(fileUploadProgressBar);
    }

    jobUploadProgressBar = new QProgressBar(m_ui->statusbar);
    jobUploadProgressBar->setFormat(QStringLiteral("Job progress: %v bytes [%p%]"));
    m_ui->statusbar->addWidget(jobUploadProgressBar);

    setInputsEnabled(false);
}

void MainWindow::slotFileCreateJobFinished(KGAPI2::Job *job)
{
    qDebug() << "Completed job" << job << "error code:" << job->error() << "- message:" << job->errorString();

    KGAPI2::Drive::FileResumableCreateJob *fileCreateJob = qobject_cast<KGAPI2::Drive::FileResumableCreateJob*>(job);
    Q_ASSERT(fileCreateJob);
    fileCreateJob->deleteLater();

    if (fileCreateJob->error() != KGAPI2::NoError) {
        m_ui->statusbar->showMessage(QStringLiteral("Error: %1").arg(fileCreateJob->errorString()));
    } else {
        KGAPI2::Drive::FilePtr file = fileCreateJob->metadata();
        m_ui->statusbar->showMessage(QStringLiteral("Upload complete, id %1, size %2 (uploaded %3), mimeType %4").arg(file->id()).arg(file->fileSize()).arg(bytesUploaded).arg(file->mimeType()));
    }

    if (m_ui->writeOnSignalRadioButton->isChecked()) {
        m_ui->statusbar->removeWidget(fileUploadProgressBar);
        fileUploadProgressBar->deleteLater();
    }

    m_ui->statusbar->removeWidget(jobUploadProgressBar);
    jobUploadProgressBar->deleteLater();

    setInputsEnabled(true);
}


void MainWindow::slotFileCreateJobReadyWrite(KGAPI2::Drive::FileAbstractResumableJob *job)
{
    QByteArray data = uploadingFile->read(50000);
    bytesUploaded += data.size();
    job->write(data);

    fileUploadProgressBar->setValue(bytesUploaded);
}

void MainWindow::slotFileCreateJobProgress(KGAPI2::Job *job, int base, int total)
{
    Q_UNUSED(job)

    jobUploadProgressBar->setValue(base);
    jobUploadProgressBar->setMaximum(total);
}

void MainWindow::setInputsEnabled(bool enabled)
{
    m_ui->sourceLineEdit->setEnabled(enabled);
    m_ui->browseButton->setEnabled(enabled);
    m_ui->writeOnSignalRadioButton->setEnabled(enabled);
    m_ui->passFileToJobRadioButton->setEnabled(enabled);
    m_ui->uploadButton->setEnabled(enabled);
}
