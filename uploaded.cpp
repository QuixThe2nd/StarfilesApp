#include "uploaded.h"
#include "ui_uploaded.h"

Uploaded::Uploaded(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::Uploaded)
{
	ui->setupUi(this);
}

Uploaded::~Uploaded()
{
	delete ui;
}

void Uploaded::Init(QString FID, QString FN, QString DL)
{
	ui->FileID->setText(FID);
	ui->FileName->setText(FN);
	ui->DownloadLink->setText(DL);
}

void Uploaded::on_BtnCopyFID_clicked()
{
	ui->FileID->selectAll();
	ui->FileID->copy();
	ui->FileID->deselect();
}

void Uploaded::on_BtnCopyFN_clicked()
{
	ui->FileName->selectAll();
	ui->FileName->copy();
	ui->FileName->deselect();
}

void Uploaded::on_BtnCopyDL_clicked()
{
	ui->DownloadLink->selectAll();
	ui->DownloadLink->copy();
	ui->DownloadLink->deselect();
}

void Uploaded::on_BtnOk_clicked()
{
	this->close();
}

