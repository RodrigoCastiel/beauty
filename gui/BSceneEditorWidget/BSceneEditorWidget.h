#pragma once

#include <QWidget>
#include "ui_BSceneEditorWidget.h"

class BSceneEditorWidget : public QWidget
{
	Q_OBJECT

public:
	BSceneEditorWidget(QWidget *parent = Q_NULLPTR);
	~BSceneEditorWidget();

public slots:

	// On text edit changed.
	void OnTextEdited();

	// Scene file methods.
	void OnNewSceneFile();
	void OnOpenSceneFile();
	void OnSaveSceneFile(bool forceNewName = false);
	void OnSaveSceneAs();

private:
	void UpdateStatusLabel();

	Ui::BSceneEditorWidget ui;

	QString mCurrentFilePath;
	bool mDoesFileExist;
	bool mIsFileModified;
};
