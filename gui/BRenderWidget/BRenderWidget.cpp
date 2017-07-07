#include "BRenderWidget.h"

#include <QFileDialog>
#include <QMessageBox>

BRenderWidget::BRenderWidget(QWidget *parent)
    : QWidget(parent)
{
    ui.setupUi(this);

    mSettingsFileLoaded = false;
    
    QString styleSheet = "background-color: #FFFFFF; color: #000000";
    ui.lineEdit_imgWidth->setStyleSheet(styleSheet);
    ui.lineEdit_imgWidth->setStyleSheet(styleSheet);
    ui.lineEdit_imgHeight->setStyleSheet(styleSheet);
    ui.lineEdit_recursionDepth->setStyleSheet(styleSheet);
    ui.lineEdit_sceneFile->setStyleSheet(styleSheet);
    ui.lineEdit_ConfigFile->setStyleSheet(styleSheet);
    ui.lineEdit_outputImg->setStyleSheet(styleSheet);

    // Setup SIGNAL-SLOT connections.

    // Settings File (push-buttons).
    connect(ui.pb_saveConfigFile, SIGNAL(released()), this, SLOT(OnSaveSettings()));
    connect(ui.pb_loadConfigFile, SIGNAL(released()), this, SLOT(OnLoadSettings()));
    connect(ui.pb_saveConfigFileAs, SIGNAL(released()), this, SLOT(OnSaveSettingsAs()));

    // Render, Output Image and Scene file (push-buttons).
    connect(ui.pb_browseSceneFile, SIGNAL(released()), this, SLOT(OnSceneFileBrowse()));
    connect(ui.pb_browseOutputImg, SIGNAL(released()), this, SLOT(OnOutputImgBrowse()));
    connect(ui.pb_render, SIGNAL(released()), this, SLOT(OnRender()));
    connect(ui.pb_preview, SIGNAL(released()), this, SLOT(OnPreview()));
}

BRenderWidget::~BRenderWidget()
{

}

// === Settings File ====================================================================

void BRenderWidget::OnSaveSettings()
{
    if (!mSettingsFileLoaded)  // No file was loaded - ask a new one.
    {
        QString filePath = QFileDialog::getSaveFileName(this, tr("Save .config file"), 
                                                        "", tr("CONFIG (*.config);"));

        if (filePath.isEmpty())
            return;
        
        // Update line edit.
        ui.lineEdit_ConfigFile->setText(filePath);
        mSettingsFileLoaded = true;
    }

    QString filePath = ui.lineEdit_ConfigFile->text();
    renderer::RenderingConfig config;

    // Extract data from UI form.
    config.SetImgWidth(ui.lineEdit_imgWidth->text().toInt());
    config.SetImgHeight(ui.lineEdit_imgHeight->text().toInt());
    config.SetRecursionDepth(ui.lineEdit_recursionDepth->text().toInt());
    config.SetAntiAliasingOn(ui.checkBox_antiAliasing->isChecked());

    // Write into file.
    bool success = config.SaveIntoFile(filePath.toStdString());
    if (!success)
    {
        QMessageBox::information(this, "Warning", 
            "Could not save file at '" + filePath + "'!", QMessageBox::Ok);
        return;
    }
}

void BRenderWidget::OnLoadSettings()
{
    // Request existing file path.
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open .config file"), 
                                                    "", tr("CONFIG (*.config);"));
    if (filePath.isEmpty())  // Canceled.
        return;

    // Load rendering config.
    renderer::RenderingConfig config;
    bool success = config.LoadFromFile(filePath.toStdString());

    if (!success)
    {
        QMessageBox::information(this, "Warning", 
            "The file at '" + filePath + "' could not be read!", QMessageBox::Ok);
        return;
    }

    // Set internal rendering config.
    mModel.SetRenderingConfig(config);

    // Finally, update UI.
    ui.lineEdit_imgWidth->setText(QString::number(config.GetImgWidth()));
    ui.lineEdit_imgHeight->setText(QString::number(config.GetImgHeight()));
    ui.lineEdit_recursionDepth->setText(QString::number(config.GetRecursionDepth()));
    bool anti_aliasing = config.AntiAliasingOn();
    ui.checkBox_antiAliasing->setCheckState(anti_aliasing ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);

    ui.lineEdit_ConfigFile->setText(filePath);
    mSettingsFileLoaded = true;
}

void BRenderWidget::OnSaveSettingsAs()
{
    // Always ask a new file path.
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save .config file as"), 
                                                        "", tr("CONFIG (*.config);"));
    if (filePath.isEmpty())
        return;
        
    // Update line edit if and only if it is not empty.
    ui.lineEdit_ConfigFile->setText(filePath);
    mSettingsFileLoaded = true;

    // Extract data from UI form.
    renderer::RenderingConfig config;
    config.SetImgWidth(ui.lineEdit_imgWidth->text().toInt());
    config.SetImgHeight(ui.lineEdit_imgHeight->text().toInt());
    config.SetRecursionDepth(ui.lineEdit_recursionDepth->text().toInt());
    config.SetAntiAliasingOn(ui.checkBox_antiAliasing->isChecked());

    // Write into file.
    bool success = config.SaveIntoFile(filePath.toStdString());
    if (!success)
    {
        QMessageBox::information(this, "Warning", 
            "Could not save file at '" + filePath + "'!", QMessageBox::Ok);
        return;
    }
}

// === Render, Output Image and Scene file. =============================================

void BRenderWidget::OnSceneFileBrowse()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open .scene file"), "", tr("SCENE (*.scene);"));

    if (!filePath.isEmpty())
        ui.lineEdit_sceneFile->setText(filePath);
}

void BRenderWidget::OnOutputImgBrowse()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Render image file"), "", tr("JPEG (*.jpg); PNG (*.png)"));
    
    if (!filePath.isEmpty())
        ui.lineEdit_outputImg->setText(filePath);
}

void BRenderWidget::OnRender()
{
    mModel.Render();
}

void BRenderWidget::OnPreview()
{
    mModel.Preview();
}