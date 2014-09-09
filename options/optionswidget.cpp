#include "optionswidget.h"
OptionsWidget::OptionsWidget(QSettings * settings,QWidget * parent) : QWidget(parent) {
  m_dirty = false;
  m_settings = settings;
}
void OptionsWidget::readSettings() {
}
void OptionsWidget::writeSettings() {
}
void OptionsWidget::notifyChange() {
  m_dirty = true;
  emit(valueChanged());
}
OptionsWidget::~OptionsWidget() {

}
void OptionsWidget::stateChanged(int /* state */) {
  //  m_dirty = this->isModified();
  emit(modified(m_dirty));
}
void OptionsWidget::editingFinished() {
  //  m_dirty = this->isModified();
  emit(modified(m_dirty));
}
void OptionsWidget::textChanged(const QString & /* text */) {
  //  m_dirty = this->isModified();
  emit(modified(m_dirty));

}

bool OptionsWidget::isModified()  {
  return false;
}
void OptionsWidget::setupConnections() {
  QList<QKeySequenceEdit *> edits = this->findChildren<QKeySequenceEdit *>();
  foreach(QKeySequenceEdit *  widget,edits) {
      connect(widget,SIGNAL(editingFinished()),this,SLOT(editingFinished()));
  }
  QList<QLineEdit *> lineedits = this->findChildren<QLineEdit *>();
  foreach(QLineEdit *  widget,lineedits) {
      connect(widget,SIGNAL(textChanged(const QString &)),this,SLOT(textChanged(const QString &)));
  }
  QList<QCheckBox *> checkboxes = this->findChildren<QCheckBox *>();
  foreach(QCheckBox *  widget,checkboxes) {
      connect(widget,SIGNAL(stateChanged(int)),this,SLOT(stateChanged(int)));
  }
  QList<QComboBox *> comboboxes = this->findChildren<QComboBox *>();
  foreach(QComboBox *  widget,comboboxes) {
      connect(widget,SIGNAL(currentIndexChanged(int)),this,SLOT(stateChanged(int)));
  }

  /*
  connect(<checkbox>,SIGNAL(stateChanged(int)),this,SLOT(stateChanged(int)));
  */

  /*
  connect(<lineqedit>,SIGNAL(textChanged(const QString &)),this,SLOT(textChanged(const QString &)));
  */

  /*
  connect(<combobox>,SIGNAL(currentIndexChanged(int)),this,SLOT(stateChanged(int)));
  */

}