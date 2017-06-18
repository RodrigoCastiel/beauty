#include "BSceneEditorWidget.h"

#include <QFileInfo>
#include <QFileDialog>
#include <QTextStream>
#include <QMessageBox>

BSceneEditorWidget::BSceneEditorWidget(QWidget *parent)
	: QWidget(parent)
{
	ui.setupUi(this);

	QString styleSheet = "background-color: #222222; color: #EEEEEE";
	ui.textEdit_Scene->setStyleSheet(styleSheet);
	ui.statusLabel->setStyleSheet(styleSheet);

	// SIGNAL-SLOT connections.
	connect(ui.textEdit_Scene, SIGNAL(textChanged()), this, SLOT(OnTextEdited()));

	mDoesFileExist = false;
	mIsFileModified = false;
}

BSceneEditorWidget::~BSceneEditorWidget()
{

}

void BSceneEditorWidget::UpdateStatusLabel()
{
	if (mIsFileModified)
		ui.statusLabel->setStyleSheet("background-color: #222222; color: #FF2222;");
	else
		ui.statusLabel->setStyleSheet("background-color: #222222; color: #EEEEEE;");

	if (mDoesFileExist)
		ui.statusLabel->setText("File open at '" + mCurrentFilePath + "'");
	else
		ui.statusLabel->setText("New scene");
}

void BSceneEditorWidget::OnTextEdited()
{
	mIsFileModified = true;
	this->UpdateStatusLabel();
}

void BSceneEditorWidget::OnNewSceneFile()
{
	if (!mIsFileModified)
	{
		ui.textEdit_Scene->setPlainText("");

		mIsFileModified = false;
		mDoesFileExist = false;
	}
	else
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, "Current Scene not saved", "Do you want to save it?",
			QMessageBox::Yes | QMessageBox::Cancel | QMessageBox::No );
		if (reply == QMessageBox::Cancel)
			return;

		else 
		{
			if (reply == QMessageBox::Yes)
			{
				this->OnSaveSceneFile();
			}

			ui.textEdit_Scene->setPlainText("");
			mIsFileModified = false;
			mDoesFileExist = false;
		}
	}

	this->UpdateStatusLabel();
}

void BSceneEditorWidget::OnOpenSceneFile()
{
	if (mIsFileModified)
	{
		QMessageBox::StandardButton reply;
		reply = QMessageBox::question(this, "Current Scene not saved", "Do you want to save it?",
									  QMessageBox::Yes | QMessageBox::Cancel | QMessageBox::No );
		if (reply == QMessageBox::Cancel)
			return;

		else 
		{
			if (reply == QMessageBox::Yes)
				this->OnSaveSceneFile();
			
			QString filePath = QFileDialog::getOpenFileName(this, tr("Open .scene file"), "", tr("SCENE (*.scene);"));

			if (filePath.isEmpty())
				return;

			QFile file( filePath );
			if ( !file.open(QFile::ReadOnly) )
			{
				QMessageBox::information(this, "Warning", 
					"The file at '" + filePath + "' could not be open!", QMessageBox::Ok);
				return;
			}

			QTextStream stream( &file );
			QString content = stream.readAll();
			ui.textEdit_Scene->setPlainText(content);

			mIsFileModified = false;
			mDoesFileExist = true;
			mCurrentFilePath = filePath;
		}
	}
	else
	{
		QString filePath = QFileDialog::getOpenFileName(this, tr("Open .scene file"), "", tr("SCENE (*.scene);"));

		if (filePath.isEmpty())
			return;

		QFile file( filePath );
		if ( !file.open(QFile::ReadOnly) )
		{
			QMessageBox::information(this, "Warning", 
				"The file at '" + filePath + "' could not be open!", QMessageBox::Ok);
			return;
		}

		QTextStream stream( &file );
		QString content = stream.readAll();
		ui.textEdit_Scene->setPlainText(content);

		mIsFileModified = false;
		mDoesFileExist = true;
		mCurrentFilePath = filePath;
	}

	this->UpdateStatusLabel();
}

void BSceneEditorWidget::OnSaveSceneFile(bool forceNewName)
{
	if (mDoesFileExist && !forceNewName)
	{
		QFile file( mCurrentFilePath );
		if ( !file.open(QFile::WriteOnly) )
		{
			QMessageBox::information(this, "Warning", 
				"The file at '" + mCurrentFilePath + "' could not be written!", QMessageBox::Ok);
			return;
		}

		QTextStream stream( &file );
		stream << ui.textEdit_Scene->toPlainText();
		mIsFileModified = false;
	}
	else
	{
		QString filePath = QFileDialog::getSaveFileName(this, tr("Save .scene file"), "", tr("SCENE (*.scene);"));

		if (filePath.isEmpty())
			return;

		QFile file( filePath );
		if ( !file.open(QFile::WriteOnly) )
		{
			QMessageBox::information(this, "Warning", 
				"The file at '" + mCurrentFilePath + "' could not be written!", QMessageBox::Ok);
			return;
		}
		
		QTextStream stream( &file );
		stream << ui.textEdit_Scene->toPlainText();

		mDoesFileExist = true;
		mIsFileModified = false;
		mCurrentFilePath = filePath;
	}

	this->UpdateStatusLabel();
}

void BSceneEditorWidget::OnSaveSceneAs()
{
	this->OnSaveSceneFile(true);
}