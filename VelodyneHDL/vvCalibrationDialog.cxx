// Copyright 2013 Velodyne Acoustics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
#include "vvCalibrationDialog.h"

#include "ui_vvCalibrationDialog.h"

#include <pqApplicationCore.h>
#include <pqSettings.h>

#include <QDialog>
#include <QFileDialog>
#include <QListWidget>
#include <QListWidgetItem>

//-----------------------------------------------------------------------------
class vvCalibrationDialog::pqInternal : public Ui::vvCalibrationDialog
{
public:
  pqInternal()
    : Settings(pqApplicationCore::instance()->settings())
  {
    const unsigned int nFile = 5; // WARNING update this accordingly.
    const char* filenames[nFile] = { "HDL-32.xml", "VLP-16.xml", "VLP-32c.xml", "Puck Hi-Res.xml",
      "Puck LITE.xml" };
    std::vector<QString> calibrationBuiltIn(filenames, filenames + nFile);
    QString prefix;
#if defined(_WIN32)
    prefix = QCoreApplication::applicationDirPath() + "/../share/";
#elif defined(__APPLE__)
    prefix = QCoreApplication::applicationDirPath() + "/../Resources/";
#else
    prefix = QCoreApplication::applicationDirPath() + "/../../share/";
#endif
    for (size_t k = 0; k < calibrationBuiltIn.size(); ++k)
    {
      calibrationBuiltIn[k] = prefix + calibrationBuiltIn[k];
      this->BuiltInCalibrationFiles << calibrationBuiltIn[k];
    }
  }

  void saveFileList();
  void saveSelectedRow();
  void restoreSelectedRow();

  void saveSensorTransform();
  void saveGpsTransform();
  void saveLidarPort();
  void saveGpsPort();
  void saveLidarForwardingPort();
  void saveGPSForwardingPort();
  void saveEnableForwarding();
  void saveAdvancedConfiguration();
  void saveForwardIpAddress();

  void restoreSensorTransform();
  void restoreGpsTransform();
  void restoreLidarPort();
  void restoreGpsPort();
  void restoreLidarForwardingPort();
  void restoreGPSForwardingPort();
  void restoreEnableForwarding();
  void restoreAdvancedConfiguration();
  void restoreForwardIpAddress();

  pqSettings* const Settings;
  QStringList BuiltInCalibrationFiles;
};

//-----------------------------------------------------------------------------
void vvCalibrationDialog::pqInternal::saveFileList()
{
  QStringList files;
  for (int i = this->BuiltInCalibrationFiles.size(); i < this->ListWidget->count(); ++i)
  {
    files << this->ListWidget->item(i)->data(Qt::UserRole).toString();
  }

  this->Settings->setValue("VelodyneHDLPlugin/CalibrationFileDialog/Files", files);
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::pqInternal::saveSelectedRow()
{
  this->Settings->setValue(
    "VelodyneHDLPlugin/CalibrationFileDialog/CurrentRow", this->ListWidget->currentRow());
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::pqInternal::restoreSelectedRow()
{
  int row = this->Settings->value("VelodyneHDLPlugin/CalibrationFileDialog/CurrentRow").toInt();
  this->ListWidget->setCurrentRow(row);
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::pqInternal::saveSensorTransform()
{
  this->Settings->setValue(
    "VelodyneHDLPlugin/CalibrationFileDialog/LidarOriginX", this->LidarXSpinBox->value());
  this->Settings->setValue(
    "VelodyneHDLPlugin/CalibrationFileDialog/lidarOriginY", this->LidarYSpinBox->value());
  this->Settings->setValue(
    "VelodyneHDLPlugin/CalibrationFileDialog/LidarOriginZ", this->LidarZSpinBox->value());
  this->Settings->setValue(
    "VelodyneHDLPlugin/CalibrationFileDialog/LidarYaw", this->LidarYawSpinBox->value());
  this->Settings->setValue(
    "VelodyneHDLPlugin/CalibrationFileDialog/LidarPitch", this->LidarPitchSpinBox->value());
  this->Settings->setValue(
    "VelodyneHDLPlugin/CalibrationFileDialog/LidarRoll", this->LidarRollSpinBox->value());
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::pqInternal::saveGpsTransform()
{
  this->Settings->setValue(
    "VelodyneHDLPlugin/CalibrationFileDialog/GpsOriginX", this->GpsXSpinBox->value());
  this->Settings->setValue(
    "VelodyneHDLPlugin/CalibrationFileDialog/GpsOriginY", this->GpsYSpinBox->value());
  this->Settings->setValue(
    "VelodyneHDLPlugin/CalibrationFileDialog/GpsOriginZ", this->GpsZSpinBox->value());
  this->Settings->setValue(
    "VelodyneHDLPlugin/CalibrationFileDialog/GpsYaw", this->GpsYawSpinBox->value());
  this->Settings->setValue(
    "VelodyneHDLPlugin/CalibrationFileDialog/GpsRoll", this->GpsRollSpinBox->value());
  this->Settings->setValue(
    "VelodyneHDLPlugin/CalibrationFileDialog/GpsPitch", this->GpsPitchSpinBox->value());
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::pqInternal::saveLidarPort()
{
  this->Settings->setValue(
    "VelodyneHDLPlugin/CalibrationFileDialog/LidarPort", this->LidarPortSpinBox->value());
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::pqInternal::saveGpsPort()
{
  this->Settings->setValue(
    "VelodyneHDLPlugin/CalibrationFileDialog/GpsPort", this->GPSPortSpinBox->value());
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::pqInternal::saveGPSForwardingPort()
{
  this->Settings->setValue("VelodyneHDLPlugin/CalibrationFileDialog/GpsForwardingPort",
    this->GPSForwardingPortSpinBox->value());
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::pqInternal::saveLidarForwardingPort()
{
  this->Settings->setValue("VelodyneHDLPlugin/CalibrationFileDialog/LidarForwardingPort",
    this->LidarForwardingPortSpinBox->value());
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::pqInternal::saveEnableForwarding()
{
  this->Settings->setValue("VelodyneHDLPlugin/CalibrationFileDialog/EnableForwarding",
    this->EnableForwardingCheckBox->isChecked());
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::pqInternal::saveAdvancedConfiguration()
{
  this->Settings->setValue("VelodyneHDLPlugin/CalibrationFileDialog/AdvancedConfiguration",
    this->AdvancedConfiguration->isChecked());
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::pqInternal::saveForwardIpAddress()
{
  this->Settings->setValue(
    "VelodyneHDLPlugin/CalibrationFileDialog/ForwardIpAddress", this->ipAddresslineEdit->text());
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::pqInternal::restoreSensorTransform()
{
  this->LidarXSpinBox->setValue(this->Settings
                                   ->value("VelodyneHDLPlugin/CalibrationFileDialog/LidarOriginX",
                                     this->LidarXSpinBox->value())
                                   .toDouble());
  this->LidarYSpinBox->setValue(this->Settings
                                   ->value("VelodyneHDLPlugin/CalibrationFileDialog/LidarOriginY",
                                     this->LidarYSpinBox->value())
                                   .toDouble());
  this->LidarZSpinBox->setValue(this->Settings
                                   ->value("VelodyneHDLPlugin/CalibrationFileDialog/LidarOriginZ",
                                     this->LidarZSpinBox->value())
                                   .toDouble());
  this->LidarYawSpinBox->setValue(
    this->Settings
      ->value("VelodyneHDLPlugin/CalibrationFileDialog/LidarYaw", this->LidarYawSpinBox->value())
      .toDouble());
  this->LidarPitchSpinBox->setValue(
    this->Settings
      ->value("VelodyneHDLPlugin/CalibrationFileDialog/LidarPitch", this->LidarPitchSpinBox->value())
      .toDouble());
  this->LidarRollSpinBox->setValue(
    this->Settings
      ->value("VelodyneHDLPlugin/CalibrationFileDialog/LidarRoll", this->LidarRollSpinBox->value())
      .toDouble());
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::pqInternal::restoreGpsTransform()
{
  this->GpsXSpinBox->setValue(this->Settings
                                   ->value("VelodyneHDLPlugin/CalibrationFileDialog/GpsOriginX",
                                     this->GpsXSpinBox->value())
                                   .toDouble());
  this->GpsYSpinBox->setValue(this->Settings
                                   ->value("VelodyneHDLPlugin/CalibrationFileDialog/GpsOriginY",
                                     this->GpsYSpinBox->value())
                                   .toDouble());
  this->GpsZSpinBox->setValue(this->Settings
                                   ->value("VelodyneHDLPlugin/CalibrationFileDialog/GpsOriginZ",
                                     this->GpsZSpinBox->value())
                                   .toDouble());

  this->GpsYawSpinBox->setValue(
    this->Settings
      ->value("VelodyneHDLPlugin/CalibrationFileDialog/GpsYaw", this->GpsYawSpinBox->value())
      .toDouble());
  this->GpsRollSpinBox->setValue(
    this->Settings
      ->value("VelodyneHDLPlugin/CalibrationFileDialog/GpsRoll", this->GpsRollSpinBox->value())
      .toDouble());
  this->GpsPitchSpinBox->setValue(
    this->Settings
      ->value("VelodyneHDLPlugin/CalibrationFileDialog/GpsPitch", this->GpsPitchSpinBox->value())
      .toDouble());
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::pqInternal::restoreLidarPort()
{
  this->LidarPortSpinBox->setValue(
    this->Settings
      ->value("VelodyneHDLPlugin/CalibrationFileDialog/LidarPort", this->LidarPortSpinBox->value())
      .toInt());
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::pqInternal::restoreGpsPort()
{
  this->GPSPortSpinBox->setValue(
    this->Settings
      ->value("VelodyneHDLPlugin/CalibrationFileDialog/GpsPort", this->GPSPortSpinBox->value())
      .toInt());
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::pqInternal::restoreGPSForwardingPort()
{
  this->GPSForwardingPortSpinBox->setValue(
    this->Settings
      ->value("VelodyneHDLPlugin/CalibrationFileDialog/GpsForwardingPort",
        this->GPSForwardingPortSpinBox->value())
      .toInt());
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::pqInternal::restoreLidarForwardingPort()
{
  this->LidarForwardingPortSpinBox->setValue(
    this->Settings
      ->value("VelodyneHDLPlugin/CalibrationFileDialog/LidarForwardingPort",
        this->LidarForwardingPortSpinBox->value())
      .toInt());
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::pqInternal::restoreEnableForwarding()
{
  bool tempIsChecked =
    this->Settings->value("VelodyneHDLPlugin/CalibrationFileDialog/EnableForwarding").toBool();
  this->EnableForwardingCheckBox->setChecked(tempIsChecked);
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::pqInternal::restoreAdvancedConfiguration()
{
  bool tempIsChecked =
    this->Settings->value("VelodyneHDLPlugin/CalibrationFileDialog/AdvancedConfiguration").toBool();
  this->AdvancedConfiguration->setChecked(tempIsChecked);
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::pqInternal::restoreForwardIpAddress()
{
  this->ipAddresslineEdit->setText(
    this->Settings->value("VelodyneHDLPlugin/CalibrationFileDialog/ForwardIpAddress").toString());
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::clearAdvancedSettings()
{
  this->setDefaultConfiguration();
}

namespace
{
QListWidgetItem* createEntry(QString path, bool useBaseName)
{
  QFileInfo info(path);
  QListWidgetItem* wi = new QListWidgetItem();
  if (useBaseName)
  {
    wi->setText(info.baseName());
  }
  else
  {
    wi->setText(info.fileName());
  }
  wi->setToolTip(path);
  wi->setData(Qt::UserRole, path);
  return wi;
}
}

//-----------------------------------------------------------------------------
vvCalibrationDialog::vvCalibrationDialog(QWidget* p)
  : QDialog(p)
  , Internal(new pqInternal)
{
  this->Internal->setupUi(this);
  this->setDefaultConfiguration();
  QListWidgetItem* liveCalibrationItem = new QListWidgetItem();

  liveCalibrationItem->setText("HDL64 Live Corrections");
  liveCalibrationItem->setToolTip("Get Corrections from the data stream");
  liveCalibrationItem->setData(Qt::UserRole, "");

  this->Internal->ListWidget->addItem(liveCalibrationItem);

  foreach (QString fullname, this->Internal->BuiltInCalibrationFiles)
  {
    this->Internal->ListWidget->addItem(createEntry(fullname, true));
  }

  foreach (QString fullname, this->calibrationFiles())
  {
    this->Internal->ListWidget->addItem(createEntry(fullname, false));
  }

  connect(this->Internal->ListWidget, SIGNAL(currentRowChanged(int)), this,
    SLOT(onCurrentRowChanged(int)));
  connect(this->Internal->AddButton, SIGNAL(clicked()), this, SLOT(addFile()));
  connect(this->Internal->RemoveButton, SIGNAL(clicked()), this, SLOT(removeSelectedFile()));
  // The advancedConfiguration checkbox hides the three followings groupbox
  connect(this->Internal->AdvancedConfiguration, SIGNAL(toggled(bool)),
    this->Internal->LidarPositionOrientationGroup, SLOT(setVisible(bool)));
  connect(this->Internal->AdvancedConfiguration, SIGNAL(toggled(bool)),
    this->Internal->GPSPositionOrientationGroup, SLOT(setVisible(bool)));
  connect(this->Internal->AdvancedConfiguration, SIGNAL(toggled(bool)),
    this->Internal->NetworkGroup, SLOT(setVisible(bool)));
  connect(this->Internal->AdvancedConfiguration, SIGNAL(toggled(bool)),
    this->Internal->NetworkForwardingGroup, SLOT(setVisible(bool)));
  connect(this->Internal->EnableForwardingCheckBox, SIGNAL(toggled(bool)),
    this->Internal->GPSForwardingPortSpinBox, SLOT(setEnabled(bool)));
  connect(this->Internal->EnableForwardingCheckBox, SIGNAL(toggled(bool)),
    this->Internal->LidarForwardingPortSpinBox, SLOT(setEnabled(bool)));
  connect(this->Internal->EnableForwardingCheckBox, SIGNAL(toggled(bool)),
    this->Internal->ipAddresslineEdit, SLOT(setEnabled(bool)));
  connect(this->Internal->ClearSettingsPushButton, SIGNAL(clicked()), this,
    SLOT(clearAdvancedSettings()));

  this->Internal->restoreSelectedRow();
  this->Internal->restoreSensorTransform();
  this->Internal->restoreGpsTransform();
  this->Internal->restoreLidarPort();
  this->Internal->restoreGpsPort();
  this->Internal->restoreEnableForwarding();
  this->Internal->restoreGPSForwardingPort();
  this->Internal->restoreLidarForwardingPort();
  this->Internal->restoreForwardIpAddress();
  this->Internal->restoreAdvancedConfiguration();

  const QVariant& geometry =
    this->Internal->Settings->value("VelodyneHDLPlugin/CalibrationFileDialog/Geometry");
  this->restoreGeometry(geometry.toByteArray());
}

//-----------------------------------------------------------------------------
vvCalibrationDialog::~vvCalibrationDialog()
{
  this->Internal->Settings->setValue(
    "VelodyneHDLPlugin/CalibrationFileDialog/Geometry", this->saveGeometry());
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::setDefaultConfiguration()
{
  const double defaultSensorValue = 0.00;
  const int minAllowedPort = 1024;   // The port between 0 and 1023 are reserved
  const int defaultLidarPort = 2368; // The port between 0 and 1023 are reserved
  const int defaultGpsPort = 8308;   // There is 16 bit to encode the ports : from 0 to 65535
  const QString defaultIpAddress = "127.0.0.1"; // Local host

  // Set the visibility
  this->Internal->LidarPositionOrientationGroup->setVisible(false);
  this->Internal->GPSPositionOrientationGroup->setVisible(false);
  this->Internal->NetworkGroup->setVisible(false);
  this->Internal->NetworkForwardingGroup->setVisible(false);

  // set minimum
  this->Internal->LidarPortSpinBox->setMinimum(minAllowedPort);
  this->Internal->GPSPortSpinBox->setMinimum(minAllowedPort);
  this->Internal->GPSForwardingPortSpinBox->setMinimum(minAllowedPort);
  this->Internal->LidarForwardingPortSpinBox->setMinimum(minAllowedPort);
  // set value
  // network configuration values
  this->Internal->LidarPortSpinBox->setValue(defaultLidarPort);
  this->Internal->GPSPortSpinBox->setValue(defaultGpsPort);
  this->Internal->GPSForwardingPortSpinBox->setValue(defaultGpsPort);
  this->Internal->LidarForwardingPortSpinBox->setValue(defaultLidarPort);
  this->Internal->ipAddresslineEdit->setText(defaultIpAddress);
  this->Internal->AdvancedConfiguration->setChecked(false);
  this->Internal->EnableForwardingCheckBox->setChecked(false);
  // lidar orientation values
  this->Internal->LidarPitchSpinBox->setValue(defaultSensorValue);
  this->Internal->LidarYawSpinBox->setValue(defaultSensorValue);
  this->Internal->LidarRollSpinBox->setValue(defaultSensorValue);
  // Lidar origin values
  this->Internal->LidarXSpinBox->setValue(defaultSensorValue);
  this->Internal->LidarYSpinBox->setValue(defaultSensorValue);
  this->Internal->LidarZSpinBox->setValue(defaultSensorValue);
  // GPS orientation values
  this->Internal->GpsYawSpinBox->setValue(defaultSensorValue);
  this->Internal->GpsRollSpinBox->setValue(defaultSensorValue);
  this->Internal->GpsPitchSpinBox->setValue(defaultSensorValue);
  // GPS origin values
  this->Internal->GpsXSpinBox->setValue(defaultSensorValue);
  this->Internal->GpsYSpinBox->setValue(defaultSensorValue);
  this->Internal->GpsZSpinBox->setValue(defaultSensorValue);
}

//-----------------------------------------------------------------------------
QStringList vvCalibrationDialog::calibrationFiles() const
{
  return this->Internal->Settings->value("VelodyneHDLPlugin/CalibrationFileDialog/Files")
    .toStringList();
}

//-----------------------------------------------------------------------------
QString vvCalibrationDialog::selectedCalibrationFile() const
{
  const int row = this->Internal->ListWidget->currentRow();
  return this->Internal->ListWidget->item(row)->data(Qt::UserRole).toString();
}

//-----------------------------------------------------------------------------
QMatrix4x4 vvCalibrationDialog::sensorTransform() const
{
  // The aim here is to compute the 4x4 matrix that represents
  // the affine transform:
  // Y = RX + T
  // QMatrix4x4 class uses openGL / renderer conventions which
  // is counterintuitive from a linear algebra point of view regarding
  // the sequence of operations (mathematically we first rotate
  // around X, Y, Z and then add T).
  QMatrix4x4 transform;
  transform.translate(this->Internal->LidarXSpinBox->value(),
    this->Internal->LidarYSpinBox->value(), this->Internal->LidarZSpinBox->value());
  transform.rotate(this->Internal->LidarYawSpinBox->value(), 0.0, 0.0, 1.0);
  transform.rotate(this->Internal->LidarPitchSpinBox->value(), 0.0, 1.0, 0.0);
  transform.rotate(this->Internal->LidarRollSpinBox->value(), 1.0, 0.0, 0.0);

  return transform;
}

//-----------------------------------------------------------------------------
QMatrix4x4 vvCalibrationDialog::gpsTransform() const
{
  // The aim here is to compute the 4x4 matrix that represents
  // the affine transform:
  // Y = RX + T
  // QMatrix4x4 class uses openGL / renderer conventions which
  // is counterintuitive from a linear algebra point of view regarding
  // the sequence of operations (mathematically we first rotate
  // around X, Y, Z and then add T).
  QMatrix4x4 transform;
  transform.translate(this->Internal->GpsXSpinBox->value(),
    this->Internal->GpsYSpinBox->value(), this->Internal->GpsZSpinBox->value());
  transform.rotate(this->Internal->GpsYawSpinBox->value(), 0.0, 0.0, 1.0);
  transform.rotate(this->Internal->GpsPitchSpinBox->value(), 0.0, 1.0, 0.0);
  transform.rotate(this->Internal->GpsRollSpinBox->value(), 1.0, 0.0, 0.0);

  return transform;
}

//-----------------------------------------------------------------------------
double vvCalibrationDialog::gpsYaw() const
{
  return this->Internal->GpsYawSpinBox->value();
}

//-----------------------------------------------------------------------------
double vvCalibrationDialog::gpsRoll() const
{
  return this->Internal->GpsRollSpinBox->value();
}

//-----------------------------------------------------------------------------
double vvCalibrationDialog::gpsPitch() const
{
  return this->Internal->GpsPitchSpinBox->value();
}

//-----------------------------------------------------------------------------
int vvCalibrationDialog::lidarPort() const
{
  return this->Internal->LidarPortSpinBox->value();
}

//-----------------------------------------------------------------------------
int vvCalibrationDialog::gpsPort() const
{
  return this->Internal->GPSPortSpinBox->value();
}

//-----------------------------------------------------------------------------
int vvCalibrationDialog::lidarForwardingPort() const
{
  return this->Internal->LidarForwardingPortSpinBox->value();
}

//-----------------------------------------------------------------------------
int vvCalibrationDialog::gpsForwardingPort() const
{
  return this->Internal->GPSForwardingPortSpinBox->value();
}

//-----------------------------------------------------------------------------
bool vvCalibrationDialog::isForwarding() const
{
  return this->Internal->EnableForwardingCheckBox->isChecked();
}

QString vvCalibrationDialog::ipAddressForwarding() const
{
  return this->Internal->ipAddresslineEdit->text();
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::accept()
{
  this->Internal->saveSelectedRow();
  this->Internal->saveSensorTransform();
  this->Internal->saveGpsTransform();
  this->Internal->saveLidarPort();
  this->Internal->saveGpsPort();
  this->Internal->saveLidarForwardingPort();
  this->Internal->saveGPSForwardingPort();
  this->Internal->saveEnableForwarding();
  this->Internal->saveAdvancedConfiguration();
  this->Internal->saveForwardIpAddress();
  QDialog::accept();
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::onCurrentRowChanged(int row)
{
  this->Internal->RemoveButton->setEnabled(row > this->Internal->BuiltInCalibrationFiles.size());
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::addFile()
{
  QString defaultDir =
    this->Internal->Settings->value("VelodyneHDLPlugin/OpenData/DefaultDir", QDir::homePath())
      .toString();

  QString selectedFiler("*.xml");
  QString fileName = QFileDialog::getOpenFileName(
    this, tr("Choose Calibration File"), defaultDir, tr("xml (*.xml)"), &selectedFiler);

  if (fileName.isEmpty())
  {
    return;
  }

  this->Internal->ListWidget->addItem(createEntry(fileName, false));
  this->Internal->ListWidget->setCurrentRow(this->Internal->ListWidget->count() - 1);
  this->Internal->saveFileList();

  this->Internal->Settings->setValue(
    "VelodyneHDLPlugin/OpenData/DefaultDir", QFileInfo(fileName).absoluteDir().absolutePath());
}

//-----------------------------------------------------------------------------
void vvCalibrationDialog::removeSelectedFile()
{
  const int row = this->Internal->ListWidget->currentRow();
  if (row > this->Internal->BuiltInCalibrationFiles.size())
  {
    delete this->Internal->ListWidget->takeItem(row);
    this->Internal->saveFileList();
  }
}
