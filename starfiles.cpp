#include "starfiles.h"
#include "ui_starfiles.h"

Starfiles::Starfiles(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::Starfiles)
{
	ui->setupUi(this);
	ui->progressBar->setHidden(true);
	ui->BtnCancelUpload->setHidden(true);
	if(!QDir(QDir::homePath() + "/AppData/Local/StarfilesApp").exists())
		QDir().mkdir(QDir::homePath() + "/AppData/Local/StarfilesApp");
	if(!QFile::exists(QDir::homePath() + "/AppData/Local/StarfilesApp/logo.png"))
		QFile::copy(":/images/images/logo/128x128.png", QDir::homePath() + "/AppData/Local/StarfilesApp/logo.png");
	QString DatabasePath = QDir::homePath() + "/AppData/Local/StarfilesApp/Database.db";
	if(!QFile::exists(DatabasePath)){
		QFile::copy(":/Database/common/Database.db", DatabasePath);
		QFile(DatabasePath).setPermissions(QFileDevice::ReadOwner | QFileDevice::WriteOwner);
	}
	db.setDatabaseName(DatabasePath);
	bool ok = db.open();
	if(!ok){
		QMessageBox messageBox;
		messageBox.setIcon(QMessageBox::Critical);
		messageBox.setWindowIcon(QIcon(":/images/images/logo/favicon.ico"));
		messageBox.setWindowTitle("Starfiles");
		messageBox.setText("the connect to the database are refused, please contact the developers to fix this problem as soon as possible.");
		messageBox.setDetailedText("E-mail: anas.y.bal@outlook.com");
		messageBox.exec();
		exit(1);
	}
	WinToast::instance()->setAppName(L"Starfiles");
	WinToast::instance()->setAppUserModelId(WinToast::configureAUMI(L"Anas Yousef", L"Starfiles", L"App", L"1.0.0"));
	WinToast::instance()->initialize();
	LoadSettings();
	DefaultSettings();
	if(List2Show == "local"){
		LoadFilesFromDB(false);
	}else{
		if(IsOnline()){
			LoadFilesFromAPI(false);
		}
	}
}

Starfiles::~Starfiles()
{
	delete ui;
}

void Starfiles::AppendToFilesList(QString FileID, QString FileName, QString DownloadLink){
	ui->FileList->insertRow(ui->FileList->rowCount());
	ui->FileList->setItem(ui->FileList->rowCount()-1, 0, new QTableWidgetItem(FileID));
	ui->FileList->setItem(ui->FileList->rowCount()-1, 1, new QTableWidgetItem(FileName));
	ui->FileList->setItem(ui->FileList->rowCount()-1, 2, new QTableWidgetItem(DownloadLink));
}

void Starfiles::LoadFilesFromDB(bool clear){
	if(clear){
		while(ui->FileList->rowCount() > 0)
			ui->FileList->removeRow(0);
	}
	QSqlQuery query;
	query.exec("SELECT * FROM files;");
	while(query.next()){
		QString FileID, FileName, DownloadLink;
		FileID = query.value(query.record().indexOf("File_ID")).toString();
		FileName = query.value(query.record().indexOf("File_Name")).toString();
		DownloadLink = query.value(query.record().indexOf("Download_Link")).toString();
		AppendToFilesList(FileID, FileName, DownloadLink);
	}
}

void Starfiles::LoadFilesFromAPI(bool clear){
	if(clear){
		while(ui->FileList->rowCount() > 0)
			ui->FileList->removeRow(0);
	}
	QUrl url("https://api.starfiles.co/user/files");
	QUrlQuery parameters;
	parameters.addQueryItem("profile", pk_token);
	url.setQuery(parameters);
	QNetworkRequest request(url);
	QNetworkAccessManager *manager = new QNetworkAccessManager(this);
	QNetworkReply *reply = manager->get(request);
	connect(manager, &QNetworkAccessManager::finished, this, &Starfiles::LoadFilesFromAPIFinished);
	Q_UNUSED(reply);
}

void Starfiles::LoadFilesFromAPIFinished(QNetworkReply *reply)
{
	QByteArray response = reply->readAll();
	QJsonDocument doc = QJsonDocument::fromJson(response);
	QJsonArray Json = doc.toVariant().toJsonArray();
	if(Json.size() > 0){
		for(int index=0; index < Json.size(); index++){
			QJsonObject CurrentObject = Json[index].toObject();
			QString FileID, DownloadLink, FileName;
			FileID = CurrentObject.value("id").toString();
			FileName = CurrentObject.value("name").toString();
			DownloadLink = "https://starfiles.co/file/" + FileID;
			AppendToFilesList(FileID, FileName, DownloadLink);
		}
	}
}

void Starfiles::SaveSettings()
{
	if(ui->RadUserFiles->isChecked() && ui->TextPk->text() == nullptr){
		QMessageBox messageBox;
		messageBox.setIcon(QMessageBox::Information);
		messageBox.setWindowIcon(QIcon(":/images/images/logo/favicon.ico"));
		messageBox.setWindowTitle("Starfiles");
		messageBox.setText("to list all your files on starfiles you need a private key, insert one and try again.");
		messageBox.exec();
		return;
	}
	if(ui->TextPk->text() != pk_token && ui->TextPk->text() != nullptr){
		if(IsOnline()){
			ui->TextPk->setDisabled(true);
			ui->RadLocalFiles->setDisabled(true);
			ui->RadUserFiles->setDisabled(true);
			ui->ChkSave2db->setDisabled(true);
			ui->BtnCancelSettings->setDisabled(true);
			ui->BtnSaveSettings->setDisabled(true);
			QUrl url("https://api.starfiles.co/user/validate_private_key");
			QUrlQuery parameters;
			parameters.addQueryItem("private_key", ui->TextPk->text());
			url.setQuery(parameters);
			QNetworkRequest request(url);
			QNetworkAccessManager *manager = new QNetworkAccessManager(this);
			QNetworkReply *reply = manager->get(request);
			connect(manager, &QNetworkAccessManager::finished, this, &Starfiles::PKValidateFinished);
			Q_UNUSED(reply);
		}else{
			QMessageBox messageBox;
			messageBox.setIcon(QMessageBox::Information);
			messageBox.setWindowIcon(QIcon(":/images/images/logo/favicon.ico"));
			messageBox.setWindowTitle("Starfiles");
			messageBox.setText("Please check your network connection, and try again.");
			messageBox.exec();
		}
	}else{
		QSqlQuery query;
		query.prepare("UPDATE settings SET pk_token = :pk, files_area = :fa, save_to_db = :std WHERE 1 = 1;");
		query.bindValue(":pk", (ui->TextPk->text() == nullptr)? "NULL" : ui->TextPk->text());
		query.bindValue(":fa", (ui->RadUserFiles->isChecked() == true)? "api" : "local");
		query.bindValue(":std", ui->ChkSave2db->isChecked());
		query.exec();
		LoadSettings();
		DefaultSettings();
	}
}

void Starfiles::LoadSettings()
{
	QSqlQuery query;
	query.exec("SELECT * FROM settings;");
	if(query.next()){
		QString pk = query.value(query.record().indexOf("pk_token")).toString();
		pk_token = (pk == "NULL")? nullptr : pk;
		List2Show = query.value(query.record().indexOf("files_area")).toString();
		Save2db = query.value(query.record().indexOf("save_to_db")).toBool();
	}
}

void Starfiles::DefaultSettings()
{
	ui->TextPk->setText(pk_token);
	ui->RadLocalFiles->setChecked((List2Show == "local"));
	ui->RadUserFiles->setChecked((List2Show == "api"));
	ui->ChkSave2db->setChecked(Save2db);
}

bool Starfiles::CreateToast(QString Title, QString Message){
	WinToastTemplate notify = WinToastTemplate(WinToastTemplate::ImageAndText02);
	notify.setTextField(Title.toStdWString(), WinToastTemplate::FirstLine);
	notify.setTextField(Message.toStdWString(), WinToastTemplate::SecondLine);
	notify.setAudioPath(static_cast<WinToastTemplate::AudioSystemFile>(WinToastTemplate::AudioSystemFile::SMS));
	notify.setAudioOption(static_cast<WinToastTemplate::AudioOption>(WinToastTemplate::AudioOption::Default));
	notify.setImagePath(QString(QDir::homePath() + "/AppData/Local/StarfilesApp/logo.png").toStdWString());
	if (WinToast::instance()->showToast(notify, new NotifyHandler())) {
		return false;
	}else{
		return true;
	}
}

bool Starfiles::IsOnline(){
	QTcpSocket* sock = new QTcpSocket(this);
	sock->connectToHost("starfiles.co", 80);
	bool connected = sock->waitForConnected(30000);
	if (!connected){
		sock->abort();
		return false;
	}
	sock->close();
	return true;
}

void Starfiles::on_BtnUpload_clicked()
{
	QString FilePath = QFileDialog().getOpenFileName(this, "Select file to upload", QDir::homePath(), "All files (*.*)|*.*");
	if(FilePath != NULL){
		if((QFileInfo(FilePath).size()/1024000)>=100){
			QMessageBox messageBox;
			messageBox.setIcon(QMessageBox::Information);
			messageBox.setWindowIcon(QIcon(":/images/images/logo/favicon.ico"));
			messageBox.setWindowTitle("Starfiles");
			messageBox.setText("Max file size 100 Mb");
			messageBox.exec();
			return;
		}
		if(IsOnline()){
			FileBaseName = QFileInfo(FilePath).fileName();
			ui->BtnUpload->setDisabled(true);
			ui->BtnUpload->setText("Uploading...");
			ui->BtnCancelUpload->setHidden(false);
			ui->progressBar->setHidden(false);
			QHttpMultiPart *multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);
			QHttpPart FiletoUpload;
			FiletoUpload.setHeader(QNetworkRequest::ContentDispositionHeader, QVariant("form-data; name=\"upload\"; filename=\"" + FileBaseName + "\""));
			QFile *file = new QFile(FilePath);
			file->open(QIODevice::ReadOnly);
			FiletoUpload.setBodyDevice(file);
			file->setParent(multiPart);
			multiPart->append(FiletoUpload);
			QUrl url("https://api.starfiles.co/upload/upload_file");
			if(pk_token != nullptr){
				QUrlQuery parameters;
				parameters.addQueryItem("folder", "");
				parameters.addQueryItem("profile", pk_token);
				url.setQuery(parameters);
			}
			QNetworkRequest request(url);
			QNetworkAccessManager *manager = new QNetworkAccessManager(this);
			QNetworkReply *reply = manager->post(request, multiPart);
			connect(manager, &QNetworkAccessManager::finished, this, &Starfiles::finished);
			connect(ui->BtnCancelUpload, &QPushButton::clicked, reply, &QNetworkReply::abort);
			connect(reply, &QNetworkReply::uploadProgress, this, &Starfiles::uploadProgress);
			multiPart->setParent(reply);
		}else{
			QMessageBox messageBox;
			messageBox.setIcon(QMessageBox::Warning);
			messageBox.setWindowIcon(QIcon(":/images/images/logo/favicon.ico"));
			messageBox.setWindowTitle("Starfiles");
			messageBox.setText("Please check your network connection, and try again.");
			messageBox.exec();
			return;
		}
	}
}

void Starfiles::uploadProgress(qint64 received, qint64 total){
	if(received != 0 && total != 0){
		qint64 percent;
		percent = received * 100 / total;
		ui->progressBar->setValue(percent);
	}
}

void Starfiles::finished(QNetworkReply *reply){
	ui->BtnUpload->setEnabled(true);
	ui->BtnUpload->setText("Upload file");
	ui->progressBar->setValue(0);
	ui->progressBar->setHidden(true);
	ui->BtnCancelUpload->setHidden(true);
	QByteArray response = reply->readAll();
	if(response != nullptr){
		QJsonDocument doc = QJsonDocument::fromJson(response);
		QJsonObject Json = doc.toVariant().toJsonObject();
		if(Json.value("status").toBool() == true){
			QString FileID, DownloadLink;
			FileID = Json.value("file").toString();
			DownloadLink = "https://starfiles.co/file/" + FileID;
			if(Save2db){
				QSqlQuery query;
				query.prepare("INSERT INTO files (File_ID, File_Name, Download_Link) VALUES (?, ?, ?);");
				query.addBindValue(FileID);
				query.addBindValue(FileBaseName);
				query.addBindValue(DownloadLink);
				query.exec();
			}
			CreateToast("All done", "Your file has been successfully uploaded.");
			AppendToFilesList(FileID, FileBaseName, DownloadLink);
			Upl.Init(FileID, FileBaseName, DownloadLink);
			Upl.exec();
		}else{
			QMessageBox messageBox;
			messageBox.setIcon(QMessageBox::Critical);
			messageBox.setWindowIcon(QIcon(":/images/images/logo/favicon.ico"));
			messageBox.setWindowTitle("Starfiles");
			messageBox.setText("Unknown Error while uploading your file, read the details for more information.");
			messageBox.setDetailedText(response);
			messageBox.exec();
			return;
		}
	}
	FileBaseName = nullptr;
}

void Starfiles::PKValidateFinished(QNetworkReply *reply)
{
	ui->TextPk->setDisabled(false);
	ui->RadLocalFiles->setDisabled(false);
	ui->RadUserFiles->setDisabled(false);
	ui->ChkSave2db->setDisabled(false);
	ui->BtnCancelSettings->setDisabled(false);
	ui->BtnSaveSettings->setDisabled(false);
	QByteArray response = reply->readAll();
	QJsonDocument doc = QJsonDocument::fromJson(response);
	QJsonObject Json = doc.toVariant().toJsonObject();
	if(Json.value("status").toBool()){
		QSqlQuery query;
		query.prepare("UPDATE settings SET pk_token = :pk, files_area = :fa, save_to_db = :std WHERE 1 = 1;");
		query.bindValue(":pk", (ui->TextPk->text() == nullptr)? "NULL" : ui->TextPk->text());
		query.bindValue(":fa", (ui->RadUserFiles->isChecked())? "api" : "local");
		query.bindValue(":std", ui->ChkSave2db->isChecked());
		query.exec();
		LoadSettings();
		DefaultSettings();
		CreateToast("all done", "Your private key has been changed successfully.");
	}else{
		QMessageBox messageBox;
		messageBox.setIcon(QMessageBox::Warning);
		messageBox.setWindowIcon(QIcon(":/images/images/logo/favicon.ico"));
		messageBox.setWindowTitle("Starfiles");
		messageBox.setText("this private key is invalid, please insert another one or contact with the support team for more information.");
		messageBox.setDetailedText(Json.value("message").toString());
		messageBox.exec();
	}
}

void Starfiles::on_BtnDeleteDatabase_clicked()
{
	QSqlQuery query;
	query.exec("DELETE FROM files WHERE 1 = 1;");
	if(List2Show == "local"){
		LoadFilesFromDB(true);
	}
}

void Starfiles::on_BtnCancelSettings_clicked()
{
	DefaultSettings();
}

void Starfiles::on_BtnSaveSettings_clicked()
{
	SaveSettings();
}
