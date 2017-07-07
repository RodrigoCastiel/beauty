#pragma once

#include <QWidget>
#include "ui_BRenderWidget.h"

#include "BModel/BModel.h"

class BRenderWidget : public QWidget
{
    Q_OBJECT

public:
    BRenderWidget(QWidget *parent = Q_NULLPTR);
    ~BRenderWidget();

public slots:
    // Settings file. 
    void OnSaveSettings();    // Push-button or action menu.
    void OnLoadSettings();    // Push-button or action menu.
    void OnSaveSettingsAs();  // Push-button or action menu.

    // Render, Output Image and Scene file.
    void OnSceneFileBrowse();
    void OnOutputImgBrowse();
    void OnRender();
    void OnPreview();
 
private:
    Ui::BRenderWidget ui;

    // Internal model (MVC design pattern).
    BModel mModel;

    bool mSettingsFileLoaded;

};
