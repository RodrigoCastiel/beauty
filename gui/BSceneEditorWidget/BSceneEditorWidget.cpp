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

	// SIGNAL-SLOT connections.
	connect(ui.textEdit_Scene, SIGNAL(textChanged()), this, SLOT(OnTextEdited()));

	mDoesFileExist = false;
	mIsFileModified = false;
}

BSceneEditorWidget::~BSceneEditorWidget()
{

}

void BSceneEditorWidget::OnTextEdited()
{
	mIsFileModified = true;
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
			{
				this->OnSaveSceneFile();
			}
			
			mCurrentFilePath = QFileDialog::getOpenFileName(this, tr("Open .scene file"), "", tr("SCENE (*.scene);"));

			if (mCurrentFilePath.isEmpty())
				return;

			QFile file( mCurrentFilePath );
			if ( file.open(QFile::ReadOnly) )
			{
				QTextStream stream( &file );
				QString content = stream.readAll();
				ui.textEdit_Scene->setPlainText(content);
			}

			mIsFileModified = false;
			mDoesFileExist = true;
		}
	}
	else
	{
		mCurrentFilePath = QFileDialog::getOpenFileName(this, tr("Open .scene file"), "", tr("SCENE (*.scene);"));

		if (mCurrentFilePath.isEmpty())
			return;

		QFile file( mCurrentFilePath );
		if ( file.open(QFile::ReadOnly) )
		{
			QTextStream stream( &file );
			QString content = stream.readAll();
			ui.textEdit_Scene->setPlainText(content);
		}

		mIsFileModified = false;
		mDoesFileExist = true;
	}
}

void BSceneEditorWidget::OnSaveSceneFile()
{
	if (mDoesFileExist)
	{
		QFile file( mCurrentFilePath );
		if ( file.open(QFile::WriteOnly) )
		{
			QTextStream stream( &file );
			stream << ui.textEdit_Scene->toPlainText();
		}

		mIsFileModified = false;
	}
	else
	{
		mCurrentFilePath = QFileDialog::getSaveFileName(this, tr("Save .scene file"), "", tr("SCENE (*.scene);"));

		if (mCurrentFilePath.isEmpty())
			return;

		QFile file( mCurrentFilePath );
		if ( file.open(QFile::WriteOnly) )
		{
			QTextStream stream( &file );
			stream << ui.textEdit_Scene->toPlainText();
		}

		mDoesFileExist = true;
		mIsFileModified = false;
	}
}