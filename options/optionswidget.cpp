#include "optionswidget.h"
#ifdef LANE
#include "definedsettings.h"
#include "application.h"
#include "externs.h"
#include "QsLog.h"
#else
#define QLOG_DEBUG() qDebug()
#define QLOG_INFO() qDebug()
#endif
OptionsWidget::OptionsWidget(QWidget * parent) : QWidget(parent) {
  m_debug = false;
  m_dirty = false;
  m_btns = 0;
}
OptionsWidget::OptionsWidget(const QString & theme,QWidget * parent) : QWidget(parent) {
  m_debug = false;
  m_dirty = false;
#ifdef LANE
  m_settingsFileName = getLexicon()->settingsFileName(theme);
#else
  m_settingsFileName = QString("%1.ini").arg(theme);
#endif
}
QSize OptionsWidget::sizeHint() const {
  return QSize(800,400);
}
void OptionsWidget::setFileName(const QString & theme) {
  m_settingsFileName = theme;
  m_btns = 0;
}
void OptionsWidget::setDebug(bool v) {
  m_debug = v;
}
void OptionsWidget::addButtons() {
  m_btns = new QDialogButtonBox(QDialogButtonBox::Save|QDialogButtonBox::Reset|QDialogButtonBox::Help);
  m_btns->button(QDialogButtonBox::Save)->setEnabled(false);
  m_btns->button(QDialogButtonBox::Reset)->setEnabled(false);

  layout()->addWidget(m_btns);
  connect(m_btns->button(QDialogButtonBox::Save),SIGNAL(clicked()),this,SLOT(onSave()));
  connect(m_btns->button(QDialogButtonBox::Reset),SIGNAL(clicked()),this,SLOT(onReset()));
  connect(m_btns->button(QDialogButtonBox::Help),SIGNAL(clicked()),this,SLOT(onHelp()));
  m_btns->button(QDialogButtonBox::Help)->setEnabled(true);
}
void OptionsWidget::onSave() {
  writeSettings();
  setButtons(false);
}
void OptionsWidget::onReset() {
  readSettings(true);
  setButtons(false);
}
void OptionsWidget::setButtons(bool dirty) {
  if (m_btns != 0) {
  m_btns->button(QDialogButtonBox::Save)->setEnabled(dirty);
  m_btns->button(QDialogButtonBox::Reset)->setEnabled(dirty);
  }
}
/*
void OptionsWidget::readSettings() {
}
void OptionsWidget::writeSettings() {
}
*/
void OptionsWidget::notifyChange() {
  bool v =  isModified();
  setButtons(v);
  emit(modified(v));
  //  emit(valueChanged());
}
OptionsWidget::~OptionsWidget() {
}
void OptionsWidget::stateChanged(int /* state */) {
  bool v =  isModified();
  setButtons(v);
  emit(modified(v));

  //  emit(modified(m_dirty));
}
void OptionsWidget::editingFinished() {
  bool v =  isModified();
  setButtons(v);
  emit(modified(v));

  //  m_dirty = this->isModified();
  //  emit(modified(m_dirty));
}
void OptionsWidget::textChanged(const QString & /* text */) {
  bool v =  isModified();
  setButtons(v);
  emit(modified(v));

  //  m_dirty = this->isModified();
  //  emit(modified(m_dirty));

}
void OptionsWidget::valueChanged(int /* v */) {
  bool v =  isModified();
  setButtons(v);
  emit(modified(v));
}
void OptionsWidget::valueChanged(double /* v */) {
  bool v =  isModified();
  setButtons(v);
  emit(modified(v));
}
void OptionsWidget::valueChanged(bool /* v */) {
  bool v =  isModified();
  setButtons(v);
  emit(modified(v));
}
void OptionsWidget::keySequenceChanged(const QKeySequence & /* v */) {
  bool v =  isModified();
  setButtons(v);
  emit(modified(v));
}
void OptionsWidget::dateTimeChanged(const QDateTime & /* v */) {
  bool v =  isModified();
  setButtons(v);
  emit(modified(v));
}
void OptionsWidget::toggled(bool /* v */) {
  bool v = isModified();
  setButtons(v);
  emit(modified(v));
}
/*
bool OptionsWidget::isModified()  {
  return false;
}
*/
void OptionsWidget::blockAllSignals(bool block) {
  QList<QKeySequenceEdit *> edits = this->findChildren<QKeySequenceEdit *>();
  foreach(QKeySequenceEdit *  widget,edits) {
    widget->blockSignals(block);
  }
  QList<QLineEdit *> lineedits = this->findChildren<QLineEdit *>();
  foreach(QLineEdit *  widget,lineedits) {
    widget->blockSignals(block);
  }
  QList<QCheckBox *> checkboxes = this->findChildren<QCheckBox *>();
  foreach(QCheckBox *  widget,checkboxes) {
    widget->blockSignals(block);
  }
  QList<QRadioButton *> radios = this->findChildren<QRadioButton *>();
  foreach(QRadioButton *  widget,radios) {
    widget->blockSignals(block);
  }
  QList<QComboBox *> comboboxes = this->findChildren<QComboBox *>();
  foreach(QComboBox *  widget,comboboxes) {
    widget->blockSignals(block);
  }
  QList<QDoubleSpinBox *> doublespinboxes = this->findChildren<QDoubleSpinBox *>();
  foreach(QDoubleSpinBox *  widget,doublespinboxes) {
    widget->blockSignals(block);
  }
  QList<QSpinBox *> spinboxes = this->findChildren<QSpinBox *>();
  foreach(QSpinBox *  widget,spinboxes) {
    widget->blockSignals(block);
  }
  QList<QKeySequenceEdit *> keyedits = this->findChildren<QKeySequenceEdit *>();
  foreach(QKeySequenceEdit *  widget,keyedits) {
    widget->blockSignals(block);
  }
  QList<QDateTimeEdit *> datetimeedits = this->findChildren<QDateTimeEdit *>();
  foreach(QDateTimeEdit *  widget,datetimeedits) {
    widget->blockSignals(block);
  }

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
  QList<QRadioButton *> radios = this->findChildren<QRadioButton *>();
  foreach(QRadioButton *  widget,radios) {
    connect(widget,SIGNAL(toggled(bool)),this,SLOT(toggled(bool)));
  }
  QList<QComboBox *> comboboxes = this->findChildren<QComboBox *>();
  foreach(QComboBox *  widget,comboboxes) {
      connect(widget,SIGNAL(currentIndexChanged(int)),this,SLOT(stateChanged(int)));
  }
  QList<QDoubleSpinBox *> doublespinboxes = this->findChildren<QDoubleSpinBox *>();
  foreach(QDoubleSpinBox *  widget,doublespinboxes) {
    connect(widget,SIGNAL(valueChanged(double)),this,SLOT(valueChanged(double)));
  }
  QList<QSpinBox *> spinboxes = this->findChildren<QSpinBox *>();
  foreach(QSpinBox *  widget,spinboxes) {
    connect(widget,SIGNAL(valueChanged(int)),this,SLOT(valueChanged(int)));
  }
  QList<QKeySequenceEdit *> keyedits = this->findChildren<QKeySequenceEdit *>();
  foreach(QKeySequenceEdit *  widget,keyedits) {
    connect(widget,SIGNAL(keySequenceChanged(const QKeySequence &)),this,SLOT(keySequenceChanged(const QKeySequence &)));
  }
  QList<QDateTimeEdit *> datetimeedits = this->findChildren<QDateTimeEdit *>();
  foreach(QDateTimeEdit *  widget,datetimeedits) {
    connect(widget,SIGNAL(dateTimeChanged(const QDateTime &)),this,SLOT(dateTimeChanged(const QDateTime &)));
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
/**
 * Returns true is key value does not match the widget value
 *
 * @param settings
 * @param key
 * @param p
 *
 * @return
 */
bool OptionsWidget::compare(const QSettings * settings,const QString & key, QWidget * p) {
  QLineEdit * edit = qobject_cast<QLineEdit *>(p);
  if (edit) {
    if (settings->value(key).toString() != edit->text()) {
        m_changes  << QString("%1 | %2 | %3").arg(key).arg(settings->value(key).toString()).arg(edit->text());
      if (m_debug) {
        QLOG_DEBUG() << m_changes.last();
      }
      return true;
    }
  }
  QCheckBox * box = qobject_cast<QCheckBox *>(p);
  if (box) {
    if (settings->value(key).toBool() != box->isChecked()) {
        m_changes  << QString("%1 | %2 | %3")
          .arg(key)
          .arg(settings->value(key).toBool())
          .arg((box->isChecked()));
      if (m_debug) {
        QLOG_DEBUG() << m_changes.last();
      }
      return true;
    }
  }
  QSpinBox * spin = qobject_cast<QSpinBox *>(p);
  if (spin) {
    if  (settings->value(key).toInt() != spin->value()) {
      m_changes  << QString("%1 | %2 | %3")
        .arg(key)
        .arg(settings->value(key).toInt())
        .arg(spin->value());
      if (m_debug) {
        QLOG_DEBUG() << m_changes.last();
      }
      //        QLOG_DEBUG() << "Is modified" << key << settings->value(key).toInt() << spin->value();
      return true;
    }
  }
  QDoubleSpinBox * dspin = qobject_cast<QDoubleSpinBox *>(p);
  if (dspin) {
    if (settings->value(key).toDouble() != dspin->value()) {
      m_changes  << QString("%1 | %2 | %3")
        .arg(key)
        .arg(settings->value(key).toDouble())
        .arg(dspin->value());
      if (m_debug) {
        QLOG_DEBUG() << m_changes.last();
      }
      //        QLOG_DEBUG() << "Is modified" << key << settings->value(key).toDouble() << dspin->value();
      return true;
    }
  }
  QKeySequenceEdit * keyedit = qobject_cast<QKeySequenceEdit *>(p);
  if (keyedit) {
    QString k1 = settings->value(key).toString();
    QString k2 = keyedit->keySequence().toString();
    k1.remove(QChar(' '));
    k2.remove(QChar(' '));
    if  (k1  != k2 ) {
      m_changes  << QString("%1 | %2 | %3")
        .arg(key)
        .arg(k1)
        .arg(k2);
      if (m_debug) {
        QLOG_DEBUG() << m_changes.last();
      }
      //        QLOG_DEBUG() << "Is modified" << key << k1 << k2;
      return true;
    }
  }
  QDateTimeEdit * datetimeedit = qobject_cast<QDateTimeEdit *>(p);
  if (datetimeedit) {
    if  (settings->value(key).toString() != datetimeedit->dateTime().toString(Qt::ISODate)) {
      m_changes  << QString("%1 | %2 | %3")
        .arg(key)
        .arg(settings->value(key).toString())
        .arg(datetimeedit->dateTime().toString(Qt::ISODate));
      if (m_debug) {
        QLOG_DEBUG() << m_changes.last();
      }
      //        QLOG_DEBUG() << "Is modified" << key << settings->value(key).toString() << datetimeedit->dateTime().toString(Qt::ISODate);
      return true;

    }
  }
  QComboBox * combo = qobject_cast<QComboBox *>(p);
  if (combo) {
    if ((key == SID_TEXTSEARCH_FAILURE_ACTION) ||
        (key == SID_HEADSEARCH_FAILURE_ACTION) ||
        (key == SID_ROOTSEARCH_FAILURE_ACTION) ||
        (key == SID_LOCALSEARCH_FAILURE_ACTION))
        {
      int ix = combo->currentIndex();
      if (settings->value(key).toInt() != ix) {
        m_changes  << QString("%1 | %2 | %3")
          .arg(key)
          .arg(settings->value(key).toInt())
          .arg(ix);
        if (m_debug) {
          QLOG_DEBUG() << m_changes.last();
        }
        return true;
      }
    }
    else {
      QString v = combo->currentText();
      if (settings->value(key) != v) {
        m_changes  << QString("%1 | %2 | %3")
          .arg(key)
          .arg(settings->value(key).toString())
          .arg(v);
        if (m_debug) {
          QLOG_DEBUG() << m_changes.last();
        }
        return true;
      }
    }
  }
  QRadioButton * radio = qobject_cast<QRadioButton *>(p);
  if (radio) {
    QString v = radio->text();
    if (settings->value(key).toBool() != radio->isChecked()) {
        m_changes  << QString("%1 | %2 | %3")
          .arg(key)
          .arg(settings->value(key).toBool())
          .arg(radio->isChecked());
        if (m_debug) {
          QLOG_DEBUG() << m_changes.last();
        }
      return true;
    }
  }
  return false;
}
void OptionsWidget::onHelp() {
  emit(showHelp(this->metaObject()->className()));
}
void OptionsWidget::setLineEditSize(int size) {
  QList<QLineEdit *> edits = this->findChildren<QLineEdit *>();
  foreach(QLineEdit *  widget,edits) {
#ifdef Q_OS_OSX
    widget->setMinimumWidth(size);
#else
    widget->setMaximumWidth(size);
#endif
  }
}
void OptionsWidget::setKeySequenceEditSize(int size) {
  QList<QKeySequenceEdit *> edits = this->findChildren<QKeySequenceEdit *>();
  foreach(QKeySequenceEdit *  widget,edits) {
#ifdef Q_OS_OSX
    widget->setMinimumWidth(size);
#else
    widget->setMaximumWidth(size);
#endif

  }
}
void OptionsWidget::setComboSize(int size) {
  QList<QComboBox *> edits = this->findChildren<QComboBox *>();
  foreach(QComboBox *  widget,edits) {
#ifdef Q_OS_OSX
    widget->setMinimumWidth(size);
#else
    widget->setMaximumWidth(size);
#endif

  }
}
void OptionsWidget::setControlSize(QWidget * widget,int size) {
#ifdef Q_OS_OSX
    widget->setMinimumWidth(size);
#else
    widget->setMaximumWidth(size);
#endif
 }
QStringList OptionsWidget::getChanges() const {
  return m_changes;
}
