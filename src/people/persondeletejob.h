/*
 * This file is part of LibKGAPI library
 *
 * SPDX-FileCopyrightText: 2023 Claudio Cambra <claudio.cambra@kde.org>
 *
 * SPDX-License-Identifier: LGPL-2.1-only OR LGPL-3.0-only OR LicenseRef-KDE-Accepted-LGPL
 */

#pragma once

#include "deletejob.h"
#include "kgapipeople_export.h"

namespace KGAPI2::People
{

class KGAPIPEOPLE_EXPORT PersonDeleteJob : public KGAPI2::DeleteJob
{
    Q_OBJECT

public:
    explicit PersonDeleteJob(const QString &personResourceName, const AccountPtr &account, QObject* parent = nullptr);
    explicit PersonDeleteJob(const QStringList &peopleResourceName, const AccountPtr &account, QObject* parent = nullptr);
    explicit PersonDeleteJob(const PersonPtr &person, const AccountPtr &account, QObject* parent = nullptr);
    explicit PersonDeleteJob(const PersonList &people, const AccountPtr &account, QObject* parent = nullptr);
    ~PersonDeleteJob();

protected:
    void start() override;

    void handleReply(const QNetworkReply *reply,
                     const QByteArray &rawData) override;

private:
    class Private;
    std::unique_ptr<Private> d;
    friend class Private;
};

}
