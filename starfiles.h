#ifndef STARFILES_H
#define STARFILES_H

#include <QMainWindow>
#include <QMessageBox>

#include <QFile>
#include <QFileInfo>
#include <QFileDialog>
#include <QDir>

#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QHttpPart>
#include <QUrlQuery>
#include <QUrl>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include <QSqlDatabase>
#include <QSqlDriver>
#include <QSqlQuery>
#include <QSqlRecord>

#include "uploaded.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Starfiles; }
QT_END_NAMESPACE

class Starfiles : public QMainWindow
{
    Q_OBJECT

public:
    Starfiles(QWidget *parent = nullptr);
    ~Starfiles();

private slots:
	// UI SLOTS :
    void on_BtnUpload_clicked();
	void on_BtnDeleteDatabase_clicked();
	void on_BtnCancelSettings_clicked();
	void on_BtnSaveSettings_clicked();

	// UPLOAD SLOTS :
	void UploadProgress(qint64 received, qint64 total);
	void UploadFinished(QNetworkReply *reply);

	// PK VALIDATE :
	void PKValidateFinished(QNetworkReply *reply);

private:
    Ui::Starfiles *ui;
	QString pk_token;
	bool Save2db;
	QSqlDatabase db;
    bool IsOnline();
    QString FileBaseName = nullptr;
	void ClearFilesList();
	void AppendToFilesList(QString FileID, QString FileName, QString DownloadLink);
	void LoadFilesFromDB(bool clear);
	void SaveSettings();
	void LoadSettings();
	void DefaultSettings();
	Uploaded Upl;
};
#endif // STARFILES_H
