#ifndef UPLOADED_H
#define UPLOADED_H

#include <QDialog>

namespace Ui {
	class Uploaded;
}

class Uploaded : public QDialog
{
		Q_OBJECT

	public:
		explicit Uploaded(QWidget *parent = nullptr);
		~Uploaded();
		void Init(QString FID, QString FN, QString DL);

	private slots:
		void on_BtnCopyFID_clicked();

		void on_BtnCopyFN_clicked();

		void on_BtnCopyDL_clicked();

		void on_BtnOk_clicked();

	private:
		Ui::Uploaded *ui;
};

#endif // UPLOADED_H
