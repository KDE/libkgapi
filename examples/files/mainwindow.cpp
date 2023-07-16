/*
    SPDX-FileCopyrightText: 2020 David Barchiesi <david@barchie.si>

    SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
*/

#include "mainwindow.h"

#include "core/account.h"
#include "core/authjob.h"
#include "drive/file.h"

#include <QDebug>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    /* Initialize GUI */
    ui.setupUi(this);
    connect(ui.authButton, &QAbstractButton::clicked, this, &MainWindow::authenticate);
    connect(ui.browseButton, &QAbstractButton::clicked, this, &MainWindow::browseFiles);
    connect(ui.uploadButton, &QAbstractButton::clicked, this, &MainWindow::uploadFile);
    setInputsEnabled(false);
}

void MainWindow::authenticate()
{
    auto account = KGAPI2::AccountPtr::create();
    account->setScopes({KGAPI2::Account::driveScopeUrl()});

    /* Create AuthJob to retrieve OAuth tokens for the account */
    auto *authJob = new KGAPI2::AuthJob(account, QStringLiteral("554041944266.apps.googleusercontent.com"), QStringLiteral("mdT1DjzohxN3npUUzkENT0gO"));
    connect(authJob, &KGAPI2::Job::finished, this, [this, authJob]() {
        /* Always remember to delete the jobs, otherwise your application will
         * leak memory. */
        authJob->deleteLater();

        if (authJob->error() != KGAPI2::NoError) {
            ui.statusbar->showMessage(QStringLiteral("Error: %1").arg(authJob->errorString()));
            return;
        }

        m_account = authJob->account();

        ui.authStatusLabel->setText(QStringLiteral("Authenticated"));
        ui.authButton->setEnabled(false);
        setInputsEnabled(true);
    });
}

void MainWindow::browseFiles()
{
    const auto fileName = QFileDialog::getOpenFileName(this, tr("Select file"));
    ui.sourceLineEdit->setText(fileName);
}

void MainWindow::uploadFile()
{
    if (ui.sourceLineEdit->text().isEmpty()) {
        ui.statusbar->showMessage(QStringLiteral("Error: must specify source file."));
        return;
    }

    m_uploadingFile = std::make_unique<QFile>(ui.sourceLineEdit->text());
    if (!m_uploadingFile->open(QIODevice::ReadOnly)) {
        ui.statusbar->showMessage(QStringLiteral("Error: source file not valid."));
        return;
    }

    auto uploadFile = KGAPI2::Drive::FilePtr::create();
    QFileInfo fileInfo(m_uploadingFile->fileName());
    uploadFile->setTitle(fileInfo.fileName());

    KGAPI2::Drive::FileResumableCreateJob *fileCreateJob = nullptr;
    if (ui.writeOnSignalRadioButton->isChecked()) {
        fileCreateJob = new KGAPI2::Drive::FileResumableCreateJob(uploadFile, m_account, this);
        connect(fileCreateJob, &KGAPI2::Drive::FileResumableCreateJob::readyWrite, this, &MainWindow::slotFileCreateJobReadyWrite);
    } else {
        fileCreateJob = new KGAPI2::Drive::FileResumableCreateJob(m_uploadingFile.get(), uploadFile, m_account, this);
    }
    fileCreateJob->setUploadSize(m_uploadingFile->size());
    connect(fileCreateJob, &KGAPI2::Drive::FileResumableCreateJob::finished, this, &MainWindow::slotFileCreateJobFinished);
    connect(fileCreateJob, &KGAPI2::Drive::FileResumableCreateJob::progress, this, &MainWindow::slotFileCreateJobProgress);

    ui.statusbar->clearMessage();
    m_bytesUploaded = 0;
    if (ui.writeOnSignalRadioButton->isChecked()) {
        m_fileUploadProgressBar = std::make_unique<QProgressBar>(ui.statusbar);
        m_fileUploadProgressBar->setFormat(QStringLiteral("Sent to Job: %v bytes [%p%]"));
        m_fileUploadProgressBar->setMaximum(m_uploadingFile->size());
        ui.statusbar->addWidget(m_fileUploadProgressBar.get());
    }

    m_jobUploadProgressBar = std::make_unique<QProgressBar>(ui.statusbar);
    m_jobUploadProgressBar->setFormat(QStringLiteral("Job progress: %v bytes [%p%]"));
    ui.statusbar->addWidget(m_jobUploadProgressBar.get());

    setInputsEnabled(false);
}

void MainWindow::slotFileCreateJobFinished(KGAPI2::Job *job)
{
    qDebug() << "Completed job" << job << "error code:" << job->error() << "- message:" << job->errorString();

    auto fileCreateJob = qobject_cast<KGAPI2::Drive::FileResumableCreateJob *>(job);
    Q_ASSERT(fileCreateJob);
    fileCreateJob->deleteLater();

    if (fileCreateJob->error() != KGAPI2::NoError) {
        ui.statusbar->showMessage(QStringLiteral("Error: %1").arg(fileCreateJob->errorString()));
    } else {
        auto file = fileCreateJob->metadata();
        ui.statusbar->showMessage(QStringLiteral("Upload complete, id %1, size %2 (uploaded %3), mimeType %4")
                                      .arg(file->id())
                                      .arg(file->fileSize())
                                      .arg(m_bytesUploaded)
                                      .arg(file->mimeType()));
    }

    m_fileUploadProgressBar.reset();
    m_jobUploadProgressBar.reset();

    setInputsEnabled(true);
}

void MainWindow::slotFileCreateJobReadyWrite(KGAPI2::Drive::FileAbstractResumableJob *job)
{
    const auto data = m_uploadingFile->read(50000);
    m_bytesUploaded += data.size();
    job->write(data);

    m_fileUploadProgressBar->setValue(m_bytesUploaded);
}

void MainWindow::slotFileCreateJobProgress(KGAPI2::Job *job, int base, int total)
{
    Q_UNUSED(job)

    m_jobUploadProgressBar->setValue(base);
    m_jobUploadProgressBar->setMaximum(total);
}

void MainWindow::setInputsEnabled(bool enabled)
{
    ui.sourceLineEdit->setEnabled(enabled);
    ui.browseButton->setEnabled(enabled);
    ui.writeOnSignalRadioButton->setEnabled(enabled);
    ui.passFileToJobRadioButton->setEnabled(enabled);
    ui.uploadButton->setEnabled(enabled);
}

#include "moc_mainwindow.cpp"
