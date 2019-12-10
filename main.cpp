/*
 * Radiator
 * Copyright (C) 2018  Stefan Böhmann
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <QGuiApplication>
#include <QTranslator>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QCommandLineParser>
#include <QFontDatabase>
#include <QJsonDocument>
#include <QTranslator>
#include <QLocale>
#include <QLibraryInfo>
#include <QIcon>
#include <QDir>
#include <QTimer>

#ifdef Q_OS_WIN
#include <QQuickStyle>
#endif

#include <iostream>

#include "Config/config.h"
#include "modelaccessor.h"
#include "buildmodel.h"
#include "branchfiltermodel.h"
#include "Config/configremotemodel.h"

QList<QTranslator*> translators()
{
    const QString lc = QLocale().name();

    QList<QTranslator*> list;
    auto loadTr = [&](const QString &l, const QString &p) {
         QTranslator *qtl = new QTranslator(qApp);
         if(qtl->load(l, p)) {
             //qDebug().noquote() << QString("Translation '%1' loaded from path '%2'.").arg(l, p);
             list.append(qtl);
         }
         else {
             delete qtl;
             //qDebug().noquote() << QString("Could not find translation '%1' in path '%2'.").arg(l, p);
         }
    };

    loadTr(QStringLiteral("qt_en"), QLibraryInfo::location(QLibraryInfo::TranslationsPath));
    loadTr(QStringLiteral("qt_") + lc, QLibraryInfo::location(QLibraryInfo::TranslationsPath));

    loadTr(QStringLiteral("radiator_en"), QStringLiteral(":/l10n"));
    loadTr(QStringLiteral("radiator_") + lc, QStringLiteral(":/l10n"));

    QString app_tr = qApp->applicationDirPath() + QStringLiteral("/l10n");
    if(QDir(app_tr).exists()) {
        loadTr(QStringLiteral("radiator_en"), app_tr);
        loadTr(QStringLiteral("radiator_") + lc, app_tr);
    }

    return list;
}

//class MouseCursorFilter : public QObject
//{
//    Q_OBJECT

//public:
//    explicit MouseCursorFilter(QObject *parent = nullptr)
//      : QObject(parent)
//    {
//        m_timer.setInterval(5000);
//        connect(&m_timer, &QTimer::timeout, []() {
//            qApp->setOverrideCursor(Qt::BlankCursor);
//        });

//        m_timer.start();
//    }

//protected:
//    bool eventFilter(QObject *obj, QEvent *event)
//    {
//        Q_UNUSED(obj);
//        if(event->type() == QEvent::MouseMove
//                || event->type() == QEvent::MouseButtonPress
//                || event->type() == QEvent::MouseButtonRelease)
//        {
//            qApp->restoreOverrideCursor();
//            m_timer.start();
//        }

//        return false;
//    }

//private:
//    QTimer m_timer;
//};

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QCoreApplication::setApplicationName(QStringLiteral("Radiator"));
    QCoreApplication::setApplicationVersion(QStringLiteral("0.0.1"));

    QGuiApplication app(argc, argv);
    app.setWindowIcon(QIcon(":/assets/radiator.svg"));

//    app.installEventFilter(new MouseCursorFilter(&app));

    QList<QTranslator*> trl = translators();
    for(int i = 0; i < trl.size(); ++i) {
        app.installTranslator(trl[i]);
    }

#ifdef Q_OS_WIN
    QQuickStyle::setStyle(QStringLiteral("Material"));
#endif

    QCommandLineParser parser;
    parser.setApplicationDescription(QStringLiteral("Radiator"));
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption dumpConfigOption(QStringList() << QStringLiteral("dump-config"), QCoreApplication::translate("main", "Dump configuration options to stdout and exit."));
    parser.addOption(dumpConfigOption);

    QCommandLineOption configOption(QStringList() << "c" << "config",
            QCoreApplication::translate("main", "Read configuration file <config>."),
            QCoreApplication::translate("main", "config"));
    parser.addOption(configOption);

    QCommandLineOption radiatorOption(QStringList() << QStringLiteral("r") << QStringLiteral("radiator"), QCoreApplication::translate("main", "Launch radiator"));
    parser.addOption(radiatorOption);

    parser.process(app);

    auto cfg = Config::instance();
    if(parser.isSet(configOption)) {
        if(!cfg->readFile(parser.value(configOption))) {
            std::cerr << "Failed to read configuration file '" << parser.value(configOption).toStdString() << "'" << std::endl;
            return -1;
        }
    }
    else {
        cfg->readFile();
    }

    if(parser.isSet(dumpConfigOption)) {
        std::cout << QJsonDocument(cfg->toJson()).toJson().toStdString() << std::endl;
        return 0;
    }

    qmlRegisterUncreatableType<Build>("Radiator", 1, 0, "Build", "");

    qmlRegisterType<ModelAccessor>("Radiator", 1, 0, "ModelAccessor");
    qmlRegisterType<BuildModel>("Radiator", 1, 0, "BuildModel");
    qmlRegisterType<BranchFilterModel>("Radiator", 1, 0, "BranchFilterModel");

    qmlRegisterType<Config>("Radiator.Config", 1, 0, "Config");
    qmlRegisterType<ConfigRemoteModel>("Radiator.Config", 1, 0, "RemoteModel");

    qmlRegisterAnonymousType<ConfigRadiator>("Radiator", 1);
    qmlRegisterAnonymousType<ConfigRadiatorHeader>("Radiator", 1);
    qmlRegisterAnonymousType<ConfigProxy>("Radiator", 1);
    qmlRegisterAnonymousType<BranchFilter>("Radiator", 1);
    qmlRegisterAnonymousType<ConfigRemote>("Radiator", 1);
    qmlRegisterAnonymousType<ConfigUser>("Radiator", 1);

    QQmlApplicationEngine engine;
    QObject::connect(&engine, &QQmlApplicationEngine::quit, &app, &QCoreApplication::quit);

    engine.rootContext()->setContextProperty("_config", Config::instance());

    engine.load(QUrl(QLatin1String("qrc:/Radiator.qml")));
    if (engine.rootObjects().isEmpty()) {
        return -1;
    }

    return app.exec();
}
